#include <generics/JSON.h>
#include <gtest/gtest.h>

namespace WPEFramework {
namespace Core {
    namespace Test {

        typedef enum {
            ExecuteShell,
            WiFiSettings,
            FanControl,
            PlayerControl

        } CmdType;
    } // namespace Test

    ENUM_CONVERSION_BEGIN(Test::CmdType)

    { Test::ExecuteShell, _TXT("ExecuteShell") },
    { Test::WiFiSettings, _TXT("WiFiSettings") },
    { Test::FanControl, _TXT("FanControl") },
    { Test::PlayerControl, _TXT("PlayerControl") },

    ENUM_CONVERSION_END(Test::CmdType)

    namespace Test {
        class CommandParameters : public JSON::Container {
            CommandParameters(const CommandParameters&) = delete;
            CommandParameters& operator=(const CommandParameters&) = delete;

        public:
            CommandParameters()
            {
                Add(_T("speed"), &Speed);
                Add(_T("duration"), &Duration);
                Add(_T("command"), &Command);
                Add(_T("settings"), &Settings);
            }
            ~CommandParameters()
            {
            }

        public:
            JSON::OctSInt16 Speed;
            JSON::DecUInt16 Duration;
            JSON::EnumType<CmdType> Command;
            JSON::ArrayType<JSON::DecUInt16> Settings;
        };

        class CommandRequest : public JSON::Container {
            CommandRequest(const CommandRequest&) = delete;
            CommandRequest& operator=(const CommandRequest&) = delete;

        public:
            CommandRequest()
                : Name("BullShit", true)
            {
                Add(_T("id"), &Identifier);
                Add(_T("name"), &Name);
                Add(_T("baseAddress"), &BaseAddress);
                Add(_T("trickFlag"), &TrickFlag);
                Add(_T("parameters"), &Params);
            }
            ~CommandRequest()
            {
            }

        public:
            JSON::DecUInt32 Identifier;
            JSON::String Name;
            JSON::HexUInt32 BaseAddress;
            JSON::Boolean TrickFlag;
            CommandParameters Params;
        };

        template <uint16 SIZE, typename INSTANCEOBJECT>
        class Tester {
        private:
            typedef Tester<SIZE, INSTANCEOBJECT> ThisClass;

            class SerializerImpl : public INSTANCEOBJECT::Serializer {
            private:
                SerializerImpl(const SerializerImpl&) = delete;
                SerializerImpl& operator=(const SerializerImpl&) = delete;

            public:
                SerializerImpl(bool& readyFlag)
                    : INSTANCEOBJECT::Serializer()
                    , _ready(readyFlag)
                {
                }
                ~SerializerImpl()
                {
                }

            public:
                virtual void Serialized(const INSTANCEOBJECT& /* element */)
                {
                    _ready = true;
                }
                virtual void Serialized(const Core::JSON::IElement& /* element */)
                {
                    _ready = true;
                }

            private:
                bool& _ready;
            };

            class DeserializerImpl : public INSTANCEOBJECT::Deserializer {
            private:
                DeserializerImpl() = delete;
                DeserializerImpl(const DeserializerImpl&) = delete;
                DeserializerImpl& operator=(const DeserializerImpl&) = delete;

            public:
                DeserializerImpl(ThisClass& parent, bool& ready)
                    : INSTANCEOBJECT::Deserializer()
                    , _element(NULL)
                    , _parent(parent)
                    , _ready(ready)
                {
                }
                ~DeserializerImpl()
                {
                }

            public:
                inline void SetElement(Core::JSON::IElement* element)
                {
                    ASSERT(((element == nullptr) ^ (_element == nullptr)) || (element == _element));

                    _element = element;
                }
                virtual Core::JSON::IElement* Element(const string& identifier VARIABLE_IS_NOT_USED)
                {
                    return (_element);
                }
                virtual void Deserialized(INSTANCEOBJECT& element)
                {
                    ASSERT(&element == _element);
                    _element = NULL;
                    _ready = true;
                }
                virtual void Deserialized(Core::JSON::IElement& element)
                {
                    ASSERT(&element == _element);
                    _element = NULL;
                    _ready = true;
                }

            private:
                Core::JSON::IElement* _element;
                ThisClass& _parent;
                bool& _ready;
            };

        private:
            Tester(const Tester<SIZE, INSTANCEOBJECT>&) = delete;
            Tester<SIZE, INSTANCEOBJECT>& operator=(const Tester<SIZE, INSTANCEOBJECT>&) = delete;

        public:
            Tester()
                : _ready(false)
                , _serializer(_ready)
                , _deserializer(*this, _ready)
            {
            }
            ~Tester()
            {
            }

            bool FromString(const string& value, Core::ProxyType<INSTANCEOBJECT>& receptor)
            {
                uint16 fillCount = 0;
                dynamic_cast<JSON::IElement*>(&(*receptor))->Clear();

                _ready = false;
                _deserializer.SetElement(&(*(receptor)));
                // Serialize object
                while ((_ready == false) && (fillCount < value.size())) {
                    uint16 size = ((value.size() - fillCount) < SIZE ? (value.size() - fillCount) : SIZE);

                    // Prepare the deserialize buffer
                    memcpy(_buffer, &(value.data()[fillCount]), size);

                    fillCount += size;

#ifdef __DEBUG__
                    uint16 handled =
#endif // __DEBUG__

                        _deserializer.Deserialize(_buffer, size);

                    ASSERT(handled <= size);
                }

                return (_ready == true);
            }

            void ToString(const Core::ProxyType<INSTANCEOBJECT>& receptor, string& value)
            {
                uint16 fillCount = 0;
                _ready = false;

                // Request an object to e serialized..
                _serializer.Submit(*receptor);

                // Serialize object
                while (_ready == false) {
                    uint16 loaded = _serializer.Serialize(_buffer, SIZE);

                    ASSERT(loaded <= SIZE);

                    fillCount += loaded;

                    value += string(reinterpret_cast<char*>(&_buffer[0]), loaded);
                }
            }

        private:
            bool _ready;
            SerializerImpl _serializer;
            DeserializerImpl _deserializer;
            uint8 _buffer[SIZE];
        };

        class JSONSerializationTest : public ::testing::Test {
        public:
            JSONSerializationTest()
                : _command(ProxyType<JSON::LabelType<CommandRequest>>::Create())
            {
            }

        protected:
            virtual void SetUp()
            {
                _command->Identifier = 35;
                _command->Name = _T("TestIdentifier");
                _command->BaseAddress = 0x567;
                _command->TrickFlag = true;
                _command->Params.Duration = 44;
                _command->Params.Speed = -12;
                _command->Params.Settings.Add(JSON::DecUInt16(6, true));
                _command->Params.Settings.Add(JSON::DecUInt16(14, true));
                _command->Params.Settings.Add(JSON::DecUInt16(22, true));
                _command->Params.Command = PlayerControl;
            }

            virtual void TearDown() {}

            ProxyType<JSON::LabelType<CommandRequest>> _command;
        };

        TEST_F(JSONSerializationTest, SmallParser)
        {
            string text, response;
            Tester<1, JSON::LabelType<CommandRequest>> smallParser;
            ProxyType<JSON::LabelType<CommandRequest>> received = ProxyType<JSON::LabelType<CommandRequest>>::Create();

            // Convert command to JSON(text)
            smallParser.ToString(_command, text);

            // Convert JSON(text) to received
            smallParser.FromString(text, received);

            // Convert received to JSON(response)
            smallParser.ToString(received, response);

            EXPECT_EQ(text, response);
        }

        TEST_F(JSONSerializationTest, BigParser)
        {
            string text, response;
            Tester<4096, JSON::LabelType<CommandRequest>> bigParser;
            ProxyType<JSON::LabelType<CommandRequest>> received = ProxyType<JSON::LabelType<CommandRequest>>::Create();

            // Convert command to JSON(text
            bigParser.ToString(_command, text);

            // Convert JSON(text) to received
            bigParser.FromString(text, received);

            // Convert received to JSON(response)
            bigParser.ToString(received, response);

            EXPECT_EQ(text, response);
        }

        TEST_F(JSONSerializationTest, FreeFormattedTest)
        {
            static const char* input = "CommandRequest : { \"id\"          : 35,             \n"
                                       "                   \"name\"        : \"TestIdentifier\",\n"
                                       "                   \"baseAddress\" : 0x567,          \n"
                                       "                   \"trickFlag\"   : true,           \n"
                                       "                   \"parameters\"  :                 \n"
                                       "                                    { \"speed\"    : -014,\n             "
                                       "                                      \"duration\" : 44,  \n             "
                                       "                                      \"command\"  : \"PlayerControl\",\n"
                                       "                                      \"settings\" : [ 6, 14, 22] \n     "
                                       "                                    }                             \n     "
                                       "                 }   ";

            string text, response;
            Tester<1, JSON::LabelType<CommandRequest>> smallParser;
            Tester<4096, JSON::LabelType<CommandRequest>> bigParser;
            ProxyType<JSON::LabelType<CommandRequest>> received = ProxyType<JSON::LabelType<CommandRequest>>::Create();

            // Against what shoudl we compare, the JSON(tet) from _command..
            bigParser.ToString(_command, text);

            // Convert JSON(input) to response
            smallParser.FromString(input, received);
            // And back to string as they are easy to compare..
            smallParser.ToString(received, response);

            EXPECT_EQ(text, response);

            response.clear();

            // Convert JSON(input) to response
            bigParser.FromString(input, received);
            // And back to string as they are easy to compare..
            bigParser.ToString(received, response);

            EXPECT_EQ(text, response);
        }

    } // namespace Test
} // namespace Core
} // namespace WPEFramework
