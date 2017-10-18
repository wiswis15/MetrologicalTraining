#include "Module.h"

namespace WPEFramework {

namespace ProxyStubs {

    using namespace PluginHost;

    // -------------------------------------------------------------------------------------------
    // STUB
    // -------------------------------------------------------------------------------------------
    ProxyStub::MethodHandler PluginStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>& channel, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual const string Initialize (IShell* service) = 0;
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Output& response(message->Response());
            RPC::Data::Frame::Reader reader(parameters.Reader());
            RPC::Data::Frame::Writer writer(response.Writer());

			IPlugin* implementation(parameters.Implementation<IPlugin>());
            string result(_T("Implementation of ProxyStub NOT supplied."));

            ASSERT(implementation != nullptr);

            if (implementation != nullptr) {
                IShell* proxy = RPC::Administrator::Instance().CreateProxy<IShell>(channel, reader.Number<IShell*>(), false, false);

                result = (implementation->Initialize(proxy));

                if (proxy != nullptr) {
                    proxy->Release();
                }
            }

            writer.Text(result);

        },
        [](Core::ProxyType<Core::IPCChannel>& channel, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual void Deinitialize (IShell* service) = 0;
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());

            IShell* proxy = RPC::Administrator::Instance().CreateProxy<IShell>(channel, parameters.Number<IShell*>(), false, false);

            message->Parameters().Implementation<IPlugin>()->Deinitialize(proxy);

            proxy->Release();
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual string Information () const = 0;
            RPC::Data::Frame::Writer response(message->Response().Writer());

            response.Text(message->Parameters().Implementation<IPlugin>()->Information());
        },
        nullptr
    };

    ProxyStub::MethodHandler ShellStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            //virtual void EnableWebServer(const string& URLPath, const string& fileSystemPath) = 0;
            RPC::Data::Frame::Reader reader(message->Parameters().Reader());
            string URLPath(reader.Text());
            string fileSystemPath(reader.Text());

            message->Parameters().Implementation<IShell>()->EnableWebServer(URLPath, fileSystemPath);
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            //virtual void DisableWebServer() = 0;
            message->Parameters().Implementation<IShell>()->DisableWebServer();
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            //virtual string ConfigLine() const = 0;
            RPC::Data::Frame::Writer response(message->Response().Writer());

            response.Text(message->Parameters().Implementation<IShell>()->ConfigLine());
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            //virtual string Version () const = 0;
            RPC::Data::Frame::Writer response(message->Response().Writer());

            response.Text(message->Parameters().Implementation<IShell>()->Version());
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            //virtual bool Background () const = 0;
            RPC::Data::Frame::Writer response(message->Response().Writer());

            response.Boolean(message->Parameters().Implementation<IShell>()->Background());
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            //virtual string Accessor() const = 0;
            RPC::Data::Frame::Writer response(message->Response().Writer());

            response.Text(message->Parameters().Implementation<IShell>()->Accessor());
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            //virtual string WebPrefix() const = 0;
            RPC::Data::Frame::Writer response(message->Response().Writer());

            response.Text(message->Parameters().Implementation<IShell>()->WebPrefix());
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            //virtual string Locator() const = 0;
            RPC::Data::Frame::Writer response(message->Response().Writer());

            response.Text(message->Parameters().Implementation<IShell>()->Locator());
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            //virtual string Callsign() const = 0;
            RPC::Data::Frame::Writer response(message->Response().Writer());

            response.Text(message->Parameters().Implementation<IShell>()->Callsign());
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            //virtual string PersistentPath() const = 0;
            RPC::Data::Frame::Writer response(message->Response().Writer());

            response.Text(message->Parameters().Implementation<IShell>()->PersistentPath());
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            //virtual string DataPath() const = 0;
            RPC::Data::Frame::Writer response(message->Response().Writer());

            response.Text(message->Parameters().Implementation<IShell>()->DataPath());
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            //virtual string HashKey() const = 0;
            RPC::Data::Frame::Writer response(message->Response().Writer());

            response.Text(message->Parameters().Implementation<IShell>()->HashKey());
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            //virtual bool AutoStart() const = 0;
            RPC::Data::Frame::Writer response(message->Response().Writer());

            response.Boolean(message->Parameters().Implementation<IShell>()->AutoStart());
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual void Notify(const string& message) = 0;
            RPC::Data::Frame::Reader reader(message->Parameters().Reader());

            message->Parameters().Implementation<IShell>()->Notify(reader.Text());
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            //virtual void* QueryInterfaceByCallsign(const uint32 id, const string& name) = 0;
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());

            uint32 id(parameters.Number<uint32>());
            string name(parameters.Text());

            response.Number<void*>(message->Parameters().Implementation<IShell>()->QueryInterfaceByCallsign(id, name));
        },
        [](Core::ProxyType<Core::IPCChannel>& channel, Core::ProxyType<RPC::InvokeMessage>& message) {
			//virtual void Register (Plugin::IPluginNotification*) = 0;
			RPC::Data::Input& parameters(message->Parameters());
			RPC::Data::Frame::Reader reader(parameters.Reader());

			IPlugin::INotification* proxy = RPC::Administrator::Instance().CreateProxy<IPlugin::INotification>(channel, reader.Number<IPlugin::INotification*>(), true, false);

			ASSERT((proxy != nullptr) && "Failed to create proxy");

			if (proxy == nullptr) {
				TRACE_L1(_T("Could not create a stub for Plugin::IPluginNotification: %d"), IPlugin::INotification::ID);
			}
			else {
				parameters.Implementation<IShell>()->Register(proxy);
				if (proxy->Release() != Core::ERROR_NONE) {
					TRACE_L1("Oops seems like we did not maintain a reference to this sink. %d", __LINE__);
				}
			}
		},
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            //virtual void Unregister (IPluginNotification*) = 0;
			RPC::Data::Input& parameters(message->Parameters());
			RPC::Data::Frame::Reader reader(parameters.Reader());

			// Need to find the proxy that goes with the given implementation..
			IPlugin::INotification* stub = reader.Number<IPlugin::INotification*>();

			// NOTE: FindProxy does *NOT* AddRef the result. Do not release what is obtained via FindProxy..
			IPlugin::INotification* proxy = RPC::Administrator::Instance().FindProxy<IPlugin::INotification>(stub);

			if (proxy == nullptr) {
				TRACE_L1(_T("Coud not find stub for Plugin::IPluginNotification: %p"), stub);
			}
			else {
				parameters.Implementation<IShell>()->Unregister(proxy);
			}
		},
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            //virtual void* Instantiate (const uint32 waitTime, const string& className, const uint32 interfaceId, const uint32 version, uint32* pid, const string& locator) = 0;
            RPC::Data::Frame::Reader reader(message->Parameters().Reader());
            RPC::Data::Frame::Writer writer(message->Response().Writer());

            uint32 pid = 0;
            uint32 waitTime(reader.Number<uint32>());
            string className(reader.Text());
            uint32 interfaceId(reader.Number<uint32>());
            uint32 version(reader.Number<uint32>());
            string locator(reader.Text());

            writer.Number<void*>(message->Parameters().Implementation<IShell>()->Instantiate(waitTime, className, interfaceId, version, pid, locator));
            writer.Number<uint32>(pid);
        },
		[](Core::ProxyType<Core::IPCChannel>& channel, Core::ProxyType<RPC::InvokeMessage>& message) {
			//virtual void Register (RPC::IRemoteProcess::INotification*) = 0;
			RPC::Data::Input& parameters(message->Parameters());
			RPC::Data::Frame::Reader reader(parameters.Reader());

			RPC::IRemoteProcess::INotification* proxy = RPC::Administrator::Instance().CreateProxy<RPC::IRemoteProcess::INotification>(channel, reader.Number<RPC::IRemoteProcess::INotification*>(), true, false);

			ASSERT((proxy != nullptr) && "Failed to create proxy");

			if (proxy == nullptr) {
				TRACE_L1(_T("Could not create a stub for RPC::IRemoteProcess::INotification: %d"), RPC::IRemoteProcess::INotification::ID);
			}
			else {
				parameters.Implementation<IShell>()->Register(proxy);
				if (proxy->Release() != Core::ERROR_NONE) {
					TRACE_L1("Oops seems like we did not maintain a reference to this sink. %d", __LINE__);
				}
			}
		},
		[](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
			//virtual void Unregister (RPC::IRemoteProcess::INotification*) = 0;
			RPC::Data::Input& parameters(message->Parameters());
			RPC::Data::Frame::Reader reader(parameters.Reader());

			// Need to find the proxy that goes with the given implementation..
			RPC::IRemoteProcess::INotification* stub = reader.Number<RPC::IRemoteProcess::INotification*>();

			// NOTE: FindProxy does *NOT* AddRef the result. Do not release what is obtained via FindProxy..
			RPC::IRemoteProcess::INotification* proxy = RPC::Administrator::Instance().FindProxy<RPC::IRemoteProcess::INotification>(stub);

			if (proxy == nullptr) {
				TRACE_L1(_T("Coud not find stub for RPC::IRemoteProcess::INotification: %p"), stub);
			}
			else {
				parameters.Implementation<IShell>()->Unregister(proxy);
			}
		},
		[](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            // RPC::IRemoteProcess* RemoteProcess(uint32 pid)
            RPC::Data::Frame::Reader reader(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());

            response.Number<Core::IUnknown*>(
                message->Parameters().Implementation<IShell>()->RemoteProcess(reader.Number<uint32>()));
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Frame::Reader reader(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());

            //virtual uint32 Activate (reason, detached) = 0;
            IShell::reason reason(reader.Number<IShell::reason>());

            response.Number<uint32>(message->Parameters().Implementation<IShell>()->Activate(reason));
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Frame::Reader reader(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());

            //virtual uint32 Deactivate(reason, detached) = 0;
            IShell::reason reason(reader.Number<IShell::reason>());

            response.Number<uint32>(message->Parameters().Implementation<IShell>()->Deactivate(reason));
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Frame::Writer response(message->Response().Writer());
            //virtual state State() = 0;
            response.Number<IShell::state>(message->Parameters().Implementation<IShell>()->State());
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Frame::Writer response(message->Response().Writer());
            //virtual state Reason() = 0;
            response.Number<IShell::reason>(message->Parameters().Implementation<IShell>()->Reason());
        },
        [](Core::ProxyType<Core::IPCChannel>& /* channel */, Core::ProxyType<RPC::InvokeMessage>& message) {
            //virtual string Model() const = 0;
            RPC::Data::Frame::Writer response(message->Response().Writer());

            response.Text(message->Parameters().Implementation<IShell>()->Model());
        },
        nullptr
    };

	ProxyStub::MethodHandler StateControlStubMethods[] = {
		[](Core::ProxyType<Core::IPCChannel>& channel, Core::ProxyType<RPC::InvokeMessage>& message) {

			// virtual void Configure (IShell* service) = 0;
			RPC::Data::Input& parameters(message->Parameters());
			RPC::Data::Frame::Reader reader(parameters.Reader());
			RPC::Data::Frame::Writer writer(message->Response().Writer());

			IShell* proxy = RPC::Administrator::Instance().CreateProxy<IShell>(channel, reader.Number<IShell*>(), false, false);

			writer.Number<uint32>(message->Parameters().Implementation<IStateControl>()->Configure(proxy));

			if (proxy != nullptr) {
				proxy->Release();
			}
		},
			[](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {

			// virtual IStateControl::state State() const = 0;
			RPC::Data::Frame::Writer response(message->Response().Writer());
			response.Number<IStateControl::state>(message->Parameters().Implementation<IStateControl>()->State());
		},
			[](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {

			// virtual void Request(const IstateControl::Command command) = 0;
			RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
			RPC::Data::Frame::Writer response(message->Response().Writer());
			IStateControl::command command = parameters.Number<IStateControl::command>();
			response.Number(message->Parameters().Implementation<IStateControl>()->Request(command));
		},
			[](Core::ProxyType<Core::IPCChannel>& channel, Core::ProxyType<RPC::InvokeMessage>& message) {
			RPC::Data::Input& parameters(message->Parameters());
			RPC::Data::Frame::Reader reader(parameters.Reader());

			IStateControl::INotification* implementation = reader.Number<IStateControl::INotification*>();
			IStateControl::INotification* proxy = RPC::Administrator::Instance().CreateProxy<IStateControl::INotification>(channel, implementation, true, false);

			ASSERT((proxy != nullptr) && "Failed to create proxy");

			if (proxy == nullptr) {
				TRACE_L1(_T("Could not create a stub for IStateControlNotification: %p"), implementation);
			}
			else {
				parameters.Implementation<IStateControl>()->Register(proxy);
				if (proxy->Release() != Core::ERROR_NONE) {
					TRACE_L1("Oops seems like we did not maintain a reference to this sink. %d", __LINE__);
				}
			}
		},
		[](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
			RPC::Data::Input& parameters(message->Parameters());
			RPC::Data::Frame::Reader reader(parameters.Reader());

			// Need to find the proxy that goes with the given implementation..
			IStateControl::INotification* stub = reader.Number<IStateControl::INotification*>();

			// NOTE: FindProxy does *NOT* AddRef the result. Do not release what is obtained via FindProxy..
			IStateControl::INotification* proxy = RPC::Administrator::Instance().FindProxy<IStateControl::INotification>(stub);

			if (proxy == nullptr) {
				TRACE_L1(_T("Coud not find stub for IBrowserNotification: %p"), stub);
			}
			else {
				parameters.Implementation<IStateControl>()->Unregister(proxy);
			}
		},
		nullptr
	};

	ProxyStub::MethodHandler StateControlNotificationStubMethods[] = {
		[](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {

			// virtual void StateChange (const uint32 newState) = 0;
			RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
			IStateControl::state newState(parameters.Number<IStateControl::state>());

			message->Parameters().Implementation<IStateControl::INotification>()->StateChange(newState);
		},
		nullptr
	};

	ProxyStub::MethodHandler StringIteratorStubMethods[] = {
		[](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {

			// virtual bool IsValid() const = 0;
			RPC::Data::Frame::Writer response(message->Response().Writer());
			response.Boolean(message->Parameters().Implementation<IStringIterator>()->IsValid());
		},
		[](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {

			// virtual void Reset() = 0;
			message->Parameters().Implementation<IStringIterator>()->Reset();
		},
		[](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {

			// virtual bool Next(string& result) = 0;
			RPC::Data::Frame::Writer response(message->Response().Writer());
			string result;
			bool valid = message->Parameters().Implementation<IStringIterator>()->Next(result);
			response.Boolean(valid);
			if (valid == true) {
				response.Text(result);
			}
		},
		nullptr
	};

	ProxyStub::MethodHandler ValueIteratorStubMethods[] = {
		[](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {

			// virtual bool IsValid() const = 0;
			RPC::Data::Frame::Writer response(message->Response().Writer());
			response.Boolean(message->Parameters().Implementation<IValueIterator>()->IsValid());
		},
		[](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {

			// virtual void Reset() = 0;
			message->Parameters().Implementation<IValueIterator>()->Reset();
		},
		[](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {

			// virtual bool Next(string& result) = 0;
			RPC::Data::Frame::Writer response(message->Response().Writer());
			uint32 result;
			bool valid = message->Parameters().Implementation<IValueIterator>()->Next(result);
			response.Boolean(valid);
			if (valid == true) {
				response.Number(result);
			}
		},
		nullptr
	};

	typedef ProxyStub::StubType<IPlugin, PluginStubMethods, ProxyStub::UnknownStub> PluginStub;
    typedef ProxyStub::StubType<IShell, ShellStubMethods, ProxyStub::UnknownStub> ShellStub;
	typedef ProxyStub::StubType<IStateControl, StateControlStubMethods, ProxyStub::UnknownStub> StateControlStub;
	typedef ProxyStub::StubType<IStateControl::INotification, StateControlNotificationStubMethods, ProxyStub::UnknownStub> StateControlNotificationStub;
	typedef ProxyStub::StubType<IStringIterator, StringIteratorStubMethods, ProxyStub::UnknownStub> StringIteratorStub;
	typedef ProxyStub::StubType<IValueIterator, ValueIteratorStubMethods, ProxyStub::UnknownStub> ValueIteratorStub;

    // -------------------------------------------------------------------------------------------
    // PROXY
    // -------------------------------------------------------------------------------------------
    class PluginProxy : public ProxyStub::UnknownProxyType<IPlugin> {
    public:
        PluginProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~PluginProxy()
        {
        }

    public:
        virtual const string Initialize(IShell* service)
        {
            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<IShell*>(service);

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Text());
        }
        virtual void Deinitialize(IShell* service)
        {
            IPCMessage newMessage(BaseClass::Message(1));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<IShell*>(service);

            Invoke(newMessage);
        }
        virtual string Information() const
        {
            IPCMessage newMessage(BaseClass::Message(2));

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Text());
        }
    };

    class ShellProxy : public ProxyStub::UnknownProxyType<IShell> {
    public:
        ShellProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~ShellProxy()
        {
        }

    public:
        virtual void EnableWebServer(const string& URLPath, const string& fileSystemPath)
        {
            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());

            writer.Text(URLPath);
            writer.Text(fileSystemPath);

            Invoke(newMessage);
        }
        virtual void DisableWebServer()
        {
            IPCMessage newMessage(BaseClass::Message(1));

            Invoke(newMessage);
        }
        virtual string ConfigLine() const
        {
            IPCMessage newMessage(BaseClass::Message(2));

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Text());
        }
        virtual string Version() const
        {
            IPCMessage newMessage(BaseClass::Message(3));

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Text());
        }
        virtual bool Background() const
        {
            IPCMessage newMessage(BaseClass::Message(4));

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Boolean());
        }
        virtual string Accessor() const
        {
            IPCMessage newMessage(BaseClass::Message(5));

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Text());
        }
        virtual string WebPrefix() const
        {
            IPCMessage newMessage(BaseClass::Message(6));

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Text());
        }
        virtual string Locator() const
        {
            IPCMessage newMessage(BaseClass::Message(7));

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Text());
        }
        virtual string Callsign() const
        {
            IPCMessage newMessage(BaseClass::Message(8));

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Text());
        }
        virtual string PersistentPath() const
        {
            IPCMessage newMessage(BaseClass::Message(9));

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Text());
        }
        virtual string DataPath() const
        {
            IPCMessage newMessage(BaseClass::Message(10));

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Text());
        }
        virtual string HashKey() const
        {
            IPCMessage newMessage(BaseClass::Message(11));

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Text());
        }
        virtual bool AutoStart() const
        {
            IPCMessage newMessage(BaseClass::Message(12));

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Boolean());
        }
        virtual void Notify(const string& message)
        {
            IPCMessage newMessage(BaseClass::Message(13));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());

            writer.Text(message);

            Invoke(newMessage);
        }
        virtual void* QueryInterfaceByCallsign(const uint32 id, const string& name)
        {
            IPCMessage newMessage(BaseClass::Message(14));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());

            writer.Number(id);
            writer.Text(name);

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (CreateProxy(reader.Number<Core::IUnknown*>(), id));
        }
        virtual void Register(IPlugin::INotification* sink)
        {
            IPCMessage newMessage(BaseClass::Message(15));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<IPlugin::INotification*>(sink);

            Invoke(newMessage);
        }
        virtual void Unregister(IPlugin::INotification* sink)
        {
            IPCMessage newMessage(BaseClass::Message(16));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<IPlugin::INotification*>(sink);

            Invoke(newMessage);
        }
        virtual void* Instantiate(const uint32 waitTime, const string& className, const uint32 interfaceId, const uint32 version, uint32& pid, const string& locator)
        {
            IPCMessage newMessage(BaseClass::Message(17));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());

            writer.Number<uint32>(waitTime);
            writer.Text(className);
            writer.Number<uint32>(interfaceId);
            writer.Number<uint32>(version);
            writer.Text(locator);

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
            Core::IUnknown* result = reader.Number<Core::IUnknown*>();
            pid = reader.Number<uint32>();

            return (CreateProxy(result, interfaceId));
        }
		virtual void Register(RPC::IRemoteProcess::INotification* sink)
		{
			IPCMessage newMessage(BaseClass::Message(18));
			RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
			writer.Number<RPC::IRemoteProcess::INotification*>(sink);

			Invoke(newMessage);
		}
		virtual void Unregister(RPC::IRemoteProcess::INotification* sink)
		{
			IPCMessage newMessage(BaseClass::Message(19));
			RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
			writer.Number<RPC::IRemoteProcess::INotification*>(sink);

			Invoke(newMessage);
		}
		virtual RPC::IRemoteProcess* RemoteProcess(const uint32 pid)
		{
			IPCMessage newMessage(BaseClass::Message(20));
			RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());

			writer.Number<uint32>(pid);

			Invoke(newMessage);

			RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

			return (CreateProxy<RPC::IRemoteProcess>(reader.Number<Core::IUnknown*>()));
		}
		virtual uint32 Activate(const IShell::reason theReason)
        {
            IPCMessage newMessage(BaseClass::Message(21));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());

            writer.Number<IShell::reason>(theReason);
            Invoke(newMessage);

            return (newMessage->Response().Reader().Number<uint32>());
        }
        virtual uint32 Deactivate(const IShell::reason theReason)
        {
            IPCMessage newMessage(BaseClass::Message(22));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());

            writer.Number<IShell::reason>(theReason);
            Invoke(newMessage);

            return (newMessage->Response().Reader().Number<uint32>());
        }
        virtual IShell::state State() const
        {
            IPCMessage newMessage(BaseClass::Message(23));
            Invoke(newMessage);

            return (newMessage->Response().Reader().Number<IShell::state>());
        }
        virtual IShell::reason Reason() const
        {
            IPCMessage newMessage(BaseClass::Message(24));
            Invoke(newMessage);

            return (newMessage->Response().Reader().Number<IShell::reason>());
        }
        virtual string Model() const
        {
            IPCMessage newMessage(BaseClass::Message(25));

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Text());
        }
    };

	class StateControlProxy : public ProxyStub::UnknownProxyType<IStateControl> {
	public:
		StateControlProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
			: BaseClass(channel, implementation, otherSideInformed)
		{
		}
		virtual ~StateControlProxy()
		{
		}

	public:
		virtual uint32 Configure(IShell* service)
		{
			IPCMessage newMessage(BaseClass::Message(0));
			RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
			writer.Number<IShell*>(service);
			uint32 result = Invoke(newMessage);

			RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
			return (result == Core::ERROR_NONE ? reader.Number<uint32>() : Core::ERROR_RPC_CALL_FAILED);
		}
		virtual IStateControl::state State() const
		{
			IPCMessage newMessage(BaseClass::Message(1));
			Invoke(newMessage);
			RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
			return reader.Number<IStateControl::state>();
		}
		virtual uint32 Request(const IStateControl::command command)
		{
			IPCMessage newMessage(BaseClass::Message(2));
			RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
			writer.Number<IStateControl::command>(command);
			Invoke(newMessage);
			RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
			return reader.Number<uint32>();
		}
		virtual void Register(IStateControl::INotification* notification)
		{
			IPCMessage newMessage(BaseClass::Message(3));
			RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
			writer.Number<IStateControl::INotification*>(notification);
			Invoke(newMessage);
		}
		virtual void Unregister(IStateControl::INotification* notification)
		{
			IPCMessage newMessage(BaseClass::Message(4));
			RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
			writer.Number<IStateControl::INotification*>(notification);
			Invoke(newMessage);
		}
	};

	class StateControlNotificationProxy : public ProxyStub::UnknownProxyType<IStateControl::INotification> {
	public:
		StateControlNotificationProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
			: BaseClass(channel, implementation, otherSideInformed)
		{
		}
		virtual ~StateControlNotificationProxy()
		{
		}

	public:
		virtual void StateChange(const IStateControl::state newState)
		{
			IPCMessage newMessage(BaseClass::Message(0));
			RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
			writer.Number<IStateControl::state>(newState);
			Invoke(newMessage);
		}
	};

	class StringIteratorProxy : public ProxyStub::UnknownProxyType<IStringIterator> {
	public:
		StringIteratorProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
			: BaseClass(channel, implementation, otherSideInformed)
		{
		}
		virtual ~StringIteratorProxy()
		{
		}

	public:
		virtual bool IsValid() const
		{
			IPCMessage newMessage(BaseClass::Message(0));
			Invoke(newMessage);
			RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
			return reader.Boolean();

		}
		virtual void Reset()
		{
			IPCMessage newMessage(BaseClass::Message(1));
			Invoke(newMessage);
		}
		virtual bool Next(string& result)
		{
			IPCMessage newMessage(BaseClass::Message(2));
			Invoke(newMessage);
			RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
			if (reader.Boolean() == true) {
				result = reader.Text();
				return (true);
			}

			return (false);
		}
	};

	class ValueIteratorProxy : public ProxyStub::UnknownProxyType<IStringIterator> {
	public:
		ValueIteratorProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
			: BaseClass(channel, implementation, otherSideInformed)
		{
		}
		virtual ~ValueIteratorProxy()
		{
		}

	public:
		virtual bool IsValid() const
		{
			IPCMessage newMessage(BaseClass::Message(0));
			Invoke(newMessage);
			RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
			return reader.Boolean();

		}
		virtual void Reset()
		{
			IPCMessage newMessage(BaseClass::Message(1));
			Invoke(newMessage);
		}
		virtual bool Next(string& result)
		{
			IPCMessage newMessage(BaseClass::Message(2));
			Invoke(newMessage);
			RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
			if (reader.Boolean() == true) {
				result = reader.Text();
				return (true);
			}

			return (false);
		}
	};

	// -------------------------------------------------------------------------------------------
    // Registration
    // -------------------------------------------------------------------------------------------
    static class Instantiation {
    public:
        Instantiation()
        {
            RPC::Administrator::Instance().Announce<IPlugin, PluginProxy, PluginStub>();
            RPC::Administrator::Instance().Announce<IShell, ShellProxy, ShellStub>();
			RPC::Administrator::Instance().Announce<IStateControl, StateControlProxy, StateControlStub>();
			RPC::Administrator::Instance().Announce<IStateControl::INotification, StateControlNotificationProxy, StateControlNotificationStub>();
			RPC::Administrator::Instance().Announce<IStringIterator, StringIteratorProxy, StringIteratorStub>();
			RPC::Administrator::Instance().Announce<IValueIterator, ValueIteratorProxy, ValueIteratorStub>();
		}
        ~Instantiation()
        {
		}

    } ProxyStubRegistration;
}
}
