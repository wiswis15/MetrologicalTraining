#include "ProxyMessages.h"

namespace WPEFramework {
namespace Horizon {
    const TCHAR CreateWebViewCommand[] = _T("createWebView");

    CreateWebView::CreateWebView()
        : Request()
        , _x(0)
        , _y(0)
        , _z(0)
        , _width(0)
        , _height(0)
        , _configuration("")
    {
    }

    /* virtual */ CreateWebView::~CreateWebView()
    {
    }

    /* virtual */ void CreateWebView::Serialize(Frame::Writer& write) const
    {
        Request::Serialize(write);
        write.Number(_x);
        write.Number(_y);
        write.Number(_width);
        write.Number(_height);
        write.Number(_z);
        write.NullTerminatedText(_configuration);
    }

    /* virtual */ void CreateWebView::Deserialize(const Frame::Reader& read)
    {
        Request::Deserialize(read);
        TRACE_L1("Component handle: %d", Request::ComponentHandle());
        _x = read.Number<uint32>();
        TRACE_L1("X: %d", _x);
        _y = read.Number<uint32>();
        TRACE_L1("Y: %d", _y);
        _width = read.Number<uint32>();
        TRACE_L1("width: %d", _width);
        _height = read.Number<uint32>();
        TRACE_L1("height: %d", _height);
        _z = read.Number<uint32>();
        TRACE_L1("Z: %d", _z);
        _configuration = read.NullTerminatedText();
        TRACE_L1("Configuration: %s", _configuration.c_str());
    }

    ViewHandle::ViewHandle()
        : Message()
        , _handle(0)
    {
    }

    /* virtual */ ViewHandle::~ViewHandle()
    {
    }

    /* virtual */ void ViewHandle::Serialize(Frame::Writer& write) const
    {
        Message::Serialize(write);
        write.Number(_handle);
    }

    /* virtual */ void ViewHandle::Deserialize(const Frame::Reader& read)
    {
        Message::Deserialize(read);
        _handle = read.Number<uint32>();
    }

    // ------------------------------------------------------------------------
    // REQUEST: load
    // ------------------------------------------------------------------------
    const TCHAR LoadCommand[] = _T("load");

    Load::Load()
        : Request()
        , _url("")
        , _applicationId(0)
    {
    }

    /* virtual */ Load::~Load()
    {
    }

    /* virtual */ void Load::Serialize(Frame::Writer& write) const
    {
        Request::Serialize(write);
        write.NullTerminatedText(_url);
        write.Number(_applicationId);
    }

    /* virtual */ void Load::Deserialize(const Frame::Reader& read)
    {
        Request::Deserialize(read);
        _url = read.NullTerminatedText();
        _applicationId = read.Number<uint32>();
    }

    // ------------------------------------------------------------------------
    // REQUEST: show
    // ------------------------------------------------------------------------
    const TCHAR ShowCommand[] = _T("show");

    // ------------------------------------------------------------------------
    // REQUEST: hide
    // ------------------------------------------------------------------------
    const TCHAR HideCommand[] = _T("hide");

    // ------------------------------------------------------------------------
    // REQUEST: saveCookies
    // ------------------------------------------------------------------------
    const TCHAR SaveCookiesCommand[] = _T("saveCookies");
}
}
