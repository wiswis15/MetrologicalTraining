#include "Plugin02.h"

#include <iostream>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

using namespace std;

static EGLint const attribute_list[] = {
   EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
   EGL_RED_SIZE, 1,
   EGL_GREEN_SIZE, 1,
   EGL_BLUE_SIZE, 1,
   EGL_NONE
};

#define GL_CHECK_ERROR() {                                                                     \
   GLenum glError = glGetError();                                                              \
   if (glError != GL_NO_ERROR) {                                                               \
      cerr << "GL: Something went wrong on line " << __LINE__ << ", glError: " << glError << endl; \
      Block();                                                                                 \
      return Core::infinite;                                                                          \
   }                                                                                           \
}

#define EGL_CHECK_ERROR() {                                                                    \
   EGLint eglError = eglGetError();                                                              \
   if (eglError != EGL_SUCCESS) {                                                               \
      cerr << "EGL: Something went wrong on line " << __LINE__ << ", eglError: " << eglError << endl; \
      Block();                                                                                 \
      return Core::infinite;                                                                          \
   }                                                                                           \
}


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
	   // TODO: don't return -1, but "STOPPED" ?
		sleep(1);

		string displayName = m_callsign;
	   m_display = Compositor::IDisplay::Instance(displayName);
	   if (m_display == nullptr) {
	      cerr << "ERROR: IDisplay::Instance returned nullptr!" << endl;
	      return STOPPED;
	   }

	   // Compositor code dumps a lot on stderr, wait for it to print
	   sleep(1);

	   //string surfaceName = displayName + "-0";
	   string surfaceName = displayName;
	   uint32_t width = 1280;
	   uint32_t height = 720;

	   m_graphicsSurface = m_display->Create(surfaceName, width, height);
	   m_graphicsSurface->Keyboard(&m_keyboard);

	   EGLDisplay dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	   if (dpy == EGL_NO_DISPLAY) {
         cerr << "ERROR: eglGetDisplay returned EGL_NO_DISPLAY!" << endl;
         return -1;
	   }

	   EGLint result;
	   EGLint majorVersion = 0, minorVersion = 0;
	   result = eglInitialize(dpy, &majorVersion, &minorVersion);

	   sleep(1);

	   cerr << "Plugin02, EGL version: " << majorVersion << "." << minorVersion << endl;

	   EGLint numConfigs = 0;
      result = eglGetConfigs(dpy, 0, 0, &numConfigs);

      if (result == EGL_FALSE) {
         cerr << "ERROR: eglGetConfigs returned EGL_FALSE! Line: " << __LINE__ << endl;
         return -1;
      }

      cerr << "Plugin02, numConfigs: " << numConfigs << endl;

      if (numConfigs <= 0) {
         cerr << "ERROR: eglGetConfigs returned no configs" << endl;
         return -1;
      }

/*
      const EGLint configAttributes[] = {
          EGL_RED_SIZE,        1,
          EGL_GREEN_SIZE,      1,
          EGL_BLUE_SIZE,       1,
          EGL_ALPHA_SIZE,      1,
          EGL_DEPTH_SIZE,      0,
          EGL_STENCIL_SIZE,    0,
          EGL_BUFFER_SIZE,     EGL_DONT_CARE,

          EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
          EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,

          EGL_SAMPLE_BUFFERS,  EGL_DONT_CARE,
          EGL_SAMPLES,         EGL_DONT_CARE,

          EGL_NONE
      };
*/

      EGLint const configAttributes[] = {
         EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
         EGL_RED_SIZE, 1,
         EGL_GREEN_SIZE, 1,
         EGL_BLUE_SIZE, 1,
         EGL_NONE
      };


      EGLConfig config;
      int chooseConfigs = numConfigs;
      result = eglChooseConfig(dpy, configAttributes, &config, 1, &chooseConfigs);
      if ((result != EGL_TRUE) || (chooseConfigs == 0)) {
         cerr << "ERROR: Something went wrong when calling eglChooseConfig on line " << __LINE__ << endl;
         return -1;
      }

      cerr << "Plugin02, config: " << config << endl;

	   EGLint eglContextAttributes[] = {
	      EGL_CONTEXT_CLIENT_VERSION, 2,
	      EGL_NONE
	   };

	   EGLContext nativeContext = eglCreateContext(dpy, config, EGL_NO_CONTEXT, eglContextAttributes);
	   if (nativeContext == EGL_NO_CONTEXT) {
         cerr << "ERROR: eglCreateContext returned EGL_NO_CONTEXT!" << endl;
         return -1;
	   }

	   EGLNativeWindowType nativewindow = m_graphicsSurface->Native();
      EGLSurface eglSurface = eglCreateWindowSurface(dpy, config, nativewindow, 0);
      EGL_CHECK_ERROR();

      EGLint eglWidth, eglHeight;
      eglQuerySurface(dpy, eglSurface, EGL_WIDTH, &eglWidth);
      eglQuerySurface(dpy, eglSurface, EGL_HEIGHT, &eglHeight);

      cerr << "Surface size according to EGL: " << width << "x" << height << endl;

      result = eglMakeCurrent(dpy, eglSurface, eglSurface, nativeContext);
      if (result != EGL_TRUE) {
         cerr << "eglMakeCurrent didn't return EGL_TRUE" << endl;
         return -1;
      }

      result = eglSwapInterval(dpy, 1);
      if (result != EGL_TRUE) {
         cerr << "eglSwapInterval didn't return EGL_TRUE (" << result << "), eglGetError: " << eglGetError() << endl;
         cerr << "EGL_BAD_CONTEXT: " << EGL_BAD_CONTEXT << endl;
         cerr << "EGL_BAD_SURFACE: " << EGL_BAD_SURFACE << endl;
         //return 1;
         Block();
         return (Core::infinite);
      }

      cerr << "Plugin02: surface size, according to compositor: " << m_graphicsSurface->Width() << "x" << m_graphicsSurface->Height() << endl;

      const int frameReportInterval = 300;
      int frameIndex = 0;

      GL_CHECK_ERROR();
      while (true) {
		   float fR = ((float)rand()) / ((float)RAND_MAX);

		   glClearColor(fR, 1.0, 0.0, 1.0);
		   GL_CHECK_ERROR();

		   glClear(GL_COLOR_BUFFER_BIT);
		   GL_CHECK_ERROR();

		   glFlush();
		   GL_CHECK_ERROR();

		   eglSwapBuffers(dpy, eglSurface);

/*
		   if ((frameIndex % frameReportInterval) == 0) {
		      cerr << "Plugin02, rendered frame no: " << frameIndex << endl;
		   }
 */
		   frameIndex++;
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

