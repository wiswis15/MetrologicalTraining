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

namespace WPEFramework
{
    namespace Plugin
    {

        class FilesWatcher
            : public PluginHost::IPlugin,
              public PluginHost::IWeb,
              public Exchange::IFilesWatcher,
              public PluginHost::JSONRPC,
              public WPEFramework::Core::FileSystemMonitor::ICallback
        {

        private:
            class Notification
                : public PluginHost::IPlugin::INotification,
                  public PluginHost::VirtualInput::INotifier
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

        public:
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
            //void KeyEvent(const uint32_t keyCode);
            //void StateChange(PluginHost::IShell* plugin);
            //oid ControlClients(Exchange::IPower::PCState state);

            void RegisterAll();
            void UnregisterAll();
            //inline Exchange::IPower::PCState TranslateIn(JsonData::Power::StateType value);
            //inline JsonData::Power::StateType TranslateOut(Exchange::IPower::PCState value) const;
            uint32_t endpoint_addfile(const JsonData::FilesWatcher::FileInfo &params);
            uint32_t endpoint_removeFile(const JsonData::FilesWatcher::FileInfo &params);
            uint32_t get_listOfWatchedFiles(Core::JSON::ArrayType<Core::JSON::String> &response) const;

            uint32_t AddFile(const string &file);
            uint32_t RemoveFile(const string &file);

            typedef std::unordered_set<string> ListOfFiles;

            void Updated() override
            {
                // here we receive notification from the system that the file has been changed --> fire notification for users
                std::cout << "File changed!";
            }

            bool Register(const string &filename)
            {
                // here we receive notification from the system that the file has been changed --> fire notification for users
                Core::FileSystemMonitor::Instance().Register(this, filename);
                return true;
            }

            bool UnRegister(const string &filename)
            {
                // here we receive notification from the system that the file has been changed --> fire notification for users
                Core::FileSystemMonitor::Instance().Unregister(this, filename);
                return true;
            }

        private:
            Core::CriticalSection _adminLock;
            uint32_t _skipURL;
            PluginHost::IShell *_service;
            std::list<Exchange::IFilesWatcher::INotification *> _notificationClients;
            ListOfFiles _listOfFiles;
        };
    } //namespace Plugin
} //namespace WPEFramework

#endif // __FilesWatcher_H
