#include <fcntl.h>

#include "RemoteControl.h"
#include "RemoteAdministrator.h"

namespace WPEFramework {
namespace Plugin {

    static const string DefaultMappingTable (_T("keymap"));
    static Core::ProxyPoolType<Web::JSONBodyType<RemoteControl::Data> > jsonResponseFactory(4);
    static Core::ProxyPoolType<Web::JSONBodyType<PluginHost::VirtualInput::KeyMap::KeyMapEntry> > jsonCodeFactory(1);

    SERVICE_REGISTRATION(RemoteControl, 1, 0);

    class KeyActivity {
    private:
        // -------------------------------------------------------------------
        // This object should not be copied or assigned. Prevent the copy
        // constructor and assignment constructor from being used. Compiler
        // generated assignment and copy methods will be blocked by the
        // following statments.
        // Define them but do not implement them, compile error/link error.
        // -------------------------------------------------------------------
        KeyActivity(const KeyActivity& a_Copy) = delete;
        KeyActivity& operator= (const KeyActivity& a_RHS) = delete;

    public:
        KeyActivity(const string& mapName, const uint32 code, const bool pressed) :
                _text (Core::ToString(Trace::Format(_T("Inserted Code: [%s:%08X] state %s."), mapName.c_str(), code, (pressed ? _T("pressed") : _T("released")))))
        {
        }
        ~KeyActivity()
        {
        }

    public:
        inline const char* Data() const {
            return (_text.c_str());
        }
        inline uint16 Length() const {
            return (static_cast<uint16>(_text.length()));
        }

    private:
        const std::string _text;
    };

    class UnknownKey {
    private:
        // -------------------------------------------------------------------
        // This object should not be copied or assigned. Prevent the copy
        // constructor and assignment constructor from being used. Compiler
        // generated assignment and copy methods will be blocked by the
        // following statments.
        // Define them but do not implement them, compile error/link error.
        // -------------------------------------------------------------------
        UnknownKey(const UnknownKey& a_Copy) = delete;
        UnknownKey& operator= (const UnknownKey& a_RHS) = delete;

    public:
        UnknownKey(const string& mapName, const uint32 code, const bool pressed, const uint32 result)
        {
            const TCHAR* text (pressed ? _T("pressed") : _T("released"));

            if (result == Core::ERROR_UNKNOWN_TABLE) {
                _text = Core::ToString(Trace::Format(_T("Invalid table: [%s,%08X]"), mapName.c_str(), code));
            }
            else if (result == Core::ERROR_UNKNOWN_KEY) {
                _text = Core::ToString(Trace::Format(_T("Unknown: [%s:%08X] state %s, blocked."), mapName.c_str(), code, text));
            }
            else if (result == Core::ERROR_UNKNOWN_KEY_PASSED) {
                _text = Core::ToString(Trace::Format(_T("Unknown: [%s:%08X] state %s, passed."), mapName.c_str(), code, text));
            }
        }
        ~UnknownKey()
        {
        }

    public:
        inline const char* Data() const {
            return (_text.c_str());
        }
        inline uint16 Length() const {
            return (static_cast<uint16>(_text.length()));
        }

    private:
        std::string _text;
    };

    static Core::ProxyPoolType<Web::TextBody> _remoteInfo(2);

    static string MappingFile (const string& fileName, const string& directory1, const string& directory2) {

        string result (directory1 + fileName);
        Core::File testObject (directory1 + fileName);

        if ((testObject.Exists() == false) || (testObject.IsDirectory() == true)) {
            result = directory2 + fileName;
            testObject = result;

            if ((testObject.Exists() == false) || (testObject.IsDirectory() == true)) {
                result.clear();
            }
        }

        return (result);
    }

    static string MappingFile (Core::JSON::ArrayType<RemoteControl::Config::Mapping>::Iterator& index, const TCHAR* device, const string& directory1, const string& directory2) {

        bool mapped = false;
        string loadName(device);

        index.Reset();

        while ( (index.Next() == true) && (index.Current().Device.Value() != loadName) ) { /* intentionally left empty */ }

        if (index.IsValid() == true) {
            // We found an overruling name.
            loadName = index.Current().MapFile.Value();
        }
        else {
            loadName += _T(".json");
        }

        return (MappingFile (loadName, directory1, directory2));
    }

#ifdef __WIN32__
#pragma warning( disable : 4355 )
#endif
    RemoteControl::RemoteControl()
            : _skipURL(0)
            , _inputHandler(PluginHost::InputHandler::KeyHandler())
            , _sink(this)
            , _persistentPath()
    {
        //ASSERT(_inputHandler == nullptr);
    }
#ifdef __WIN32__
#pragma warning( default : 4355 )
#endif

    /* virtual */ RemoteControl::~RemoteControl()
    {
    }

    /* virtual */ const string RemoteControl::Initialize(PluginHost::IShell* service)
    {
        string result;
        RemoteControl::Config config;
        config.FromString(service->ConfigLine());
        string mappingFile (MappingFile(config.MapFile.Value(), service->PersistentPath(), service->DataPath()));

        // First check that we at least can create a default lookup table.
        if (mappingFile.empty() == false) {

            TRACE(Trace::Information, (_T("Opening map file: %s"), mappingFile.c_str()));
            TRACE_L1(_T("Opening map file: %s"), mappingFile.c_str());

            // Keep this path for save operation
            _persistentPath = service->PersistentPath();

            // Seems like we have a default mapping file. Load it..
            PluginHost::VirtualInput::KeyMap& map (_inputHandler->Table(DefaultMappingTable));

            if (map.Load(mappingFile) == Core::ERROR_NONE) {

                map.PassThrough(config.PassOn.Value());

                if (_inputHandler != nullptr) {
                    Remotes::RemoteAdministrator& admin(Remotes::RemoteAdministrator::Instance());

                    // Strawl over all remotes (inputs) and see if you need to load mapping tables.
                    Remotes::RemoteAdministrator::Iterator index (admin.Producers());
                    Core::JSON::ArrayType<RemoteControl::Config::Mapping>::Iterator configList (config.Specific.Elements());
                    while (index.Next() == true) {

                        const TCHAR* producer ((*index)->Name());

                        TRACE_L1(_T("Searching map file for: %s"), producer);

                        // See if we need to load a table.
                        string specific (MappingFile(configList, producer, service->PersistentPath(), service->DataPath()));

                        if (specific.empty() == false) {

                            TRACE(Trace::Information, (_T("Opening map file: %s"), specific.c_str()));
                            TRACE_L1(_T("Opening map file: %s"), specific.c_str());

                            // Get our selves a table..
                            PluginHost::VirtualInput::KeyMap& map (_inputHandler->Table(producer));
                            map.Load(specific);
                            if (configList.IsValid() == true) {
                                map.PassThrough(configList.Current().PassOn.Value());
                                if (configList.Current().CodeMask.IsSet()) {
                                    uint32 codeMask(configList.Current().CodeMask.Value());
                                    const_cast<Exchange::IKeyProducer*>(*index)->CodeMask(codeMask);
                                }
                            }
                        }
                    }

                    _skipURL = service->WebPrefix().length();
                    _inputHandler->Interval(config.RepeatStart.Value(), config.RepeatInterval.Value());
                    _inputHandler->Default(DefaultMappingTable);
                    admin.Callback(this);
                } else {
                    result = "Could not open VirtualInput device.";
                }
            }
        }

        // On succes return NULL, to indicate there is no error text.
        return (result);
    }

    /* virtual */ void RemoteControl::Deinitialize(PluginHost::IShell* service)
    {

        Remotes::RemoteAdministrator& admin(Remotes::RemoteAdministrator::Instance());
        Remotes::RemoteAdministrator::Iterator index (admin.Producers());

        // Clear all injected device key maps
        while (index.Next() == true) {

            const TCHAR* producer ((*index)->Name());

            _inputHandler->ClearTable(producer);
        }

        // Clear default key map
        _inputHandler->Default(EMPTY_STRING);
        _inputHandler->ClearTable(DefaultMappingTable);

        _sink.Deinitialize(service);
        Remotes::RemoteAdministrator::Instance().RevokeAll();

    }

    /* virtual */ string RemoteControl::Information() const
    {
        // No additional info to report.
        return (EMPTY_STRING);
    }

    /* virtual */ void RemoteControl::Inbound(Web::Request& /* request */)
    {
    }

    /* virtual */ Core::ProxyType<Web::Response> RemoteControl::Process(const Web::Request& request)
    {

        ASSERT(_skipURL <= request.Path.length());

        Core::ProxyType<Web::Response> result;
        Core::TextSegmentIterator index(Core::TextFragment(request.Path, _skipURL, request.Path.length() - _skipURL), false, '/');

        // By default, we are in front of any element, jump onto the first element, which is if, there is something an empty slot.
        index.Next();

        // If there is nothing or only a slashe, we will now jump over it, and otherwise, we have data.
        if (request.Verb == Web::Request::HTTP_GET) {

            result = GetMethod(index);
        }
        else if (request.Verb == Web::Request::HTTP_PUT) {

            result = PutMethod(index, request);
        }
        else if (request.Verb == Web::Request::HTTP_POST) {

            result = PostMethod(index, request);
        }
        else if (request.Verb == Web::Request::HTTP_DELETE) {

            result = DeleteMethod(index);
        }
        else {
            result = PluginHost::Factories::Instance().Response();
            result->ErrorCode = Web::STATUS_NOT_IMPLEMENTED;
            result->Message = string(_T("Unknown request path specified."));
        }

        return (result);
    }

    /* virtual */ uint32 RemoteControl::KeyEvent(const bool pressed, const uint32 code, const string& mapName)
    {

        uint32 result = _inputHandler->KeyEvent(pressed, code, mapName);

        if (result == Core::ERROR_NONE) {
            TRACE(KeyActivity, (mapName, code, pressed));
        }
        else {
            TRACE(UnknownKey, (mapName, code, pressed, result));
        }
        return (result);
    }

    const string RemoteControl::FindDevice(Core::TextSegmentIterator& index) const
    {
        string deviceName;

        if (index.Next() == true) {
            if (index.Current() != DefaultMappingTable) {

                Remotes::RemoteAdministrator &admin(Remotes::RemoteAdministrator::Instance());
                Remotes::RemoteAdministrator::Iterator remoteDevices(admin.Producers());

                while (remoteDevices.Next() == true) {
                    const TCHAR *producer((*remoteDevices)->Name());
                    if (index.Current() == producer) {
                        deviceName = producer;
                        break;
                    }
                }
            } else {

                deviceName = DefaultMappingTable;
            }
        }
        return (deviceName);
    }

    Core::ProxyType<Web::Response> RemoteControl::GetMethod(Core::TextSegmentIterator& index) const
    {

        Core::ProxyType<Web::Response> result(PluginHost::Factories::Instance().Response());

        result->ErrorCode = Web::STATUS_NOT_IMPLEMENTED;
        result->Message = string(_T("Unknown request path specified."));

        if (index.IsValid() == true) {
            // Perform operations on that specific device
            const string deviceName = FindDevice(index);

            if (deviceName.empty() == false) {
                // GET .../RemoteControl/<DEVICE_NAME>/Code/XXX : Get code of DEVICE_NAME
                if (index.Next() == true) {

                    if (index.Current() == _T("Code")) {
                        // move over the Code keyword.
                        if (index.Next() == true) {

                            result->ErrorCode = Web::STATUS_NO_CONTENT;
                            result->Message = string(_T("Key does not exist in ") + deviceName);

                            Core::NumberType <uint32> code(index.Current());

                            if (code.Value() != 0) {
                                // Load default or specific device mapping
                                PluginHost::VirtualInput::KeyMap &map(_inputHandler->Table(deviceName));

                                const PluginHost::VirtualInput::KeyMap::ConversionInfo *codeElements = map[code];
                                if (codeElements != nullptr) {

                                    Core::ProxyType<Web::JSONBodyType<PluginHost::VirtualInput::KeyMap::KeyMapEntry> > response(jsonCodeFactory.Element());

                                    response->Code = code;
                                    response->Key = codeElements->Code;

                                    uint16 flag(1);
                                    uint16 modifiers(codeElements->Modifiers);

                                    while (modifiers != 0) {

                                        if ((modifiers & 0x01) != 0) {
                                            switch (flag) {
                                                case PluginHost::VirtualInput::KeyMap::modifier::LEFTSHIFT:
                                                case PluginHost::VirtualInput::KeyMap::modifier::RIGHTSHIFT:
                                                case PluginHost::VirtualInput::KeyMap::modifier::LEFTALT:
                                                case PluginHost::VirtualInput::KeyMap::modifier::RIGHTALT:
                                                case PluginHost::VirtualInput::KeyMap::modifier::LEFTCTRL:
                                                case PluginHost::VirtualInput::KeyMap::modifier::RIGHTCTRL: {
                                                    Core::JSON::EnumType <PluginHost::VirtualInput::KeyMap::modifier> &jsonRef = response->Modifiers.Add();
                                                    jsonRef = static_cast<PluginHost::VirtualInput::KeyMap::modifier>(flag);
                                                    break;
                                                }
                                                default:
                                                    ASSERT(false);
                                                    break;
                                            }
                                        }

                                        flag = flag << 1;
                                        modifiers = modifiers >> 1;
                                    }

                                    result->ErrorCode = Web::STATUS_OK;
                                    result->Message = string(_T("Get key info of ") + deviceName);
                                    result->ContentType = Web::MIMETypes::MIME_JSON;
                                    result->Body(Core::proxy_cast<Web::IBody>(response));
                                }
                            }
                        }
                        else {
                            result->ErrorCode = Web::STATUS_NOT_FOUND;
                            result->Message = string(_T("No key code in request"));
                        }
                    }
                }
                // GET .../RemoteControl/<DEVICE_NAME> : Return metadata of specific DEVICE_NAME
                else {

                    if (deviceName == DefaultMappingTable) {

                        result->ErrorCode = Web::STATUS_OK;
                        result->Message = string(_T("Default device is loaded"));
                    } else {

                        uint32 error = Remotes::RemoteAdministrator::Instance().Error(deviceName);

                        if (error == Core::ERROR_NONE) {

                            result->ErrorCode = Web::STATUS_OK;
                            result->Message = string(_T("Specific device is loaded"));

                            Core::ProxyType<Web::TextBody> body(_remoteInfo.Element());

                            (*body) = Remotes::RemoteAdministrator::Instance().MetaData(deviceName);
                            result->Body<Web::TextBody>(body);
                        } else {

                            result->ErrorCode = Web::STATUS_BAD_GATEWAY;
                            result->Message = string(_T("Error during loading of device. ErrorCode: ")) +
                                              Core::NumberType<uint32>(error).Text();
                        }

                    }
                }
            }
        }
        // GET .../RemoteControl : Return name-list of all registered devices
        else {

            Core::ProxyType<Web::JSONBodyType<Data> > response(jsonResponseFactory.Element());

            // Add default mapping name
            Data::DeviceInfo newElement;
            newElement.Name = DefaultMappingTable;
            response->Devices.Add(newElement);

            // Look at specific devices, if we have, append them to response
            Remotes::RemoteAdministrator& admin(Remotes::RemoteAdministrator::Instance());
            Remotes::RemoteAdministrator::Iterator remoteDevices (admin.Producers());

            while (remoteDevices.Next() == true) {

                Data::DeviceInfo newElement;
                newElement.Name = (*remoteDevices)->Name();
                response->Devices.Add(newElement);
            }

            result->ErrorCode  = Web::STATUS_OK;
            result->Message = string(_T("List of loaded remote devices"));
            result->ContentType = Web::MIMETypes::MIME_JSON;
            result->Body(Core::proxy_cast<Web::IBody>(response));
        }

        return (result);
    }

    Core::ProxyType<Web::Response> RemoteControl::PutMethod(Core::TextSegmentIterator& index, const Web::Request& request)
    {

        Core::ProxyType<Web::Response> result(PluginHost::Factories::Instance().Response());

        result->ErrorCode = Web::STATUS_NOT_IMPLEMENTED;
        result->Message = string(_T("Unknown request path specified."));

        // This is a PUT request, search command
        // PUT RemoteControl/<DEVICE_NAME>/<COMMAND>?<QUERY-1>&<QUERY-2>&<QUERY-N>
        if (index.IsValid() == true) {
            // Perform operations on that specific device
            const string deviceName = FindDevice(index);

            if (deviceName.empty() == false) {

                if (index.Next() == true) {
                    // PUT .../RemoteControl/<DEVICE_NAME>/PairingMode : activate pairing mode of specific DEVICE_NAME
                    if (index.Current() == _T("PairingMode")) {

                        if (Remotes::RemoteAdministrator::Instance().Pairing(deviceName) == true) {
                            result->ErrorCode = Web::STATUS_OK;
                            result->Message = string(_T("Pairing mode active: ") + deviceName);
                        }
                        else {
                            result->ErrorCode = Web::STATUS_BAD_REQUEST;
                            result->Message = string(_T("Failed to activate pairing: ") + deviceName);
                        }
                    }
                    // PUT .../RemoteControl/<DEVICE_NAME>/Code/XXX : send a code to DEVICE_NAME
                    else if (index.Current() == _T("Code")) {
                        // move over the Code keyword.
                        if (index.Next() == true) {

                            result->ErrorCode = Web::STATUS_NO_CONTENT;
                            result->Message = string(_T("Key does not exist in ") + deviceName);

                            // Send the keypress if it can be found...
                            Core::NumberType <uint32> code(index.Current());
                            if (code.Value() != 0) {

                                uint32 errCode = KeyEvent(true, code, deviceName);
                                if (errCode ==  Core::ERROR_NONE) {

                                    errCode = KeyEvent(false, code, deviceName);
                                    if (errCode ==  Core::ERROR_NONE) {

                                        result->ErrorCode = Web::STATUS_OK;
                                        result->Message = string(_T("Soft key is sent to ") + deviceName);
                                    }
                                }
                            }
                        }
                        else {
                            result->ErrorCode = Web::STATUS_NOT_FOUND;
                            result->Message = string(_T("No key code in request"));
                        }
                    }
                        // PUT .../RemoteControl/<DEVICE_NAME>/Add?Code=XXX&Key=YYY&[Modifier=0xZZZ] : add a new pair to specific DEVICE_NAME
                    else if (index.Current() == _T("Add")) {

                        // For Add command; we need Code, Key and Modifier queries
                        Core::URL::KeyValue options(request.Query.Value());

                        Core::NumberType <uint32> code(options.Number<uint32>(_T("Code"), 0));
                        Core::NumberType <uint16> key(options.Number<uint16>(_T("Key"), 0));
                        Core::NumberType <uint32> modifier(options.Number<uint32>(_T("Modifier"), 0));

                        result->ErrorCode = Web::STATUS_METHOD_NOT_ALLOWED;
                        result->Message = string(_T("Code is not added"));

                        // Valid code-key pair
                        if (code.Value() != 0 && key.Value() != 0) {
                            // Load default or specific device mapping
                            PluginHost::VirtualInput::KeyMap &map(_inputHandler->Table(deviceName));

                            if (map.Add(code, key, modifier) == true) {
                                result->ErrorCode = Web::STATUS_OK;
                                result->Message = string(_T("Code is added"));
                            }
                            else {
                                result->Message = string(_T("Code already exists"));
                            }
                        }
                    }
                    // PUT .../RemoteControl/<DEVICE_NAME>/Save : Save the loaded key map as DEVICE_NAME.json into persistent path
                    else if (index.Current() == _T("Save")) {

                        result->ErrorCode = Web::STATUS_METHOD_NOT_ALLOWED;
                        result->Message = string(_T("File is not created"));

                        string fileName;
                        if (_persistentPath.empty() == false) {
                            Core::Directory directory(_persistentPath.c_str());
                            if (directory.CreatePath()) {
                                fileName = _persistentPath + deviceName + _T(".json");
                            }
                        }

                        if (fileName.empty() == false) {
                            // Seems like we have a default mapping file. Load it..
                            PluginHost::VirtualInput::KeyMap &map(_inputHandler->Table(deviceName));

                            if (map.Save(fileName) == Core::ERROR_NONE) {
                                result->ErrorCode = Web::STATUS_OK;
                                result->Message = string(_T("File is created: " + fileName));
                            }
                        }
                    }
                    // PUT .../RemoteControl/<DEVICE_NAME>/Load : Re-load DEVICE_NAME.json into memory
                    else if (index.Current() == _T("Load")) {

                        result->ErrorCode = Web::STATUS_METHOD_NOT_ALLOWED;
                        result->Message = string(_T("File does not exist"));

                        string fileName;
                        if (_persistentPath.empty() == false) {
                            Core::Directory directory(_persistentPath.c_str());
                            if (directory.CreatePath()) {
                                fileName = _persistentPath + deviceName + _T(".json");
                            }
                        }

                        if (fileName.empty() == false) {
                            PluginHost::VirtualInput::KeyMap &map(_inputHandler->Table(deviceName));

                            if (map.Load(fileName) == Core::ERROR_NONE) {
                                result->ErrorCode = Web::STATUS_OK;
                                result->Message = string(_T("File is reloaded: " + deviceName));
                            }
                        }
                    }
                }
            }
        }
        return (result);
    }

    Core::ProxyType<Web::Response> RemoteControl::DeleteMethod(Core::TextSegmentIterator& index)
    {
        Core::ProxyType<Web::Response> result(PluginHost::Factories::Instance().Response());

        result->ErrorCode = Web::STATUS_NOT_IMPLEMENTED;
        result->Message = string(_T("Unknown request path specified."));

        // All delete commands require an additional parameter, so go look for it.
        // DELETE .../RemoteControl/<DEVICE_NAME>/Code/XXX : delete code from mapping of DEVICE_NAME
        if (index.IsValid() == true) {
            // Perform operations on that specific device
            const string deviceName = FindDevice(index);

            if (deviceName.empty() == false) {

                if (index.Next() == true) {

                    if (index.Current() == _T("Code")) {
                        // move over the Code keyword.
                        if (index.Next() == true) {

                            result->ErrorCode = Web::STATUS_NO_CONTENT;
                            result->Message = string(_T("Key does not exist in ") + deviceName);

                            // Send the keypress if it can be found...
                            Core::NumberType <uint32> code(index.Current());
                            if (code.Value() != 0) {

                                // Load default or specific device mapping
                                PluginHost::VirtualInput::KeyMap &map(_inputHandler->Table(deviceName));

                                map.Delete(code);

                                result->ErrorCode = Web::STATUS_OK;
                                result->Message = string(_T("Code is deleted"));
                            }
                        }
                        else {
                            result->ErrorCode = Web::STATUS_NOT_FOUND;
                            result->Message = string(_T("No key code in request"));
                        }
                    }
                }
            }
        }
        return (result);
    }

    Core::ProxyType<Web::Response> RemoteControl::PostMethod(Core::TextSegmentIterator& index, const Web::Request& request)
    {
        Core::ProxyType<Web::Response> result(PluginHost::Factories::Instance().Response());

        result->ErrorCode = Web::STATUS_NOT_IMPLEMENTED;
        result->Message = string(_T("Unknown request path specified."));

        if (index.IsValid() == true) {

            // Perform operations on that specific device
            const string deviceName = FindDevice(index);

            if (deviceName.empty() == false) {

                if (index.Next() == true) {
                    // POST .../RemoteControl/<DEVICE_NAME>/Modify?Code=XXX&Key=YYY&[Modifier=0xZZZ] : Modify a new pair in specific DEVICE_NAME
                    if (index.Current() == _T("Modify")) {
                        // For Modify command; we need Code, Key and Modifier queries
                        Core::URL::KeyValue options(request.Query.Value());

                        Core::NumberType <uint32> code(options.Number<uint32>(_T("Code"), 0));
                        Core::NumberType <uint16> key(options.Number<uint16>(_T("Key"), 0));
                        Core::NumberType <uint32> modifier(options.Number<uint32>(_T("Modifier"), 0));

                        result->ErrorCode = Web::STATUS_METHOD_NOT_ALLOWED;
                        result->Message = string(_T("Code is not modified"));

                        // Valid code-key pair
                        if (code.Value() != 0 && key.Value() != 0) {
                            // Load default or specific device mapping
                            PluginHost::VirtualInput::KeyMap &map(_inputHandler->Table(deviceName));

                            if (map.Modify(code, key, modifier) == true) {
                                result->ErrorCode = Web::STATUS_OK;
                                result->Message = string(_T("Code is modified"));
                            } else {
                                result->Message = string(_T("Code does not exist"));
                            }
                        }
                    }
                }
            }
        }
        return (result);
    }
}
}
