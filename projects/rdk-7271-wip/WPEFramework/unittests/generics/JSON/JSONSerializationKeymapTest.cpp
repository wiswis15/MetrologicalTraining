#include <generics/JSON.h>
#include <generics/Singleton.h>
#include <gtest/gtest.h>

namespace WPEFramework {
namespace Core {

enum EnumModifier {
    NONE = 0x00,
    LEFTSHIFT = 0x01,
    RIGHTSHIFT = 0x02,
    LEFTALT = 0x04,
    RIGHTALT = 0x08,
    LEFTCTRL = 0x10,
    RIGHTCTRL = 0x20
};

class KeyMapEntry : public WPEFramework::Core::JSON::Container {
private:
    KeyMapEntry& operator=(const KeyMapEntry&) = delete;

public:
    typedef WPEFramework::Core::JSON::EnumType<EnumModifier> JSONModifier;

public:
    inline KeyMapEntry()
    {
        Add(_T("code"), &Code);
        Add(_T("key"), &Key);
        Add(_T("modifiers"), &Modifiers);
    }
    inline KeyMapEntry(const KeyMapEntry& copy)
        : WPEFramework::Core::JSON::Container()
        , Code(copy.Code)
        , Key(copy.Key)
        , Modifiers()
    {
        Add(_T("code"), &Code);
        Add(_T("key"), &Key);
        Add(_T("modifiers"), &Modifiers);
    }
    virtual ~KeyMapEntry()
    {
    }

public:
    WPEFramework::Core::JSON::HexUInt32 Code;
    WPEFramework::Core::JSON::DecUInt16 Key;
    WPEFramework::Core::JSON::ArrayType<JSONModifier> Modifiers;
};

ENUM_CONVERSION_BEGIN(EnumModifier)

    { EnumModifier::LEFTSHIFT, _TXT("shift") },
    { EnumModifier::LEFTSHIFT, _TXT("leftshift") },
    { EnumModifier::RIGHTSHIFT, _TXT("rigthshift") },
    { EnumModifier::LEFTALT, _TXT("alt") },
    { EnumModifier::LEFTALT, _TXT("leftalt") },
    { EnumModifier::RIGHTALT, _TXT("rightalt") },
    { EnumModifier::LEFTCTRL, _TXT("ctrl") },
    { EnumModifier::LEFTCTRL, _TXT("leftctrl") },
    { EnumModifier::RIGHTCTRL, _TXT("rightctrl") },

    ENUM_CONVERSION_END(EnumModifier)

namespace Test {

    class JSONSerializationKeymapTest : public ::testing::Test {
    public:
        JSONSerializationKeymapTest()
        {
        }

    protected:
        virtual void SetUp()
        {
        }

        virtual void TearDown() {}
    };

    struct KeyCode
    {
        uint32 code;
        uint16 key;
        EnumModifier modifier;
    };

    TEST_F(JSONSerializationKeymapTest, KeyMap)
    {
        std::vector<KeyCode> reference({
           { 0x80000400, 30, EnumModifier::NONE },
           { 0x80001400, 16, EnumModifier::NONE },
           { 0x80001D00, 44, EnumModifier::NONE },
           { 0x1234, 22, EnumModifier(int(EnumModifier::LEFTSHIFT) | int(EnumModifier::LEFTALT)) },
           { 0x1235, 23, EnumModifier(int(EnumModifier::LEFTSHIFT) | int(EnumModifier::LEFTCTRL)) },
        });
        static const char ContainerTest[] = "[\n"
        "  { \"code\": \"0x80000400\",  \"key\": 30    }, \n"
        "  { \"code\": \"0x80001400\",  \"key\": 16    }, \n"
        "  { \"code\": \"0x80001D00\",  \"key\": 44    }, \n"
        "  { \"code\": \"1234\",        \"key\": 22,   \"modifiers\": [\"shift\", \"alt\"] }, \n"
        "	{ \"code\": \"1235\",        \"key\": 23,   \"modifiers\": [\"shift\", \"ctrl\"] } \n"
        "]\n";

        WPEFramework::Core::JSON::Tester<1024, WPEFramework::Core::JSON::ArrayType<KeyMapEntry> > keyMappingParser;

        WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::ArrayType<KeyMapEntry> > mappingObject(WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::ArrayType<KeyMapEntry> >::Create());

        keyMappingParser.FromString(ContainerTest, mappingObject);

        // Iterate over the keys..
        // Build the device info array..
        WPEFramework::Core::JSON::ArrayType<KeyMapEntry>::Iterator iter(mappingObject->Elements());

        size_t index = 0;
        while (iter.Next() == true) {
            if ((iter.Current().Code.IsSet()) && (iter.Current().Key.IsSet())) {
                uint32 code(iter.Current().Code.Value());
                uint16 key(iter.Current().Key.Value());

                uint32 modifiers = 0;

                // Build the device info array..
                WPEFramework::Core::JSON::ArrayType<KeyMapEntry::JSONModifier>::Iterator iterModifiers(iter.Current().Modifiers.Elements());

                while (iterModifiers.Next() == true) {
                    modifiers |= (iterModifiers.Current().IsSet() ? iterModifiers.Current().Value() : 0);
                }
                EXPECT_EQ(reference[index].code, code);
                EXPECT_EQ(reference[index].key, key);
                EXPECT_EQ(reference[index].modifier, modifiers);
                ++index;
            }
        }
    }

} // namespace Test
} // namespace Core
} // namespace WPEFramework
