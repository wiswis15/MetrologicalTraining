#include <generics/JSON.h>
#include <gtest/gtest.h>

namespace WPEFramework {
namespace Core {
    namespace Test {

        class ProcessInfo {
        public:
            // There are platforms/kernels being used that do not support all
            // scheduling types, like Horizon. Define a dummy value here..
#ifndef SCHED_IDLE
#define SCHED_IDLE 0x80000001
#endif
#ifdef _WIN32
#define SCHED_OTHER 0
#define SCHED_FIFO 1
#define SCHED_RR 2
#define SCHED_BATCH 3
#endif

            enum scheduler {
                BATCH = SCHED_BATCH,
                FIFO = SCHED_FIFO,
                ROUNDROBIN = SCHED_RR,
                OTHER = SCHED_OTHER,
                IDLE = SCHED_IDLE
            };

        public:
            ProcessInfo() {}

        }; // class ProcessInfo

    } // namespace Test

    ENUM_CONVERSION_BEGIN(Core::Test::ProcessInfo::scheduler)

                    { Core::Test::ProcessInfo::BATCH,      _TXT("Batch")},
                    { Core::Test::ProcessInfo::IDLE,       _TXT("Idle") },
                    { Core::Test::ProcessInfo::FIFO,       _TXT("FIFO") },
                    { Core::Test::ProcessInfo::ROUNDROBIN, _TXT("RoundRobin") },
                    { Core::Test::ProcessInfo::OTHER,      _TXT("Other") },

    ENUM_CONVERSION_END(Core::Test::ProcessInfo::scheduler)

    namespace Test {
        class PluginConfig : public Core::JSON::Container {
        public:
            PluginConfig()
                    : Core::JSON::Container()
                    , Callsign()
                    , Locator()
                    , ClassName()
                    , Version(static_cast<uint32>(~0))
                    , AutoStart(true)
                    , Configuration(false)
            {
                Add(_T("callsign"), &Callsign);
                Add(_T("locator"), &Locator);
                Add(_T("classname"), &ClassName);
                Add(_T("version"), &Version);
                Add(_T("autostart"), &AutoStart);
                Add(_T("configuration"), &Configuration);
            }
            PluginConfig(const PluginConfig & copy)
                    : Core::JSON::Container()
                    , Callsign(copy.Callsign)
                    , Locator(copy.Locator)
                    , ClassName(copy.ClassName)
                    , Version(copy.Version)
                    , AutoStart(copy.AutoStart)
                    , Configuration(copy.Configuration)
            {
                Add(_T("callsign"), &Callsign);
                Add(_T("locator"), &Locator);
                Add(_T("classname"), &ClassName);
                Add(_T("version"), &Version);
                Add(_T("autostart"), &AutoStart);
                Add(_T("configuration"), &Configuration);
            }
            ~PluginConfig()
            {
            }

            PluginConfig & operator=(const PluginConfig & RHS)
            {
                Callsign = RHS.Callsign;
                Locator = RHS.Locator;
                ClassName = RHS.ClassName;
                Version = RHS.Version;
                AutoStart = RHS.AutoStart;
                Configuration = RHS.Configuration;

                return (*this);
            }

        public:
            Core::JSON::String    Callsign;
            Core::JSON::String    Locator;
            Core::JSON::String    ClassName;
            Core::JSON::DecUInt32 Version;
            Core::JSON::Boolean   AutoStart;
            Core::JSON::String    Configuration;

        public:
            inline bool IsSameInstance(const PluginConfig & RHS) const
            {
                return ((Locator.Value() == RHS.Locator.Value()) && (ClassName.Value() == RHS.ClassName.Value()) && (Version.Value() == RHS.Version.Value()));
            }
        };

        class ServiceConfig : public Core::JSON::Container {
        private:
            ServiceConfig(const ServiceConfig &) = delete;
            ServiceConfig & operator=(const ServiceConfig &) = delete;

        public:
            class ProcessSet : public Core::JSON::Container {
            public:
                ProcessSet()
                        : Priority(0)
                        , OOMAdjust(0)
                        , Policy() {
                    Add(_T("priority"), &Priority);
                    Add(_T("policy"), &Policy);
                    Add(_T("oomadjust"), &OOMAdjust);
                }
                ProcessSet(const sint8 priority, const sint8 oomadjust, const ProcessInfo::scheduler policy)
                        : Priority(0)
                        , OOMAdjust(0)
                        , Policy() {
                    Add(_T("priority"), &Priority);
                    Add(_T("policy"), &Policy);
                    Add(_T("oomadjust"), &OOMAdjust);

                    if (priority != 0) Priority = priority;
                    if (oomadjust != 0) OOMAdjust = oomadjust;
                    Policy = policy;
                }
                ProcessSet(const sint8 priority, const sint8 oomadjust)
                        : Priority(0)
                        , OOMAdjust(0)
                        , Policy() {
                    Add(_T("priority"), &Priority);
                    Add(_T("policy"), &Policy);
                    Add(_T("oomadjust"), &OOMAdjust);

                    if (priority != 0) Priority = priority;
                    if (oomadjust != 0) OOMAdjust = oomadjust;
                }
                ProcessSet(const ProcessSet& copy)
                        : Container()
                        , Priority(copy.Priority)
                        , OOMAdjust(copy.OOMAdjust)
                        , Policy(copy.Policy) {
                    Add(_T("priority"), &Priority);
                    Add(_T("policy"), &Policy);
                    Add(_T("oomadjust"), &OOMAdjust);
                }
                ~ProcessSet() {
                }

                ProcessSet& operator= (const ProcessSet& RHS) {
                    Priority = RHS.Priority;
                    Policy = RHS.Policy;
                    OOMAdjust = RHS.OOMAdjust;

                    return (*this);
                }

                Core::JSON::DecSInt8  Priority;
                Core::JSON::DecSInt8  OOMAdjust;
                Core::JSON::EnumType<ProcessInfo::scheduler>   Policy;
            };

        public:
            ServiceConfig()
                    : Version()
                    , Port(80)
                    , Binding(_T("0.0.0.0"))
                    , Interface()
                    , Prefix(_T("Service"))
                    , PersistentPath()
                    , DataPath()
                    , SystemPath()
                    , TracePath(_T("/tmp"))
                    , ProxyStubPath()
                    , Communicator(_T("/tmp/communicator"))
                    , Redirect(_T("http://127.0.0.1/Service/Controller/UI"))
                    , Signature(_T("TestSecretKey"))
                    , IdleTime(0)
                    , IPV6(false)
                    , DefaultTraceCategories(false)
                    , Process() {
                // No IdleTime
                Add(_T("version"), &Version);
                Add(_T("port"), &Port);
                Add(_T("binding"), &Binding);
                Add(_T("interface"), &Interface);
                Add(_T("prefix"), &Prefix);
                Add(_T("persistentpath"), &PersistentPath);
                Add(_T("datapath"), &DataPath);
                Add(_T("systempath"), &SystemPath);
                Add(_T("tracepath"), &TracePath);
                Add(_T("proxystubpath"), &ProxyStubPath);
                Add(_T("communicator"), &Communicator);
                Add(_T("signature"), &Signature);
                Add(_T("idletime"), &IdleTime);
                Add(_T("ipv6"), &IPV6);
                Add(_T("tracing"), &DefaultTraceCategories);
                Add(_T("redirect"), &Redirect);
                Add(_T("process"), &Process);
                Add(_T("plugins"), &Plugins);
            }
            ~ServiceConfig() {
            }

        public:
            Core::JSON::String Version;
            Core::JSON::DecUInt16 Port;
            Core::JSON::String Binding;
            Core::JSON::String Interface;
            Core::JSON::String Prefix;
            Core::JSON::String PersistentPath;
            Core::JSON::String DataPath;
            Core::JSON::String SystemPath;
            Core::JSON::String TracePath;
            Core::JSON::String ProxyStubPath;
            Core::JSON::String Communicator;
            Core::JSON::String Redirect;
            Core::JSON::String Signature;
            Core::JSON::DecUInt16  IdleTime;
            Core::JSON::Boolean IPV6;
            Core::JSON::String DefaultTraceCategories;
            ProcessSet Process;
            Core::JSON::ArrayType<PluginConfig> Plugins;
        };

        class JSONConfigDeserializationTest : public ::testing::Test {
        public:
            JSONConfigDeserializationTest()
                : _config()
            {
            }

        protected:
            virtual void SetUp() override {}

            virtual void TearDown() override {}

            ServiceConfig _config;
            
            void CheckResults();
        };

        static const string TestCase =
            "{\n"
            " \"version\":\"2.0.0\",\n"
            " \"port\":8081,\n"
            " \"binding\":\"0.0.0.0\",\n"
            " \"ipv6\":false,\n"
            " \"idletime\":180,\n"
            " \"persistentpath\":\"/root/persistentpath\",\n"
            " \"datapath\":\"/usr/share/wpeframework\",\n"
            " \"systempath\":\"/usr/lib/wpeframework\",\n"
            " \"proxystubpath\":\"/usr/lib/wpeframework/proxystubs\",\n"
            " \"redirect\":\"/Service/Controller/UI\",\n"
            " \"tracing\":[\n"
            "  {\n"
            "   \"category\":\"Startup\",\n"
            "   \"enabled\":true\n"
            "  },\n"
            "  {\n"
            "   \"category\":\"Shutdown\",\n"
            "   \"enabled\":true\n"
            "  }\n"
            " ],\n"
            " \"plugins\":[\n"
            "  {\n"
            "   \"callsign\":\"Controller\",\n"
            "   \"configuration\":{\n"
            "    \"location\":\"external\",\n"
            "    \"time\":\"external\",\n"
            "    \"resumes\":[\n"
            "     \"WebKitBrowser\",\n"
            "     \"WebServer\"\n"
            "    ]\n"
            "   }\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"Tracing\",\n"
            "   \"locator\":\"libtracecontrol.so\",\n"
            "   \"classname\":\"TraceControl\"\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"Monitor\",\n"
            "   \"locator\":\"libmonitor.so\",\n"
            "   \"classname\":\"Monitor\",\n"
            "   \"autostart\":true,\n"
            "   \"configuration\":{\n"
            "    \"observables\":[\n"
            "     {\n"
            "      \"callsign\":\"WebKitBrowser\",\n"
            "      \"memory\":5,\n"
            "      \"memorylimit\":307200,\n"
            "      \"operational\":1\n"
            "     },\n"
            "     {\n"
            "      \"callsign\":\"YouTube\",\n"
            "      \"memory\":5,\n"
            "      \"memorylimit\":307200,\n"
            "      \"operational\":1\n"
            "     },\n"
            "     {\n"
            "      \"callsign\":\"Netflix\",\n"
            "      \"memory\":5,\n"
            "      \"memorylimit\":307200\n"
            "     }\n"
            "    ]\n"
            "   }\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"Dictionary\",\n"
            "   \"locator\":\"libdictionary.so\",\n"
            "   \"classname\":\"Dictionary\",\n"
            "   \"configuration\":{\n"
            "    \"storage\":\"DataModel.json\",\n"
            "    \"lingertime\":10\n"
            "   }\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"BackOffice\",\n"
            "   \"locator\":\"libbackoffice.so\",\n"
            "   \"classname\":\"BackOffice\",\n"
            "   \"configuration\":{\n"
            "    \"server\":\"ws.metrological.com:80\",\n"
            "    \"components\":\"Components\",\n"
            "    \"profiler\":{\n"
            "     \"testdata\":\"Hello world\",\n"
            "     \"moretest\":45\n"
            "    }\n"
            "   }\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"DeviceInfo\",\n"
            "   \"locator\":\"libdeviceinfo.so\",\n"
            "   \"classname\":\"DeviceInfo\"\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"Snapshot\",\n"
            "   \"locator\":\"libsnapshot.so\",\n"
            "   \"classname\":\"Snapshot\"\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"Provisioning\",\n"
            "   \"locator\":\"libprovisioning.so\",\n"
            "   \"classname\":\"Provisioning\",\n"
            "   \"configuration\":{\n"
            "    \"operator\":\"liberty\",\n"
            "    \"provisionpath\":\"/tmp/provision\",\n"
            "    \"server\":\"provisioning.metrological.com:80\",\n"
            "    \"system\":\"Controller\"\n"
            "   }\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"Commander\",\n"
            "   \"locator\":\"libcommander.so\",\n"
            "   \"classname\":\"Commander\"\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"RemoteControl\",\n"
            "   \"locator\":\"libremotecontrol.so\",\n"
            "   \"classname\":\"RemoteControl\",\n"
            "   \"configuration\":{\n"
            "    \"mapfile\":\"keymap.json\",\n"
            "    \"repeatstart\":500,\n"
            "    \"repeatinterval\":100\n"
            "   }\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"WebProxy\",\n"
            "   \"locator\":\"libwebproxy.so\",\n"
            "   \"classname\":\"WebProxy\",\n"
            "   \"configuration\":{\n"
            "    \"connections\":10,\n"
            "    \"links\":[\n"
            "     {\n"
            "      \"name\":\"upnp\",\n"
            "      \"type\":\"udp\",\n"
            "      \"host\":\"239.255.255.250:1900\"\n"
            "     },\n"
            "     {\n"
            "      \"name\":\"serial\",\n"
            "      \"type\":\"serial\",\n"
            "      \"device\":\"/dev/ttyACM0\",\n"
            "      \"configuration\":{\n"
            "       \"baudrate\":115200,\n"
            "       \"parity\":\"none\",\n"
            "       \"data\":8,\n"
            "       \"stop\":1\n"
            "      }\n"
            "     }\n"
            "    ]\n"
            "   }\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"WebShell\",\n"
            "   \"locator\":\"libwebshell.so\",\n"
            "   \"classname\":\"WebShell\"\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"Netflix\",\n"
            "   \"locator\":\"libnrd.so\",\n"
            "   \"classname\":\"Netflix\",\n"
            "   \"autostart\":false,\n"
            "   \"configuration\":{\n"
            "    \"provisioning\":\"Provisioning\",\n"
            "    \"model\":\"Metrological Reference DPI Implementation\",\n"
            "    \"silent\":false\n"
            "   }\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"WebServer\",\n"
            "   \"locator\":\"libwebserver.so\",\n"
            "   \"classname\":\"WebServer\",\n"
            "   \"configuration\":{\n"
            "    \"port\":8080,\n"
            "    \"binding\":\"0.0.0.0\",\n"
            "    \"path\":\"/boot/www\",\n"
            "    \"proxies\":[\n"
            "     {\n"
            "      \"path\":\"/Service/DeviceInfo\",\n"
            "      \"subst\":\"/Service/DeviceInfo\",\n"
            "      \"server\":\"127.0.0.1:80\"\n"
            "     },\n"
            "     {\n"
            "      \"path\":\"/Service/DIALServer\",\n"
            "      \"subst\":\"/Service/DIALServer\",\n"
            "      \"server\":\"127.0.0.1:80\"\n"
            "     }\n"
            "    ]\n"
            "   }\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"WebKitBrowser\",\n"
            "   \"locator\":\"libwebkitbrowser.so\",\n"
            "   \"classname\":\"WebKitBrowser\",\n"
            "   \"autostart\":true,\n"
            "   \"configuration\":{\n"
            "    \"url\":\"file:///www/index.html\",\n"
            "    \"useragent\":\"Mozilla/5.0 (Macintosh, Intel Mac OS X 10_11_4) AppleWebKit/602.1.28+ (KHTML, like Gecko) Version/9.1 Safari/601.5.17 WPE-Reference\",\n"
            "    \"injectedbundle\":\"libWPEInjectedBundle.so\",\n"
            "    \"transparent\":false,\n"
            "    \"compositor\":\"noaa\",\n"
            "    \"inspector\":\"0.0.0.0:9998\",\n"
            "    \"fps\":true,\n"
            "    \"cursor\":false,\n"
            "    \"touch\":false,\n"
            "    \"msebuffers\":\"audio:2m,video:15m,text:1m\",\n"
            "    \"memoryprofile\":\"128m\",\n"
            "    \"memorypressure\":\"databaseprocess:30m,networkprocess:50m,webprocess:170m,rpcprocess:50m\",\n"
            "    \"mediadiskcache\":false,\n"
            "    \"diskcache\":\"90m\",\n"
            "    \"xhrcache\":true\n"
            "   }\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"YouTube\",\n"
            "   \"locator\":\"libwebkitbrowser.so\",\n"
            "   \"classname\":\"WebKitBrowser\",\n"
            "   \"autostart\":false,\n"
            "   \"configuration\":{\n"
            "    \"url\":\"about:blank\",\n"
            "    \"useragent\":\"Mozilla/5.0 (Macintosh, Intel Mac OS X 10_11_4) AppleWebKit/602.1.28+ (KHTML, like Gecko) Version/9.1 Safari/601.5.17 WPE-Reference\",\n"
            "    \"injectedbundle\":\"libWPEInjectedBundle.so\",\n"
            "    \"transparent\":false,\n"
            "    \"compositor\":\"noaa\",\n"
            "    \"inspector\":\"0.0.0.0:9999\",\n"
            "    \"fps\":true,\n"
            "    \"cursor\":false,\n"
            "    \"touch\":false,\n"
            "    \"msebuffers\":\"audio:2m,video:12m,text:1m\",\n"
            "    \"memoryprofile\":\"128m\",\n"
            "    \"memorypressure\":\"databaseprocess:30m,networkprocess:50m,webprocess:170m,rpcprocess:50m\",\n"
            "    \"mediadiskcache\":false,\n"
            "    \"diskcache\":\"90m\",\n"
            "    \"xhrcache\":true\n"
            "   }\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"DIALServer\",\n"
            "   \"locator\":\"libdialserver.so\",\n"
            "   \"classname\":\"DIALServer\",\n"
            "   \"configuration\":{\n"
            "    \"webserver\":\"WebServer\",\n"
            "    \"name\":\"[TV] WPEFramework\",\n"
            "    \"model\":\"Generic Platform\",\n"
            "    \"manufacturer\":\"Metrological\",\n"
            "    \"description\":\"WPEFramework DIAL reference server.\",\n"
            "    \"apps\":[\n"
            "     {\n"
            "      \"name\":\"YouTube\",\n"
            "      \"allowstop\":true,\n"
            "      \"url\":\"https://www.youtube.com/tv\"\n"
            "     },\n"
            "     {\n"
            "      \"name\":\"Netflix\",\n"
            "      \"allowstop\":true,\n"
            "      \"url\":\"https://www.netflix.com\"\n"
            "     }\n"
            "    ]\n"
            "   }\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"TimeSync\",\n"
            "   \"locator\":\"libtimesync.so\",\n"
            "   \"classname\":\"TimeSync\",\n"
            "   \"autostart\":true,\n"
            "   \"configuration\":{\n"
            "    \"system\":\"Controller\",\n"
            "    \"interval\":30,\n"
            "    \"retries\":20,\n"
            "    \"periodicity\":24,\n"
            "    \"sources\":[\n"
            "     \"ntp://0.pool.ntp.org\",\n"
            "     \"ntp://1.pool.ntp.org\",\n"
            "     \"ntp://2.pool.ntp.org\",\n"
            "     \"ntp://3.pool.ntp.org\"\n"
            "    ]\n"
            "   }\n"
            "  },\n"
            "  {\n"
            "   \"callsign\":\"LocationSync\",\n"
            "   \"locator\":\"liblocationsync.so\",\n"
            "   \"classname\":\"LocationSync\",\n"
            "   \"configuration\":{\n"
            "    \"system\":\"Controller\",\n"
            "    \"interval\":10,\n"
            "    \"retries\":20,\n"
            "    \"source\":\"jsonip.metrological.com:80\"\n"
            "   }\n"
            "  }\n"
            " ]\n"
            "}";

        static bool FromString(const string & input, ServiceConfig & _config, uint16 bufferSize) {
            bool ready = false;

            uint16 unusedBytes = 0;
            uint16 readBytes = static_cast<uint16>(~0);
            uint16 offset = 0;

            class DeserializerImpl : public WPEFramework::Core::JSON::IElement::Deserializer {
            public:
                DeserializerImpl(bool &ready)
                        : ServiceConfig::Deserializer(), _element(nullptr), _ready(ready) {
                }

                ~DeserializerImpl() {
                }

            public:
                inline void SetElement(Core::JSON::IElement *element) {
                    ASSERT(_element == nullptr);
                    _element = element;
                }

                virtual Core::JSON::IElement *Element(const string &identifier) {
                    ASSERT(identifier.empty() == true);
                    return (_element);
                }

                virtual void Deserialized(Core::JSON::IElement &element) {
                    ASSERT(&element == _element);
                    _element = nullptr;
                    _ready = true;
                }

            private:
                Core::JSON::IElement *_element;
                bool &_ready;
            } deserializer(ready);

            _config.Clear();

            deserializer.SetElement(&_config);

            while ((ready == false) && (unusedBytes == 0) && (readBytes != 0)) {
                uint8 buffer[65536];

                readBytes = static_cast<uint16>(std::min(static_cast<size_t>(bufferSize), input.length() - offset));

                if (readBytes != 0)
                {
                    ::memcpy(buffer, input.data() + offset, readBytes);
                    offset += readBytes;
                    // Deserialize object
                    uint16 usedBytes = deserializer.Deserialize(buffer, readBytes);

                    unusedBytes = (readBytes - usedBytes);
                }
            }
            return unusedBytes == 0;
        }

        void JSONConfigDeserializationTest::CheckResults()
        {
            EXPECT_EQ(_config.Version, "2.0.0");
            EXPECT_EQ(_config.Port, 8081);
            EXPECT_EQ(_config.Binding, "0.0.0.0");
            EXPECT_EQ(_config.IPV6, false);
            EXPECT_EQ(_config.IdleTime, 180);
            EXPECT_EQ(_config.PersistentPath, "/root/persistentpath");
            EXPECT_EQ(_config.DataPath, "/usr/share/wpeframework");
            EXPECT_EQ(_config.SystemPath, "/usr/lib/wpeframework");
            EXPECT_EQ(_config.ProxyStubPath, "/usr/lib/wpeframework/proxystubs");
            EXPECT_EQ(_config.Redirect, "/Service/Controller/UI");
            EXPECT_EQ(_config.DefaultTraceCategories,
                      "[\n"
                              "  {\n"
                              "   \"category\":\"Startup\",\n"
                              "   \"enabled\":true\n"
                              "  },\n"
                              "  {\n"
                              "   \"category\":\"Shutdown\",\n"
                              "   \"enabled\":true\n"
                              "  }\n"
                              " ]");
            EXPECT_EQ(_config.Plugins.Length(), 19);
            EXPECT_EQ(_config.Plugins[0].Callsign, "Controller");
            EXPECT_EQ(_config.Plugins[0].Locator, "");
            EXPECT_EQ(_config.Plugins[0].ClassName, "");
            EXPECT_EQ(_config.Plugins[0].AutoStart, true);
            EXPECT_EQ(_config.Plugins[0].Configuration,
                      "{\n"
                              "    \"location\":\"external\",\n"
                              "    \"time\":\"external\",\n"
                              "    \"resumes\":[\n"
                              "     \"WebKitBrowser\",\n"
                              "     \"WebServer\"\n"
                              "    ]\n"
                              "   }");
            EXPECT_EQ(_config.Plugins[0].Version, -1);

            EXPECT_EQ(_config.Plugins[1].Callsign, "Tracing");
            EXPECT_EQ(_config.Plugins[1].Locator, "libtracecontrol.so");
            EXPECT_EQ(_config.Plugins[1].ClassName, "TraceControl");
            EXPECT_EQ(_config.Plugins[1].AutoStart, true);
            EXPECT_EQ(_config.Plugins[1].Configuration, "");
            EXPECT_EQ(_config.Plugins[1].Version, -1);

            EXPECT_EQ(_config.Plugins[2].Callsign, "Monitor");
            EXPECT_EQ(_config.Plugins[2].Locator, "libmonitor.so");
            EXPECT_EQ(_config.Plugins[2].ClassName, "Monitor");
            EXPECT_EQ(_config.Plugins[2].AutoStart, true);
            EXPECT_EQ(_config.Plugins[2].Configuration,
                      "{\n"
                              "    \"observables\":[\n"
                              "     {\n"
                              "      \"callsign\":\"WebKitBrowser\",\n"
                              "      \"memory\":5,\n"
                              "      \"memorylimit\":307200,\n"
                              "      \"operational\":1\n"
                              "     },\n"
                              "     {\n"
                              "      \"callsign\":\"YouTube\",\n"
                              "      \"memory\":5,\n"
                              "      \"memorylimit\":307200,\n"
                              "      \"operational\":1\n"
                              "     },\n"
                              "     {\n"
                              "      \"callsign\":\"Netflix\",\n"
                              "      \"memory\":5,\n"
                              "      \"memorylimit\":307200\n"
                              "     }\n"
                              "    ]\n"
                              "   }");
            EXPECT_EQ(_config.Plugins[0].Version, -1);
        }
        TEST_F(JSONConfigDeserializationTest, SmallParser)
        {
            string input = TestCase;
            EXPECT_TRUE(FromString(input, _config, 1024));

            CheckResults();
        }

        TEST_F(JSONConfigDeserializationTest, BigParser)
        {
            string input = TestCase;
            EXPECT_TRUE(FromString(input, _config, 8192));

            CheckResults();
        }
    } // namespace Test
} // namespace Core
} // namespace WPEFramework
