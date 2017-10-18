#ifndef __CONTROLLER_H
#define __CONTROLLER_H

#include "Module.h"
#include "PluginServer.h"
#include "DownloadEngine.h"
#include "Probe.h"

namespace WPEFramework {
	namespace Plugin {

		class Controller : public PluginHost::IPlugin, public PluginHost::IWeb {
		private:
			class Downloader : public PluginHost::DownloadEngine {
			private:
				Downloader() = delete;
				Downloader(const Downloader&) = delete;
				Downloader& operator=(const Downloader&) = delete;

			public:
				Downloader(Controller& parent, const string& key)
					: PluginHost::DownloadEngine(key)
					, _parent(parent)
				{
				}
				virtual ~Downloader()
				{
				}

			private:
				virtual void Transfered(const uint32 result, const string& source, const string& destination) override
				{
					_parent.Transfered(result, source, destination);
				}

			private:
				Controller& _parent;
			};

			// GET -> URL /<MetaDataCallsign>/Plugin/<Callsign>
			// PUT -> URL /<MetaDataCallsign>/Configure
			// PUT -> URL /<MetaDataCallsign>/Activate/<Callsign>
			// PUT -> URL /<MetaDataCallsign>/Deactivate/<Callsign>
			// DELETE -> URL /<MetaDataCallsign>/Plugin/<Callsign>
			// PUT -> URL /<MetaDataCallsign>/Download
		public:
			class Config : public Core::JSON::Container {
			private:
				Config(const Config&) = delete;
				Config& operator=(const Config&) = delete;

			public:
				enum enumSource {
					Internal,
					External
				};

			public:
				Config()
					: Core::JSON::Container()
					, Location(Internal)
					, Identifier(Internal)
					, Time(Internal)
					, Hardware(Internal)
					, DownloadStore()
					, TTL(1)
					, Resumes()
				{
					Add(_T("location"), &Location);
					Add(_T("identifier"), &Identifier);
					Add(_T("time"), &Time);
					Add(_T("hardware"), &Hardware);
					Add(_T("downloadstore"), &DownloadStore);
					Add(_T("ttl"), &TTL);
					Add(_T("resumes"), &Resumes);
				}
				~Config() {
				}

			public:
				Core::JSON::EnumType<enumSource> Location;
				Core::JSON::EnumType<enumSource> Identifier;
				Core::JSON::EnumType<enumSource> Time;
				Core::JSON::EnumType<enumSource> Hardware;
				Core::JSON::String DownloadStore;
				Core::JSON::DecUInt8 TTL;
				Core::JSON::ArrayType<Core::JSON::String> Resumes;
			};
			class Download : public Core::JSON::Container {
			private:
				Download(const Download&);
				Download& operator=(const Download&);

			public:
				Download()
					: Core::JSON::Container()
				{
					Add(_T("source"), &Source);
					Add(_T("destination"), &Destination);
					Add(_T("hash"), &Hash);
				}
				~Download()
				{
				}

			public:
				Core::JSON::String Source;
				Core::JSON::String Destination;
				Core::JSON::String Hash;
			};

		private:
			Controller(const Controller&);
			Controller& operator=(const Controller&);

		protected:
			Controller()
				: _skipURL(0)
				, _webPath()
				, _pluginServer(nullptr)
				, _service(nullptr)
				, _systemInfo(nullptr)
				, _downloader(nullptr)
				, _probe(nullptr)
				, _resumes()
			{
			}

		public:
			virtual ~Controller()
			{
				SetServer(nullptr);
			}
			inline void SetServer(PluginHost::Server* pluginServer)
			{
				ASSERT((_pluginServer == nullptr) ^ (pluginServer == nullptr));

				_pluginServer = pluginServer;
			}
			inline uint32 Started()
			{
				return (ResumeRequested());
			}
			inline uint32 Stopped()
			{
				return (Core::ERROR_NONE);
			}

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

			// The plugin is unloaded from the webbridge. This is call allows the module to notify clients
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

			// If everything is received correctly, the request is passed to us, on a thread from the thread pool, to
			// do our thing and to return the result in the response object. Here the actual specific module work,
			// based on a a request is handled.
			virtual Core::ProxyType<Web::Response> Process(const Web::Request& request);

			//  IUnknown methods
			// -------------------------------------------------------------------------------------------------------
			BEGIN_INTERFACE_MAP(Controller)
				INTERFACE_ENTRY(PluginHost::IPlugin)
				INTERFACE_ENTRY(PluginHost::IWeb)
				INTERFACE_AGGREGATE(PluginHost::ISystemInfo, _systemInfo)
			END_INTERFACE_MAP

		private:
			uint32 ResumeRequested();
			Core::ProxyType<Web::Response> GetMethod(Core::TextSegmentIterator& index) const;
			Core::ProxyType<Web::Response> PutMethod(Core::TextSegmentIterator& index, const Web::Request& request);
			Core::ProxyType<Web::Response> DeleteMethod(Core::TextSegmentIterator& index, const Web::Request& request);
			void Transfered(const uint32 result, const string& source, const string& destination);
			virtual void Connectivity(const Core::NodeId& externalIP);

		private:
			uint8 _skipURL;
			string _webPath;
			PluginHost::Server* _pluginServer;
			PluginHost::IShell* _service;
			PluginHost::ISystemInfo* _systemInfo;
			Downloader* _downloader;
			Probe* _probe;
			std::list<string> _resumes;
		};
	}
}

#endif // __CONTROLLER_H
