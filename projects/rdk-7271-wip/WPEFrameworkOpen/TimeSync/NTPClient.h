#ifndef TIMESYNC_NTPCLIENT_H
#define TIMESYNC_NTPCLIENT_H

#include "Module.h"
#include <interfaces/ITimeSync.h>

namespace WPEFramework {
namespace Plugin {

    class EXTERNAL NTPClient : public Core::SocketDatagram, public Exchange::ITimeSync {
    public:
        static constexpr uint32 MilliSeconds = 1000;
        static constexpr uint32 MicroSeconds = 1000 * MilliSeconds;
        static constexpr uint32 NanoSeconds = 1000 * MicroSeconds;

        using SourceIterator = Core::JSON::ArrayType<Core::JSON::String>::Iterator;

    private:
        using ServerList = std::vector<string>;
        using ServerIterator = Core::IteratorType<const ServerList, const string&, ServerList::const_iterator>;
        using DataFrame = Core::FrameType<0>;

        // This enum tracks the state for actions begin performed. As the Worker() method is re-entered,
        // we need to keep track of state.
        enum state {
            INITIAL, // Initial state
            SENDREQUEST, // Let send out an NTP request to a legitimate server.
            INPROGRESS, // A request has been sent to a NTP server, waiting for a response
            SUCCESS, // Action succeeded, we received a valid response from an NTP server
            FAILED // Action failed, we did not receive any valid response from any of the NTP servers
        };
        // As this forms the exact package to be sent for NTP, we need to make sure all members are byte
        // aligned
        class NTPPacket {
        public:
            class Timestamp {
            private:
                // Start day of NTP time as days past the imaginary date 12/1/1 BC.
                // (This is the beginning of the Christian Era, or BCE.)
                static constexpr uint32 DayNTPStarts = 693596;

                // Start day of the UNIX epoch (1970-01-01), also counting from BCE
                static constexpr uint32 DayUNIXEpochStarts = 719163;

                // Difference in Seconds between UNIX and NTP epoch (25567).
                static constexpr uint32 SecondsPerMinute = 60;
                static constexpr uint32 MinutesPerHour = 60;
                static constexpr uint32 HoursPerDay = 24;
                static constexpr uint32 SecondsPerHour = SecondsPerMinute * MinutesPerHour;
                static constexpr uint32 SecondsPerDay = SecondsPerHour * HoursPerDay;

                static constexpr uint32 NTPToUNIXSeconds =
                    (DayUNIXEpochStarts - DayNTPStarts) * SecondsPerDay;

                static constexpr double NanoSecondFraction =
                    4294967296.0 /* 2^32 as a double*/ /
                    NanoSeconds;

            public:
                // Initialize default
                Timestamp()
                {
                    _source.tv_sec = NTPToUNIXSeconds;
                    _source.tv_nsec = 0;
                }
                // Initialize using NTP time
                Timestamp(const uint32 seconds, const uint32 fraction)
                {
                    // ASSERT(seconds >= NTPToUNIXSeconds);

                    _source.tv_sec = seconds - NTPToUNIXSeconds;
                    _source.tv_nsec = static_cast<uint32>((fraction / NanoSecondFraction) + 0.5);

                    Normalize();
                }
                Timestamp(const Timestamp& copy)
                    : _source(copy._source)
                {
                }
                // Initialize using UNIX epoch time
                Timestamp(const timespec& x)
                    : _source(x)
                {
                }
                // Initialize using the generic time methods
                Timestamp(const Core::Time& x)
                {
                    uint64 ticks = x.Ticks();
                    _source.tv_sec = static_cast<uint32>(ticks / (1000 /* MilliSeconds */ * 1000 /* MicroSeconds */));
                    uint32 microseconds = static_cast<uint32>(ticks % (1000 /* MilliSeconds */ * 1000 /* MicroSeconds */));
                    _source.tv_nsec = microseconds * 1000 /* microseconds */;
                }
                ~Timestamp() {
                }

                Timestamp& operator= (const Timestamp& rhs) {

                    _source = rhs._source;

                    return (*this);
                }

            public:
                uint32 Seconds() const {
                    return (static_cast<uint32>(_source.tv_sec) + NTPToUNIXSeconds);
                }
                uint32 Fraction() const {
                    return static_cast<uint32>((_source.tv_nsec * NanoSecondFraction) + 0.5);
                }

                operator Core::Time() const
                {
                    uint64 ticks = _source.tv_sec * MicroSeconds;
                    ticks += _source.tv_nsec / (NanoSeconds / MicroSeconds);
                    return Core::Time(ticks);
                }
                inline double TimeSeconds() const
                {
                    return _source.tv_sec + static_cast<double>(_source.tv_nsec) / NanoSeconds;
                }

            private:
                inline void Normalize()
                {
                    if (_source.tv_nsec < 0)
                        do {
                            _source.tv_nsec += NanoSeconds;
                            _source.tv_sec--;
                        } while (_source.tv_nsec < 0);
                    else if (_source.tv_nsec >= static_cast<sint32>(NanoSeconds))
                        do {
                            _source.tv_nsec -= NanoSeconds;
                            _source.tv_sec++;
                        } while (_source.tv_nsec >= static_cast<sint32>(NanoSeconds));

                        /*
                        if (_source.tv_nsec < 0) {
                        _source.tv_sec -= ((NanoSeconds - _source.tv_nsec) % NanoSeconds);
                        _source.tv_nsec += (_source.tv_nsec / NanoSeconds) * NanoSeconds;
                        }
                        else {
                        _source.tv_sec += (_source.tv_nsec % NanoSeconds);
                        _source.tv_nsec -= (_source.tv_nsec / NanoSeconds) * NanoSeconds;
                        }
                        */
                }

                timespec _source;
            };

        private:
            NTPPacket(const NTPPacket&) = delete;
            NTPPacket& operator= (const NTPPacket&) = delete;

        public:
            NTPPacket()
                : leapVersionMode()
                , stratum()
                , poll()
                , precision()
                , rootDelay()
                , rootDispersion()
                , referenceID()
                , refTimestamp()
                , origTimestamp()
                , recvTimestamp()
                , xmitTimestamp()
            {
                static_assert (sizeof(uint8) == 1, "This only works if the size of the uint8 is 1 byte");
                static_assert (sizeof(sint8) == 1, "This only works if the size of the sint8 is 1 byte");
                static_assert (sizeof(uint32) == 4, "This only works if the size of the uint32 is 4 bytes");
                static_assert (sizeof(sint32) == 4, "This only works if the size of the sint32 is 4 bytes");
            }
            ~NTPPacket() {
            }

            static const uint16 PacketSize = 48;

            uint8 LeapIndicator() const { return (leapVersionMode & 0xC0) >> 6; }
            void LeapIndicator(const uint8 value) { leapVersionMode = (leapVersionMode & 0x3F) | ((value & 0x03) << 6); }
            uint8 NTPVersion() const { return (leapVersionMode & 0x38) >> 3; }
            void NTPVersion(const uint8 value) { leapVersionMode = (leapVersionMode & 0xC7) | ((value & 0x07) << 3); }
            uint8 NTPMode() const { return (leapVersionMode & 0x07) >> 0; }
            void NTPMode(const uint8 value) { leapVersionMode = (leapVersionMode & 0xF8) | ((value & 0x07) << 0); }
            uint8 Stratum() const { return stratum; }
            void Stratum(const uint8 value) { stratum = value; }
            uint8 Poll() const { return poll; }
            void Poll(const uint8 value) { poll = value; }
            uint8 Precision() const { return precision; }
            void Precision(const uint8 value) { precision = value; }
            uint32 RootDelay() const { return rootDelay; }
            void RootDelay(const uint32 value) { rootDelay = value; }
            uint32 RootDispersion() const { return rootDispersion; }
            void RootDispersion(uint32 value) { rootDispersion = value; }
            string ReferenceID() const
            {
                string result;
                if (stratum <= 1) {
                    if (referenceID == 0) {
                        result = _T("NULL");
                    }
                    else {
                        char name[5];
                        memcpy(name, &referenceID, 4);
                        name[4] = '\0';
                        result = name;
                    }
                }
                else {
                    TCHAR buffer[20];
                    _stprintf(buffer, "%d.%d.%d.%d",
                        ((referenceID >> 24) & 0xFF),
                        ((referenceID >> 16) & 0xFF),
                        ((referenceID >> 8) & 0xFF),
                        (referenceID & 0xFF));
                    result = buffer;
                }
                return result;
            }
            void ReferenceID(const uint32 value) { referenceID = value; }
            Timestamp ReferenceTimestamp() const { return refTimestamp; }
            void ReferenceTimestamp(Timestamp const& value) { refTimestamp = value; }
            Timestamp OriginalTimestamp() const { return origTimestamp; }
            void OriginalTimestamp(Timestamp const& value) { origTimestamp = value; }
            Timestamp ReceiveTimestamp() const { return recvTimestamp; }
            void ReceiveTimestamp(Timestamp const& value) { recvTimestamp = value; }
            Timestamp TransmitTimestamp() const { return xmitTimestamp; }
            void TransmitTimestamp(Timestamp const& value) { xmitTimestamp = value; }

            void Serialize(DataFrame::Writer& frame) const
            {
                frame.Number(leapVersionMode);
                frame.Number(stratum);
                frame.Number(poll);
                frame.Number(precision);
                frame.Number(rootDelay);
                frame.Number(rootDispersion);
                frame.Number(referenceID);
                Pack(frame, refTimestamp);
                Pack(frame, origTimestamp);
                Pack(frame, recvTimestamp);
                Pack(frame, xmitTimestamp);
            }

            void Deserialize(const DataFrame::Reader& frame)
            {
                leapVersionMode = frame.Number<uint8>();
                stratum = frame.Number<uint8>();
                poll = frame.Number<uint8>();
                precision = frame.Number<sint8>();
                rootDelay = frame.Number<uint32>();
                rootDispersion = frame.Number<uint32>();
                referenceID = frame.Number<uint32>();
                Unpack(frame, refTimestamp);
                Unpack(frame, origTimestamp);
                Unpack(frame, recvTimestamp);
                Unpack(frame, xmitTimestamp);
            }

            #ifdef __DEBUG__
            void DisplayPacket()
            {
                TRACE_L1("Leap indicator:      %d", LeapIndicator());
                TRACE_L1("NTP version:         %d", NTPVersion());
                TRACE_L1("NTP mode:            %d", NTPMode());
                TRACE_L1("Stratum:             %d", Stratum());
                TRACE_L1("Poll interval:       %d = %d s", Poll(), 1 << Poll());
                TRACE_L1("Precision:           %d = %lf s", Precision(), 1.0 / (1 << -Precision()));
                const double Fraction_16_16 = 65536.0;
                uint32_t delay = RootDelay();
                TRACE_L1("Root delay:          %d = %lf s", delay, delay / Fraction_16_16);
                uint32_t dispersion = RootDispersion();
                TRACE_L1("Root dispersion:     %d = %lf", dispersion, dispersion / Fraction_16_16);
                TRACE_L1("Reference id:        %s", ReferenceID().c_str());
                TRACE_L1("Reference timestamp: %s", Core::Time(ReferenceTimestamp()).ToRFC1123(false).c_str());
                TRACE_L1("Origin timestamp:    %s", Core::Time(OriginalTimestamp()).ToRFC1123(false).c_str());
                TRACE_L1("Receive timestamp:   %s", Core::Time(ReceiveTimestamp()).ToRFC1123(false).c_str());
                TRACE_L1("Transmit timestamp:  %s", Core::Time(TransmitTimestamp()).ToRFC1123(false).c_str());
            }
            #endif

        private:
            void Pack(DataFrame::Writer& frame, Timestamp const& data) const
            {
                frame.Number(data.Seconds());
                frame.Number(data.Fraction());
            }
            void Unpack(const DataFrame::Reader& frame, Timestamp& data)
            {
                uint32 seconds = frame.Number<uint32>();
                data = Timestamp(seconds, frame.Number<uint32>());
            }

            uint8 leapVersionMode; // LLVVVMMM for LL = Leap Indicator, VVV = NTP version, MMM = mode
                                   // LL: 0 = No warning, 1 = insert leap second, 2 = delete leap second, 3 = unsynchronized
                                   // VVV: 1, 2, 3, 4
                                   // MMM: 0 = reserved, 1 = symmetric active, 2 = symmetric passive, 3 = client
                                   //      4 = server, 5 = broadcast, 6 = NTP control, 7 = private use
            uint8 stratum; // Stratum number (0 = invalid)
            uint8 poll; // Log2 poll interval (seconds)
            sint8 precision; // Log2 precision (seconds)
            uint32 rootDelay; // Delay fixed precision 16.16 bit
            uint32 rootDispersion; // Dispersion fixed precision 16.16 bit
            uint32 referenceID; // Well known reference for stratum 1, IP address (V4) for stratum > 1
            Timestamp refTimestamp; // Timestamp time last set, fixed precision 32.32 bit (NTP time)
            Timestamp origTimestamp; // Timestamp time send request, fixed precision 32.32 bit (NTP time)
            Timestamp recvTimestamp; // Timestamp time receive request, fixed precision 32.32 bit (NTP time)
            Timestamp xmitTimestamp; // Timestamp time transmit request / response, fixed precision 32.32
                                     // bit (NTP time)
        };

        class Activity : public Core::IDispatchType<void> {
        private:
            Activity() = delete;
            Activity(const Activity&) = delete;
            Activity& operator=(const Activity&) = delete;

        public:
            Activity(NTPClient* parent)
                : _parent(*parent)
            {
                ASSERT(parent != nullptr);
            }
            ~Activity()
            {
            }

        public:
            virtual void Dispatch() override
            {
                _parent.Dispatch();
            }

        private:
            NTPClient& _parent;
        };

    private:
        NTPClient(const NTPClient&) = delete;
        NTPClient& operator=(const NTPClient&) = delete;

    public:
        NTPClient();
        virtual ~NTPClient();

    public:
        void Initialize(SourceIterator& sources, const uint16 retries, const uint16 delay);
        virtual void Register(Exchange::ITimeSync::INotification* notification) override;
        virtual void Unregister(Exchange::ITimeSync::INotification* notification) override;

        virtual uint32 Synchronize() override;
        virtual void Cancel() override;
        virtual string Source() const override;
        virtual uint64 SyncTime() const override;

        BEGIN_INTERFACE_MAP(NTPClient)
            INTERFACE_ENTRY(Exchange::ITimeSync)
        END_INTERFACE_MAP

    private:
        // Implement Core::SocketDatagram
        virtual uint16 SendData(uint8* dataFrame, const uint16 maxSendSize) override;
        virtual uint16 ReceiveData(uint8* dataFrame, const uint16 receivedSize) override;

        // Signal a state change, Opened, Closed or Accepted
        virtual void StateChange() override;

        void Update();
        void Dispatch();
        bool FireRequest();

    private:
        Core::CriticalSection _adminLock;
        NTPPacket _packet;
        Core::Time _syncedTimestamp;
        state _state;
        bool _fired;
        uint32 _WaitForNetwork;
        uint32 _retryAttempts;
        ServerList _servers;
        ServerIterator _serverIndex;
        Core::ProxyType<Core::IDispatchType<void> > _activity;
        std::list<Exchange::ITimeSync::INotification*> _clients;
    };

} // namespace Plugin
} // namespace WPEFramework

#endif // TIMESYNC_NTPCLIENT_H
