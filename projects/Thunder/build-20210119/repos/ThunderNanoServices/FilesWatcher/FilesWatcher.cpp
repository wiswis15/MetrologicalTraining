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
            string id = "FilesWatcherFileChange";
            // here we receive notification from the system that the file has been changed --> fire notification for users
            event_statechange(id, filePath);
        }

    } // namespace Plugin
} // namespace WPEFramework