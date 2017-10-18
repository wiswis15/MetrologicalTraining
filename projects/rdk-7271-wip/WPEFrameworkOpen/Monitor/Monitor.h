#ifndef __MONITOR_H
#define __MONITOR_H

#include "Module.h"
#include <interfaces/IMemory.h>

static uint32 gcd(uint32 a, uint32 b)
{
    return b == 0 ? a : gcd(b, a % b);
}

namespace WPEFramework {
namespace Plugin {

    class Monitor : public PluginHost::IPlugin, public PluginHost::IWeb {
    public:
        class Memory {
        public:
            Memory()
                : _resident()
                , _allocated()
                , _shared()
                , _process()
            {
            }
            Memory(const Memory& copy)
                : _resident(copy._resident)
                , _allocated(copy._allocated)
                , _shared(copy._shared)
                , _process(copy._process)
            {
            }
            ~Memory()
            {
            }

        public:
            void Measure(Exchange::IMemory* memInterface)
            {
                _resident.Set(memInterface->Resident());
                _allocated.Set(memInterface->Allocated());
                _shared.Set(memInterface->Shared());
                _process.Set(memInterface->Processes());
            }
            void Reset()
            {
                _resident.Reset();
                _allocated.Reset();
                _shared.Reset();
                _process.Reset();
            }

        public:
            inline const Core::MeasurementType<uint64>& Resident() const
            {
                return (_resident);
            }
            inline const Core::MeasurementType<uint64>& Allocated() const
            {
                return (_allocated);
            }
            inline const Core::MeasurementType<uint64>& Shared() const
            {
                return (_shared);
            }
            inline const Core::MeasurementType<uint8>& Process() const
            {
                return (_process);
            }

        private:
            Core::MeasurementType<uint64> _resident;
            Core::MeasurementType<uint64> _allocated;
            Core::MeasurementType<uint64> _shared;
            Core::MeasurementType<uint8> _process;
        };

        class Data : public Core::JSON::Container {
        public:
            class Memory : public Core::JSON::Container {
            public:
                class Measurement : public Core::JSON::Container {
                public:
                    Measurement()
                        : Core::JSON::Container()
                    {
                        Add(_T("min"), &Min);
                        Add(_T("max"), &Max);
                        Add(_T("average"), &Average);
                        Add(_T("last"), &Last);
                    }
                    Measurement(const uint64 min, const uint64 max, const uint64 average, const uint64 last)
                        : Core::JSON::Container()
                    {
                        Add(_T("min"), &Min);
                        Add(_T("max"), &Max);
                        Add(_T("average"), &Average);
                        Add(_T("last"), &Last);

                        Min = min;
                        Max = max;
                        Average = average;
                        Last = last;
                    }
                    Measurement(const Core::MeasurementType<uint64>& input)
                        : Core::JSON::Container()
                    {
                        Add(_T("min"), &Min);
                        Add(_T("max"), &Max);
                        Add(_T("average"), &Average);
                        Add(_T("last"), &Last);

                        Min = input.Min();
                        Max = input.Max();
                        Average = input.Average();
                        Last = input.Last();
                    }
                    Measurement(const Core::MeasurementType<uint8>& input)
                        : Core::JSON::Container()
                    {
                        Add(_T("min"), &Min);
                        Add(_T("max"), &Max);
                        Add(_T("average"), &Average);
                        Add(_T("last"), &Last);

                        Min = input.Min();
                        Max = input.Max();
                        Average = input.Average();
                        Last = input.Last();
                    }
                    Measurement(const Measurement& copy)
                        : Core::JSON::Container()
                        , Min(copy.Min)
                        , Max(copy.Max)
                        , Average(copy.Average)
                        , Last(copy.Last)
                    {
                        Add(_T("min"), &Min);
                        Add(_T("max"), &Max);
                        Add(_T("average"), &Average);
                        Add(_T("last"), &Last);
                    }
                    ~Measurement()
                    {
                    }

                public:
                    Measurement& operator=(const Measurement& RHS)
                    {
                        Min = RHS.Min;
                        Max = RHS.Max;
                        Average = RHS.Average;
                        Last = RHS.Last;

                        return (*this);
                    }
                    Measurement& operator=(const Core::MeasurementType<uint64>& RHS)
                    {
                        Min = RHS.Min();
                        Max = RHS.Max();
                        Average = RHS.Average();
                        Last = RHS.Last();

                        return (*this);
                    }

                public:
                    Core::JSON::DecUInt64 Min;
                    Core::JSON::DecUInt64 Max;
                    Core::JSON::DecUInt64 Average;
                    Core::JSON::DecUInt64 Last;
                };

            public:
                Memory()
                    : Core::JSON::Container()
                {
                    Add(_T("allocated"), &Allocated);
                    Add(_T("resident"), &Resident);
                    Add(_T("shared"), &Shared);
                    Add(_T("process"), &Process);
                    Add(_T("count"), &Count);
                }
                Memory(const Monitor::Memory& input)
                    : Core::JSON::Container()
                {
                    Add(_T("allocated"), &Allocated);
                    Add(_T("resident"), &Resident);
                    Add(_T("shared"), &Shared);
                    Add(_T("process"), &Process);
                    Add(_T("count"), &Count);

                    Allocated = input.Allocated();
                    Resident = input.Resident();
                    Shared = input.Shared();
                    Process = input.Process();
                    Count = input.Allocated().Measurements();
                }
                Memory(const Memory& copy)
                    : Core::JSON::Container()
                    , Allocated(copy.Allocated)
                    , Resident(copy.Resident)
                    , Shared(copy.Shared)
                    , Process(copy.Process)
                    , Count(copy.Count)
                {
                    Add(_T("allocated"), &Allocated);
                    Add(_T("resident"), &Resident);
                    Add(_T("shared"), &Shared);
                    Add(_T("process"), &Process);
                    Add(_T("count"), &Count);
                }
                ~Memory()
                {
                }

                Memory& operator=(const Memory& RHS)
                {
                    Allocated = RHS.Allocated;
                    Resident = RHS.Resident;
                    Shared = RHS.Shared;
                    Process = RHS.Process;
                    Count = RHS.Count;

                    return (*this);
                }
                Memory& operator=(const Monitor::Memory& RHS)
                {
                    Allocated = RHS.Allocated();
                    Resident = RHS.Resident();
                    Shared = RHS.Shared();
                    Process = RHS.Process();
                    Count = RHS.Allocated().Measurements();

                    return (*this);
                }

            public:
                Measurement Allocated;
                Measurement Resident;
                Measurement Shared;
                Measurement Process;
                Core::JSON::DecUInt32 Count;
            };

        private:
            Data& operator=(const Data&);

        public:
            Data()
                : Core::JSON::Container()
            {
                Add(_T("name"), &Name);
                Add(_T("measurment"), &Measurement);
            }
            Data(const string& name, const Monitor::Memory& info)
                : Core::JSON::Container()
            {
                Add(_T("name"), &Name);
                Add(_T("measurment"), &Measurement);

                Name = name;
                Measurement = info;
            }
            Data(const Data& copy)
                : Core::JSON::Container()
                , Name(copy.Name)
                , Measurement(copy.Measurement)
            {
                Add(_T("name"), &Name);
                Add(_T("measurment"), &Measurement);
            }
            ~Data()
            {
            }

        public:
            Core::JSON::String Name;
            Memory Measurement;
            Core::JSON::Boolean Operational;
        };

    private:
        Monitor(const Monitor&);
        Monitor& operator=(const Monitor&);

        class ObserveInfo : public Core::JSON::Container {
        private:
            ObserveInfo& operator=(const ObserveInfo& RHS);

        public:
            ObserveInfo()
                : Core::JSON::Container()
            {
                Add(_T("callsign"), &Callsign);
                Add(_T("memory"), &Memory);
                Add(_T("memorylimit"), &MemoryLimit);
                Add(_T("operational"), &Operational);
            }
            ObserveInfo(const ObserveInfo& copy)
                : Core::JSON::Container()
                , Callsign(copy.Callsign)
		, Memory(copy.Memory)
                , MemoryLimit(copy.MemoryLimit)
                , Operational(copy.Operational)
            {
                Add(_T("callsign"), &Callsign);
                Add(_T("memory"), &Memory);
                Add(_T("memorylimit"), &MemoryLimit);
                Add(_T("operational"), &Operational);
            }
            ~ObserveInfo()
            {
            }

        public:
            Core::JSON::String Callsign;
            Core::JSON::DecUInt32 Memory;
            Core::JSON::DecUInt32 MemoryLimit;
            Core::JSON::DecUInt32 Operational;
        };
        class MonitorObjects : public PluginHost::IPlugin::INotification {
        private:
            MonitorObjects(const MonitorObjects&) = delete;
            MonitorObjects& operator=(const MonitorObjects&) = delete;

            class Job : public Core::IDispatchType<void> {
            private:
                Job() = delete;
                Job(const Job& copy) = delete;
                Job& operator=(const Job& RHS) = delete;

            public:
                Job(MonitorObjects* parent)
                    : _parent(*parent)
                {
                    ASSERT (parent != nullptr);
                }
                virtual ~Job()
                {
                }

            public:
                virtual void Dispatch() override
                {
                    _parent.Probe();
               }

            private:
                MonitorObjects& _parent;
            };

            class MonitorObject {
            public:
                MonitorObject() = delete;
                MonitorObject& operator=(const MonitorObject&) = delete;

                enum evaluation {
                    SUCCESFULL      = 0x00,
                    NOT_OPERATIONAL = 0x01,
                    EXCEEDED_MEMORY = 0x02
                };

            public:
                MonitorObject(const uint32 operationalInterval, const uint32 memoryInterval, const uint64 memoryThreshold, const uint64 absTime)
                    : _operationalInterval(operationalInterval)
                    , _memoryInterval(memoryInterval)
                    , _memoryThreshold(memoryThreshold * 1024)
                    , _operationalSlots(operationalInterval)
                    , _memorySlots(memoryInterval)
                    , _nextSlot(absTime)
                    , _measurement()
                    , _source(nullptr)
                {
                    ASSERT ((_operationalInterval != 0) || (_memoryInterval != 0));

                    if ((_operationalInterval != 0) && (_memoryInterval != 0) ) {
                        _interval = gcd(_operationalInterval, _memoryInterval);
                    }
                    else {
                        _interval = (_operationalInterval == 0 ? _memoryInterval : _operationalInterval);
                    }
                }
                MonitorObject(const MonitorObject& copy)
                    : _operationalInterval(copy._operationalInterval)
                    , _memoryInterval(copy._memoryInterval)
                    , _memoryThreshold(copy._memoryThreshold)
                    , _operationalSlots(copy._operationalSlots)
                    , _memorySlots(copy._memorySlots)
                    , _nextSlot(copy._nextSlot)
                    , _measurement(copy._measurement)
                    , _source(copy._source)
                    , _interval(copy._interval)
                {
                    if (_source != nullptr) {
                        _source->AddRef();
                    }
                }
                ~MonitorObject()
                {
                    if (_source != nullptr) {
                        _source->Release();
                        _source = nullptr;
                    }
                }

            public:
                inline bool HasRestartAllowed () const 
                {
                    return (_operationalInterval != 0);
                }
                inline uint32 Interval() const
                {
                    return (_interval);
                }
                inline const Memory& Measurement() const
                {
                    return (_measurement);
                }
                inline uint64 TimeSlot() const
                {
                    return (_nextSlot);
                }
                inline void Reset()
                {
                    _measurement.Reset();
                }
                inline void Retrigger(uint64 currentSlot)
                {
                    while (_nextSlot < currentSlot) {
                        _nextSlot += _interval;
                    }
                }
                inline void Set(Exchange::IMemory* memory)
                {
                    if (_source != nullptr) {
                        _source->Release();
                        _source = nullptr;
                    }

                    if (memory != nullptr) {
                        _source = memory;
                        _source->AddRef();
                    }
                }
                inline uint32 Evaluate()
                {
                    uint32 status(SUCCESFULL);
                    if (_source != nullptr) {
                        _operationalSlots -= _interval;
                        _memorySlots -= _interval;

                        if ((_operationalInterval != 0) && (_operationalSlots == 0)) {
                            if (false == _source->IsOperational()) {
                                status |= NOT_OPERATIONAL;
                                TRACE_L1("Status not operational. %d", __LINE__);
                            }
                            _operationalSlots = _operationalInterval;
                        }
                        if ((_memoryInterval != 0) && (_memorySlots == 0)) {
                            _measurement.Measure(_source);

                            if ( (_memoryThreshold != 0) && (_measurement.Resident().Last() > _memoryThreshold) ) {
                                status |= EXCEEDED_MEMORY;
                                TRACE_L1("Status Memory Exceeded. %d", __LINE__);
                            }
                            _memorySlots = _memoryInterval;
                        }
                    }
                    return (status);
                }

            private:
                const uint32 _operationalInterval; //!< Interval (s) to check the monitored processes
                const uint32 _memoryInterval; //!<  Interval (s) for a memory measurement.
                const uint64 _memoryThreshold; //!< Memory threshold in bytes for all processes.
                uint32 _operationalSlots;
                uint32 _memorySlots;
                uint64 _nextSlot;
                Memory _measurement;
                Exchange::IMemory* _source;
                uint32 _interval; //!< The lowest possible interval to check both memory and processes.
            };

        public:
			#ifdef __WIN32__ 
			#pragma warning( disable : 4355 )
			#endif
			MonitorObjects()
                : _adminLock()
                , _monitor()
                , _job(Core::ProxyType<Job>::Create(this))
                , _service(nullptr)
            {
            }
			#ifdef __WIN32__ 
			#pragma warning( default : 4355 )
			#endif
            virtual ~MonitorObjects()
            {
                ASSERT (_monitor.size() == 0);
            }

        public:
            inline uint32 Length() const
            {
                return (_monitor.size());
            }
            inline void Open(PluginHost::IShell* service, Core::JSON::ArrayType<Plugin::Monitor::ObserveInfo>::Iterator& index)
            {
                ASSERT((service != nullptr) && (_service == nullptr));

                uint64 baseTime = Core::Time::Now().Ticks();

                _service = service;
                _service->AddRef();

                _adminLock.Lock();

                while (index.Next() == true) {
                    ObserveInfo& element(index.Current());
                    string callSign(element.Callsign.Value());
                    uint64 memoryThreshold(element.MemoryLimit.Value());
                    uint32 interval(element.Operational.Value() * 1000 * 1000); // Move from Seconds to MicroSeconds
                    uint32 memory(element.Memory.Value() * 1000 * 1000);        // Move from Seconds to MicroSeconds

                    if ( (interval != 0) || (memory !=0) ) {

                        _monitor.insert(std::pair<string, MonitorObject>(callSign, MonitorObject(interval, memory, memoryThreshold, baseTime)));
                    }
                }

                _adminLock.Unlock();

                PluginHost::WorkerPool::Instance().Submit(_job);
            }
            inline void Close()
            {
                ASSERT(_service != nullptr);

                PluginHost::WorkerPool::Instance().Revoke(_job);

                _adminLock.Lock();
                _monitor.clear();
                _adminLock.Unlock();
                _service->Release();
                _service = nullptr;
            }
            virtual void StateChange(PluginHost::IShell* service)
            {
                _adminLock.Lock();

                std::map<string, MonitorObject>::iterator index(_monitor.find(service->Callsign()));

                if (index != _monitor.end()) {

                    // Only act on Activated or Deactivated...
                    PluginHost::IShell::state currentState(service->State());

                    if (currentState == PluginHost::IShell::ACTIVATED) {

                        // Get the Memory interface
                        Exchange::IMemory* memory = service->QueryInterface<Exchange::IMemory>();

                        if (memory != nullptr) {
                            index->second.Set(memory);
                            memory->Release();
                        }
                    }
                    else if (currentState == PluginHost::IShell::DEACTIVATION) {
                        index->second.Set(nullptr);
                    }
                    else if ( (currentState == PluginHost::IShell::DEACTIVATED) &&
                              (index->second.HasRestartAllowed() == true) && 
                              ( (service->Reason() == PluginHost::IShell::MEMORY_EXCEEDED) ||
                                (service->Reason() == PluginHost::IShell::FAILURE) ) ) {

                        TRACE(Trace::Information, (_T("Restarting %s again because we detected it was shutdown.\n"), service->Callsign().c_str()));
                        PluginHost::WorkerPool::Instance().Submit(PluginHost::IShell::Job::Create(service, PluginHost::IShell::ACTIVATED, PluginHost::IShell::AUTOMATIC));
                    }
                }

                _adminLock.Unlock();
            }
            void Snapshot(Core::JSON::ArrayType<Monitor::Data>& snapshot)
            {
                _adminLock.Lock();

                std::map<string, MonitorObject>::iterator element(_monitor.begin());

                // Go through the list of observations...
                while (element != _monitor.end()) {
                    snapshot.Add(Monitor::Data(element->first, element->second.Measurement()));

                    element++;
                }

                _adminLock.Unlock();
            }
            bool Snapshot(const string& name, Monitor::Memory& result)
            {
                bool found = false;

                _adminLock.Lock();

                std::map<string, MonitorObject>::iterator index(_monitor.find(name));

                if (index != _monitor.end()) {
                    result = index->second.Measurement();
                    found = true;
                }

                _adminLock.Unlock();

                return (found);
            }
            bool Reset(const string& name, Monitor::Memory& result)
            {
                bool found = false;

                _adminLock.Lock();

                std::map<string, MonitorObject>::iterator index(_monitor.find(name));

                if (index != _monitor.end()) {
                    result = index->second.Measurement();
                    index->second.Reset();
                    found = true;
                }

                _adminLock.Unlock();

                return (found);
            }

            BEGIN_INTERFACE_MAP(MonitorObjects)
                INTERFACE_ENTRY(PluginHost::IPlugin::INotification)
            END_INTERFACE_MAP

        private:
            // Probe can be run in an unlocked state as the destruction of the observer list
            // is always done if the thread that calls the Probe is blocked (paused)
            void Probe()
            {
                uint64 scheduledTime(Core::Time::Now().Ticks());
                uint64 nextSlot(static_cast<uint64>(~0));

                std::map<string, MonitorObject>::iterator index(_monitor.begin());

                // Go through the list of pending observations...
                while (index != _monitor.end()) {
                    MonitorObject& info(index->second);

                    if (info.TimeSlot() <= scheduledTime) {
                        uint32 value (info.Evaluate());

                        if ( (value & (MonitorObject::NOT_OPERATIONAL|MonitorObject::EXCEEDED_MEMORY)) != 0 ) {
                            PluginHost::IShell* plugin(_service->QueryInterfaceByCallsign<PluginHost::IShell>(index->first));

                            if (plugin != nullptr) {
                                PluginHost::IShell::reason why (((value & MonitorObject::EXCEEDED_MEMORY) != 0) ? PluginHost::IShell::MEMORY_EXCEEDED : PluginHost::IShell::FAILURE);
                                TRACE(Trace::Information, (_T("Shutdown: %s by reason: %d."), plugin->Callsign().c_str(), why));
                                PluginHost::WorkerPool::Instance().Submit(PluginHost::IShell::Job::Create(plugin, PluginHost::IShell::DEACTIVATED, why));

                                plugin->Release();
                            }
                        }
                        info.Retrigger(scheduledTime);
                    }

                    if (info.TimeSlot() < nextSlot) {
                        nextSlot = info.TimeSlot();
                    }

                    index++;
                }

                if (nextSlot != static_cast<uint64>(~0)) {
                    if (nextSlot < Core::Time::Now().Ticks()) {
                        PluginHost::WorkerPool::Instance().Submit(_job);
                    }
                    else {
                        nextSlot += 1000 /* Add 1 ms */;
                        PluginHost::WorkerPool::Instance().Schedule(nextSlot, _job);
                    }
                }
            }

        private:
            Core::CriticalSection _adminLock;
            std::map<string, MonitorObject> _monitor;
            Core::ProxyType< Core::IDispatchType<void> > _job;
            PluginHost::IShell* _service;
        };

    public:
        class Config : public Core::JSON::Container {
        public:
            Config(const Config&);
            Config& operator=(const Config&);

        public:
            Config()
                : Core::JSON::Container()
            {
                Add(_T("observables"), &Observables);
            }
            ~Config()
            {
            }

        public:
            Core::JSON::ArrayType<ObserveInfo> Observables;
        };

    public:
        Monitor()
            : _skipURL(0)
            , _monitor(Core::Service<MonitorObjects>::Create<MonitorObjects>())
        {
        }
        virtual ~Monitor()
        {
			_monitor->Release();
        }

        BEGIN_INTERFACE_MAP(Monitor)
        INTERFACE_ENTRY(PluginHost::IPlugin)
        INTERFACE_ENTRY(PluginHost::IWeb)
        END_INTERFACE_MAP

    public:
        //  IPlugin methods
        // -------------------------------------------------------------------------------------------------------

        // First time initialization. Whenever a plugin is loaded, it is offered a Service object with relevant
        // information and services for this particular plugin. The Service object contains configuration information that
        // can be used to initialize the plugin correctly. If Initialization succeeds, return nothing (empty string)
        // If there is an error, return a string describing the issue why the initialisation failed.
        // The Service object is *NOT* reference counted, lifetime ends if the plugin is deactivated.
        // The lifetime of the Service object is guaranteed till the deinitialize method is called.
        virtual const string Initialize(PluginHost::IShell* service);

        // The plugin is unloaded from WPEFramework. This is call allows the module to notify clients
        // or to persist information if needed. After this call the plugin will unlink from the service path
        // and be deactivated. The Service object is the same as passed in during the Initialize.
        // After theis call, the lifetime of the Service object ends.
        virtual void Deinitialize(PluginHost::IShell* service);

        // Returns an interface to a JSON struct that can be used to return specific metadata information with respect
        // to this plugin. This Metadata can be used by the MetData plugin to publish this information to the ouside world.
        virtual string Information() const;

        //  IWeb methods
        // -------------------------------------------------------------------------------------------------------
        // Whenever a request is received, it might carry some additional data in the body. This method allows
        // the plugin to attach a deserializable data object (ref counted) to be loaded with any potential found
        // in the body of the request.
        virtual void Inbound(Web::Request& request);

        // If everything is received correctly, the request is passed on to us, through a thread from the thread pool, to
        // do our thing and to return the result in the response object. Here the actual specific module work,
        // based on a a request is handled.
        virtual Core::ProxyType<Web::Response> Process(const Web::Request& request);

    private:
        bool Activated(const string& className, const string& callSign, IPlugin* plugin);
        bool Deactivated(const string& className, const string& callSign, IPlugin* plugin);

    private:
        uint8 _skipURL;
        Config _config;
        MonitorObjects* _monitor;
    };
}
}

#endif // __MONITOR_H
