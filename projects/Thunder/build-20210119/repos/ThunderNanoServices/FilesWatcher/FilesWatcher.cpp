/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "FilesWatcher.h"
#include <experimental/filesystem>
#include <fstream>

namespace WPEFramework
{
    namespace Plugin
    {

        SERVICE_REGISTRATION(FilesWatcher, 1, 0);

        static Core::ProxyPoolType<Web::JSONBodyType<FilesWatcher::Data>> jsonBodyDataFactory(2);
        static Core::ProxyPoolType<Web::JSONBodyType<FilesWatcher::Data>> jsonResponseFactory(4);

        /* virtual */ const string FilesWatcher::Initialize(PluginHost::IShell *service)
        {
            string message("");

            ASSERT(_service == nullptr);

            // Setup skip URL for right offset.
            _service = service;
            _skipURL = static_cast<uint8_t>(_service->WebPrefix().length());

            return message;
        }

        /* virtual */ void FilesWatcher::Deinitialize(PluginHost::IShell *service)
        {
            ASSERT(_service == service);

            _service = nullptr;
        }

        /* virtual */ string FilesWatcher::Information() const
        {
            // No additional info to report.
            return (string());
        }

        /* virtual */ void FilesWatcher::Inbound(Web::Request &request)
        {
            if (request.Verb == Web::Request::HTTP_POST)
                request.Body(jsonBodyDataFactory.Element());
        }

        /* virtual */ Core::ProxyType<Web::Response> FilesWatcher::Process(const Web::Request &request)
        {
            ASSERT(_skipURL <= request.Path.length());

            Core::ProxyType<Web::Response> result(PluginHost::IFactories::Instance().Response());
            Core::TextSegmentIterator index(
                Core::TextFragment(request.Path, _skipURL, request.Path.length() - _skipURL), false, '/');

            result->ErrorCode = Web::STATUS_BAD_REQUEST;
            result->Message = "Unknown error";

            /*         if ((request.Verb == Web::Request::HTTP_GET) && ((index.Next() == true) && (index.Next() == true))) {
            result->ErrorCode = Web::STATUS_OK;
            result->Message = "OK";
            if (index.Remainder() == _T("State")) {
                Core::ProxyType<Web::JSONBodyType<Data>> response(jsonResponseFactory.Element());
                response->PowerState = power_get_state();
                if (response->PowerState) {
                    result->ContentType = Web::MIMETypes::MIME_JSON;
                    result->Body(Core::proxy_cast<Web::IBody>(response));
                } else {
                    result->Message = "Invalid State";
                }
            } else {
                result->ErrorCode = Web::STATUS_BAD_REQUEST;
                result->Message = "Unknown error";
            }
        } else if ((request.Verb == Web::Request::HTTP_POST) && (index.Next() == true) && (index.Next() == true)) {
            result->ErrorCode = Web::STATUS_OK;
            result->Message = "OK";
            if (index.Remainder() == _T("State")) {
                uint32_t timeout = request.Body<const Data>()->Timeout.Value();
                Exchange::IPower::PCState state = static_cast<Exchange::IPower::PCState>(request.Body<const Data>()->PowerState.Value());

                ControlClients(state);

                Core::ProxyType<Web::JSONBodyType<Data>> response(jsonResponseFactory.Element());
                response->Status = SetState(state, timeout);
                result->ContentType = Web::MIMETypes::MIME_JSON;
                result->Body(Core::proxy_cast<Web::IBody>(response));
            } else {
                result->ErrorCode = Web::STATUS_BAD_REQUEST;
                result->Message = "Unknown error";
            }
        } */

            return result;
        }

        void FilesWatcher::Register(Exchange::IFilesWatcher::INotification *sink)
        {
            _adminLock.Lock();

            // Make sure a sink is not registered multiple times.
            ASSERT(std::find(_notificationClients.begin(), _notificationClients.end(), sink) == _notificationClients.end());

            _notificationClients.push_back(sink);
            sink->AddRef();

            _adminLock.Unlock();

            TRACE(Trace::Information, (_T("Registered a sink on the FilesWatcher")));
        }

        void FilesWatcher::Unregister(Exchange::IFilesWatcher::INotification *sink)
        {
            _adminLock.Lock();

            std::list<Exchange::IFilesWatcher::INotification *>::iterator index(std::find(_notificationClients.begin(), _notificationClients.end(), sink));

            // Make sure you do not unregister something you did not register !!!
            ASSERT(index != _notificationClients.end());

            if (index != _notificationClients.end())
            {
                (*index)->Release();
                _notificationClients.erase(index);
                TRACE(Trace::Information, (_T("Unregistered a sink on the power")));
            }

            _adminLock.Unlock();
        }

        uint32_t FilesWatcher::AddFile(const string &file)
        {

            std::ifstream ifile;
            ifile.open(file);
            if (ifile)
            {
                _listOfFiles.insert(file);
                Register(file);
                return Core::ERROR_NONE;
            }
            else
                return Core::ERROR_BAD_REQUEST;
        }

        uint32_t FilesWatcher::RemoveFile(const string &file)
        {

            if (_listOfFiles.find(file) != _listOfFiles.end())
            {
                _listOfFiles.erase(file);
                UnRegister(file);
                return Core::ERROR_NONE;
            }
            else
                return Core::ERROR_BAD_REQUEST;
        }

    } // namespace Plugin
} // namespace WPEFramework