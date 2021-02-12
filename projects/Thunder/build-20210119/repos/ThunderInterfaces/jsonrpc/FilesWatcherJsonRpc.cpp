
#include "Module.h"
#include "FilesWatcher.h"
#include <interfaces/json/JsonData_FilesWatcher.h>

/*
    // Copy the code below to FilesWatcher class definition
    // Note: The FilesWatcher class must inherit from PluginHost::JSONRPC

    private:
        void RegisterAll();
        void UnregisterAll();
        uint32_t endpoint_addfile(const JsonData::FilesWatcher::FileInfo& params);
        uint32_t endpoint_removeFile(const JsonData::FilesWatcher::FileInfo& params);
        uint32_t get_state(Core::JSON::EnumType<JsonData::FilesWatcher::StateType>& response) const;
        uint32_t get_listOfWatchedFiles(Core::JSON::ArrayType<Core::JSON::String>& response) const;
*/

namespace WPEFramework {

namespace Plugin {

    using namespace JsonData::FilesWatcher;

    // Registration
    //

    void FilesWatcher::RegisterAll()
    {
        Register<FileInfo,void>(_T("addfile"), &FilesWatcher::endpoint_addfile, this);
        Register<FileInfo,void>(_T("removeFile"), &FilesWatcher::endpoint_removeFile, this);
        Property<Core::JSON::EnumType<StateType>>(_T("state"), &FilesWatcher::get_state, nullptr, this);
        Property<Core::JSON::ArrayType<Core::JSON::String>>(_T("listOfWatchedFiles"), &FilesWatcher::get_listOfWatchedFiles, nullptr, this);
    }

    void FilesWatcher::UnregisterAll()
    {
        Unregister(_T("removeFile"));
        Unregister(_T("addfile"));
        Unregister(_T("listOfWatchedFiles"));
        Unregister(_T("state"));
    }

    // API implementation
    //

    // Method: addfile - Adds a file to the list of watched files
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL: General failure
    //  - ERROR_BAD_REQUEST: File does not exist
    uint32_t FilesWatcher::endpoint_addfile(const FileInfo& params)
    {
        uint32_t result = Core::ERROR_NONE;
        const string& path = params.Path.Value();

        // TODO...

        return result;
    }

    // Method: removeFile - Stop watching a file
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL: General failure
    //  - ERROR_BAD_REQUEST: File is not being watched
    uint32_t FilesWatcher::endpoint_removeFile(const FileInfo& params)
    {
        uint32_t result = Core::ERROR_NONE;
        const string& path = params.Path.Value();

        // TODO...

        return result;
    }

    // Property: state - FilesWatcher plugin state
    // Return codes:
    //  - ERROR_NONE: Success
    uint32_t FilesWatcher::get_state(Core::JSON::EnumType<StateType>& response) const
    {
        // response = ...

        return Core::ERROR_NONE;
    }

    // Property: listOfWatchedFiles - list of watched files
    // Return codes:
    //  - ERROR_NONE: Success
    uint32_t FilesWatcher::get_listOfWatchedFiles(Core::JSON::ArrayType<Core::JSON::String>& response) const
    {
        // response = ...

        return Core::ERROR_NONE;
    }

} // namespace Plugin

}

