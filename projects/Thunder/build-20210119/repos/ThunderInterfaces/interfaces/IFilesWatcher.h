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
 * See the License setfor the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include "Module.h"
#include <unordered_set> 

namespace WPEFramework {
namespace Exchange {

    struct EXTERNAL IFilesWatcher : virtual public Core::IUnknown {


        struct EXTERNAL INotification : virtual public Core::IUnknown {

            virtual void FileChanged(const string) = 0;
        };

        virtual void Register(IFilesWatcher::INotification* sink) = 0;
        virtual void Unregister(IFilesWatcher::INotification* sink) = 0;

	virtual std::unordered_set<string> GetListWatchedFiles() const = 0;


    };
}
}

