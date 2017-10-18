#include <iostream>

#include "../../generics/generics.h"

typedef enum {
    ExecuteShell,
    WiFiSettings,
    FanControl,
    PlayerControl

} CommandType;

ENUM_CONVERSION_BEGIN(CommandType)

    { ExecuteShell, _TXT("ExecuteShell") },
    { WiFiSettings, _TXT("WiFiSettings") },
    { FanControl, _TXT("FanControl") },
    { PlayerControl, _TXT("PlayerControl") },

    ENUM_CONVERSION_END(CommandType)

class CommandParameters : public WPEFramework::Core::JSON::Container {
private:
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
    WPEFramework::Core::JSON::OctSInt16 Speed;
    WPEFramework::Core::JSON::DecUInt16 Duration;
	WPEFramework::Core::JSON::EnumType<CommandType> Command;
    WPEFramework::Core::JSON::ArrayType<WPEFramework::Core::JSON::DecUInt16> Settings;
};

class CommandRequest : public WPEFramework::Core::JSON::Container {
private:
	CommandRequest(const CommandRequest&) = delete;
    CommandRequest& operator=(const CommandRequest&) = delete;

public:
    CommandRequest() :
		Name("BullShit", true)
    {
        Add(_T("sequence"), &Sequence);
        Add(_T("name"), &Name);
        Add(_T("baseAddress"), &BaseAddress);
        Add(_T("trickFlag"), &TrickFlag);
        Add(_T("parameters"), &Params);
    }
    ~CommandRequest()
    {
    }

public:
	WPEFramework::Core::JSON::DecUInt32 Sequence;
    WPEFramework::Core::JSON::String Name;
    WPEFramework::Core::JSON::HexUInt32 BaseAddress;
    WPEFramework::Core::JSON::Boolean TrickFlag;
    CommandParameters Params;
};

class KeyMapEntry : public WPEFramework::Core::JSON::Container {
private:
    KeyMapEntry& operator=(const KeyMapEntry&) = delete;

public:
    enum enumModifier {
        LEFTSHIFT = 0x01,
        RIGHTSHIFT = 0x02,
        LEFTALT = 0x04,
        RIGHTALT = 0x08,
        LEFTCTRL = 0x10,
        RIGHTCTRL = 0x20
    };
    typedef WPEFramework::Core::JSON::EnumType<enumModifier> JSONModifier;

public:
    inline KeyMapEntry()
    {
        Add(_T("code"), &Code);
        Add(_T("key"), &Key);
        Add(_T("modifiers"), &Modifiers);
    }

    inline KeyMapEntry(const KeyMapEntry &copy VARIABLE_IS_NOT_USED)
        : WPEFramework::Core::JSON::Container()
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

ENUM_CONVERSION_BEGIN(KeyMapEntry::enumModifier)

    { KeyMapEntry::LEFTSHIFT, _TXT("shift") },
    { KeyMapEntry::LEFTSHIFT, _TXT("leftshift") },
    { KeyMapEntry::RIGHTSHIFT, _TXT("rigthshift") },
    { KeyMapEntry::LEFTALT, _TXT("alt") },
    { KeyMapEntry::LEFTALT, _TXT("leftalt") },
    { KeyMapEntry::RIGHTALT, _TXT("rightalt") },
    { KeyMapEntry::LEFTCTRL, _TXT("ctrl") },
    { KeyMapEntry::LEFTCTRL, _TXT("leftctrl") },
    { KeyMapEntry::RIGHTCTRL, _TXT("rightctrl") },

    ENUM_CONVERSION_END(KeyMapEntry::enumModifier)

        const char FreeFormattedJSON[]
    = "{ \"sequence\"    : 35,\n"
      "  \"name\"        : \"TestIdentifier\", \n"
      "  \"baseAddress\" : 0x567,               \n"
      "  \"trickFlag\"   : true,                \n"
      "  \"parameters\"  :                      \n"
      "                   { \"speed\"    : -014,\n"
      "                     \"duration\" : 44,  \n             "
      "                     \"command\"  : \"PlayerControl\",\n"
      "                     \"settings\" : [ 6, 14, 22] \n     "
      "                   } \n"
      "}   ";

const char ConatinerTest[] = "[\n"
                             "  { \"code\": \"0x80000400\",  \"key\": 30    }, \n"
                             "  { \"code\": \"0x80001400\",  \"key\": 16    }, \n"
                             "  { \"code\": \"0x80001D00\",  \"key\": 44    }, \n"
                             "  { \"code\": \"1234\",        \"key\": 22,   \"modifiers\": [\"shift\", \"alt\"] }, \n"
                             "	{ \"code\": \"1235\",        \"key\": 23,   \"modifiers\": [\"shift\", \"ctrl\"] } \n"
                             "]\n";

int main()
{
    WPEFramework::Core::JSON::Tester<1024, WPEFramework::Core::JSON::ArrayType<KeyMapEntry> > keyMappingParser;

    WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::ArrayType<KeyMapEntry> > mappingObject(WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::ArrayType<KeyMapEntry> >::Create());

    keyMappingParser.FromString(ConatinerTest, mappingObject);

    // Iterate over the keys..
    // Build the device info array..
    WPEFramework::Core::JSON::ArrayType<KeyMapEntry>::Iterator index(mappingObject->Elements());

    while (index.Next() == true) {
        if ((index.Current().Code.IsSet()) && (index.Current().Key.IsSet())) {
//            uint32 code(index.Current().Code.Value());

            uint32 modifiers = 0;

            // Build the device info array..
            WPEFramework::Core::JSON::ArrayType<KeyMapEntry::JSONModifier>::Iterator flags(index.Current().Modifiers.Elements());

            while (flags.Next() == true) {
                modifiers |= (flags.Current().IsSet() ? flags.Current().Value() : 0);
            }
        }
    }

    WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::LabelType<CommandRequest> > command = WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::LabelType<CommandRequest> >::Create();
    WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::LabelType<CommandRequest> > received = WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::LabelType<CommandRequest> >::Create();

    command->Sequence = 35;
    command->Name = _T("TestIdentifier");
    command->BaseAddress = 0x567;
    command->TrickFlag = true;
    command->Params.Duration = 44;
    command->Params.Speed = -12;
    command->Params.Settings.Add(WPEFramework::Core::JSON::DecUInt16(6, true));
    command->Params.Settings.Add(WPEFramework::Core::JSON::DecUInt16(14, true));
    command->Params.Settings.Add(WPEFramework::Core::JSON::DecUInt16(22, true));
    command->Params.Command = PlayerControl;

    string text, response, typeIdentifier;
    WPEFramework::Core::JSON::Tester<1, WPEFramework::Core::JSON::LabelType<CommandRequest> > smallParser;

    // Convert command to JSON(text)
    smallParser.ToString(command, text);
    // Convert JSON(text) to received
    smallParser.FromString(text, received);
    // Convert received to JSON(response)
    smallParser.ToString(received, response);

    if (response == text) {
        fprintf(stderr, "Parsing (smallParser) Successfull\n");
    }
    else {
        fprintf(stderr, "Parsing (smallParser) Failed\n");
    }

    text.clear();
    response.clear();

    WPEFramework::Core::JSON::Tester<2048, WPEFramework::Core::JSON::LabelType<CommandRequest> > bigParser;
    // Convert command to JSON(text)
    bigParser.ToString(command, text);
    // Convert JSON(text) to received
    bigParser.FromString(text, received);
    // Convert received to JSON(response)
    bigParser.ToString(received, response);

    if (response == text) {
        fprintf(stderr, "Parsing (bigParser) Successfull\n");
    }
    else {
        fprintf(stderr, "Parsing (bigParser) Failed\n");
    }

    static const char* input = "CommandRequest : { \"sequence\"    : 35,             \n"
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

    text.clear();
    response.clear();

    // Against what shoudl we compare, the JSON(tet) from _command..
    bigParser.ToString(command, text);

    // Convert JSON(input) to response
    smallParser.FromString(input, received);
    // And back to string as they are easy to compare..
    smallParser.ToString(received, response);

    if (response == text) {
        fprintf(stderr, "FreeFormattedText Parsing (smallParser) Successfull\n");
    }
    else {
        fprintf(stderr, "FreeFormattedText  Parsing (smallParser) Failed\n");
    }

    response.clear();

    // Convert JSON(input) to response
    bigParser.FromString(input, received);
    // And back to string as they are easy to compare..
    bigParser.ToString(received, response);

    if (response == text) {
        fprintf(stderr, "FreeFormattedText Parsing (bigParser) Successfull\n");
    }
    else {
        fprintf(stderr, "FreeFormattedText  Parsing (bigParser) Failed\n");
    }

	static const char* nullTest = 
		"CommandRequest : { \"sequence\"    : null,    \n"
		"                   \"name\"        : null,    \n"
		"                   \"baseAddress\" : 0x234,   \n"
		"                   \"trickFlag\"   : null,    \n"
		"                   \"parameters\"  : \n"
		"                                    { \"speed\"    : -014, \n"
		"                                      \"command\"  : null, \n"
		"                                      \"duration\" : 44,   \n"
		"                                    }                      \n"
		"                 }   ";

	// Convert JSON(input) to response
	bigParser.FromString(nullTest, received);
	response.clear();
	bigParser.ToString(received, response);
	fprintf(stderr, "Recovered: %s", response.c_str());

	WPEFramework::Core::Singleton::Dispose();

    return 0;
}
