#include <generics/JSON.h>
#include <gtest/gtest.h>

namespace WPEFramework {
namespace Core {
    namespace Test {

        class JSONStringTest : public ::testing::Test {
        protected:
            virtual void SetUp() {}

            virtual void TearDown() {}
        };

        static const string DefaultString = "default";
        static const char* Default = "default";
        static const string OtherDefaultString = "default2";
        static const char* OtherDefault = "default2";
        static const string TextString = "text";
        static const char* Text = "text";
        static const char* Null = "null";
        static const string OtherTextString = "other text";
        static const char* OtherText = "other text";
#ifndef __NO_WCHAR_SUPPORT__
        static const wchar_t* WDefault = L"default";
        static const char* WOtherDefault = "default2";
        static const wchar_t* WText = L"text";
        static const char* WOtherText = "other text";
#endif
        static const size_t BufferSize = 1024;

        TEST_F(JSONStringTest, Construct)
        {
            JSON::String target;
            EXPECT_EQ(JSON::ParserType::PARSE_DIRECT, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_FALSE(((JSON::IElement*)&target)->IsSet());
            EXPECT_EQ("", target.Default());
            EXPECT_EQ("", target.Value());
            EXPECT_EQ("", string(target));
        }

        TEST_F(JSONStringTest, ConstructCopyUnassigned)
        {
            JSON::String other(DefaultString);
            JSON::String target(other);
            EXPECT_EQ(JSON::ParserType::PARSE_DIRECT, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_FALSE(((JSON::IElement*)&target)->IsSet());
            EXPECT_EQ(DefaultString, target.Default());
            EXPECT_EQ(DefaultString, target.Value());
            EXPECT_EQ(DefaultString, string(target));
        }

        TEST_F(JSONStringTest, ConstructCopyAssigned)
        {
            JSON::String other(Default);
            other = TextString;
            JSON::String target(other);
            EXPECT_EQ(JSON::ParserType::PARSE_DIRECT, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_TRUE(((JSON::IElement*)&target)->IsSet());
            EXPECT_EQ(Default, target.Default());
            EXPECT_EQ(TextString, target.Value());
            EXPECT_EQ(TextString, string(target));
        }

        TEST_F(JSONStringTest, ConstructString)
        {
            JSON::String target(DefaultString);
            EXPECT_EQ(JSON::ParserType::PARSE_DIRECT, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_FALSE(((JSON::IElement*)&target)->IsSet());
            EXPECT_EQ(DefaultString, target.Default());
            EXPECT_EQ(DefaultString, target.Value());
            EXPECT_EQ(DefaultString, string(target));
        }

        TEST_F(JSONStringTest, ConstructCharArray)
        {
            JSON::String target(Default);
            EXPECT_EQ(JSON::ParserType::PARSE_DIRECT, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_FALSE(((JSON::IElement*)&target)->IsSet());
            EXPECT_EQ(Default, target.Default());
            EXPECT_EQ(Default, target.Value());
            EXPECT_EQ(Default, string(target));
        }

#ifndef __NO_WCHAR_SUPPORT__
        TEST_F(JSONStringTest, ConstructWCharArray)
        {
            JSON::String target(WDefault);
            EXPECT_EQ(JSON::ParserType::PARSE_DIRECT, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_FALSE(((JSON::IElement*)&target)->IsSet());
            EXPECT_EQ(Default, target.Default());
            EXPECT_EQ(Default, target.Value());
            EXPECT_EQ(Default, string(target));
        }
#endif

        TEST_F(JSONStringTest, AssignString)
        {
            JSON::String target(Default);
            target = TextString;
            EXPECT_EQ(JSON::ParserType::PARSE_DIRECT, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_TRUE(((JSON::IElement*)&target)->IsSet());
            EXPECT_EQ(Default, target.Default());
            EXPECT_EQ(Text, target.Value());
            EXPECT_EQ(Text, string(target));
        }

        TEST_F(JSONStringTest, AssignJSONStringFromUnassigned)
        {
            JSON::String other(Default);
            JSON::String target;
            target = other;
            EXPECT_EQ(JSON::ParserType::PARSE_DIRECT, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_FALSE(((JSON::IElement*)&target)->IsSet());
            EXPECT_EQ(Default, target.Default());
            EXPECT_EQ(Default, target.Value());
            EXPECT_EQ(Default, string(target));
        }

        TEST_F(JSONStringTest, AssignJSONStringFromAssigned)
        {
            JSON::String other(Default);
            other = Text;
            JSON::String target;
            target = other;
            EXPECT_EQ(JSON::ParserType::PARSE_DIRECT, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_TRUE(((JSON::IElement*)&target)->IsSet());
            EXPECT_EQ(Default, target.Default());
            EXPECT_EQ(Text, target.Value());
            EXPECT_EQ(Text, string(target));
        }

        TEST_F(JSONStringTest, AssignCharArray)
        {
            JSON::String target(Default);
            target = Text;
            EXPECT_EQ(JSON::ParserType::PARSE_DIRECT, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_TRUE(((JSON::IElement*)&target)->IsSet());
            EXPECT_EQ(Default, target.Default());
            EXPECT_EQ(Text, target.Value());
            EXPECT_EQ(Text, string(target));
        }

#ifndef __NO_WCHAR_SUPPORT__
        TEST_F(JSONStringTest, AssignWCharArray)
        {
            JSON::String target(WDefault);
            target = WText;
            EXPECT_EQ(JSON::ParserType::PARSE_DIRECT, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_TRUE(((JSON::IElement*)&target)->IsSet());
            EXPECT_EQ(Default, target.Default());
            EXPECT_EQ(Text, target.Value());
            EXPECT_EQ(Text, string(target));
        }
#endif

        TEST_F(JSONStringTest, Clear)
        {
            JSON::String target(Default);
            target = Text;
            EXPECT_TRUE(((JSON::IElement*)&target)->IsSet());
            dynamic_cast<JSON::IElement*>(&target)->Clear();
            EXPECT_FALSE(((JSON::IElement*)&target)->IsSet());
        }

        TEST_F(JSONStringTest, IsEqualJSONString)
        {
            JSON::String ref1;
            JSON::String ref2(Default);
            JSON::String ref3(Default);
            JSON::String ref4(DefaultString);
            JSON::String ref5(OtherDefaultString);
            JSON::String ref6(OtherDefault);
            ref3 = Text;
            ref4 = TextString;
            ref5 = OtherText;
            ref6 = OtherTextString;
            EXPECT_TRUE(ref1 == ref1);
            EXPECT_FALSE(ref1 == ref2);
            EXPECT_FALSE(ref1 == ref3);
            EXPECT_FALSE(ref1 == ref4);
            EXPECT_FALSE(ref1 == ref5);
            EXPECT_FALSE(ref1 == ref6);
            EXPECT_FALSE(ref2 == ref1);
            EXPECT_TRUE(ref2 == ref2);
            EXPECT_FALSE(ref2 == ref3);
            EXPECT_FALSE(ref2 == ref4);
            EXPECT_FALSE(ref2 == ref5);
            EXPECT_FALSE(ref2 == ref6);
            EXPECT_FALSE(ref3 == ref1);
            EXPECT_FALSE(ref3 == ref2);
            EXPECT_TRUE(ref3 == ref3);
            EXPECT_TRUE(ref3 == ref4);
            EXPECT_FALSE(ref3 == ref5);
            EXPECT_FALSE(ref3 == ref6);
            EXPECT_FALSE(ref4 == ref1);
            EXPECT_FALSE(ref4 == ref2);
            EXPECT_TRUE(ref4 == ref3);
            EXPECT_TRUE(ref4 == ref4);
            EXPECT_FALSE(ref4 == ref5);
            EXPECT_FALSE(ref4 == ref6);
            EXPECT_FALSE(ref5 == ref1);
            EXPECT_FALSE(ref5 == ref2);
            EXPECT_FALSE(ref5 == ref3);
            EXPECT_FALSE(ref5 == ref4);
            EXPECT_TRUE(ref5 == ref5);
            EXPECT_TRUE(ref5 == ref6);
            EXPECT_FALSE(ref6 == ref1);
            EXPECT_FALSE(ref6 == ref2);
            EXPECT_FALSE(ref6 == ref3);
            EXPECT_FALSE(ref6 == ref4);
            EXPECT_TRUE(ref6 == ref5);
            EXPECT_TRUE(ref6 == ref6);
        }

        TEST_F(JSONStringTest, IsEqualCharArray)
        {
            JSON::String ref1;
            JSON::String ref2(Default);
            JSON::String ref3(Default);
            JSON::String ref4(DefaultString);
            JSON::String ref5(OtherDefaultString);
            JSON::String ref6(OtherDefault);
            ref3 = Text;
            ref4 = TextString;
            ref5 = OtherText;
            ref6 = OtherTextString;
            EXPECT_TRUE(ref1 == "");
            EXPECT_FALSE(ref2 == "");
            EXPECT_FALSE(ref3 == "");
            EXPECT_FALSE(ref4 == "");
            EXPECT_FALSE(ref5 == "");
            EXPECT_FALSE(ref6 == "");
            EXPECT_FALSE(ref1 == Default);
            EXPECT_TRUE(ref2 == Default);
            EXPECT_FALSE(ref3 == Default);
            EXPECT_FALSE(ref4 == Default);
            EXPECT_FALSE(ref5 == Default);
            EXPECT_FALSE(ref6 == Default);
            EXPECT_FALSE(ref1 == Text);
            EXPECT_FALSE(ref2 == Text);
            EXPECT_TRUE(ref3 == Text);
            EXPECT_TRUE(ref4 == Text);
            EXPECT_FALSE(ref5 == Text);
            EXPECT_FALSE(ref6 == Text);
            EXPECT_FALSE(ref1 == OtherText);
            EXPECT_FALSE(ref2 == OtherText);
            EXPECT_FALSE(ref3 == OtherText);
            EXPECT_FALSE(ref4 == OtherText);
            EXPECT_TRUE(ref5 == OtherText);
            EXPECT_TRUE(ref6 == OtherText);
        }

#ifndef __NO_WCHAR_SUPPORT__
        TEST_F(JSONStringTest, IsEqualWCharArray)
        {
            JSON::String ref1;
            JSON::String ref2(WDefault);
            JSON::String ref3(WDefault);
            JSON::String ref4(DefaultString);
            JSON::String ref5(OtherDefaultString);
            JSON::String ref6(WOtherDefault);
            ref3 = WText;
            ref4 = TextString;
            ref5 = WOtherText;
            ref6 = OtherTextString;
            EXPECT_TRUE(ref1 == "");
            EXPECT_FALSE(ref2 == "");
            EXPECT_FALSE(ref3 == "");
            EXPECT_FALSE(ref4 == "");
            EXPECT_FALSE(ref5 == "");
            EXPECT_FALSE(ref6 == "");
            EXPECT_FALSE(ref1 == Default);
            EXPECT_TRUE(ref2 == Default);
            EXPECT_FALSE(ref3 == Default);
            EXPECT_FALSE(ref4 == Default);
            EXPECT_FALSE(ref5 == Default);
            EXPECT_FALSE(ref6 == Default);
            EXPECT_FALSE(ref1 == Text);
            EXPECT_FALSE(ref2 == Text);
            EXPECT_TRUE(ref3 == Text);
            EXPECT_TRUE(ref4 == Text);
            EXPECT_FALSE(ref5 == Text);
            EXPECT_FALSE(ref6 == Text);
            EXPECT_FALSE(ref1 == OtherText);
            EXPECT_FALSE(ref2 == OtherText);
            EXPECT_FALSE(ref3 == OtherText);
            EXPECT_FALSE(ref4 == OtherText);
            EXPECT_TRUE(ref5 == OtherText);
            EXPECT_TRUE(ref6 == OtherText);
        }
#endif

        TEST_F(JSONStringTest, IsNotEqualJSONString)
        {
            JSON::String ref1;
            JSON::String ref2(Default);
            JSON::String ref3(Default);
            JSON::String ref4(DefaultString);
            JSON::String ref5(OtherDefaultString);
            JSON::String ref6(OtherDefault);
            ref3 = Text;
            ref4 = TextString;
            ref5 = OtherText;
            ref6 = OtherTextString;
            EXPECT_FALSE(ref1 != ref1);
            EXPECT_TRUE(ref1 != ref2);
            EXPECT_TRUE(ref1 != ref3);
            EXPECT_TRUE(ref1 != ref4);
            EXPECT_TRUE(ref1 != ref5);
            EXPECT_TRUE(ref1 != ref6);
            EXPECT_TRUE(ref2 != ref1);
            EXPECT_FALSE(ref2 != ref2);
            EXPECT_TRUE(ref2 != ref3);
            EXPECT_TRUE(ref2 != ref4);
            EXPECT_TRUE(ref2 != ref5);
            EXPECT_TRUE(ref2 != ref6);
            EXPECT_TRUE(ref3 != ref1);
            EXPECT_TRUE(ref3 != ref2);
            EXPECT_FALSE(ref3 != ref3);
            EXPECT_FALSE(ref3 != ref4);
            EXPECT_TRUE(ref3 != ref5);
            EXPECT_TRUE(ref3 != ref6);
            EXPECT_TRUE(ref4 != ref1);
            EXPECT_TRUE(ref4 != ref2);
            EXPECT_FALSE(ref4 != ref3);
            EXPECT_FALSE(ref4 != ref4);
            EXPECT_TRUE(ref4 != ref5);
            EXPECT_TRUE(ref4 != ref6);
            EXPECT_TRUE(ref5 != ref1);
            EXPECT_TRUE(ref5 != ref2);
            EXPECT_TRUE(ref5 != ref3);
            EXPECT_TRUE(ref5 != ref4);
            EXPECT_FALSE(ref5 != ref5);
            EXPECT_FALSE(ref5 != ref6);
            EXPECT_TRUE(ref6 != ref1);
            EXPECT_TRUE(ref6 != ref2);
            EXPECT_TRUE(ref6 != ref3);
            EXPECT_TRUE(ref6 != ref4);
            EXPECT_FALSE(ref6 != ref5);
            EXPECT_FALSE(ref6 != ref6);
        }

        TEST_F(JSONStringTest, IsNotEqualCharArray)
        {
            JSON::String ref1;
            JSON::String ref2(Default);
            JSON::String ref3(Default);
            JSON::String ref4(DefaultString);
            JSON::String ref5(OtherDefaultString);
            JSON::String ref6(OtherDefault);
            ref3 = Text;
            ref4 = TextString;
            ref5 = OtherText;
            ref6 = OtherTextString;
            EXPECT_FALSE(ref1 != "");
            EXPECT_TRUE(ref2 != "");
            EXPECT_TRUE(ref3 != "");
            EXPECT_TRUE(ref4 != "");
            EXPECT_TRUE(ref5 != "");
            EXPECT_TRUE(ref6 != "");
            EXPECT_TRUE(ref1 != Default);
            EXPECT_FALSE(ref2 != Default);
            EXPECT_TRUE(ref3 != Default);
            EXPECT_TRUE(ref4 != Default);
            EXPECT_TRUE(ref5 != Default);
            EXPECT_TRUE(ref6 != Default);
            EXPECT_TRUE(ref1 != Text);
            EXPECT_TRUE(ref2 != Text);
            EXPECT_FALSE(ref3 != Text);
            EXPECT_FALSE(ref4 != Text);
            EXPECT_TRUE(ref5 != Text);
            EXPECT_TRUE(ref6 != Text);
            EXPECT_TRUE(ref1 != OtherText);
            EXPECT_TRUE(ref2 != OtherText);
            EXPECT_TRUE(ref3 != OtherText);
            EXPECT_TRUE(ref4 != OtherText);
            EXPECT_FALSE(ref5 != OtherText);
            EXPECT_FALSE(ref6 != OtherText);
        }

#ifndef __NO_WCHAR_SUPPORT__
        TEST_F(JSONStringTest, IsNotEqualWCharArray)
        {
            JSON::String ref1;
            JSON::String ref2(WDefault);
            JSON::String ref3(WDefault);
            JSON::String ref4(DefaultString);
            JSON::String ref5(OtherDefaultString);
            JSON::String ref6(WOtherDefault);
            ref3 = WText;
            ref4 = TextString;
            ref5 = WOtherText;
            ref6 = OtherTextString;
            EXPECT_FALSE(ref1 != "");
            EXPECT_TRUE(ref2 != "");
            EXPECT_TRUE(ref3 != "");
            EXPECT_TRUE(ref4 != "");
            EXPECT_TRUE(ref5 != "");
            EXPECT_TRUE(ref6 != "");
            EXPECT_TRUE(ref1 != Default);
            EXPECT_FALSE(ref2 != Default);
            EXPECT_TRUE(ref3 != Default);
            EXPECT_TRUE(ref4 != Default);
            EXPECT_TRUE(ref5 != Default);
            EXPECT_TRUE(ref6 != Default);
            EXPECT_TRUE(ref1 != Text);
            EXPECT_TRUE(ref2 != Text);
            EXPECT_FALSE(ref3 != Text);
            EXPECT_FALSE(ref4 != Text);
            EXPECT_TRUE(ref5 != Text);
            EXPECT_TRUE(ref6 != Text);
            EXPECT_TRUE(ref1 != OtherText);
            EXPECT_TRUE(ref2 != OtherText);
            EXPECT_TRUE(ref3 != OtherText);
            EXPECT_TRUE(ref4 != OtherText);
            EXPECT_FALSE(ref5 != OtherText);
            EXPECT_FALSE(ref6 != OtherText);
        }
#endif

        TEST_F(JSONStringTest, IsSmallerJSONString)
        {
            JSON::String ref1;
            JSON::String ref2(Default);
            JSON::String ref3(Default);
            JSON::String ref4(DefaultString);
            JSON::String ref5(OtherDefaultString);
            JSON::String ref6(OtherDefault);
            ref3 = Text;
            ref4 = TextString;
            ref5 = OtherText;
            ref6 = OtherTextString;
            EXPECT_FALSE(ref1 < ref1);
            EXPECT_TRUE(ref1 < ref2);
            EXPECT_TRUE(ref1 < ref3);
            EXPECT_TRUE(ref1 < ref4);
            EXPECT_TRUE(ref1 < ref5);
            EXPECT_TRUE(ref1 < ref6);
            EXPECT_FALSE(ref2 < ref1);
            EXPECT_FALSE(ref2 < ref2);
            EXPECT_TRUE(ref2 < ref3);
            EXPECT_TRUE(ref2 < ref4);
            EXPECT_TRUE(ref2 < ref5);
            EXPECT_TRUE(ref2 < ref6);
            EXPECT_FALSE(ref3 < ref1);
            EXPECT_FALSE(ref3 < ref2);
            EXPECT_FALSE(ref3 < ref3);
            EXPECT_FALSE(ref3 < ref4);
            EXPECT_FALSE(ref3 < ref5);
            EXPECT_FALSE(ref3 < ref6);
            EXPECT_FALSE(ref4 < ref1);
            EXPECT_FALSE(ref4 < ref2);
            EXPECT_FALSE(ref4 < ref3);
            EXPECT_FALSE(ref4 < ref4);
            EXPECT_FALSE(ref4 < ref5);
            EXPECT_FALSE(ref4 < ref6);
            EXPECT_FALSE(ref5 < ref1);
            EXPECT_FALSE(ref5 < ref2);
            EXPECT_TRUE(ref5 < ref3);
            EXPECT_TRUE(ref5 < ref4);
            EXPECT_FALSE(ref5 < ref5);
            EXPECT_FALSE(ref5 < ref6);
            EXPECT_FALSE(ref6 < ref1);
            EXPECT_FALSE(ref6 < ref2);
            EXPECT_TRUE(ref6 < ref3);
            EXPECT_TRUE(ref6 < ref4);
            EXPECT_FALSE(ref6 < ref5);
            EXPECT_FALSE(ref6 < ref6);
        }

        TEST_F(JSONStringTest, IsGreaterJSONString)
        {
            JSON::String ref1;
            JSON::String ref2(Default);
            JSON::String ref3(Default);
            JSON::String ref4(DefaultString);
            JSON::String ref5(OtherDefaultString);
            JSON::String ref6(OtherDefault);
            ref3 = Text;
            ref4 = TextString;
            ref5 = OtherText;
            ref6 = OtherTextString;
            EXPECT_FALSE(ref1 > ref1);
            EXPECT_FALSE(ref1 > ref2);
            EXPECT_FALSE(ref1 > ref3);
            EXPECT_FALSE(ref1 > ref4);
            EXPECT_FALSE(ref1 > ref5);
            EXPECT_FALSE(ref1 > ref6);
            EXPECT_TRUE(ref2 > ref1);
            EXPECT_FALSE(ref2 > ref2);
            EXPECT_FALSE(ref2 > ref3);
            EXPECT_FALSE(ref2 > ref4);
            EXPECT_FALSE(ref2 > ref5);
            EXPECT_FALSE(ref2 > ref6);
            EXPECT_TRUE(ref3 > ref1);
            EXPECT_TRUE(ref3 > ref2);
            EXPECT_FALSE(ref3 > ref3);
            EXPECT_FALSE(ref3 > ref4);
            EXPECT_TRUE(ref3 > ref5);
            EXPECT_TRUE(ref3 > ref6);
            EXPECT_TRUE(ref4 > ref1);
            EXPECT_TRUE(ref4 > ref2);
            EXPECT_FALSE(ref4 > ref3);
            EXPECT_FALSE(ref4 > ref4);
            EXPECT_TRUE(ref4 > ref5);
            EXPECT_TRUE(ref4 > ref6);
            EXPECT_TRUE(ref5 > ref1);
            EXPECT_TRUE(ref5 > ref2);
            EXPECT_FALSE(ref5 > ref3);
            EXPECT_FALSE(ref5 > ref4);
            EXPECT_FALSE(ref5 > ref5);
            EXPECT_FALSE(ref5 > ref6);
            EXPECT_TRUE(ref6 > ref1);
            EXPECT_TRUE(ref6 > ref2);
            EXPECT_FALSE(ref6 > ref3);
            EXPECT_FALSE(ref6 > ref4);
            EXPECT_FALSE(ref6 > ref5);
            EXPECT_FALSE(ref6 > ref6);
        }

        TEST_F(JSONStringTest, IsSmallerEqualJSONString)
        {
            JSON::String ref1;
            JSON::String ref2(Default);
            JSON::String ref3(Default);
            JSON::String ref4(DefaultString);
            JSON::String ref5(OtherDefaultString);
            JSON::String ref6(OtherDefault);
            ref3 = Text;
            ref4 = TextString;
            ref5 = OtherText;
            ref6 = OtherTextString;
            EXPECT_TRUE(ref1 <= ref1);
            EXPECT_TRUE(ref1 <= ref2);
            EXPECT_TRUE(ref1 <= ref3);
            EXPECT_TRUE(ref1 <= ref4);
            EXPECT_TRUE(ref1 <= ref5);
            EXPECT_TRUE(ref1 <= ref6);
            EXPECT_FALSE(ref2 <= ref1);
            EXPECT_TRUE(ref2 <= ref2);
            EXPECT_TRUE(ref2 <= ref3);
            EXPECT_TRUE(ref2 <= ref4);
            EXPECT_TRUE(ref2 <= ref5);
            EXPECT_TRUE(ref2 <= ref6);
            EXPECT_FALSE(ref3 <= ref1);
            EXPECT_FALSE(ref3 <= ref2);
            EXPECT_TRUE(ref3 <= ref3);
            EXPECT_TRUE(ref3 <= ref4);
            EXPECT_FALSE(ref3 <= ref5);
            EXPECT_FALSE(ref3 <= ref6);
            EXPECT_FALSE(ref4 <= ref1);
            EXPECT_FALSE(ref4 <= ref2);
            EXPECT_TRUE(ref4 <= ref3);
            EXPECT_TRUE(ref4 <= ref4);
            EXPECT_FALSE(ref4 <= ref5);
            EXPECT_FALSE(ref4 <= ref6);
            EXPECT_FALSE(ref5 <= ref1);
            EXPECT_FALSE(ref5 <= ref2);
            EXPECT_TRUE(ref5 <= ref3);
            EXPECT_TRUE(ref5 <= ref4);
            EXPECT_TRUE(ref5 <= ref5);
            EXPECT_TRUE(ref5 <= ref6);
            EXPECT_FALSE(ref6 <= ref1);
            EXPECT_FALSE(ref6 <= ref2);
            EXPECT_TRUE(ref6 <= ref3);
            EXPECT_TRUE(ref6 <= ref4);
            EXPECT_TRUE(ref6 <= ref5);
            EXPECT_TRUE(ref6 <= ref6);
        }

        TEST_F(JSONStringTest, IsGreaterEqualJSONString)
        {
            JSON::String ref1;
            JSON::String ref2(Default);
            JSON::String ref3(Default);
            JSON::String ref4(DefaultString);
            JSON::String ref5(OtherDefaultString);
            JSON::String ref6(OtherDefault);
            ref3 = Text;
            ref4 = TextString;
            ref5 = OtherText;
            ref6 = OtherTextString;
            EXPECT_TRUE(ref1 >= ref1);
            EXPECT_FALSE(ref1 >= ref2);
            EXPECT_FALSE(ref1 >= ref3);
            EXPECT_FALSE(ref1 >= ref4);
            EXPECT_FALSE(ref1 >= ref5);
            EXPECT_FALSE(ref1 >= ref6);
            EXPECT_TRUE(ref2 >= ref1);
            EXPECT_TRUE(ref2 >= ref2);
            EXPECT_FALSE(ref2 >= ref3);
            EXPECT_FALSE(ref2 >= ref4);
            EXPECT_FALSE(ref2 >= ref5);
            EXPECT_FALSE(ref2 >= ref6);
            EXPECT_TRUE(ref3 >= ref1);
            EXPECT_TRUE(ref3 >= ref2);
            EXPECT_TRUE(ref3 >= ref3);
            EXPECT_TRUE(ref3 >= ref4);
            EXPECT_TRUE(ref3 >= ref5);
            EXPECT_TRUE(ref3 >= ref6);
            EXPECT_TRUE(ref4 >= ref1);
            EXPECT_TRUE(ref4 >= ref2);
            EXPECT_TRUE(ref4 >= ref3);
            EXPECT_TRUE(ref4 >= ref4);
            EXPECT_TRUE(ref4 >= ref5);
            EXPECT_TRUE(ref4 >= ref6);
            EXPECT_TRUE(ref5 >= ref1);
            EXPECT_TRUE(ref5 >= ref2);
            EXPECT_FALSE(ref5 >= ref3);
            EXPECT_FALSE(ref5 >= ref4);
            EXPECT_TRUE(ref5 >= ref5);
            EXPECT_TRUE(ref5 >= ref6);
            EXPECT_TRUE(ref6 >= ref1);
            EXPECT_TRUE(ref6 >= ref2);
            EXPECT_FALSE(ref6 >= ref3);
            EXPECT_FALSE(ref6 >= ref4);
            EXPECT_TRUE(ref6 >= ref5);
            EXPECT_TRUE(ref6 >= ref6);
        }

        TEST_F(JSONStringTest, SerializeQuotedNonAssigned)
        {
            JSON::String target(Default);
            char buffer[BufferSize];
            char expected[] = "\"\"";
            uint16 offset = 0;
            uint16 resultActual = dynamic_cast<JSON::IDirect*>(&target)->Serialize(buffer, BufferSize, offset);
            uint16 resultExpected = 2;

            EXPECT_EQ(0, offset);
            EXPECT_EQ(resultExpected, resultActual);
            EXPECT_EQ(string(expected), string(buffer, resultActual));
        }

        TEST_F(JSONStringTest, SerializeQuotedAssigned)
        {
            JSON::String target(Default);
            target = Text;
            char buffer[BufferSize];
            char expected[] = "\"text\"";
            uint16 offset = 0;
            uint16 resultActual = dynamic_cast<JSON::IDirect*>(&target)->Serialize(buffer, BufferSize, offset);
            uint16 resultExpected = 6;

            EXPECT_EQ(0, offset);
            EXPECT_EQ(resultExpected, resultActual);
            EXPECT_EQ(string(expected), string(buffer, resultActual));
        }

        TEST_F(JSONStringTest, SerializeNonQuotedNonAssigned)
        {
            JSON::String target(Default, false);
            char buffer[BufferSize];
            char expected[] = "";
            uint16 offset = 0;
            uint16 resultActual = dynamic_cast<JSON::IDirect*>(&target)->Serialize(buffer, BufferSize, offset);
            uint16 resultExpected = 0;

            EXPECT_EQ(0, offset);
            EXPECT_EQ(resultExpected, resultActual);
            EXPECT_EQ(string(expected), string(buffer, resultActual));
        }

        TEST_F(JSONStringTest, SerializeNonQuotedAssigned)
        {
            JSON::String target(Default, false);
            target = Text;
            char buffer[BufferSize];
            char expected[] = "text";
            uint16 offset = 0;
            uint16 resultActual = dynamic_cast<JSON::IDirect*>(&target)->Serialize(buffer, BufferSize, offset);
            uint16 resultExpected = 4;

            EXPECT_EQ(0, offset);
            EXPECT_EQ(resultExpected, resultActual);
            EXPECT_EQ(string(expected), string(buffer, resultActual));
        }

        TEST_F(JSONStringTest, DeserializeQuoted)
        {
            JSON::String target;
            const char* input = "\"text\"";

            uint16 offset = 0;
            uint16 resultActual = dynamic_cast<JSON::IDirect*>(&target)->Deserialize(input, static_cast<uint16>(strlen(input)), offset);
            uint16 resultExpected = static_cast<uint16>(strlen(input));

            const char* expected = Text;

            EXPECT_EQ(0, offset);
            EXPECT_EQ(resultExpected, resultActual);
            EXPECT_EQ(string(expected), string(target));
        }

        TEST_F(JSONStringTest, DeserializeUnquoted)
        {
            JSON::String target(false);
            const char* input = "text";

            uint16 offset = 0;
            uint16 resultActual = dynamic_cast<JSON::IDirect*>(&target)->Deserialize(input, static_cast<uint16>(strlen(input)), offset);
            uint16 resultExpected = static_cast<uint16>(strlen(Text));

            const char* expected = "";

            EXPECT_EQ(strlen(input), offset);
            EXPECT_EQ(resultExpected, resultActual);
            EXPECT_EQ(string(expected), string(target));
        }

        TEST_F(JSONStringTest, DeserializeUnquotedWithTerminator)
        {
            JSON::String target(false);
            const char* input = "text,";

            uint16 offset = 0;
            uint16 resultActual = dynamic_cast<JSON::IDirect*>(&target)->Deserialize(input, static_cast<uint16>(strlen(input)), offset);
            uint16 resultExpected = static_cast<uint16>(strlen(Text));

            const char* expected = Text;

            EXPECT_EQ(0, offset);
            EXPECT_EQ(resultExpected, resultActual);
            EXPECT_EQ(string(expected), string(target));
        }

        TEST_F(JSONStringTest, DeserializeQuotedWithNullInput)
        {
            JSON::String target;
            const char* input = Null;

            uint16 offset = 0;
            uint16 resultActual = dynamic_cast<JSON::IDirect*>(&target)->Deserialize(input, static_cast<uint16>(strlen(input)), offset);
            uint16 resultExpected = static_cast<uint16>(strlen(Null));

            const char* expected = ""; // We expect nothing as a literal null is swallowed, and nothing is set.

            EXPECT_EQ(strlen(Null), offset);
            EXPECT_EQ(resultExpected, resultActual);
            EXPECT_EQ(string(expected), string(target));
        }

        TEST_F(JSONStringTest, DeserializeQuotedWithNullInputAndTerminator)
        {
            JSON::String target;
            const char* input = "null,";

            uint16 offset = 0;
            uint16 resultActual = dynamic_cast<JSON::IDirect*>(&target)->Deserialize(input, static_cast<uint16>(strlen(input)), offset);
            uint16 resultExpected = static_cast<uint16>(strlen(Null));

            const char* expected = ""; // We expect nothing as a literal null is swallowed, and nothing is set.

            EXPECT_EQ(0, offset);
            EXPECT_EQ(resultExpected, resultActual);
            EXPECT_EQ(string(expected), string(target));
        }

        TEST_F(JSONStringTest, DeserializeQuotedWithNullInputAndTermEnding)
        {
            JSON::String target;
            const char* input = "null,";

            uint16 offset = 0;
            uint16 resultActual = dynamic_cast<JSON::IDirect*>(&target)->Deserialize(input, static_cast<uint16>(strlen(input)), offset);
            uint16 resultExpected = static_cast<uint16>(strlen(Null));

            const char* expected = ""; // We expect nothing as a literal null is swallowed, and nothing is set.

            EXPECT_EQ(0, offset);
            EXPECT_EQ(resultExpected, resultActual);
            EXPECT_EQ(string(expected), string(target));
        }

    } // namespace Test
} // namespace Core
} // namespace WPEFramework
