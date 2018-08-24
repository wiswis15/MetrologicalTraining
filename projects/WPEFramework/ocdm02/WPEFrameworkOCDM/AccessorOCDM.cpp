#include "AccessorOCDM.h"

//Core::CriticalSection media::AccessorOCDM::_systemLock;

Core::CriticalSection media::_systemLock;

media::AccessorOCDM * media::AccessorOCDM::_singleton = nullptr;

namespace media {

WPEFramework::Core::Error AccessorOCDM::IsTypeSupported(const std::string& keySystem, const std::string& mimeType) const
{
   return _remote->IsTypeSupported(keySystem, mimeType);
}

OCDM::IAccessor* AccessorOCDM::Instance()
{
   _systemLock.Lock();

   if (_singleton == nullptr) {
       AccessorOCDM* result = new AccessorOCDM ("/tmp/ocdm"); 

       if (result->_remote != nullptr) {
           _singleton = result;
       }
   } else {
       _singleton->AddRef();
   }

   _systemLock.Unlock();

   return (_singleton);
}


}
