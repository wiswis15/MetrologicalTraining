#ifndef __REMOTECONTROL_IR__
#define __REMOTECONTROL_IR__

#include "Module.h"
#include <interfaces/IKeyHandler.h>
#include <refsw/nexus_config.h>
#include <refsw/nexus_platform.h>
#include <refsw/nxclient.h>
#include <refsw/nexus_input_client.h>

namespace WPEFramework {
	namespace Plugin {

		class IRRemote : public Exchange::IKeyProducer {
		private:
			IRRemote(const IRRemote&) = delete;
			IRRemote& operator=(const IRRemote&) = delete;

		public:
			IRRemote();
			virtual ~IRRemote();

			BEGIN_INTERFACE_MAP(IRRemote)
			INTERFACE_ENTRY(Exchange::IKeyProducer)
			END_INTERFACE_MAP

			void Initialize();
			void Deinitialize();

			virtual const TCHAR* Name() const
			{
				return (_resourceName.c_str());
			}

            virtual void CodeMask(uint32 mask)
            {
                _codeMask = mask;
            }

			virtual bool Pairing();
			virtual uint32 Callback(Exchange::IKeyHandler* callback);
			virtual uint32 Error() const;
			virtual string MetaData() const;

		private:
			static void NexusCallback(void* nexusIRHandle, int deviceID);
			void SendEvent();

		private:
			uint64 _lastKeyTicks;
			uint32 _error;
			Exchange::IKeyHandler* _callback;
            static const string _resourceName;
            NEXUS_InputClientHandle _NxInputClient;
			NxClient_AllocResults _allocResults;
            uint32 _codeMask;
		};
	}
}

#endif // __REMOTECONTROL_IR__
