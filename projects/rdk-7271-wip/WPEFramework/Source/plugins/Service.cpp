#include "Service.h"
#include "Channel.h"

namespace WPEFramework {
namespace PluginHost {

	PluginHost::Request::Request()
		: Web::Request()
		, _state(INCOMPLETE)
		, _service()
	{
	}
	/* virtual */ PluginHost::Request::~Request()
	{
	}

	void PluginHost::Request::Clear()
	{
		Web::Request::Clear();
		_state = INCOMPLETE;

		if (_service.IsValid()) {
			_service.Release();
		}
	}
	void PluginHost::Request::Service(const bool correctSignature, const Core::ProxyType<PluginHost::Service>& service)
	{
		ASSERT(_service.IsValid() == false);
		ASSERT(_state == INCOMPLETE);

		if (service.IsValid() == true) {
			_state = COMPLETE;
			_service = service;
		}
		else if (correctSignature == false) {
			_state = OBLIVIOUS;
		}
		else {
			_state = MISSING_CALLSIGN;
		}
	}

    /* static */ Core::ProxyType<Core::IDispatchType<void> > IShell::Job::Create(IShell* shell, IShell::state toState, IShell::reason why)
    {
        return (Core::proxy_cast<Core::IDispatchType<void>> (Core::ProxyType<IShell::Job>::Create(shell, toState, why)));
    }

    WorkerPool::WorkerPool(const uint32 stackSize)
        : _workers(stackSize, _T("WorkerPool"))
        , _timer(stackSize, _T("WorkerTimer"))
    {
        TRACE_L1("WorkerPool created with stacksize %d", stackSize);
    }

    WorkerPool::~WorkerPool()
    {
    }

    /* static */ WorkerPool& WorkerPool::Instance(const uint32 stackSize)
    {
        static WorkerPool& instance (Core::SingletonType<WorkerPool>::Instance(stackSize));

        return (instance);
    }

    Factories::Factories()
        : _requestFactory(10)
        , _responseFactory(10)
        , _fileBodyFactory(10)
    {
    }

    Factories::~Factories()
    {
    }

    /* static */ Factories& Factories::Instance()
    {
		return (Core::SingletonType<Factories>::Instance());
	}

    void Service::Notification(const string& message)
    {
		_notifierLock.Lock();

        ASSERT(message.empty() != true);
        {
            std::list<Channel*>::iterator index(_notifiers.begin());

            while (index != _notifiers.end()) {
                (*index)->Submit(message);
                index++;
            }
        }

		_notifierLock.Unlock();
    }

    void Service::FileToServe(const string& webServiceRequest, Web::Response& response)
    {
        Web::MIMETypes result;
        uint16 offset = static_cast<uint16>(_config.WebPrefix().length()) + (_webURLPath.empty() ? 1 : static_cast<uint16>(_webURLPath.length()) + 2);
        string fileToService = _webServerFilePath;

        if ((webServiceRequest.length() <= offset) || (Web::MIMETypeForFile(webServiceRequest.substr(offset, -1), fileToService, result) == false)) {
            Core::ProxyType<Web::FileBody> fileBody(Factories::Instance().FileBody());

            // No filename gives, be default, we go for the index.html page..
            *fileBody = fileToService + _T("index.html");
            response.ContentType = Web::MIME_HTML;
            response.Body<Web::FileBody>(fileBody);
        }
        else {
            Core::ProxyType<Web::FileBody> fileBody(Factories::Instance().FileBody());
            *fileBody = fileToService;
            response.ContentType = result;
            response.Body<Web::FileBody>(fileBody);
        }
    }

    bool Service::IsWebServerRequest(const string& segment) const
    {
        // Prefix length, no need to compare, that has already been doen, otherwise
        // this call would not be placed.
        uint32 prefixLength = _config.WebPrefix().length();
        uint32 webLength = _webURLPath.length();

        return ((_webServerFilePath.empty() == false) && (segment.length() >= prefixLength + webLength) && ((webLength == 0) || (segment.compare(prefixLength + 1, webLength, _webURLPath) == 0)));
    }
}
}
