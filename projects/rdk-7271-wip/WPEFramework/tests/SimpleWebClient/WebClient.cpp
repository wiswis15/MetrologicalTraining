#include "WebClient.h"

namespace WPEFramework {

// Constructor: calls constructor of base class with response factory,
//   node ID (which we build from hostname and port number), and buffer sizes.
WebClient::WebClient(const Core::NodeId remoteNode, const char* path)
    : BaseClass(5, _responseFactory, false, remoteNode.AnyInterface(), remoteNode, 2048, 208)
    , _hostName(remoteNode.HostName())
    , _path(path)
    , _request(Core::ProxyType<Web::Request>::Create())
    , _body(Core::ProxyType<Web::TextBody>::Create())
{
}

// Destructor
/* virtual */ WebClient::~WebClient()
{
    Close(WPEFramework::Core::infinite);
}

// Deals with partial received message by telling where to store body.
/* virtual */ void WebClient::LinkBody(Core::ProxyType<WPEFramework::Web::Response>& element)
{
    element->Body(_body);
}

// Received all data, which is stored in _body, print contents.
/* virtual */ void WebClient::Received(Core::ProxyType<WPEFramework::Web::Response> &element VARIABLE_IS_NOT_USED)
{
    printf(_T("%s"), _body->c_str());
}

// We don't have to do anything special when sending the request.
/* virtual */ void WebClient::Send(const Core::ProxyType<WPEFramework::Web::Request> &response VARIABLE_IS_NOT_USED)
{
}

// On open, submit web request.
/* virtual */ void WebClient::StateChange()
{
    if (IsOpen()) {
        _request->Verb = Web::Request::HTTP_GET;
        _request->Host = _hostName;
        _request->Path = _path;

        Submit(_request);
    }
}

Core::ProxyPoolType<Web::Response> WebClient::_responseFactory(5);

} // namespace WPEFramework
