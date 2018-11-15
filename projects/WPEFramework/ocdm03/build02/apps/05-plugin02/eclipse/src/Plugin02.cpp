#include "Plugin02.h"

#include <iostream>

using namespace std;

namespace WPEFramework {
namespace Plugin {

SERVICE_REGISTRATION(Plugin02, 1, 0);

Plugin02::Plugin02()
   : m_skipURL(0)
   , m_display(nullptr)
{
   cerr << __FILE__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << endl;
}

Plugin02::~Plugin02()
{
   cerr << __FILE__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << endl;
}

/* virtual */ const string Plugin02::Initialize(PluginHost::IShell* service)
{
   cerr << __FILE__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << ", callsign: " << service->Callsign() << endl;
   string result;

   // Setup skip URL for right offset.
   m_skipURL = service->WebPrefix().length();

   string displayName = service->Callsign();
   m_display = Compositor::IDisplay::Instance(displayName);

   cerr << "m_display: " << m_display << endl;

   return (result);
}

/* virtual */ void Plugin02::Deinitialize(PluginHost::IShell* service)
{
   cerr << __FILE__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << endl;
}

/* virtual */ string Plugin02::Information() const
{
   cerr << __FILE__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << endl;
   // No additional info to report.
   return (string());
}

/* virtual */ void Plugin02::Inbound(Web::Request& /* request */)
{
   cerr << __FILE__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << endl;
}

/* virtual */ Core::ProxyType<Web::Response> Plugin02::Process(const Web::Request& request)
{
   cerr << __FILE__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << endl;
   ASSERT(m_skipURL <= request.Path.length());

   // Proxy object for response type.
   Core::ProxyType<Web::Response> response(PluginHost::Factories::Instance().Response());

   // Default is not allowed.
   response->Message = _T("Method not allowed");
   response->ErrorCode = Web::STATUS_METHOD_NOT_ALLOWED;

   // Decode request path.
   Core::TextSegmentIterator index(Core::TextFragment(request.Path, m_skipURL, request.Path.length() - m_skipURL), false, '/');

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

