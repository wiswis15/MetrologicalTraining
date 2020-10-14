#include "ResourceMonitorImplementation.h"

namespace WPEFramework {
namespace Plugin {

/*
ResourceMonitorImplementation::ResourceMonitorImplementation()
{
   // TODO ?
}

ResourceMonitorImplementation::~ResourceMonitorImplementation()
{
}
*/

void ResourceMonitorImplementation::AddRef() const
{
}

uint32_t ResourceMonitorImplementation::Release() const
{
   return 1;
}

}
}

