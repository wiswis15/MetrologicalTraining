#include <generics/generics.h>
#include <websocket/websocket.h>

namespace WPEFramework {

class WebInspectorConnection : public Web::TextWebSocket {
private:
    class ServiceThread : public Core::Thread {
    private:
        ServiceThread(const ServiceThread&);
        ServiceThread& operator=(const ServiceThread&);

    public:
        ServiceThread(Web::TextWebSocket& serviceObject)
            : Core::Thread(false, "WebInspectorService")
            , _service(serviceObject)
        {
        }
        ~ServiceThread()
        {
        }

    public:
        void Stop()
        {
            Core::Thread::Block();
            _service.Abort();
            Core::Thread::Wait(Core::Thread::STOPPED | Core::Thread::BLOCKED, WPEFramework::Core::infinite);
        }
        void Start()
        {
            Core::Thread::Run();
        }

    private:
        virtual uint32 Worker()
        {
            uint32 value = (_service.Service() == false ? WPEFramework::Core::infinite : 0);
            return (value);
        }

    private:
        Web::TextWebSocket& _service;
    };

private:
    WebInspectorConnection();
    WebInspectorConnection(const WebInspectorConnection&);
    WebInspectorConnection& operator=(const WebInspectorConnection&);

public:
    WebInspectorConnection(const string& remoteAddress, const uint16 port, const string& pathName, const bool SSL)
        : TextWebSocket(remoteAddress, port, pathName, SSL, "")
        , _thread(*this)
    {
    }
    ~WebInspectorConnection()
    {
        _thread.Stop();
    }

private:
    virtual void StateChange(const State newState)
    {
        switch (newState) {
        case CONNECTING: {
            _thread.Start();
            break;
        }
        case DISCONNECTED: {
            _thread.Stop();
            break;
        }
        }

        fprintf(stderr, "New State: %d\n", newState);
    }
    virtual void Message(const string& message)
    {
        fprintf(stderr, "Receivedi message: %s\n", message.c_str());
    }
    virtual bool Supported(const string& type) const
    {
        bool supported = false;

        if (type == "deflate-stream") {
            fprintf(stderr, "supported deflate-stream extension\n");
            supported = true;
        }
        if (type == "deflate-frame") {
            fprintf(stderr, "supported deflate-frame extension\n");
            supported = true;
        }
        if (type == "x-google-mux") {
            fprintf(stderr, "supported x-google-mux extension\n");
            supported = true;
        }

        return (supported);
    }

private:
    ServiceThread _thread;
};

} // Namepsace WPEFramework.Web
