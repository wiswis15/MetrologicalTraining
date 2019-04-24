#include "Plugin01.h"

#include <iostream>

using namespace std;

namespace WPEFramework {
namespace Plugin {

SERVICE_REGISTRATION(Plugin01, 1, 0);

Plugin01::Plugin01()
   : _skipURL(0)
{
   cerr << __FILE__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << endl;
}

Plugin01::~Plugin01()
{
   cerr << __FILE__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << endl;
}

/* virtual */ const string Plugin01::Initialize(PluginHost::IShell* service)
{
   cerr << __FILE__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << ", callsign: " << service->Callsign() << endl;
   string result;

   // Capture PNG file name
   ASSERT(service->PersistentPath() != _T(""));

   Core::Directory directory(service->PersistentPath().c_str());

   // Setup skip URL for right offset.
   _skipURL = service->WebPrefix().length();

   return (result);
}

/* virtual */ void Plugin01::Deinitialize(PluginHost::IShell* service)
{
   cerr << __FILE__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << endl;
}

/* virtual */ string Plugin01::Information() const
{
   cerr << __FILE__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << endl;
   // No additional info to report.
   return (string());
}

/* virtual */ void Plugin01::Inbound(Web::Request& /* request */)
{
   cerr << __FILE__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << endl;
}

/* virtual */ Core::ProxyType<Web::Response> Plugin01::Process(const Web::Request& request)
{
   cerr << __FILE__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << endl;
   ASSERT(_skipURL <= request.Path.length());

   // Proxy object for response type.
   Core::ProxyType<Web::Response> response(PluginHost::Factories::Instance().Response());

   // Default is not allowed.
   response->Message = _T("Method not allowed");
   response->ErrorCode = Web::STATUS_METHOD_NOT_ALLOWED;

   // Decode request path.
   Core::TextSegmentIterator index(Core::TextFragment(request.Path, _skipURL, request.Path.length() - _skipURL), false, '/');

   // Get first plugin verb.
   index.Next();

   // Move to next segment if it exists.
   if (request.Verb == Web::Request::HTTP_GET) {
      if (false == index.Next()) {
          response->Message = _T("Plugin is up and running");
          response->ErrorCode = Web::STATUS_OK;
      }
   }

   return (response);
}

}
}

