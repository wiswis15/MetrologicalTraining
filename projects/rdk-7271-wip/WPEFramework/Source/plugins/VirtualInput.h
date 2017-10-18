#ifndef __VIRTUAL_INPUT__
#define __VIRTUAL_INPUT__

#include "Module.h"

namespace WPEFramework {
namespace PluginHost {

    class EXTERNAL VirtualInput {
    private:
        VirtualInput(const VirtualInput&) = delete;
        VirtualInput& operator=(const VirtualInput&) = delete;

        class RepeatKeyTimer : Core::WatchDogType<RepeatKeyTimer&> {
        private:
            RepeatKeyTimer() = delete;
            RepeatKeyTimer(const RepeatKeyTimer&) = delete;
            RepeatKeyTimer& operator=(const RepeatKeyTimer&) = delete;

            typedef Core::WatchDogType<RepeatKeyTimer&> BaseClass;
            static constexpr uint32 RepeatStackSize = 64 * 1024;

        public:
#ifdef __WIN32__
#pragma warning(disable : 4355)
#endif
            RepeatKeyTimer(VirtualInput& parent)
                : BaseClass(RepeatStackSize, _T("RepeatKeyTimer"), *this)
                , _parent(parent)
                , _startTime(0)
                , _intervalTime(0)
                , _code(0)
            {
            }
#ifdef __WIN32__
#pragma warning(default : 4355)
#endif
            ~RepeatKeyTimer()
            {
            }

        public:
            void Interval(const uint16 startTime, const uint16 intervalTime)
            {
                _startTime = startTime;
                _intervalTime = intervalTime;
            }
            void Arm(const uint16 code)
            {
                if (_startTime != 0) {
                    BaseClass::Lock();

                    BaseClass::Arm(_startTime);
                    _code = code;

                    BaseClass::Unlock();
                }
            }
            void Reset()
            {
                if (_startTime != 0) {
                    BaseClass::Reset();
                }
            }
            uint32 Expired()
            {
                _parent.RepeatKey(_code);

                // Let's retrigger at the same time..
                return (_intervalTime);
            }

            virtual void SelectListener(const string& /* listener */)
            {
            }

        private:
            VirtualInput& _parent;
            uint16 _startTime;
            uint16 _intervalTime;
            uint16 _code;
        };

        enum enumModifier {
            LEFTSHIFT = 0, //  0..3  bits are for LeftShift reference counting (max 15)
            RIGHTSHIFT = 4, //  4..7  bits are for RightShift reference counting (max 15)
            LEFTALT = 8, //  8..11 bits are for LeftAlt reference counting (max 15)
            RIGHTALT = 12, // 12..15 bits are for RightAlt reference counting (max 15)
            LEFTCTRL = 16, // 16..19 bits are for LeftCtrl reference counting (max 15)
            RIGHTCTRL = 20 // 20..23 bits are for RightCtrl reference counting (max 15)
        };

    public:
        class EXTERNAL KeyMap {
        public:
            enum modifier {
                LEFTSHIFT = 0x01,
                RIGHTSHIFT = 0x02,
                LEFTALT = 0x04,
                RIGHTALT = 0x08,
                LEFTCTRL = 0x10,
                RIGHTCTRL = 0x20
            };

        private:
            typedef Core::JSON::EnumType<modifier> JSONModifier;

            KeyMap(const KeyMap&) = delete;
            KeyMap& operator=(const KeyMap&) = delete;

        public:
            class KeyMapEntry : public Core::JSON::Container {
            private:
                KeyMapEntry& operator=(const KeyMapEntry&) = delete;

            public:
                inline KeyMapEntry()
                    : Core::JSON::Container()
                {
                    Add(_T("code"), &Code);
                    Add(_T("key"), &Key);
                    Add(_T("modifiers"), &Modifiers);
                }
                inline KeyMapEntry(const KeyMapEntry& copy)
                    : Core::JSON::Container()
                    , Code(copy.Code)
                    , Key(copy.Key)
                    , Modifiers(copy.Modifiers)
                {
                    Add(_T("code"), &Code);
                    Add(_T("key"), &Key);
                    Add(_T("modifiers"), &Modifiers);
                }
                virtual ~KeyMapEntry()
                {
                }

            public:
                Core::JSON::HexUInt32 Code;
                Core::JSON::DecUInt16 Key;
                Core::JSON::ArrayType<JSONModifier> Modifiers;
            };

        public:
            struct ConversionInfo {
                uint16 Code;
                uint16 Modifiers;
            };

        private:
            typedef std::map<const uint32, const ConversionInfo> LookupMap;

        public:
            typedef Core::IteratorMapType<const LookupMap, const ConversionInfo&, uint32, LookupMap::const_iterator> Iterator;

        public:
            KeyMap(KeyMap&&) = default;
            KeyMap(VirtualInput& parent)
                : _parent(parent)
                , _passThrough(false)
            {
            }
            ~KeyMap()
            {

                std::map<uint16, sint16 > removedKeys;

                while (_keyMap.size() > 0) {

                    // Negative reference counts
                    removedKeys[_keyMap.begin()->second.Code]--;

                    _keyMap.erase(_keyMap.begin());

                }

                ChangeIterator removed(removedKeys);
                _parent.MapChanges(removed);
            }

        public:
            inline bool PassThrough() const
            {
                return (_passThrough);
            }
            inline void PassThrough(const bool enabled)
            {
                _passThrough = enabled;
            }
            uint32 Load(const string& mappingFile);
            uint32 Save(const string& mappingFile);

            inline const ConversionInfo* operator[](const uint32 code) const
            {
                std::map<const uint32, const ConversionInfo>::const_iterator index(_keyMap.find(code));

                return (index != _keyMap.end() ? &(index->second) : nullptr);
            }
            inline bool Add(const uint32 code, const uint16 key, const uint16 modifiers)
            {
                bool added = false;
                std::map<const uint32, const ConversionInfo>::const_iterator index(_keyMap.find(code));

                if (index == _keyMap.end()) {
                    ConversionInfo element;
                    element.Code = key;
                    element.Modifiers = modifiers;

                    _keyMap.insert(std::pair<const uint32, const ConversionInfo>(code, element));
                    added = true;
                }
                return (added);
            }
            inline void Delete(const uint32 code)
            {
                std::map<const uint32, const ConversionInfo>::const_iterator index(_keyMap.find(code));

                if (index != _keyMap.end()) {
                    _keyMap.erase(index);
                }
            }

            inline bool Modify(const uint32 code, const uint16 key, const uint16 modifiers)
            {
                // Delete if exist
                Delete(code);

                return (Add(code, key, modifiers));
            }

        private:
            VirtualInput& _parent;
            LookupMap _keyMap;
            bool _passThrough;
        };

    public:
        enum actiontype {
            RELEASED = 0,
            PRESSED = 1,
            REPEAT = 2,
            COMPLETED = 3
        };

    private:
        typedef std::map<const string, KeyMap> TableMap;

    public:
        VirtualInput();
        virtual ~VirtualInput();

    public:
        inline void Interval(const uint16 startTime, const uint16 intervalTime)
        {
            _repeatKey.Interval(startTime, intervalTime);
        }

        virtual uint32 Open() = 0;
        virtual uint32 Close() = 0;
        void Default(const string& table)
        {

            if (table.empty() == true) {

                _defaultMap = nullptr;
            }
            else {
                TableMap::iterator index(_mappingTables.find(table));

                ASSERT(index != _mappingTables.end());

                if (index != _mappingTables.end()) {
                    _defaultMap = &(index->second);
                }
            }
        }

        KeyMap& Table(const string& table)
        {
            TableMap::iterator index(_mappingTables.find(table));
            if (index == _mappingTables.end()) {
                std::pair<TableMap::iterator, bool> result = _mappingTables.insert(std::make_pair(table, KeyMap(*this)));
                index = result.first;
            }

            ASSERT(index != _mappingTables.end());

            return (index->second);
        }

        inline void ClearTable(const std::string& name)
        {
            TableMap::iterator index(_mappingTables.find(name));

            if (index != _mappingTables.end()) {
                _mappingTables.erase(index);
            }
        }

        // -------------------------------------------------------------------------------------------------------
        // Whenever a key is pressed or released, let this object know, it will take the proper arrangements and timings
        // to announce this key event to the linux system. Repeat event is triggered by the watchdog implementation
        // in this plugin. No need to signal this.
        uint32 KeyEvent(const bool pressed, const uint32 code, const string& tablename);

    protected:
        typedef Core::IteratorMapType<const std::map<uint16,sint16>, uint16, sint16, std::map<uint16,sint16>::const_iterator> ChangeIterator;

    private:
        virtual void MapChanges(ChangeIterator& updated) = 0;

        void Expired();
        void RepeatKey(const uint32 code);
        void ModifierKey(const actiontype type, const uint16 modifiers);
        bool SendModifier(const actiontype type, const enumModifier mode);
        void AdministerAndSendKey(const actiontype type, const uint32 code);

        virtual void SendKey(const actiontype type, const uint32 code) = 0;

    private:
        RepeatKeyTimer _repeatKey;
        uint32 _modifiers;
        std::map<const string, KeyMap> _mappingTables;
        KeyMap* _defaultMap;
    };

#if !defined(__WIN32__) && !defined(__APPLE__)
    class EXTERNAL LinuxKeyboardInput : public VirtualInput {
    private:
        LinuxKeyboardInput(const LinuxKeyboardInput&) = delete;
        LinuxKeyboardInput& operator=(const LinuxKeyboardInput&) = delete;

    public:
        LinuxKeyboardInput(const string& source, const string& inputName);
        virtual ~LinuxKeyboardInput();

        virtual uint32 Open();
        virtual uint32 Close();
        virtual void MapChanges(ChangeIterator& updated);

    private:
        virtual void SendKey(const actiontype type, const uint32 code);
        bool Updated(ChangeIterator& updated);

    private:
        struct uinput_user_dev _uidev;
        int _eventDescriptor;
        const string _source;
        std::map<uint16, uint16> _deviceKeys;
        Core::CriticalSection _lock;
    };
#endif

    class EXTERNAL IPCKeyboardInput : public VirtualInput {
    private:
        struct KeyData {
            VirtualInput::actiontype Action;
            uint32 Code;
        };

        typedef Core::IPCMessageType<0, KeyData, Core::Void> KeyMessage;
        typedef Core::IPCMessageType<1, Core::Void, Core::IPC::Text<20> > NameMessage;

        IPCKeyboardInput(const IPCKeyboardInput&) = delete;
        IPCKeyboardInput& operator=(const IPCKeyboardInput&) = delete;

    public:
        class KeyboardLink : public Core::IDispatchType<Core::IIPC> {
        private:
            KeyboardLink(const KeyboardLink&) = delete;
            KeyboardLink& operator=(const KeyboardLink&) = delete;

        public:
            KeyboardLink(Core::IPCChannelType<Core::SocketPort, KeyboardLink>*)
                : _enabled(false)
                , _name()
            {
            }
            virtual ~KeyboardLink()
            {
            }

        public:
            inline void Enable(const bool enabled)
            {
                _enabled = enabled;
            }
            inline bool InvokeAllowed() const
            {
                return (_enabled);
            }
            inline const string& Name() const
            {
                return (_name);
            }

        private:
            virtual void Dispatch(Core::IIPC& element) override
            {
                ASSERT(dynamic_cast<NameMessage*>(&element) != nullptr);

                _name = (static_cast<NameMessage&>(element).Response().Value());
                _enabled = true;
            }

        private:
            bool _enabled;
            string _name;
        };

        class VirtualInputChannelServer : public Core::IPCChannelServerType<KeyboardLink, true> {
        private:
            typedef Core::IPCChannelServerType<KeyboardLink, true> BaseClass;

        public:
            VirtualInputChannelServer(const Core::NodeId& sourceName)
                : BaseClass(sourceName, 32)
            {
            }

            virtual void Added(Core::ProxyType<Client>& client) override
            {
                TRACE_L1("VirtualInputChannelServer::Added -- %d", __LINE__);

                Core::ProxyType<Core::IIPC> message(Core::ProxyType<NameMessage>::Create());

                // TODO: The reference to this should be held by the IPC mechanism.. Testing showed it did
                //       not, to be further investigated..
                message.AddRef();

                client->Invoke(message, &(client->Extension()));
            }
        };

    public:
        typedef Core::ProxyType<Core::IPCChannelServerType<KeyboardLink, true>::Client> Link;

    public:
        IPCKeyboardInput(const Core::NodeId& sourceName);
        virtual ~IPCKeyboardInput();

        virtual uint32 Open();
        virtual uint32 Close();
        virtual void MapChanges(ChangeIterator& updated);

        inline Link Client(const uint32 index)
        {
            return (_service[index]);
        }

    private:
        virtual void SendKey(const actiontype type, const uint32 code);

    private:
        VirtualInputChannelServer _service;
    };

    class EXTERNAL InputHandler {
    private:
        InputHandler(const InputHandler&) = delete;
        InputHandler& operator=(const InputHandler&) = delete;

    public:
        InputHandler()
        {
        }
        ~InputHandler()
        {
        }

      enum type {
        DEVICE,
        VIRTUAL
      };
    public:
        void Initialize(const type t, const string& locator)
        {
            ASSERT(_inputHandler == nullptr);
#if  defined(__WIN32__) || defined(__APPLE__)
            ASSERT(t == VIRTUAL)
            _inputHandler = new PluginHost::IPCKeyboardInput(Core::NodeId(locator.c_str()));
            TRACE_L1("Creating a IPC Channel for key communication. %d", 0);
#else
            if (t == VIRTUAL) {
                _inputHandler = new PluginHost::IPCKeyboardInput(Core::NodeId(locator.c_str()));
                TRACE_L1("Creating a IPC Channel for key communication. %d", 0);
            }
            else {
                if (Core::File(locator, false).Exists() == true) {
                    TRACE_L1("Creating a /dev/input device for key communication. %d", 0);

                    // Seems we have a possibility to use /dev/input, create it.
                    _inputHandler = new PluginHost::LinuxKeyboardInput(locator, _T("remote_input"));
                }
            }
#endif
            if (_inputHandler != nullptr) {
                TRACE_L1("Opening VirtualInput %s", locator.c_str());
                if (_inputHandler->Open() != Core::ERROR_NONE) {
                    TRACE_L1("ERROR: Could not open VirtualInput %s.", locator.c_str());
                }
            } else {
                TRACE_L1("ERROR: Could not create '%s' as key communication channel.", locator.c_str());
            }
        }

        void Deinitialize()
        {
            if (_inputHandler != nullptr) {
                _inputHandler->Close();
                delete (_inputHandler);
                _inputHandler = nullptr;
            }
        }

        static VirtualInput* KeyHandler()
        {
            return _inputHandler;
        }

    private:
        static VirtualInput* _inputHandler;
    };
}
} // PluginHost

#endif // KeyHandler
