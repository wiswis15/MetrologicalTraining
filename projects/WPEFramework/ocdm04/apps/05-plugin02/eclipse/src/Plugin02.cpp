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

class ProcessThread : public Core::Thread
{
public:
   explicit ProcessThread(Compositor::IDisplay * display)
      : m_display(display)
   {
   }

   ~ProcessThread() {
      Core::Thread::Stop();
      m_display->QuitKeyProcess();
      Core::Thread::Wait(STOPPED, Core::infinite);
   }

protected:
   virtual uint32_t Worker()
   {
      m_display->Process(1);
      return(0);
   }

private:
   Compositor::IDisplay * m_display;
};

class RenderThread : public Core::Thread
{
public:
	explicit RenderThread(string callsign)
		: m_callsign(callsign)
	   , m_display(nullptr)
	   , m_graphicsSurface(nullptr)
	   , m_processThread(nullptr)
	   , m_dpy(EGL_NO_DISPLAY)
	   , m_eglSurface(EGL_NO_SURFACE)
	   , m_nativeContext(EGL_NO_CONTEXT)
	   , m_red(0.0f)
	   , m_green(1.0f)
	   , m_blue(0.0f)
      , m_redDir(0.003f)
      , m_greenDir(0.01f)
      , m_blueDir(0.015f)
	{

	}

	~RenderThread()
	{
	   // Stop worker (render) loop
      Core::Thread::Stop();
      Core::Thread::Wait(STOPPED, Core::infinite);

      eglMakeCurrent(m_dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
      fprintf(stderr, __FILE__ ":%d\n", __LINE__);

      eglDestroySurface(m_dpy, m_eglSurface);
      fprintf(stderr, __FILE__ ":%d\n", __LINE__);

      eglDestroyContext(m_dpy, m_nativeContext);
      fprintf(stderr, __FILE__ ":%d\n", __LINE__);

      eglTerminate(m_dpy);
      fprintf(stderr, __FILE__ ":%d\n", __LINE__);

      m_graphicsSurface->Release();
      fprintf(stderr, __FILE__ ":%d\n", __LINE__);

      m_display->Release();
      fprintf(stderr, __FILE__ ":%d\n", __LINE__);
	}

	virtual bool Initialize()
	{
      m_display = Compositor::IDisplay::Instance(m_callsign);
      if (m_display == nullptr) {
         cerr << "ERROR: IDisplay::Instance returned nullptr!" << endl;
         return false;
      }

      // TODO: make configurable
      const uint32_t width = 1280;
      const uint32_t height = 720;

      m_graphicsSurface = m_display->Create(m_callsign, width, height);
      m_graphicsSurface->Keyboard(&m_keyboard);

      m_dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
      if (m_dpy == EGL_NO_DISPLAY) {
         cerr << "ERROR: eglGetDisplay returned EGL_NO_DISPLAY!" << endl;
         return false;
      }


      EGLint result;
      EGLint majorVersion = 0, minorVersion = 0;
      result = eglInitialize(m_dpy, &majorVersion, &minorVersion);

      sleep(1);

      cerr << "Plugin02, EGL version: " << majorVersion << "." << minorVersion << endl;

      EGLint numConfigs = 0;
      result = eglGetConfigs(m_dpy, 0, 0, &numConfigs);

      if (result == EGL_FALSE) {
         cerr << "ERROR: eglGetConfigs returned EGL_FALSE! Line: " << __LINE__ << endl;
         return false;
      }

      cerr << "Plugin02, numConfigs: " << numConfigs << endl;

      if (numConfigs <= 0) {
         cerr << "ERROR: eglGetConfigs returned no configs" << endl;
         return false;
      }

      EGLint const configAttributes[] = {
         EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
         EGL_RED_SIZE, 1,
         EGL_GREEN_SIZE, 1,
         EGL_BLUE_SIZE, 1,
         EGL_NONE
      };

      EGLConfig config;
      int chooseConfigs = numConfigs;
      result = eglChooseConfig(m_dpy, configAttributes, &config, 1, &chooseConfigs);
      if ((result != EGL_TRUE) || (chooseConfigs == 0)) {
         cerr << "ERROR: Something went wrong when calling eglChooseConfig on line " << __LINE__ << endl;
         return false;
      }

      cerr << "Plugin02, config: " << config << endl;

      EGLint eglContextAttributes[] = {
         EGL_CONTEXT_CLIENT_VERSION, 2,
         EGL_NONE
      };

      m_nativeContext = eglCreateContext(m_dpy, config, EGL_NO_CONTEXT, eglContextAttributes);
      if (m_nativeContext == EGL_NO_CONTEXT) {
         cerr << "ERROR: eglCreateContext returned EGL_NO_CONTEXT!" << endl;
         return false;
      }

      EGLNativeWindowType nativewindow = m_graphicsSurface->Native();
      m_eglSurface = eglCreateWindowSurface(m_dpy, config, nativewindow, nullptr);
      EGL_CHECK_ERROR();

      EGLint eglWidth, eglHeight;
      eglQuerySurface(m_dpy, m_eglSurface, EGL_WIDTH, &eglWidth);
      eglQuerySurface(m_dpy, m_eglSurface, EGL_HEIGHT, &eglHeight);

      cerr << "Surface size according to EGL: " << width << "x" << height << endl;

      result = eglMakeCurrent(m_dpy, m_eglSurface, m_eglSurface, m_nativeContext);
      if (result != EGL_TRUE) {
         cerr << "eglMakeCurrent didn't return EGL_TRUE" << endl;
         return false;
      }

      result = eglSwapInterval(m_dpy, 1);
      if (result != EGL_TRUE) {
         cerr << "eglSwapInterval didn't return EGL_TRUE (" << result << "), eglGetError: " << eglGetError() << endl;
         cerr << "EGL_BAD_CONTEXT: " << EGL_BAD_CONTEXT << endl;
         cerr << "EGL_BAD_SURFACE: " << EGL_BAD_SURFACE << endl;
         return false;
      }

      cerr << "Plugin02: surface size, according to compositor: " << m_graphicsSurface->Width() << "x" << m_graphicsSurface->Height() << endl;

      m_processThread = new ProcessThread(m_display);
      m_processThread->Run();

      return true;
	}

protected:
	virtual uint32_t Worker()
	{
      GL_CHECK_ERROR();

      UpdateColors();

      glClearColor(m_red, m_green, m_blue, 1.0);
      GL_CHECK_ERROR();

      glClear(GL_COLOR_BUFFER_BIT);
      GL_CHECK_ERROR();

      glFlush();
      GL_CHECK_ERROR();

      eglSwapBuffers(m_dpy, m_eglSurface);
      EGL_CHECK_ERROR();

      return 0;
	}

private:
	void UpdateColor(float & color, float & colorDir)
	{
	   color += colorDir;

	   if (color < 0.0f) {
	      color = 0.0f;
	      colorDir *= -1.0f;
	   }

	   if (color > 1.0f) {
         color = 1.0f;
         colorDir *= -1.0f;
      }
	}

	void UpdateColors()
	{
	   UpdateColor(m_red, m_redDir);
	   UpdateColor(m_green, m_greenDir);
	   UpdateColor(m_blue, m_blueDir);
	}

	string m_callsign;
	Compositor::IDisplay * m_display;
	Compositor::IDisplay::ISurface * m_graphicsSurface;
	Keyboard m_keyboard;
	ProcessThread * m_processThread;
	EGLDisplay m_dpy;
	EGLSurface m_eglSurface;
	EGLContext m_nativeContext;
	float m_red, m_green, m_blue;
	float m_redDir, m_greenDir, m_blueDir;
};

SERVICE_REGISTRATION(Plugin02, 1, 0);

Plugin02::Plugin02()
   : m_skipURL(0)
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

   delete m_renderThread;
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

