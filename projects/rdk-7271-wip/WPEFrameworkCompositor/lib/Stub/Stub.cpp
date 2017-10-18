#include "Module.h"

#include <interfaces/IResourceCenter.h>
#include <interfaces/IComposition.h>

MODULE_NAME_DECLARATION(BUILD_REFERENCE)

namespace WPEFramework {
namespace Plugin {
    /* -------------------------------------------------------------------------------------------------------------
     *
     * ------------------------------------------------------------------------------------------------------------- */
    static uint8 _idBuffer[13];

    static Exchange::IResourceCenter* _implementation = nullptr;

    static void CloseDown()
    {
        // Seems we are destructed.....If we still have a pointer to the implementation, Kill it..
        if (_implementation != nullptr) {
            delete _implementation;
            _implementation = nullptr;
        }
    }

    /* -------------------------------------------------------------------------------------------------------------
     *
     * ------------------------------------------------------------------------------------------------------------- */
    class PlatformImplementation : public Exchange::IResourceCenter,
                                   public Exchange::IComposition
    {
    private:
        PlatformImplementation(const PlatformImplementation&) = delete;
        PlatformImplementation& operator=(const PlatformImplementation&) = delete;

    public:
        PlatformImplementation()
            : _adminLock()
            , _clients()
            , _job(Core::ProxyType<Job>::Create(this))
            , _hardwarestate(Exchange::IResourceCenter::UNITIALIZED)
            , _identifier(nullptr)
            , _initServer(false)
        {
            // Register an @Exit, in case we are killed, with an incorrect ref count !!
            if (atexit(CloseDown) != 0) {
                TRACE(Trace::Information, (("Could not register @exit handler. Error: %d."), errno));
                exit(EXIT_FAILURE);
            }

            ASSERT(_implementation == nullptr);

            _implementation = this;
        }

       /* virtual */ uint32 Configure(const string& Config)
        {
            PlatformImplementation::Config config;
            config.FromString(Config);

            _initServer = (config.Service.Value() == Plugin::PlatformImplementation::Config::enumSource::Internal) ? true : false;

            _hardwarestate = Exchange::IResourceCenter::INITIALIZING;

            // fake config.HWDelay.Value() miliseconds hardware initiation time.
            Core::Time timestamp(Core::Time::Now());
            timestamp.Add(config.HWDelay.Value());
            PluginHost::WorkerPool::Instance().Schedule(timestamp, _job);

            TRACE(Trace::Information, (_T("PlatformImplementation ResourceCenter configured")));

            return (Core::ERROR_NONE);
        }

        ~PlatformImplementation()
        {
            PluginHost::WorkerPool::Instance().Revoke(_job);
            _implementation = nullptr;
        }

    public:
        BEGIN_INTERFACE_MAP(PlatformImplementation)
        INTERFACE_ENTRY(Exchange::IResourceCenter)
        INTERFACE_ENTRY(Exchange::IComposition)
        END_INTERFACE_MAP

    public:
        class Job : public Core::IDispatchType<void> {
        private:
            Job() = delete;
            Job(const Job&) = delete;
            Job& operator=(const Job&) = delete;

        public:
            Job(PlatformImplementation* parent)
                : _parent(*parent)
            {
                ASSERT(parent != nullptr);
            }
            virtual ~Job()
            {
            }

        public:
            virtual void Dispatch()
            {
                _parent.IsRunning();
            }

        private:
            PlatformImplementation& _parent;
        };

        class Config : public Core::JSON::Container {
        public:
            Config(const Config&);
            Config& operator=(const Config&);

        public:
            enum enumSource {
                Internal,
                External
            };

        public:
            Config()
                : Core::JSON::Container()
                , Service()
                , HWDelay()
            {
                Add(_T("service"), &Service);
                Add(_T("hardwareready"), &HWDelay);
            }
            ~Config()
            {
            }

        public:
            Core::JSON::EnumType<enumSource> Service;
            Core::JSON::DecUInt16 HWDelay;
        };

    public:
        // -------------------------------------------------------------------------------------------------------
        //   IResourceCenter methods
        // -------------------------------------------------------------------------------------------------------
        /* virtual */ void Register(Exchange::IResourceCenter::INotification* notification)
        {
            // Do not double register a notification sink.
            _adminLock.Lock();
            ASSERT(std::find(_clients.begin(), _clients.end(), notification) == _clients.end());

            notification->AddRef();

            _clients.push_back(notification);

            notification->StateChange(HardwareState());

            _adminLock.Unlock();
        }

        /* virtual */ void Unregister(Exchange::IResourceCenter::INotification* notification)
        {
            _adminLock.Lock();
            std::list<Exchange::IResourceCenter::INotification*>::iterator index(std::find(_clients.begin(), _clients.end(), notification));

            // Do not un-register something you did not register.
            ASSERT(index != _clients.end());

            notification->Release();

            _clients.erase(index);
            _adminLock.Unlock();
        }

        /* virtual */ void* InfraRedHandle() const
        {
            return nullptr;
        }

        /* virtual */ Exchange::IResourceCenter::hardware_state HardwareState() const
        {
            return (_hardwarestate);
        }

        /* virtual */ uint8 Identifier(const uint8 maxLength, uint8 buffer[]) const
        {
            if (_identifier == nullptr) {
                string id("TESTSTUB");

                ::memcpy(&(_idBuffer[1]), id.c_str(), id.size());
                _idBuffer[0] = static_cast<uint8>(id.size());

                _identifier = _idBuffer;
            }

            ::memcpy(buffer, &_identifier[1], maxLength);

            return (_identifier[0]);
        }

        /* virtual */ void Register(IComposition::INotification* notification)
        {
        }

        /* virtual */ void Unregister(IComposition::INotification* notification)
        {
        }

        /* virtual */ IClient* Client(const uint8 index)
        {
            return nullptr;
        }

        /* virtual */ IClient* Client(const string& name)
        {
            return nullptr;
        }

    private:
        void IsRunning()
        {
            TRACE(Trace::Information, (_T("All hardware initialized")));

            _hardwarestate = Exchange::IResourceCenter::OPERATIONAL;

            Notify();
        }

        void Notify()
        {
            _adminLock.Lock();

            if (_clients.size() > 0) {
                std::list<Exchange::IResourceCenter::INotification*>::iterator index(_clients.begin());
                (*index)->StateChange(HardwareState());
            }
            _adminLock.Unlock();
        }

    private:
        Core::CriticalSection _adminLock;
        std::list<Exchange::IResourceCenter::INotification*> _clients;
        Core::ProxyType<Core::IDispatchType<void> > _job;
        hardware_state _hardwarestate;
        mutable uint8* _identifier;
        bool _initServer;
        Config _config;
    };

    SERVICE_REGISTRATION(PlatformImplementation, 1, 0);

    ENUM_CONVERSION_BEGIN(Plugin::PlatformImplementation::Config::enumSource)
    { Plugin::PlatformImplementation::Config::enumSource::External, _TXT("external") },
    { Plugin::PlatformImplementation::Config::enumSource::Internal, _TXT("internal") },
    ENUM_CONVERSION_END(Plugin::PlatformImplementation::Config::enumSource)

} // namespace Plugin
} // namespace WPEFramework

