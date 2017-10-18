#include "MetaData.h"
#include "IStateControl.h"
#include "ISystemInfo.h"

#define __TXT(KeyWord) KeyWord, (sizeof(KeyWord) / sizeof(TCHAR)) - 1

namespace WPEFramework {

ENUM_CONVERSION_BEGIN(PluginHost::MetaData::Channel::state)

{ PluginHost::MetaData::Channel::WEBSERVER, _TXT("WebServer") },
{ PluginHost::MetaData::Channel::WEBSOCKET, _TXT("WebSocket") },
{ PluginHost::MetaData::Channel::RAWSOCKET, _TXT("RawSocket") },
{ PluginHost::MetaData::Channel::CLOSED,    _TXT("Closed")    },
{ PluginHost::MetaData::Channel::SUSPENDED, _TXT("Suspended") },

ENUM_CONVERSION_END(PluginHost::MetaData::Channel::state)

ENUM_CONVERSION_BEGIN(PluginHost::MetaData::Service::state)

	{ PluginHost::MetaData::Service::DEACTIVATED,  _TXT("deactivated")  },
	{ PluginHost::MetaData::Service::DEACTIVATION, _TXT("deactivation") },
	{ PluginHost::MetaData::Service::ACTIVATED,    _TXT("activated")    },
	{ PluginHost::MetaData::Service::ACTIVATION,   _TXT("activation")   },
	{ PluginHost::MetaData::Service::SUSPENDED,    _TXT("suspended")    },
	{ PluginHost::MetaData::Service::RESUMED,      _TXT("resumed")      },

ENUM_CONVERSION_END(PluginHost::MetaData::Service::state)

ENUM_CONVERSION_BEGIN(PluginHost::ISystemInfo::network_type)

{ PluginHost::ISystemInfo::UNKNOWN, _TXT("Unknown") },
{ PluginHost::ISystemInfo::IPV4,    _TXT("IPv4")    },
{ PluginHost::ISystemInfo::IPV6,    _TXT("IPv6")    },

ENUM_CONVERSION_END(PluginHost::ISystemInfo::network_type)

ENUM_CONVERSION_BEGIN(PluginHost::ISystemInfo::system_state)

	{ PluginHost::ISystemInfo::POWERON,      _TXT("PowerOn")      },
	{ PluginHost::ISystemInfo::INITIALIZING, _TXT("Initializing") },
	{ PluginHost::ISystemInfo::OPERATIONAL,  _TXT("Operational")  },

ENUM_CONVERSION_END(PluginHost::ISystemInfo::system_state)

	namespace PluginHost {

		/* static */ const TCHAR* ISystemInfo::ToString(const network_type value) {
			return (Core::EnumerateType<network_type>(value).Data());
		}

		/* static */ const TCHAR* ISystemInfo::ToString(const system_state value) {
			return (Core::EnumerateType<system_state>(value).Data());
		}

		MetaData::Service::State& MetaData::Service::State::operator= (const PluginHost::IShell* RHS)
		{
			Core::JSON::EnumType<state>::operator=(static_cast<state>(RHS->State()));

			if (RHS->State() == PluginHost::IShell::ACTIVATED) {
				// See if there is a state interface, if so, we are suspended or resumed :-)
				PluginHost::IStateControl* mode = const_cast<PluginHost::IShell*>(RHS)->QueryInterface<PluginHost::IStateControl>();

				if (mode != nullptr) {
					Core::JSON::EnumType<state>::operator=(mode->State() == PluginHost::IStateControl::RESUMED ? RESUMED : SUSPENDED);
					mode->Release();
				}
			}

			return (*this);
		}

		string MetaData::Service::State::Data() const
		{
			return (Core::JSON::EnumType<state>::Data());
		}

		MetaData::Channel::State& MetaData::Channel::State::operator= (const MetaData::Channel::state RHS)
		{
			Core::JSON::EnumType<state>::operator=(RHS);

			return (*this);
		}

		string MetaData::Channel::State::Data() const
		{
			return (Core::JSON::EnumType<state>::Data());
		}

		MetaData::Service::Service()
			: Plugin::Config()
		{
			Add(_T("state"), &JSONState);
#ifdef RUNTIME_STATISTICS
			Add(_T("processedrequests"), &ProcessedRequests);
			Add(_T("processedobjects"), &ProcessedObjects);
#endif
			Add(_T("observers"), &Observers);
			Add(_T("module"), &Module);
			Add(_T("hash"), &Hash);
		}
		MetaData::Service::Service(const MetaData::Service& copy)
			: Plugin::Config(copy)
			, JSONState(copy.JSONState)
#ifdef RUNTIME_STATISTICS
			, ProcessedRequests(copy.ProcessedRequests)
			, ProcessedObjects(copy.ProcessedObjects)
#endif
			, Observers(copy.Observers)
			, Module(copy.Module)
			, Hash(copy.Hash)
		{
			Add(_T("state"), &JSONState);
#ifdef RUNTIME_STATISTICS
			Add(_T("processedrequests"), &ProcessedRequests);
			Add(_T("processedobjects"), &ProcessedObjects);
#endif
			Add(_T("observers"), &Observers);
			Add(_T("module"), &Module);
			Add(_T("hash"), &Hash);
		}
		MetaData::Service::~Service()
		{
		}

		MetaData::Service& MetaData::Service::operator=(const Plugin::Config& RHS)
		{
			Plugin::Config::operator=(RHS);
			return (*this);
		}

		MetaData::Channel::Channel()
                    : Core::JSON::Container()
		{
			Core::JSON::Container::Add(_T("remote"), &Remote);
			Core::JSON::Container::Add(_T("state"), &JSONState);
			Core::JSON::Container::Add(_T("activity"), &Activity);
			Core::JSON::Container::Add(_T("id"), &ID);
			Core::JSON::Container::Add(_T("name"), &Name);
		}
		MetaData::Channel::Channel(const MetaData::Channel& copy)
                        : Core::JSON::Container()
			, Remote(copy.Remote)
			, JSONState(copy.JSONState)
			, Activity(copy.Activity)
			, ID(copy.ID)
			, Name(copy.Name) {
			Core::JSON::Container::Add(_T("remote"), &Remote);
			Core::JSON::Container::Add(_T("state"), &JSONState); 
			Core::JSON::Container::Add(_T("activity"), &Activity);
			Core::JSON::Container::Add(_T("id"), &ID);
			Core::JSON::Container::Add(_T("name"), &Name);
		}
		MetaData::Channel::~Channel() {
		}

		MetaData::Channel& MetaData::Channel::operator=(const MetaData::Channel& RHS) {
			Remote = RHS.Remote;
			JSONState = RHS.JSONState;
			Activity = RHS.Activity;
			ID = RHS.ID;
			Name = RHS.Name;

			return (*this);
		}

		MetaData::Bridge::Bridge()
                        : Core::JSON::Container() {
			Add(_T("locator"), &Locator);
			Add(_T("latency"), &Latency);
			Add(_T("model"), &Model);
			Add(_T("secure"), &Secure);
		}
		MetaData::Bridge::Bridge(const string& text, const uint32 latency, const string& model, const bool secure)
                        : Core::JSON::Container() {
			Add(_T("locator"), &Locator);
			Add(_T("latency"), &Latency);
			Add(_T("model"), &Model);
			Add(_T("secure"), &Secure);

			Locator = text;
			Latency = latency;
                        Secure = secure;
                        if (model.empty() == false)
                        {
                            Model = model;
                        }
		}
		MetaData::Bridge::Bridge(const Bridge& copy) 
                        : Core::JSON::Container()
			, Locator (copy.Locator)
			, Latency (copy.Latency)
			, Model (copy.Model)
			, Secure (copy.Secure) {
			Add(_T("locator"), &Locator);
			Add(_T("latency"), &Latency);
			Add(_T("model"), &Model);
			Add(_T("secure"), &Secure);
		}
		MetaData::Bridge::~Bridge() {
		}

		MetaData::Server::Server() {
			Core::JSON::Container::Add(_T("threads"), &ThreadPoolRuns);
			Core::JSON::Container::Add(_T("pending"), &PendingRequests);
			Core::JSON::Container::Add(_T("occupation"), &PoolOccupation);
		}
		MetaData::Server::~Server() {
		}

		MetaData::MetaData() {
			Core::JSON::Container::Add(_T("plugins"), &Plugins);
			Core::JSON::Container::Add(_T("channel"), &Channels);
			Core::JSON::Container::Add(_T("server"), &Process);
			Core::JSON::Container::Add(_T("bridges"), &Bridges);
			Core::JSON::Container::Add(_T("value"), &Value);
		}
		MetaData::~MetaData() {
		}

	}
}

