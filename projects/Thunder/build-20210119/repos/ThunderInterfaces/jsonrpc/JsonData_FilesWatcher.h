
// C++ classes for FilesWatcher API JSON-RPC API.
// Generated automatically from 'FilesWatcher.json'. DO NOT EDIT.

// Note: This code is inherently not thread safe. If required, proper synchronisation must be added.

#pragma once

#include <core/JSON.h>
#include <core/Enumerate.h>

namespace WPEFramework {

namespace JsonData {

    namespace FilesWatcher {

        // Common enums
        //

        // Files watcher plugin state
        enum StateType {
            ACTIVE = 1,
            INACTIVE = 2
        };

        // Common classes
        //

        class FileInfo : public Core::JSON::Container {
        public:
            FileInfo()
                : Core::JSON::Container()
            {
                Add(_T("path"), &Path);
            }

            FileInfo(const FileInfo&) = delete;
            FileInfo& operator=(const FileInfo&) = delete;

        public:
            Core::JSON::String Path; // path of the file to watch
        }; // class FileInfo

        // Method params/result classes
        //

    } // namespace FilesWatcher

} // namespace JsonData

// Enum conversion handlers
ENUM_CONVERSION_HANDLER(JsonData::FilesWatcher::StateType);

}

