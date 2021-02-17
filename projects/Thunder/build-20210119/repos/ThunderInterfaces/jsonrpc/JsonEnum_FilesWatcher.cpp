
// Enumeration code for FilesWatcher API JSON-RPC API.
// Generated automatically from 'FilesWatcher.json'.

#include "definitions.h"
#include <core/Enumerate.h>
#include "JsonData_FilesWatcher.h"

namespace WPEFramework {

ENUM_CONVERSION_BEGIN(JsonData::FilesWatcher::StateType)
    { JsonData::FilesWatcher::StateType::ACTIVE, _TXT("active") },
    { JsonData::FilesWatcher::StateType::INACTIVE, _TXT("inactive") },
ENUM_CONVERSION_END(JsonData::FilesWatcher::StateType);

}
