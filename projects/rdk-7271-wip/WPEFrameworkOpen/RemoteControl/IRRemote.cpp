#include "IRRemote.h"
#include "RemoteAdministrator.h"

#include <linux/uinput.h>

extern int getIRHandle();

namespace WPEFramework {
namespace Plugin {

	/* static */ const string IRRemote::_resourceName(_T("NexusIR"));
	static IRRemote* _singleton(Core::Service<IRRemote>::Create<IRRemote>());

	IRRemote::IRRemote()
		: _lastKeyTicks(0)
		, _error(Core::ERROR_UNAVAILABLE)
		, _callback(nullptr)
		, _NxInputClient(nullptr)
		, _allocResults()
        , _codeMask(static_cast<uint32>(~0))
    {

		Remotes::RemoteAdministrator::Instance().Announce(*this);
	}

	/* virtual */ IRRemote::~IRRemote() {

		Remotes::RemoteAdministrator::Instance().Revoke(*this);
	}

	void IRRemote::Initialize()
	{
		NEXUS_Error rc = NEXUS_SUCCESS;
		DEBUG_VARIABLE(rc);

		NxClient_AllocSettings allocSettings;
		NEXUS_InputClientSettings settings;

		rc = NxClient_Join(NULL);
		ASSERT(rc == NEXUS_SUCCESS);

		NxClient_GetDefaultAllocSettings(&allocSettings);
		allocSettings.inputClient = 1;

		rc = NxClient_Alloc(&allocSettings, &_allocResults);
		ASSERT(rc == NEXUS_SUCCESS);

		_NxInputClient = NEXUS_InputClient_Acquire(_allocResults.inputClient[0].id);
		if (_NxInputClient) {
			TRACE_L1("%s: Acquired NEXUS Input Client", __FUNCTION__);
			NEXUS_InputClient_GetSettings(_NxInputClient, &settings);
			settings.filterMask = 1<<NEXUS_InputRouterDevice_eIrInput;

			settings.codeAvailable.callback = IRRemote::NexusCallback;
			settings.codeAvailable.param = _allocResults.inputClient[0].id;
			settings.codeAvailable.context = static_cast<NEXUS_InputClientHandle>(_NxInputClient);
			rc = NEXUS_InputClient_SetSettings(_NxInputClient, &settings);
			TRACE_L1("%s: NEXUS_InputClient_SetSettings rc=%d", __FUNCTION__, rc);
			_error = Core::ERROR_NONE;
		} else {
			_error = Core::ERROR_UNAVAILABLE;
			TRACE_L1("%s: NEXUS_InputClient_Acquire Failed. inputClient=%p", __FUNCTION__, _NxInputClient);
		}
	}

	void IRRemote::Deinitialize()
	{
		if (_NxInputClient) {
			NEXUS_InputClientSettings settings;
			NEXUS_InputClient_GetSettings(_NxInputClient, &settings);
			settings.codeAvailable.callback = nullptr;
			settings.codeAvailable.param = 0;
			settings.codeAvailable.context = nullptr;
			NEXUS_InputClient_SetSettings(_NxInputClient, &settings);
			NEXUS_InputClient_Release(_NxInputClient);
			NxClient_Free(&_allocResults);
			NxClient_Uninit();
		}

		_error = Core::ERROR_UNAVAILABLE;
	}

	/* virtual */ bool IRRemote::Pairing() {
		return (false);
	}

	/* virtual */ uint32 IRRemote::Callback(Exchange::IKeyHandler* callback) {

		ASSERT((callback == nullptr) ^ (_callback == nullptr));

		if (callback == nullptr) {
			// We are unlinked. Deinitialize the stuff.
			Deinitialize();
			_callback = nullptr;
		}
		else {

			Initialize();
			TRACE_L1("%s: callback=%p _callback=%p", __FUNCTION__, callback, _callback);
			_callback = callback;
		}

		return (Core::ERROR_NONE);
	}

	/* virtual */ uint32 IRRemote::Error() const {
		return (_error);
	}

	/* virtual */ string IRRemote::MetaData() const {
		return _T("BCM Nexus");
	}

	/* static */ void IRRemote::NexusCallback(void* nexusIRHandle, int deviceID)
	{
		_singleton->SendEvent();
	}

	void IRRemote::SendEvent() {
		NEXUS_InputRouterCode inputRouterCode;
		unsigned num = 0;
		int rc;

		if (_NxInputClient) {
			// always get the key, else the queue fills up and weird things happen
			rc = NEXUS_InputClient_GetCodes((NEXUS_InputClientHandle) _NxInputClient, &inputRouterCode, 1, &num);
			int rawCode = inputRouterCode.data.irInput.code & _codeMask;
			int repeat = inputRouterCode.data.irInput.repeat;

			if ((rc == 0) && (num != 0)) {

				uint64 currentTicks = Core::Time::Now().Ticks();

				// See if this key is coming in more than 150ms aftre the last one..
				if (((currentTicks - _lastKeyTicks) < 150000) && (true == repeat)) {

					TRACE_L1("%s: Ignoring key <150ms", __FUNCTION__);
				} else {
					TRACE_L1("%s: sending keycode=%x", __FUNCTION__, rawCode);
					_callback->KeyEvent(true, rawCode, _resourceName);
					_callback->KeyEvent(false, rawCode, _resourceName);
					_lastKeyTicks = currentTicks;
				}
			}
		} else {
			TRACE_L1("%s: Device is not connected", __FUNCTION__);
		}
	}
}
}
