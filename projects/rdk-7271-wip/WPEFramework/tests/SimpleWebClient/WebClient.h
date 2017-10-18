#ifndef __WEBCLIENT_H
#define __WEBCLIENT_H

#include <WPEFramework/websocket/websocket.h>
#include <stdio.h>

namespace WPEFramework {

class WebClient : public Web::WebLinkType<Core::SocketStream, Web::Response, Web::Request,
                      WPEFramework::Core::ProxyPoolType<Web::Response>&> {
private:
    typedef Web::WebLinkType<Core::SocketStream, Web::Response,
        Web::Request, WPEFramework::Core::ProxyPoolType<Web::Response>&> BaseClass;

    // Static factory for responses. Needed by base class.
    static WPEFramework::Core::ProxyPoolType<Web::Response> _responseFactory;

public:
    WebClient() = delete;
    WebClient(const WebClient & copy) = delete;
    WebClient & operator =(const WebClient &) = delete;
    WebClient(const Core::NodeId remoteNode, const char* path);
    virtual ~WebClient();

public:
    virtual void LinkBody(Core::ProxyType<WPEFramework::Web::Response>& element);
    virtual void Received(Core::ProxyType<WPEFramework::Web::Response>& element);
    virtual void Send(const Core::ProxyType<WPEFramework::Web::Request>& response);
    virtual void StateChange();

    string _hostName;
    string _path;
    Core::ProxyType<Web::Request> _request;
    Core::ProxyType<Web::TextBody> _body;
};

} // namespace WPEFramework

#endif // __WEBCLIENT_H
