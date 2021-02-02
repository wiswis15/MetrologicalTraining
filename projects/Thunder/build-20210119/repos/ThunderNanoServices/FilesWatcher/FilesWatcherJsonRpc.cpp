
#include "Module.h"
#include <interfaces/json/JsonData_FilesWatcher.h>
#include "FilesWatcher.h"


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

   
   
    // Registration
    //

    void FilesWatcher::RegisterAll()
    {
        PluginHost::JSONRPC::Register<WPEFramework::JsonData::FilesWatcher::FileInfo,void>(_T("addfile"), &FilesWatcher::endpoint_addfile, this);
        PluginHost::JSONRPC::Register<WPEFramework::JsonData::FilesWatcher::FileInfo,void>(_T("removeFile"), &FilesWatcher::endpoint_removeFile, this);
        PluginHost::JSONRPC::Property<Core::JSON::ArrayType<Core::JSON::String>>(_T("listOfWatchedFiles"), &FilesWatcher::get_listOfWatchedFiles, nullptr,this);
    }

    void FilesWatcher::UnregisterAll()
    {
        PluginHost::JSONRPC::Unregister(_T("removeFile"));
        PluginHost::JSONRPC::Unregister(_T("addfile"));
        PluginHost::JSONRPC::Unregister(_T("listOfWatchedFiles"));
    }

    // API implementation
    //

    // Method: addfile - Adds a file to the list of watched files
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL: General failure
    //  - ERROR_BAD_REQUEST: File does not exist
    uint32_t FilesWatcher::endpoint_addfile(const WPEFramework::JsonData::FilesWatcher::FileInfo& params)
    {
        std::string path=params.Path.Value();
        return AddFile(path);
    }

    // Method: removeFile - Stop watching a file
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL: General failure
    //  - ERROR_BAD_REQUEST: File is not being watched
    uint32_t FilesWatcher::endpoint_removeFile(const WPEFramework::JsonData::FilesWatcher::FileInfo& params)
    {
        std::string path=params.Path.Value();
        return RemoveFile(path);
    }

/*     // Property: state - FilesWatcher plugin state
    // Return codes:
    //  - ERROR_NONE: Success
    uint32_t FilesWatcher::get_state(Core::JSON::EnumType<StateType>& response) const
    {
        // response = ...

        return Core::ERROR_NONE;
    } */

    // Property: listOfWatchedFiles - list of watched files
    // Return codes:
    //  - ERROR_NONE: Success
    uint32_t FilesWatcher::get_listOfWatchedFiles(Core::JSON::ArrayType<Core::JSON::String>& response) const
    {
        for (auto& path :_listOfFiles)
        {
            response.Add(Core::JSON::String(path));
        }

        return Core::ERROR_NONE;
    }

} // namespace Plugin

}

