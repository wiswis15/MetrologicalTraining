#include <core/ProcessInfo.h>
#include <interfaces/IMemory.h>
#include <interfaces/IResourceMonitor.h>
#include <sstream>
#include <vector>

namespace WPEFramework {
namespace Plugin {
   class ResourceMonitorImplementation : public Exchange::IResourceMonitor {
   public:
      ResourceMonitorImplementation();
      virtual ~ResourceMonitorImplementation();
   
      void AddRef() const override;
      uint32_t Release() const override;
      void* QueryInterface(const uint32_t interfaceNummer) override;
      uint32_t Configure(PluginHost::IShell* framework) override;
      string CompileMemoryCsv() override;
      
      uint32_t Configure(const string& path, int interval, const string& mode, const string& parentName);
   };
}
}

