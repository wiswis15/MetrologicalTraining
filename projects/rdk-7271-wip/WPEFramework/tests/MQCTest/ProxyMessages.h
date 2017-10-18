#ifndef __HORIZON_PROXYMESSAGE_H
#define __HORIZON_PROXYMESSAGE_H

#include "../../mqc/mqc.h"

namespace WPEFramework {
namespace Horizon {
    class CreateWebView : public MQC::Request {
    public:
        CreateWebView();
        virtual ~CreateWebView();

        uint32 X() const
        {
            return (_x);
        }
        uint32 Y() const
        {
            return (_y);
        }
        uint32 Z() const
        {
            return (_z);
        }
        uint32 Width() const
        {
            return (_width);
        }
        uint32 Height() const
        {
            return (_height);
        }
        const string& Configuration() const
        {
            return (_configuration);
        }

        void X(const uint32 x)
        {
            _x = x;
        }
        void Y(const uint32 y)
        {
            _y = y;
        }
        void Z(const uint32 z)
        {
            _z = z;
        }
        void Width(const uint32 width)
        {
            _width = width;
        }
        void Height(const uint32 height)
        {
            _height = height;
        }
        void Configuration(const string& configuration)
        {
            _configuration = configuration;
        }

    private:
        virtual void Serialize(Frame::Writer& write) const;
        virtual void Deserialize(const Frame::Reader& read);

    private:
        uint32 _x, _y, _z, _width, _height;
        string _configuration;
    };

    class ViewHandle : public MQC::Message {
    public:
        ViewHandle();
        virtual ~ViewHandle();

        uint32 Handle() const
        {
            return (_handle);
        }
        void Handle(const uint32 handle)
        {
            _handle = handle;
        }

    private:
        virtual void Serialize(Frame::Writer& write) const;
        virtual void Deserialize(const Frame::Reader& read);

    private:
        uint32 _handle;
    };

    extern const TCHAR CreateWebViewCommand[];

    typedef WPEFramework::MQC::MessageType<CreateWebViewCommand, 2, CreateWebView, ViewHandle> CreateWebViewMessage;

    // -------------------------------------------------------------------------------
    // REQUEST: Load
    // -------------------------------------------------------------------------------
    class Load : public MQC::Request {
    public:
        Load();
        virtual ~Load();

    public:
        uint32 ApplicationId() const
        {
            return (_applicationId);
        }
        const string& URL() const
        {
            return (_url);
        }

        void ApplicationId(const uint32 id)
        {
            _applicationId = id;
        }
        void URL(const string& url)
        {
            _url = url;
        }

        void ComponentHandle(const uint32 handle)
        {
            MQC::Request::ComponentHandle(handle);
        }

    private:
        virtual void Serialize(Frame::Writer& write) const;
        virtual void Deserialize(const Frame::Reader& read);

    private:
        string _url;
        uint32 _applicationId;
    };

    extern const TCHAR LoadCommand[];
    typedef WPEFramework::MQC::MessageType<LoadCommand, 1000, Load> LoadMessage;

    // -------------------------------------------------------------------------------
    // REQUEST: show
    // -------------------------------------------------------------------------------
    extern const TCHAR ShowCommand[];
    typedef WPEFramework::MQC::MessageType<ShowCommand, 1000 /*webview*/, MQC::Request> ShowMessage;

    // -------------------------------------------------------------------------------
    // REQUEST: hide
    // -------------------------------------------------------------------------------
    extern const TCHAR HideCommand[];
    typedef WPEFramework::MQC::MessageType<HideCommand, 1000 /*webview*/, MQC::Request> HideMessage;

    // -------------------------------------------------------------------------------
    // REQUEST: saveCookies
    // -------------------------------------------------------------------------------
    extern const TCHAR SaveCookiesCommand[];
    typedef WPEFramework::MQC::MessageType<SaveCookiesCommand, 1000 /*webview*/, MQC::Request> SaveCookiesMessage;
}
}

#endif /* __HORIZON_PROXYMESSAGE_H */
