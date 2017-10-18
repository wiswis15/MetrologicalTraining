#include "PluginServer.h"

#ifndef __WIN32__
#include <syslog.h>
#endif

MODULE_NAME_DECLARATION(BUILD_REFERENCE)

#define MAX_EXTERNAL_WAITS 2000 /* Wait for 2 Seconds */

namespace WPEFramework {
namespace PluginHost {

static PluginHost::Server* _dispatcher = nullptr;
static bool _background = false;

class ConsoleOptions : public Core::Options {
public:
    ConsoleOptions(int argumentCount, TCHAR* arguments[])
        : Core::Options(argumentCount, arguments, _T(":bhc:"))
        , configFile(Server::ConfigFile)
    {
        Parse();
    }
    ~ConsoleOptions()
    {
    }

public:
    const TCHAR* configFile;

private:
    virtual void Option(const TCHAR option, const TCHAR* argument)
    {
        switch (option) {
        case 'c':
            configFile = argument;
            break;
            #ifndef __WIN32__
        case 'b':
            _background = true;
            break;
            #endif
        case 'h':
        default:
            RequestUsage(true);
            break;
        }
    }
};

class SecurityOptions : public ISecurity {
private:
    SecurityOptions(const SecurityOptions&);
    SecurityOptions& operator=(const SecurityOptions&);

public:
    SecurityOptions()
        : _optionResponse(Core::ProxyType<Web::Response>::Create())
    {
    }
    ~SecurityOptions()
    {
    }

public:
    // Allow a request to be checked before it is offered for processing.
    virtual bool Allowed(const Web::Request& /* request */)
    {
        // Regardless of the WebAgent, there is no differnt priocessing and we allow it :-)
        return (true);
    }

    // What options are allowed to be passed to this service???
    virtual Core::ProxyType<Web::Response> Options(const Web::Request& request)
    {
        _optionResponse->ErrorCode = Web::STATUS_NO_CONTENT;
        _optionResponse->Message = _T("No Content"); // Core::EnumerateType<Web::WebStatus>(_optionResponse->ErrorCode).Text();
        _optionResponse->Allowed = request.AccessControlMethod.Value();
        _optionResponse->AccessControlMethod = Request::HTTP_GET | Request::HTTP_POST | Request::HTTP_PUT | Request::HTTP_DELETE;
        _optionResponse->AccessControlOrigin = _T("*");
        _optionResponse->AccessControlHeaders = _T("Content-Type");

        // This will last for an hour, try again after an hour :-)
        _optionResponse->AccessControlMaxAge = 60 * 60;

        _optionResponse->Date = Core::Time::Now();

        return (_optionResponse);
    }

    //  IUnknown methods
    // -------------------------------------------------------------------------------------------------------
    BEGIN_INTERFACE_MAP(SecurityOptions)
    INTERFACE_ENTRY(ISecurity)
    END_INTERFACE_MAP

private:
    Core::ProxyType<Web::Response> _optionResponse;
};

extern "C" {

    #ifndef __WIN32__
    static Core::Event g_QuitEvent(false, true);

    void ExitDaemonHandler(int signo)
    {
        if (_background) {
            syslog(LOG_NOTICE, "Signal received %d.", signo);
        } else {
            fprintf(stderr, "Signal received %d.\n", signo);
        }

        if (signo == SIGTERM) {
            g_QuitEvent.SetEvent();
        }
    }
    #endif
    static void CloseDown()
    {
        TRACE_L1("Entering @Exit. Cleaning up process: %d.", Core::ProcessInfo().Id());

        if (_dispatcher != nullptr) {
            PluginHost::Server* destructor = _dispatcher;
            _dispatcher = nullptr;
            delete destructor;
        }

        #ifndef __WIN32__
        if (_background) {
            syslog(LOG_NOTICE, "WPEFramework Daemon closed down.");
        } else
        #endif
        {
            fprintf(stdout, "WPEFramework closed down.\n");
        }

        // Now clear all singeltons we created.
        Core::Singleton::Dispose();
        #ifndef __WIN32__
        //Close the log
        closelog();
        #endif

        TRACE_L1("Leaving @Exit. Cleaning up process: %d.", Core::ProcessInfo().Id());
    }

    void LoadPlugins (const string& name, Server::Config& config) {

        Core::Directory pluginDirectory (name.c_str(), _T("*.json"));

        while (pluginDirectory.Next() == true) {

            Core::File file (pluginDirectory.Current(), true);

            if (file.Exists()) {
                if (file.IsDirectory()) {
                    if ( (file.FileName() != ".") && (file.FileName() != "..") ) {
                        LoadPlugins(file.Name(), config);
                    }
                }
                else if (file.Open(true) == false) {
                    SYSLOG(PluginHost::Startup, (_T("Plugin config file [%s] could not be opened."), file.Name().c_str()));
                }
                else {
                    Plugin::Config pluginConfig;
                    pluginConfig.FromFile(file);
                    file.Close();

                    if ((pluginConfig.ClassName.Value().empty() == true) || (pluginConfig.Locator.Value().empty() == true)) {
                        SYSLOG(PluginHost::Startup, (_T("Plugin config file [%s] does not contain classname or locator."), file.Name().c_str()));
                    }
                    else {
                        if (pluginConfig.Callsign.Value().empty() == true) {
                            pluginConfig.Callsign = Core::File::FileName(file.FileName());
                        }

                        Core::JSON::ArrayType<Plugin::Config>::Iterator index (config.Plugins.Elements());

                        // Check if there is already a plugin config with this callsign
                        while ((index.Next() == true) && (index.Current().Callsign.Value() != pluginConfig.Callsign.Value()));

                        if (index.IsValid() == true) {
                            SYSLOG(PluginHost::Startup, (_T("Plugin config file [%s] can not be reconfigured."), file.Name().c_str()));
                        }
                        else {
                            config.Plugins.Add(pluginConfig);
                        }
                    }
                }
            }
        }
    }


    #ifdef __WIN32__
    int _tmain(int argc, _TCHAR* argv[])
    #else
    int main(int argc, char** argv)
    #endif
    {
        #ifndef __WIN32__
        //Set our Logging Mask and open the Log
        setlogmask(LOG_UPTO(LOG_NOTICE));
        openlog(argv[0], LOG_PID, LOG_USER);

        setsid();
        #endif

        ConsoleOptions options(argc, argv);

        if (atexit(CloseDown) != 0) {
            TRACE_L1("Could not register @exit handler. Argc %d.", argc);
            CloseDown();
            exit(EXIT_FAILURE);
        } else if (options.RequestUsage()) {
            #ifndef __WIN32__
            syslog(LOG_ERR, "WPEFramework Daemon failed to start. Incorrect Options.");
            #endif
            if ((_background == false) && (options.RequestUsage())) {
                fprintf(stderr, "Usage: WPEFramework [-c <config file>] -b\n");
				fprintf(stderr, "       -c <config file>  Define the configuration file to use.\n");
				fprintf(stderr, "       -b                Run WPEFramework in the background.\n");
			}
            exit(EXIT_FAILURE);
            ;
        }
        #ifndef __WIN32__
        else {
            struct sigaction sa;
            memset(&sa, 0, sizeof(struct sigaction));
            sa.sa_handler = ExitDaemonHandler;
            sa.sa_flags = 0; // not SA_RESTART!;

            sigaction(SIGINT, &sa, NULL);
            sigaction(SIGTERM, &sa, NULL);
        }

        if (_background == true) {
            //Close Standard File Descriptors
            // close(STDIN_FILENO);
            // close(STDOUT_FILENO);
            // close(STDERR_FILENO);
            syslog(LOG_NOTICE, "WPEFramework Daemon starting");
        } else
        #endif

        PluginHost::SysLog(true);

        // Read the config file, to instantiate the proper plugins and for us to open up the right listening ear.
        Core::File configFile(string(options.configFile), false);
        Server::Config serviceConfig;

        if (configFile.Open(true) == true) {
            serviceConfig.FromFile(configFile);

            configFile.Close();

       } else {
            #ifndef __WIN32__
            if (_background == true) {
                syslog(LOG_WARNING, "WPEFramework Daemon failed to start. Incorrect Config file.");
            } else
            #endif
            {
                fprintf(stdout, "Config file [%s] could not be opened.\n", options.configFile);
            }
        }

        // Time to open up, the trace buffer for this process and define it for the out-of-proccess systems
        // Define the environment variable for Tracing files, if it is not already set.
        const string tracePath(serviceConfig.TracePath.Value());

        Trace::TraceUnit::Instance().Open(tracePath);

        // Time to open up the LOG tracings by default.
        Trace::TraceType<PluginHost::Startup,  &PluginHost::MODULE_LOGGING>::Enable(true);
        Trace::TraceType<PluginHost::Shutdown, &PluginHost::MODULE_LOGGING>::Enable(true);

        Trace::TraceUnit::Instance().SetDefaultCategoriesJson(serviceConfig.DefaultTraceCategories);

        // Set the path for the out-of-process thingies
        Core::SystemInfo::SetEnvironment(TRACE_CYCLIC_BUFFER_ENVIRONMENT, tracePath);

        ISecurity* securityOptions = Core::Service<SecurityOptions>::Create<ISecurity>();

		SYSLOG(PluginHost::Startup, (_T("WPEFramework")));
		SYSLOG(PluginHost::Startup, (_T("Starting time: %s"), Core::Time::Now().ToRFC1123(false).c_str()));
		SYSLOG(PluginHost::Startup, (_T("SystemId:      %s"), Core::SystemInfo::Instance().Id(Core::SystemInfo::Instance().RawDeviceId(), ~0).c_str()));
		SYSLOG(PluginHost::Startup, (_T("Build ref:     " _T(EXPAND_AND_QUOTE(BUILD_REFERENCE)))));
		SYSLOG(PluginHost::Startup, (_T("Version:       %s"), serviceConfig.Version.Value().c_str()));

		// Before we do any translation of IP, make sure we have the right network info...
		if (serviceConfig.IPV6.Value() == false) {
			SYSLOG(PluginHost::Startup, (_T("Forcing the network to IPv4 only.")));
			Core::NodeId::ClearIPV6Enabled();
		}

        if ((serviceConfig.Process.IsSet() == true) && (serviceConfig.Process.StackSize.IsSet() == true)) {
            Core::Thread::DefaultStackSize(serviceConfig.Process.StackSize.Value());
        }

        // TIme to start loading the config of the plugins.
        string pluginPath (serviceConfig.Configs.Value());

        if (pluginPath.empty() == true) {
            pluginPath = Core::Directory::Normalize(Core::File::PathName(options.configFile));
            pluginPath += Server::PluginConfigDirectory;
        }

        // Load plugin configs from a directory.
        LoadPlugins(pluginPath, serviceConfig);

        // Startup/load/initialize what we found in the configuration.
        _dispatcher = new PluginHost::Server(serviceConfig, securityOptions, _background);

        // We don't need it anymore..
        securityOptions->Release();

        SYSLOG(PluginHost::Startup, (_T("WPEFramework actively listening.")));

        // If we have handlers open up the gates to analyze...
        _dispatcher->Dispatcher().Open(MAX_EXTERNAL_WAITS);

        #ifndef __WIN32__
        if (_background == true) {
            g_QuitEvent.Lock(Core::infinite);
        } else
        #endif
        {

            char keyPress;

            do {
                keyPress = toupper(getchar());

                switch (keyPress) {
                case 'C': {
                    Core::JSON::ArrayType<MetaData::Channel> metaData;
                    _dispatcher->Dispatcher().GetMetaData(metaData);
                    Core::JSON::ArrayType<MetaData::Channel>::Iterator index(metaData.Elements());

                    printf("\nChannels:\n");
                    printf("============================================================\n");
                    while (index.Next() == true) {
                        printf("ID:         %d\n", index.Current().ID.Value());
                        printf("State:      %s\n", index.Current().JSONState.Data().c_str());
                        printf("Active:     %s\n", (index.Current().Activity.Value() == true ? _T("true") : _T("false")));
                        printf("Remote:     %s\n", (index.Current().Remote.Value().c_str()));
                        printf("Name:       %s\n\n", (index.Current().Name.Value().c_str()));
                    }
                    break;
                }
                case 'P': {
                    Core::JSON::ArrayType<MetaData::Service> metaData;
                    _dispatcher->Services().GetMetaData(metaData);
                    Core::JSON::ArrayType<MetaData::Service>::Iterator index(metaData.Elements());

                    printf("\nPlugins:\n");
                    printf("============================================================\n");
                    while (index.Next() == true) {
                        printf("Callsign:   %s\n", index.Current().Callsign.Value().c_str());
                        printf("State:      %s\n", index.Current().JSONState.Data().c_str());
                        printf("Locator:    %s\n", index.Current().Locator.Value().c_str());
                        printf("Classname:  %s\n", index.Current().ClassName.Value().c_str());
                        printf("Autostart:  %s\n", (index.Current().AutoStart.Value() == true ? _T("true") : _T("false")));
                        printf("Observers:  %d\n", index.Current().Observers.Value());
                        printf("Requests:   %d\n", index.Current().ProcessedRequests.Value());
                        printf("JSON:       %d\n\n", index.Current().ProcessedObjects.Value());
                    }
                    break;
                }
                case 'S': {
                    uint32 count = 0;
                    MetaData::Server metaData;
                    PluginHost::WorkerPool::Instance().GetMetaData(metaData);
                    PluginHost::ISystemInfo* status(_dispatcher->SystemInfo());
                    Core::JSON::ArrayType<Core::JSON::DecUInt32>::Iterator index(metaData.ThreadPoolRuns.Elements());

                    printf("\nServer statistics:\n");
                    printf("============================================================\n");
                    #ifdef SOCKET_TEST_VECTORS
                    printf("Monitorruns: %d\n", Core::SocketPort::MonitorRuns());
                    #endif
                    if (status != nullptr) {
                        uint8 buffer[64];
                        buffer[0] = static_cast<const ISystemInfo*>(status)->Identifier(sizeof(buffer) - 1, &(buffer[1]));
                        
                        string identifier = Core::SystemInfo::Instance().Id(buffer, ~0);

                        printf("------------------------------------------------------------\n");
                        printf("SystemState: %s\n", Core::EnumerateType<PluginHost::ISystemInfo::system_state>(status->SystemState()).Data());
                        printf("------------------------------------------------------------\n");
                        printf("Network:     %s\n", (status->NetworkType() == PluginHost::ISystemInfo::UNKNOWN ? "Unknown" : 
                                                        (status->NetworkType() == PluginHost::ISystemInfo::IPV6 ? "IPv6" : "IPv4")));
                        printf("Public IP:   %s\n", status->PublicIPAddress().c_str());
                        printf("TimeZone:    %s\n", status->TimeZone().c_str());
                        printf("Country:     %s\n", status->Country().c_str());
                        printf("Region:      %s\n", status->Region().c_str());
                        printf("City:        %s\n", status->City().c_str());
                        printf("------------------------------------------------------------\n");

                        uint64 lastSync = status->TimeSync();

                        if (lastSync != 0) {
 
                            printf("Time sync:   %s\n", Core::Time(lastSync).ToRFC1123(false).c_str());
                        }
                        else {
                            printf("Time sync:   NOT SYNCHRONISED\n");
                        }
                        printf("------------------------------------------------------------\n");
                        printf("Identifier:  %s\n", identifier.c_str());
                        printf("------------------------------------------------------------\n");
                        status->Release();
                    }
                    else {
                        printf("------------------------------------------------------------\n");
                        printf("SystemState: UNKNOWN\n");
                        printf("------------------------------------------------------------\n");
                    }
                    printf("Pending:     %d\n", metaData.PendingRequests.Value());
                    printf("Occupation:  %d\n", metaData.PoolOccupation.Value());
                    printf("Poolruns:\n");
                    while (index.Next() == true) {
                        printf("  Thread%02d:  %d\n", count++, index.Current().Value());
                    }
                    break;
                }
                #if !defined(__WIN32__) && !defined(__APPLE__)
                case 'M': {
                    #ifdef __DEBUG__
                    printf("\nMonitor callstack:\n");
                    printf("============================================================\n");

                    void* buffer[32];
                    uint32 length = Core::SocketPort::GetCallStack(buffer, sizeof(buffer));
                    backtrace_symbols_fd(buffer, length, fileno(stderr));
                    #else
                    printf("Callstack for the monitor is only available in DEBUG mode.\n");
                    #endif
                    break;
                }
                #endif
                case 'Q':
                    break;

                case '?':
                    printf("\nOptions are: [P]lugins, [C]hannels, [S]erver stats and [Q]uit\n\n");
                    break;

                default:
                    break;
                }

            } while (keyPress != 'Q');
        }

        CloseDown();

        return 0;

    } // End main.
} // extern "C"
}
}
