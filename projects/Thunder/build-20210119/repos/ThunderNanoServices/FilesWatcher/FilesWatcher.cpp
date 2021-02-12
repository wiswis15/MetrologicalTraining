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

            //not ready yet
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
            auto found = _listOfFiles.find(FileObserver(this, file)) != _listOfFiles.end();
            if (ifile && !found)
            {
                auto newObserver = FilesWatcher::FileObserver(this, file);
                auto inserted = _listOfFiles.insert(FilesWatcher::FileObserver(this, file));
                if (inserted.second)
                {
                    const_cast<FileObserver &>(*inserted.first).Register();
                }
                return Core::ERROR_NONE;
            }
            else
                return Core::ERROR_BAD_REQUEST;
        }

        uint32_t FilesWatcher::RemoveFile(const string &file)
        {

            //auto found=std::find_if(_listOfFiles.begin(),_listOfFiles.end(),[&](const FileObserver& observer){return observer.GetPath()==file;});
            auto found = _listOfFiles.find(FileObserver(this, file));
            if (found != _listOfFiles.end())
            {
                const_cast<FileObserver &>(*found).UnRegister();
                _listOfFiles.erase(found);
                return Core::ERROR_NONE;
            }
            else
                return Core::ERROR_BAD_REQUEST;
        }

        void FilesWatcher::Updated(const string &filePath)
        {
            // here we receive notification from the system that the file has been changed --> fire notification for users
            _adminLock.Lock();

            std::list<Exchange::IFilesWatcher::INotification *>::iterator index(_notificationClients.begin());

            while (index != _notificationClients.end())
            {
                //Here fire the notification
                //(*index)->FileChanged(state);
                index++;
            }

            _adminLock.Unlock();
        }

        /*void FilesWatcher::FileChanged(PluginHost::IShell *plugin)
        {
            const string callsign(plugin->Callsign());

            _adminLock.Lock();

            Clients::iterator index(_clients.find(callsign));

            if (plugin->State() == PluginHost::IShell::ACTIVATED)
            {

                if (index == _clients.end())
                {
                    PluginHost::IStateControl *stateControl(plugin->QueryInterface<PluginHost::IStateControl>());

                    if (stateControl != nullptr)
                    {
                        _clients.emplace(std::piecewise_construct,
                                         std::forward_as_tuple(callsign),
                                         std::forward_as_tuple(stateControl));
                        TRACE(Trace::Information, (_T("%s plugin is add to power control list"), callsign.c_str()));
                        stateControl->Release();
                    }
                }
            }
            else if (plugin->State() == PluginHost::IShell::DEACTIVATION)
            {

                if (index != _clients.end())
                { // Remove from the list, if it is already there
                    _clients.erase(index);
                    TRACE(Trace::Information, (_T("%s plugin is removed from power control list"), plugin->Callsign().c_str()));
                }
            }

            _adminLock.Unlock();
        }
        */

    } // namespace Plugin
} // namespace WPEFramework