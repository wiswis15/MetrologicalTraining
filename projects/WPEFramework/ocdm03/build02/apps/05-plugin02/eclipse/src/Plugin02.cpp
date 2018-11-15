#include "Plugin02.h"

#include <iostream>

using namespace std;

namespace WPEFramework {
namespace Plugin {

class Keyboard : public WPEFramework::Compositor::IDisplay::IKeyboard {
private:
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;

public:
    Keyboard() { }
    virtual ~Keyboard() { }

public:
    virtual void AddRef() const
    {
       cerr << "Keyboard::AddRef" << endl;
    }

    virtual uint32_t Release() const
    {
       cerr << "Keyboard::Release" << endl;
       return 0;
    }

    virtual void KeyMap(const char information[], const uint16_t size)
    {
       cerr << "Keyboard::KeyMap" << endl;
    }

    virtual void Key(const uint32_t key, const IKeyboard::state action, const uint32_t time)
    {
       cerr << "Keyboard::Key, key: " << key << ", action: " << action << endl;
    }

    virtual void Modifiers(uint32_t depressedMods, uint32_t latchedMods, uint32_t lockedMods, uint32_t group)
    {
       cerr << "Keyboard::Modifiers" << endl;
    }

    virtual void Repeat(int32_t rate, int32_t delay)
    {
       cerr << "Keyboard::Repeat" << endl;
    }

    virtual void Direct(const uint32_t code, const state action)
    {
       cerr << "Keyboard::Direct, code: " << code << ", action: " << action << endl;
    }
};

class RenderThread : public Core::Thread
{
public:
	explicit RenderThread(string callsign)
		: m_callsign(callsign)
	   , m_display(nullptr)
	   , m_graphicsSurface(nullptr)
	{
	}

protected:
	virtual uint32_t Worker()
	{
		sleep(1);

		string displayName = m_callsign;
	   m_display = Compositor::IDisplay::Instance(displayName);
	   if (m_display == nullptr) {
	      cerr << "ERROR: IDisplay::Instance returned nullptr!" << endl;
	      return -1;
	   }

	   string surfaceName = displayName + "-0";
	   uint32_t width = 1280;
	   uint32_t height = 720;

	   m_graphicsSurface = m_display->Create(surfaceName, width, height);
	   m_graphicsSurface->Keyboard(&m_keyboard);

		while (true) {

		}

		// TODO: clean up

	   return 0;
	}

private:
	string m_callsign;
	Compositor::IDisplay * m_display;
	Compositor::IDisplay::ISurface * m_graphicsSurface;
	Keyboard m_keyboard;
};

SERVICE_REGISTRATION(Plugin02, 1, 0);

Plugin02::Plugin02()
   : m_skipURL(0)
   , m_display(nullptr)
   , m_renderThread(nullptr)
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

   string callsign = service->Callsign();

   m_renderThread = new RenderThread(callsign);

   m_renderThread->Run();

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

