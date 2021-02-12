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

#ifndef __FILESWATCHER_H
#define __FILESWATCHER_H

#include "Module.h"
#include <interfaces/IFilesWatcher.h>
#include <interfaces/json/JsonData_FilesWatcher.h>
#include <../FileTransfer/FileTransfer.h>
#include <unordered_set>

namespace WPEFramework
{
    namespace Plugin
    {

        class FilesWatcher
            : public PluginHost::IPlugin,
              public PluginHost::IWeb,
              public Exchange::IFilesWatcher,
              public PluginHost::JSONRPC

        {

        private:
            class FileObserver : public WPEFramework::Core::FileSystemMonitor::ICallback
            {

            public:
                FileObserver() = delete;
                FileObserver(const FileObserver &) = delete;
                FileObserver &operator=(const FileObserver &) = delete;
                FileObserver(FileObserver &&o) noexcept : _path(std::move(o._path)),_parent(o._parent)
                {                   

                }

                explicit FileObserver(FilesWatcher *parent, const string &filePath)
                    : _path(filePath),
                      _parent(parent)

                {
                }
                ~FileObserver()
                {
                }

                bool Register() 
                {
                    Core::FileSystemMonitor::Instance().Register(this, _path);
                    return true;
                }
                bool UnRegister() 
                {

                    Core::FileSystemMonitor::Instance().Unregister(this, _path);
                    return true;
                }

                void Updated() override
                {
                    _parent->Updated(_path);
                }
                string GetPath() const
                {
                    return _path;
                }

            private:
                string _path;
                FilesWatcher* _parent;
            };

            struct FileObserverHasher
            {
                size_t operator()(const FileObserver &obj) const
                {
                    return std::hash<std::string>()(obj.GetPath());
                }
            };

            struct FileObserverComparator
            {
                bool operator()(const FileObserver &obj1, const FileObserver &obj2) const
                {
                    return (obj1.GetPath() == obj2.GetPath());
                }
            };

            class Notification
                : public PluginHost::IPlugin::INotification
            {

            public:
                Notification() = delete;
                Notification(const Notification &) = delete;
                Notification &operator=(const Notification &) = delete;

                explicit Notification(FilesWatcher *parent)
                    : _parent(*parent)
                {
                    ASSERT(parent != nullptr);
                }
                ~Notification()
                {
                }

            public:
                BEGIN_INTERFACE_MAP(Notification)
                INTERFACE_ENTRY(PluginHost::IPlugin::INotification)
                END_INTERFACE_MAP

            private:
                FilesWatcher &_parent;
            };

            class Config : public Core::JSON::Container
            {
            private:
                Config(const Config &);
                Config &operator=(const Config &);

            public:
                Config()
                    : Core::JSON::Container()
                {
                    Add(_T("listOfFiles"), &ListOfWatchedFiles);
                }
                ~Config()
                {
                }

            public:
                Core::JSON::ArrayType<Core::JSON::String> ListOfWatchedFiles;
            };

        public:
            class Data : public Core::JSON::Container
            {
            private:
                Data(const Data &) = delete;
                Data &operator=(const Data &) = delete;

            public:
                Data()
                    : Core::JSON::Container()
                {
                    Add(_T("listOfFiles"), &ListOfWatchedFiles);
                }
                ~Data()
                {
                }

            public:
                Core::JSON::ArrayType<Core::JSON::String> ListOfWatchedFiles;
            };

            FilesWatcher(const FilesWatcher &) = delete;
            FilesWatcher &operator=(const FilesWatcher &) = delete;

#ifdef __WINDOWS__
#pragma warning(disable : 4355)
#endif
            FilesWatcher()
                : _adminLock(), _skipURL(0), _service(nullptr), _notificationClients()
            {
                RegisterAll();
            }
#ifdef __WINDOWS__
#pragma warning(default : 4355)
#endif
            virtual ~FilesWatcher()
            {
                UnregisterAll();
            }

        public:
            BEGIN_INTERFACE_MAP(IFilesWatcher)
            INTERFACE_ENTRY(PluginHost::IPlugin)
            INTERFACE_ENTRY(PluginHost::IWeb)
            INTERFACE_ENTRY(Exchange::IFilesWatcher)
            INTERFACE_ENTRY(PluginHost::IDispatcher)
            END_INTERFACE_MAP

        public:
            //  IPlugin methods
            // -------------------------------------------------------------------------------------------------------
            // First time initialization. Whenever a plugin is loaded, it is offered a Service object with relevant
            // information and services for this particular plugin. The Service object contains configuration information that
            // can be used to initialize the plugin correctly. If Initialization succeeds, return nothing (empty string)
            // If there is an error, return a string describing the issue why the initialisation failed.
            // The Service object is *NOT* reference counted, lifetime ends if the plugin is deactivated.
            // The lifetime of the Service object is guaranteed till the deinitialize method is called.
            const string Initialize(PluginHost::IShell *service) override;

            // The plugin is unloaded from WPEFramework. This is call allows the module to notify clients
            // or to persist information if needed. After this call the plugin will unlink from the service path
            // and be deactivated. The Service object is the same as passed in during the Initialize.
            // After theis call, the lifetime of the Service object ends.
            void Deinitialize(PluginHost::IShell *service) override;

            // Returns an interface to a JSON struct that can be used to return specific metadata information with respect
            // to this plugin. This Metadata can be used by the MetData plugin to publish this information to the ouside world.
            string Information() const override;

            //  IWeb methods
            // -------------------------------------------------------------------------------------------------------
            void Inbound(Web::Request &request) override;
            Core::ProxyType<Web::Response> Process(const Web::Request &request) override;

            //  IFilesWatcher methods
            // -------------------------------------------------------------------------------------------------------
            void Register(Exchange::IFilesWatcher::INotification *sink) override;
            void Unregister(Exchange::IFilesWatcher::INotification *sink) override;

        private:
            void RegisterAll();
            void UnregisterAll();
            uint32_t endpoint_addfile(const JsonData::FilesWatcher::FileInfo &params);
            uint32_t endpoint_removeFile(const JsonData::FilesWatcher::FileInfo &params);
            uint32_t get_listOfWatchedFiles(Core::JSON::ArrayType<Core::JSON::String> &response) const;

            uint32_t AddFile(const string &file);
            uint32_t RemoveFile(const string &file);

            typedef std::unordered_set<FileObserver, FileObserverHasher, FileObserverComparator> ListOfObservedFiles;

            void Updated(const string &filePath);

            //void StateChange(PluginHost::IShell *plugin);

        private:
            Core::CriticalSection _adminLock;
            uint32_t _skipURL;
            PluginHost::IShell *_service;
            std::list<Exchange::IFilesWatcher::INotification *> _notificationClients;
            ListOfObservedFiles _listOfFiles;
        };
    } //namespace Plugin
} //namespace WPEFramework

#endif // __FilesWatcher_H
