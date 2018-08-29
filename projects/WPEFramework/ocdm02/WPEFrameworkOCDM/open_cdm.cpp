#include "open_cdm.h"

#include "AccessorOCDM.h"
#include "IOCDM.h"

using namespace WPEFramework;

Core::CriticalSection g_accessorMutex;
OCDM::IAccessor * g_accessor = nullptr;

OpenCDMError opencdm_is_type_supported(const char keySystem[], const char mimeType[])
{
	if (g_accessor == nullptr) {
		g_accessorMutex.Lock();

		if (g_accessor == nullptr) {
			g_accessor = media::AccessorOCDM::Instance();
		}

		g_accessorMutex.Unlock();
	}

    Core::Error result = g_accessor->IsTypeSupported(keySystem, mimeType);

    return (result == Core::ERROR_NONE ? 0 : 1);
}
