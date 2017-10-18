#include <generics/JSON.h>
#include <gtest/gtest.h>

namespace WPEFramework {
namespace Core {
    namespace Test {

        static const size_t BufferSize = 1024;
        static const char* TrueText = "true";
        static const char* FalseText = "false";
        static const char* TrueTextUpperCase = "TRUE";
        static const char* FalseTextUpperCase = "FALSE";
        static const char* TrueTextNumeric = "1";
        static const char* FalseTextNumeric = "0";

        class JSONBoolTest : public ::testing::Test {
        protected:
            virtual void SetUp() {}

            virtual void TearDown() {}
        };

        TEST_F(JSONBoolTest, Construct)
        {
            JSON::Boolean target;
            EXPECT_EQ(JSON::ParserType::PARSE_BUFFERED, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_FALSE(((JSON::IElement*)&target)->IsSet());
            EXPECT_FALSE(target.Default());
            EXPECT_FALSE(target.Value());
            EXPECT_FALSE(bool(target));
        }

        TEST_F(JSONBoolTest, ConstructValue)
        {
            JSON::Boolean target(true);
            EXPECT_EQ(JSON::ParserType::PARSE_BUFFERED, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_FALSE(((JSON::IElement*)&target)->IsSet());
            EXPECT_TRUE(target.Default());
            EXPECT_TRUE(target.Value());
            EXPECT_TRUE(bool(target));
        }

        TEST_F(JSONBoolTest, ConstructCopyNonAssigned)
        {
            JSON::Boolean other(true);
            JSON::Boolean target(other);
            EXPECT_EQ(JSON::ParserType::PARSE_BUFFERED, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_FALSE(((JSON::IElement*)&target)->IsSet());
            EXPECT_TRUE(target.Default());
            EXPECT_TRUE(target.Value());
            EXPECT_TRUE(bool(target));
        }

        TEST_F(JSONBoolTest, ConstructCopyAssigned)
        {
            JSON::Boolean other(false);
            other = true;
            JSON::Boolean target(other);
            EXPECT_EQ(JSON::ParserType::PARSE_BUFFERED, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_TRUE(((JSON::IElement*)&target)->IsSet());
            EXPECT_FALSE(target.Default());
            EXPECT_TRUE(target.Value());
            EXPECT_TRUE(bool(target));
        }

        TEST_F(JSONBoolTest, AssignJSONBooleanNonAssigned)
        {
            JSON::Boolean other(true);
            JSON::Boolean target;
            target = other;
            EXPECT_EQ(JSON::ParserType::PARSE_BUFFERED, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_FALSE(((JSON::IElement*)&target)->IsSet());
            EXPECT_FALSE(target.Default()); // Default is not overwritten when the assigned value is not set
            EXPECT_FALSE(target.Value());
            EXPECT_FALSE(bool(target));
        }

        TEST_F(JSONBoolTest, AssignJSONBooleanAssignedDefaultFalse)
        {
            JSON::Boolean other(false);
            other = true;
            JSON::Boolean target;
            target = other;
            EXPECT_EQ(JSON::ParserType::PARSE_BUFFERED, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_TRUE(((JSON::IElement*)&target)->IsSet());
            EXPECT_FALSE(target.Default());
            EXPECT_TRUE(target.Value());
            EXPECT_TRUE(bool(target));
        }

        TEST_F(JSONBoolTest, AssignJSONBooleanAssignedDefaultTrue)
        {
            JSON::Boolean other(true);
            other = true;
            JSON::Boolean target;
            target = other;
            EXPECT_EQ(JSON::ParserType::PARSE_BUFFERED, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_TRUE(((JSON::IElement*)&target)->IsSet());
            EXPECT_TRUE(target.Default()); // Default is not overwritten
            EXPECT_TRUE(target.Value());
            EXPECT_TRUE(bool(target));
        }

        TEST_F(JSONBoolTest, AssignBooleanDefaultFalse)
        {
            JSON::Boolean target(false);
            target = true;
            EXPECT_EQ(JSON::ParserType::PARSE_BUFFERED, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_TRUE(((JSON::IElement*)&target)->IsSet());
            EXPECT_FALSE(target.Default());
            EXPECT_TRUE(target.Value());
            EXPECT_TRUE(bool(target));
        }

        TEST_F(JSONBoolTest, AssignBooleanDefaultTrue)
        {
            JSON::Boolean target(true);
            target = true;
            EXPECT_EQ(JSON::ParserType::PARSE_BUFFERED, dynamic_cast<JSON::IElement*>(&target)->Type());
            EXPECT_EQ(&target, dynamic_cast<JSON::IElement*>(&target)->BufferParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->DirectParser());
            EXPECT_EQ(nullptr, dynamic_cast<JSON::IElement*>(&target)->ElementIterator());
            EXPECT_TRUE(((JSON::IElement*)&target)->IsSet());
            EXPECT_TRUE(target.Default());
            EXPECT_TRUE(target.Value());
            EXPECT_TRUE(bool(target));
        }

        TEST_F(JSONBoolTest, Clear)
        {
            JSON::Boolean target(false);
            target = true;
            EXPECT_TRUE(((JSON::IElement*)&target)->IsSet());
            dynamic_cast<JSON::IElement*>(&target)->Clear();
            EXPECT_FALSE(((JSON::IElement*)&target)->IsSet());
        }

        TEST_F(JSONBoolTest, SerializeNonAssignedDefaultFalse)
        {
            JSON::Boolean target(false);
            string buffer;
            string expected = FalseText;
            dynamic_cast<JSON::IBuffered*>(&target)->Serialize(buffer);

            EXPECT_EQ(expected, buffer);
        }

        TEST_F(JSONBoolTest, SerializeNonAssignedDefaultTrue)
        {
            JSON::Boolean target(true);
            string buffer;
            string expected = TrueText;
            dynamic_cast<JSON::IBuffered*>(&target)->Serialize(buffer);

            EXPECT_EQ(expected, buffer);
        }

        TEST_F(JSONBoolTest, SerializeAssigned)
        {
            JSON::Boolean target(true);
            target = false;
            string buffer;
            string expected = FalseText;
            dynamic_cast<JSON::IBuffered*>(&target)->Serialize(buffer);

            EXPECT_EQ(expected, buffer);
        }

        TEST_F(JSONBoolTest, DeserializeNumeric)
        {
            JSON::Boolean target;

            string input = FalseTextNumeric;
            bool expected = false;

            dynamic_cast<JSON::IBuffered*>(&target)->Deserialize(input);

            EXPECT_FALSE(target.Default());
            EXPECT_EQ(expected, target.Value());
            EXPECT_EQ(expected, bool(target));

            input = TrueTextNumeric;
            expected = true;

            dynamic_cast<JSON::IBuffered*>(&target)->Deserialize(input);

            EXPECT_FALSE(target.Default());
            EXPECT_EQ(expected, target.Value());
            EXPECT_EQ(expected, bool(target));
        }

        TEST_F(JSONBoolTest, DeserializeUpperCase)
        {
            JSON::Boolean target;

            string input = FalseTextUpperCase;
            bool expected = false;

            dynamic_cast<JSON::IBuffered*>(&target)->Deserialize(input);

            EXPECT_FALSE(target.Default());
            EXPECT_EQ(expected, target.Value());
            EXPECT_EQ(expected, bool(target));

            input = TrueTextUpperCase;
            expected = true;

            dynamic_cast<JSON::IBuffered*>(&target)->Deserialize(input);

            EXPECT_FALSE(target.Default());
            EXPECT_EQ(expected, target.Value());
            EXPECT_EQ(expected, bool(target));
        }

        TEST_F(JSONBoolTest, Deserialize)
        {
            JSON::Boolean target;

            string input = FalseText;
            bool expected = false;

            dynamic_cast<JSON::IBuffered*>(&target)->Deserialize(input);

            EXPECT_FALSE(target.Default());
            EXPECT_EQ(expected, target.Value());
            EXPECT_EQ(expected, bool(target));

            input = TrueText;
            expected = true;

            dynamic_cast<JSON::IBuffered*>(&target)->Deserialize(input);

            EXPECT_FALSE(target.Default());
            EXPECT_EQ(expected, target.Value());
            EXPECT_EQ(expected, bool(target));
        }

    } // namespace Test
} // namespace Core
} // namespace WPEFramework
