#ifndef __IWEBBRDIGE_H
#define __IWEBBRDIGE_H

#include "Module.h"
#include "IPlugin.h"

namespace WPEFramework {
namespace PluginHost {

    struct EXTERNAL IShell : virtual public Core::IUnknown {

        enum { ID = 0x00000029 };

        virtual ~IShell() {}

        // State of the IPlugin interface associated with this shell.
        enum state {
            DEACTIVATED,
            DEACTIVATION,
            ACTIVATED,
            ACTIVATION,
            DESTROYED
        };

		static const TCHAR* ToString(const state value);

        enum reason {
            REQUESTED,
            AUTOMATIC,
            FAILURE,
            MEMORY_EXCEEDED,
            STARTUP,
            SHUTDOWN
        };

		static const TCHAR* ToString(const reason value);

        class EXTERNAL Job : public Core::IDispatchType<void> {
        private:
            Job() = delete;
            Job(const Job&) = delete;
            Job& operator=(const Job&) = delete;

        public:
            Job(IShell* shell, IShell::state toState, IShell::reason why)
                : _shell(shell)
                , _state(toState)
                , _reason(why)
            {
				if (shell != nullptr) {
					shell->AddRef();
				}
            }
            virtual ~Job()
            {
                if (_shell != nullptr) {
                    _shell->Release();
                }
            }

        public:
            static Core::ProxyType<Core::IDispatchType<void> > Create(IShell* shell, IShell::state toState, IShell::reason why);

            virtual void Dispatch()
            {
                ASSERT(_shell != nullptr);

                if (_shell != nullptr) {

                    switch (_state) {
                    case ACTIVATED:
                        _shell->Activate(_reason);
                        break;
                    case DEACTIVATED:
                        _shell->Deactivate(_reason);
                        break;
                    default:
                        ASSERT(false);
                        break;
                    }
                }
            }

        private:
            IShell* _shell;
            state _state;
            reason _reason;
        };

        //! @{
        //! =========================== ACCESSOR TO THE SHELL AROUND THE PLUGIN ===============================
        //! This interface allows access to the shell that scontrolls the lifetimeof the Plugin. It's access
        //! and responses is based on configuration applicable to the plugin e.g. enabling a WebServer is
        //! enabling a webserver on a path, applicable to the plugin being initialized..
        //! The full network URL looks like: http://<accessor>/<webprefix>/<callsign> for accessing the plugin.
        //!
        //! Methods to enable/disable a webserver. URLPath is "added" to the end of the path that reaches this
        //! plugin, based on the DataPath.
        //! URL: http://<accessor>/<webprefix>/<callsign>/<URLPath> for accessing the webpages on
        //! <DataPath>/<fileSystemPath>.
        //! @}
        virtual void EnableWebServer(const string& URLPath, const string& fileSystemPath) = 0;
        virtual void DisableWebServer() = 0;

        //! Version: Returns a Human Readable version as indicated in the configuration.
        virtual string Version () const = 0;

        //! Version: Returns a Human Readable name for the platform it is running on.
        virtual string Model() const = 0;

        //! Background: If enabled, the PluginHost is running in daemon mode
        virtual bool Background() const = 0;

        //! Accessor: Identifier that can be used for Core:NodeId to connect to the webbridge.
        virtual string Accessor() const = 0;

        //! WebPrefix: First part of the pathname in the HTTP request to select the webbridge components.
        virtual string WebPrefix() const = 0;

        //! Callsign: Instantiation name of this specific plugin. It is the name given in the config for the classname.
        virtual string Locator() const = 0;

        //! Callsign: Instantiation name of this specific plugin. It is the name given in the config for the classname.
        virtual string Callsign() const = 0;

        //! PersistentPath: <config:persistentpath>/<plugin:callsign>/
        virtual string PersistentPath() const = 0;

        //! DataPath: <config:datapath>/<plugin:classname>/
        virtual string DataPath() const = 0;

        //! AutoStart: boolean to inidcate wheter we need to start up this plugin at start
        virtual bool AutoStart() const = 0;

        virtual string HashKey() const = 0;
        virtual string ConfigLine() const = 0;

        // Notify all subscribers of this service with the given string.
        // It is expected to be JSON formatted strings as it is assumed that this is for reaching websockets clients living in
        // the web world that have build in functionality to parse JSON structs.
        virtual void Notify(const string& message) = 0;

        // Use the base framework (webbridge) to start/stop processes and the service in side of the given binary.
        virtual void Register(RPC::IRemoteProcess::INotification* sink) = 0;
        virtual void Unregister(RPC::IRemoteProcess::INotification* sink) = 0;
        virtual void* Instantiate(const uint32 waitTime, const string& className, const uint32 interfaceId, const uint32 version, uint32& pid, const string& locator) = 0;
        virtual RPC::IRemoteProcess* RemoteProcess(const uint32 pid) = 0;

        // Allow access to the Shells, configured for the different Plugins found in the configuration.
        // Calling the QueryInterfaceByCallsign with an empty callsign will query for interfaces located
        // on the controller. 
        virtual void Register(IPlugin::INotification* sink) = 0;
        virtual void Unregister(IPlugin::INotification* sink) = 0;
        virtual state State() const = 0;
        virtual void* QueryInterfaceByCallsign(const uint32 id, const string& name) = 0;
        
        // Methods to Activate and Deactivate the aggregated Plugin to this shell.
        // NOTE: These are Blocking calls!!!!!
        virtual uint32 Activate(const reason) = 0;
        virtual uint32 Deactivate(const reason) = 0;
        virtual reason Reason() const = 0;

        template <typename REQUESTEDINTERFACE>
        REQUESTEDINTERFACE* Instantiate(const uint32 waitTime, const string& className, const uint32 version, uint32& pid, const string& locator)
        {
            void* baseInterface(Instantiate(waitTime, className, REQUESTEDINTERFACE::ID, version, pid, locator));

            if (baseInterface != NULL) {
                return (reinterpret_cast<REQUESTEDINTERFACE*>(baseInterface));
            }

            return (nullptr);
        }
        
        template <typename REQUESTEDINTERFACE>
        REQUESTEDINTERFACE* QueryInterfaceByCallsign(const string& name)
        {
            void* baseInterface(QueryInterfaceByCallsign(REQUESTEDINTERFACE::ID, name));

            if (baseInterface != NULL) {
                return (reinterpret_cast<REQUESTEDINTERFACE*>(baseInterface));
            }

            return (nullptr);
        }
    };
}
}

#endif // __IWEBBRIDGE_H
