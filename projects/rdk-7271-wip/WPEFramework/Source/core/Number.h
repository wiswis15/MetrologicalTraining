#ifndef __NUMBER_H
#define __NUMBER_H

#include "Module.h"
#include "Portability.h"
#include "TypeTraits.h"
#include "TextFragment.h"

namespace WPEFramework {
namespace Core {
    extern "C" {
    EXTERNAL unsigned char FromDigits(const TCHAR element);
    EXTERNAL unsigned char FromHexDigits(const TCHAR element);
    EXTERNAL unsigned char FromBase64(const TCHAR element);
    EXTERNAL unsigned char FromDirect(const TCHAR element);
    EXTERNAL TCHAR ToDigits(const unsigned char element);
    EXTERNAL TCHAR ToHexDigits(const unsigned char element);
    EXTERNAL TCHAR ToBase64(const unsigned char element);
    EXTERNAL TCHAR ToDirect(const unsigned char element);
    }

    template <class TYPE, bool SIGNED = (TypeTraits::sign<TYPE>::Signed == 1), const NumberBase BASETYPE = BASE_UNKNOWN>
    class NumberType {
    public:
        NumberType()
            : m_Value(0)
        {
        }
        NumberType(
            const TYPE Value)
            : m_Value(Value)
        {
        }
        NumberType(
            const TextFragment& text,
            const NumberBase Type = BASE_UNKNOWN)
            : m_Value(0)
        {
            NumberType<TYPE, SIGNED, BASETYPE>::Convert(text.Data(), text.Length(), m_Value, Type);
        }
        NumberType(
            const TCHAR Value[],
            const uint32 Length,
            const NumberBase Type = BASE_UNKNOWN)
            : m_Value(0)
        {
            NumberType<TYPE, SIGNED, BASETYPE>::Convert(Value, Length, m_Value, Type);
        }
        NumberType(
            const NumberType<TYPE, SIGNED, BASETYPE>& rhs)
            : m_Value(rhs.m_Value)
        {
        }
        ~NumberType()
        {
        }

        inline NumberType<TYPE, SIGNED, BASETYPE>&
        operator=(
            const NumberType<TYPE, SIGNED, BASETYPE>& Value)
        {
            return (NumberType<TYPE, SIGNED, BASETYPE>::operator=(Value.m_Value));
        }
        inline NumberType<TYPE, SIGNED, BASETYPE>&
        operator=(const TYPE Value)
        {
            m_Value = Value;

            return (*this);
        }

    public:
        inline static uint8 ToNetwork(uint8 input)
        {
            return (input);
        }
        inline static uint8 FromNetwork(uint8 input)
        {
            return (input);
        }
        inline static sint8 ToNetwork(sint8 input)
        {
            return (input);
        }
        inline static sint8 FromNetwork(sint8 input)
        {
            return (input);
        }
        inline static uint16 ToNetwork(uint16 input)
        {
            return (htons(input));
        }
        inline static uint16 FromNetwork(uint16 input)
        {
            return (ntohs(input));
        }
        inline static sint16 ToNetwork(sint16 input)
        {
            return (htons(input));
        }
        inline static sint16 FromNetwork(sint16 input)
        {
            return (ntohs(input));
        }
        inline static uint32 ToNetwork(uint32 input)
        {
            return (htonl(input));
        }
        inline static uint32 FromNetwork(uint32 input)
        {
            return (ntohl(input));
        }
        inline static sint32 ToNetwork(sint32 input)
        {
            return (htonl(input));
        }
        inline static sint32 FromNetwork(sint32 input)
        {
            return (ntohl(input));
        }
        inline static uint64 ToNetwork(uint64 input)
        {
#ifdef LITTLE_ENDIAN_PLATFORM
            return (input);
#else
            ASSERT(false); //TODO: To be implemented
            return (input);
#endif
        }
        inline static uint64 FromNetwork(uint64 input)
        {
#ifdef LITTLE_ENDIAN_PLATFORM
            return (input);
#else
            ASSERT(false); //TODO: To be implemented
            return (input);
#endif
        }
        inline static sint64 ToNetwork(sint64 input)
        {
#ifdef LITTLE_ENDIAN_PLATFORM
            return (input);
#else
            ASSERT(false); //TODO: To be implemented
            return (input);
#endif
        }
        inline static sint64 FromNetwork(sint64 input)
        {
#ifdef LITTLE_ENDIAN_PLATFORM
            return (input);
#else
            ASSERT(false); //TODO: To be implemented
            return (input);
#endif
        }
        static uint8 MaxSize()
        {
            if (BASETYPE == BASE_HEXADECIMAL) {
                return ((sizeof(TYPE) * 2) + (SIGNED == true ? 1 : 0) + 2 /* Number prefix 0x */ + 1 /* Closing null char */);
            }
            else if (BASETYPE == BASE_OCTAL) {
                return ((sizeof(TYPE) * 4) + (SIGNED == true ? 1 : 0) + 1 /* Number prefix 0 */ + 1 /* Closing null char */);
            }
            else {
                return ((sizeof(TYPE) * 3) + (SIGNED == true ? 1 : 0) + 1 /* Closing null char */);
            }
        }
        static uint32 Convert(
            const wchar_t* value,
            const uint32 length,
            TYPE& number,
            const NumberBase formatting)
        {
            return (Convert<TYPE>(value, length, number, formatting, TemplateIntToType<SIGNED>()));
        }
        static uint32 Convert(
            const char* value,
            const uint32 length,
            TYPE& number,
            const NumberBase formatting)
        {
            return (Convert<TYPE>(value, length, number, formatting, TemplateIntToType<SIGNED>()));
        }

        operator string() const
        {
            return (NumberType<TYPE, SIGNED, BASETYPE>::Text());
        }
        string Text() const
        {
            // Max size needed to dreate
            TCHAR Buffer[36];
            uint16 Index = FillBuffer(Buffer, sizeof(Buffer), BASETYPE);

            return (&Buffer[Index]);
        }
        uint16 Serialize(std::string& buffer) const
        {
            char Buffer[36];
            uint16 Index = FillBuffer(Buffer, sizeof(Buffer), BASETYPE);
            uint16 Result = (sizeof(Buffer) - Index - 1 /* Do not account for the closing char */);

            // Move it to the actual buffer
            buffer = std::string(&Buffer[Index], Result);

            return (Result);
        }

#ifndef __NO_WCHAR_SUPPORT__
        uint16 Serialize(std::wstring& buffer)
        {
            wchar_t Buffer[36];
            uint16 Index = FillBuffer(Buffer, sizeof(Buffer), BASETYPE);
            uint16 Result = (sizeof(Buffer) - Index - 1 /* Do not account for the closing char */);

            // Move it to the actual buffer
            buffer = std::wstring(&Buffer[Index], Result);

            return (Result);
        }
        uint16 Deserialize(const std::wstring& buffer)
        {
            return (Convert(buffer.data(), buffer.length(), m_Value, BASETYPE));
        }
#endif

        uint16 Deserialize(const std::string& buffer)
        {
            return (Convert(buffer.data(), buffer.length(), m_Value, BASETYPE));
        }
        inline TYPE& Value()
        {
            return (m_Value);
        }
        inline const TYPE& Value() const
        {
            return (m_Value);
        }
        inline operator TYPE() const
        {
            return (m_Value);
        }
        inline bool operator==(const NumberType<TYPE, SIGNED>& rhs) const
        {
            return (m_Value == rhs.m_Value);
        }
        inline bool operator!=(const NumberType<TYPE, SIGNED>& rhs) const
        {
            return (m_Value != rhs.m_Value);
        }
        inline bool operator<=(const NumberType<TYPE, SIGNED>& rhs) const
        {
            return (m_Value <= rhs.m_Value);
        }
        inline bool operator>=(const NumberType<TYPE, SIGNED>& rhs) const
        {
            return (m_Value >= rhs.m_Value);
        }
        inline bool operator<(const NumberType<TYPE, SIGNED>& rhs) const
        {
            return (m_Value < rhs.m_Value);
        }
        inline bool operator>(const NumberType<TYPE, SIGNED>& rhs) const
        {
            return (m_Value > rhs.m_Value);
        }

        inline NumberType<TYPE, SIGNED> operator+(const NumberType<TYPE, SIGNED>& rhs) const
        {
            return (NumberType<TYPE, SIGNED>(m_Value + rhs.m_Value));
        }
        inline NumberType<TYPE, SIGNED>& operator+=(const NumberType<TYPE, SIGNED>& rhs)
        {
            m_Value += rhs.m_Value;
            return (*this);
        }
        inline NumberType<TYPE, SIGNED> operator-(const NumberType<TYPE, SIGNED>& rhs) const
        {
            return (NumberType<TYPE, SIGNED>(m_Value - rhs.m_Value));
        }
        inline NumberType<TYPE, SIGNED>& operator-=(const NumberType<TYPE, SIGNED>& rhs)
        {
            m_Value -= rhs.m_Value;
            return (*this);
        }
        inline NumberType<TYPE, SIGNED> operator*(const NumberType<TYPE, SIGNED>& rhs) const
        {
            return (NumberType<TYPE, SIGNED>(m_Value * rhs.m_Value));
        }
        inline NumberType<TYPE, SIGNED>& operator*=(const NumberType<TYPE, SIGNED>& rhs)
        {
            m_Value *= rhs.m_Value;
            return (*this);
        }
        inline NumberType<TYPE, SIGNED> operator/(const NumberType<TYPE, SIGNED>& rhs) const
        {
            return (NumberType<TYPE, SIGNED>(m_Value / rhs.m_Value));
        }
        inline NumberType<TYPE, SIGNED>& operator/=(const NumberType<TYPE, SIGNED>& rhs)
        {
            m_Value /= rhs.m_Value;
            return (*this);
        }
        inline NumberType<TYPE, SIGNED> operator+(const TYPE Number) const
        {
            return (NumberType<TYPE, SIGNED>(m_Value + Number));
        }
        inline NumberType<TYPE, SIGNED>& operator+=(const TYPE Number)
        {
            m_Value += Number;
            return (*this);
        }
        inline NumberType<TYPE, SIGNED> operator-(const TYPE Number) const
        {
            return (NumberType<TYPE, SIGNED>(m_Value - Number));
        }
        inline NumberType<TYPE, SIGNED>& operator-=(const TYPE Number)
        {
            m_Value -= Number;
            return (*this);
        }
        inline NumberType<TYPE, SIGNED> operator*(const TYPE Number) const
        {
            return (NumberType<TYPE, SIGNED>(m_Value * Number));
        }
        inline NumberType<TYPE, SIGNED>& operator*=(const TYPE Number)
        {
            m_Value *= Number;
            return (*this);
        }
        inline NumberType<TYPE, SIGNED> operator/(const TYPE Number) const
        {
            return (NumberType<TYPE, SIGNED>(m_Value / Number));
        }
        inline NumberType<TYPE, SIGNED>& operator/=(const TYPE Number)
        {
            m_Value /= Number;
            return (*this);
        }

        static const TYPE Min()
        {
            return (TypedMin(TemplateIntToType<SIGNED>()));
        }
        static const TYPE Max()
        {
            return (TypedMax(TemplateIntToType<SIGNED>()));
        }
        inline bool Negative() const
        {
            return (TypedNegative(TemplateIntToType<SIGNED>()));
        }
        inline const TYPE Abs() const
        {
            return (TypedAbs(TemplateIntToType<SIGNED>()));
        }

    private:
        uint16 FillBuffer(char* buffer, const uint16 maxLength, const NumberBase BaseType) const
        {
            TCHAR* Location = &buffer[maxLength - 1];
            TYPE Value = NumberType<TYPE, SIGNED>(m_Value).Abs();
            uint16 Index = maxLength - 1 /* closing character */ - (Negative() ? 1 : 0) - (BaseType == BASE_OCTAL ? 1 : (BaseType == BASE_HEXADECIMAL ? 2 : 0));
            uint8 Divider = (BaseType == BASE_UNKNOWN ? BASE_DECIMAL : BaseType);

            // Close it with a terminating character!!
            *Location-- = '\0';

            // Convert the number to a string
            do {
                uint8 newDigit = (Value % Divider);
                if (newDigit < 10) {
                    *Location-- = static_cast<wchar_t>(newDigit + '0');
                }
                else {
                    *Location-- = static_cast<wchar_t>(newDigit - 10 + 'A');
                }
                Value = Value / Divider;
                Index--;

            } while ((Value != 0) && (Index > 0));

            if ((BaseType == BASE_OCTAL) || (BaseType == BASE_HEXADECIMAL)) {
                if (BaseType == BASE_HEXADECIMAL) {
                    *Location-- = 'x';
                }
                *Location-- = '0';
            }

            if (Negative()) {
                *Location = '-';
            }

            return (Index);
        }
        uint16 FillBuffer(wchar_t* buffer, const uint16 maxLength, const NumberBase BaseType)
        {
            wchar_t* Location = &buffer[maxLength];
            TYPE Value = NumberType<TYPE, SIGNED>(m_Value).Abs();
            uint16 Index = maxLength - (Negative() ? 1 : 0) - (BaseType == BASE_OCTAL ? 1 : (BaseType == BASE_HEXADECIMAL ? 2 : 0));
            uint8 Divider = (BaseType == BASE_UNKNOWN ? BASE_DECIMAL : BaseType);

            // Close it with a terminating character!!
            *Location-- = '\0';

            // Convert the number to a string
            do {
                uint8 newDigit = (Value % Divider);
                if (newDigit < 10) {
                    *Location-- = static_cast<wchar_t>(newDigit + '0');
                }
                else {
                    *Location-- = static_cast<wchar_t>(newDigit - 10 + 'A');
                }
                Value = Value / Divider;
                Index--;

            } while ((Value != 0) && (Index > 0));

            if ((BaseType == BASE_OCTAL) || (BaseType == BASE_HEXADECIMAL)) {
                if (BaseType == BASE_HEXADECIMAL) {
                    *Location-- = 'x';
                }
                *Location-- = '0';
            }

            if (Negative()) {
                *Location = '-';
            }

            return (Index);
        }
        template <typename NUMBER>
        static uint32
        Convert(
            const wchar_t* Start,
            const uint32 MaxLength,
            NUMBER& Value,
            const NumberBase Type,
            const TemplateIntToType<true>& /* For compile time diffrentiation */)
        {
            /* Do the conversion from string to the proper number. */
            bool Success = true;
            const wchar_t* Text = Start;
            bool Sign = false;
            NumberBase Base = Type;
            NUMBER Max = NUMBER_MAX_SIGNED(NUMBER);
            uint32 ItemsLeft = MaxLength;

            // We start at 0
            Value = 0;

            // Convert the number until we reach the 0 character.
            while ((ItemsLeft != 0) && (Success == true) && (*Text != '\0')) {
                if ((Value == 0) && (*Text == '0') && (Base == BASE_UNKNOWN)) {
                    // Base change, move over to an OCTAL conversion
                    Base = BASE_OCTAL;
                }
                else if ((Value == 0) && (toupper(*Text) == 'X') && (Base == BASE_OCTAL)) {
                    // Base change, move over to an HEXADECIMAL conversion
                    Base = BASE_HEXADECIMAL;
                }
                else if ((Value == 0) && ((*Text == '+') || ((*Text == '-')) || (*Text == ' ') || (*Text == '\t') || (*Text == '0'))) {
                    // Is it a sign change ???
                    if (*Text == '-') {
                        Sign = true;
                        Max = NUMBER_MIN_SIGNED(NUMBER);
                    }
                }
                else {
                    if (Base == BASE_UNKNOWN) {
                        Base = BASE_DECIMAL;
                    }

                    if ((*Text >= '0') && (*Text <= '7')) {
                        if (Sign) {
                            sint8 Digit = ('0' - *Text);

                            if ((Value >= (Max / Base)) && (Digit >= (Max - (Value * Base)))) {
                                Value = (Value * Base) + Digit;
                            }
                            else {
                                Success = false;
                            }
                        }
                        else {
                            sint8 Digit = (*Text - '0');

                            if ((Value <= (Max / Base)) && (Digit <= (Max - (Value * Base)))) {
                                Value = (Value * Base) + Digit;
                            }
                            else {
                                Success = false;
                            }
                        }
                    }
                    else if ((*Text >= '8') && (*Text <= '9') && (Base != BASE_OCTAL)) {
                        if (Sign) {
                            sint8 Digit = ('0' - *Text);

                            if ((Value >= (Max / Base)) && (Digit >= (Max - (Value * Base)))) {
                                Value = (Value * Base) + Digit;
                            }
                            else {
                                Success = false;
                            }
                        }
                        else {
                            sint8 Digit = (*Text - '0');

                            if ((Value <= (Max / Base)) && (Digit <= (Max - (Value * Base)))) {
                                Value = (Value * Base) + Digit;
                            }
                            else {
                                Success = false;
                            }
                        }
                    }
                    else if ((toupper(*Text) >= 'A') && (toupper(*Text) <= 'F') && (Base == BASE_HEXADECIMAL)) {
                        if (Sign) {
                            sint8 Digit = static_cast<sint8>('A' - toupper(*Text) - 10);

                            if ((Value >= (Max / Base)) && (Digit >= (Max - (Value * Base)))) {
                                Value = (Value * Base) + Digit;
                            }
                            else {
                                Success = false;
                            }
                        }
                        else {
                            sint8 Digit = static_cast<sint8>(toupper(*Text) - 'A' + 10);

                            if ((Value <= (Max / Base)) && (Digit <= (Max - (Value * Base)))) {
                                Value = (Value * Base) + Digit;
                            }
                            else {
                                Success = false;
                            }
                        }
                    }
                    else {
                        Success = false;
                    }
                }

                // Get the next character in line.
                if (Success) {
                    Text++;
                    ItemsLeft--;
                }
            }

            return (MaxLength - ItemsLeft);
        }

        template <typename NUMBER>
        static uint32
        Convert(
            const wchar_t* Start,
            const uint32 MaxLength,
            NUMBER& Value,
            const NumberBase Type,
            const TemplateIntToType<false>& /* For compile time diffrentiation */)
        {
            /* Do the conversion from string to the proper number. */
            bool Success = true;
            const wchar_t* Text = Start;
            NumberBase Base = Type;
            NUMBER Max = NUMBER_MAX_UNSIGNED(NUMBER);
            uint32 ItemsLeft = MaxLength;

            // We start at 0
            Value = 0;

            // Convert the number until we reach the 0 character.
            while ((ItemsLeft != 0) && (Success == true) && (*Text != '\0')) {
                if ((Value == 0) && (*Text == '0') && (Base == BASE_UNKNOWN)) {
                    // Base change, move over to an OCTAL conversion
                    Base = BASE_OCTAL;
                }
                else if ((Value == 0) && (toupper(*Text) == 'X') && (Base == BASE_OCTAL)) {
                    // Base change, move over to an HEXADECIMAL conversion
                    Base = BASE_HEXADECIMAL;
                }
                else if ((Value == 0) && ((*Text == '+') || (*Text == ' ') || (*Text == '\t') || (*Text == '0'))) {
                    // Skip all shit and other white spaces
                }
                else {
                    if (Base == BASE_UNKNOWN) {
                        Base = BASE_DECIMAL;
                    }

                    if ((*Text >= '0') && (*Text <= '7')) {
                        uint8 Digit = (*Text - '0');

                        if ((Value <= (Max / Base)) && (Digit <= (Max - (Value * Base)))) {
                            Value = (Value * static_cast<uint8>(Base)) + Digit;
                        }
                        else {
                            Success = false;
                        }
                    }
                    else if ((*Text >= '8') && (*Text <= '9') && (Base != BASE_OCTAL)) {
                        uint8 Digit = (*Text - '0');

                        if ((Value <= (Max / Base)) && (Digit <= (Max - (Value * Base)))) {
                            Value = (Value * static_cast<uint8>(Base)) + Digit;
                        }
                        else {
                            Success = false;
                        }
                    }
                    else if ((toupper(*Text) >= 'A') && (toupper(*Text) <= 'F') && (Base == BASE_HEXADECIMAL)) {
                        uint8 Digit = static_cast<uint8>(toupper(*Text) - 'A' + 10);

                        if ((Value <= (Max / Base)) && (Digit <= (Max - (Value * Base)))) {
                            Value = (Value * static_cast<uint8>(Base)) + Digit;
                        }
                        else {
                            Success = false;
                        }
                    }
                    else {
                        Success = false;
                    }
                }

                // Get the next character in line.
                if (Success) {
                    Text++;
                    ItemsLeft--;
                }
            }

            return (MaxLength - ItemsLeft);
        }
        template <typename NUMBER>
        static uint32
        Convert(
            const char* Start,
            const uint32 MaxLength,
            NUMBER& Value,
            const NumberBase Type,
            const TemplateIntToType<true>& /* For compile time diffrentiation */)
        {
            /* Do the conversion from string to the proper number. */
            bool Success = true;
            const char* Text = Start;
            bool Sign = false;
            NumberBase Base = Type;
            NUMBER Max = NUMBER_MAX_SIGNED(NUMBER);
            uint32 ItemsLeft = MaxLength;

            // We start at 0
            Value = 0;

            // Convert the number until we reach the 0 character.
            while ((ItemsLeft != 0) && (Success == true) && (*Text != '\0')) {
                if ((Value == 0) && (*Text == '0') && (Base == BASE_UNKNOWN)) {
                    // Base change, move over to an OCTAL conversion
                    Base = BASE_OCTAL;
                }
                else if ((Value == 0) && (toupper(*Text) == 'X') && (Base == BASE_OCTAL)) {
                    // Base change, move over to an HEXADECIMAL conversion
                    Base = BASE_HEXADECIMAL;
                }
                else if ((Value == 0) && ((*Text == '+') || ((*Text == '-')) || (*Text == ' ') || (*Text == '\t') || (*Text == '0'))) {
                    // Is it a sign change ???
                    if (*Text == '-') {
                        Sign = true;
                        Max = NUMBER_MIN_SIGNED(NUMBER);
                    }
                }
                else {
                    if (Base == BASE_UNKNOWN) {
                        Base = BASE_DECIMAL;
                    }

                    if ((*Text >= '0') && (*Text <= '7')) {
                        if (Sign) {
                            sint8 Digit = ('0' - *Text);

                            if ((Value >= (Max / Base)) && (Digit >= (Max - (Value * Base)))) {
                                Value = (Value * Base) + Digit;
                            }
                            else {
                                Success = false;
                            }
                        }
                        else {
                            sint8 Digit = (*Text - '0');

                            if ((Value <= (Max / Base)) && (Digit <= (Max - (Value * Base)))) {
                                Value = (Value * Base) + Digit;
                            }
                            else {
                                Success = false;
                            }
                        }
                    }
                    else if ((*Text >= '8') && (*Text <= '9') && (Base != BASE_OCTAL)) {
                        if (Sign) {
                            sint8 Digit = ('0' - *Text);

                            if ((Value >= (Max / Base)) && (Digit >= (Max - (Value * Base)))) {
                                Value = (Value * Base) + Digit;
                            }
                            else {
                                Success = false;
                            }
                        }
                        else {
                            sint8 Digit = (*Text - '0');

                            if ((Value <= (Max / Base)) && (Digit <= (Max - (Value * Base)))) {
                                Value = (Value * Base) + Digit;
                            }
                            else {
                                Success = false;
                            }
                        }
                    }
                    else if ((toupper(*Text) >= 'A') && (toupper(*Text) <= 'F') && (Base == BASE_HEXADECIMAL)) {
                        if (Sign) {
                            sint8 Digit = static_cast<sint8>('A' - toupper(*Text) - 10);

                            if ((Value >= (Max / Base)) && (Digit >= (Max - (Value * Base)))) {
                                Value = (Value * Base) + Digit;
                            }
                            else {
                                Success = false;
                            }
                        }
                        else {
                            sint8 Digit = static_cast<sint8>(toupper(*Text) - 'A' + 10);

                            if ((Value <= (Max / Base)) && (Digit <= (Max - (Value * Base)))) {
                                Value = (Value * Base) + Digit;
                            }
                            else {
                                Success = false;
                            }
                        }
                    }
                    else {
                        Success = false;
                    }
                }

                // Get the next character in line.
                if (Success) {
                    Text++;
                    ItemsLeft--;
                }
            }

            return (MaxLength - ItemsLeft);
        }

        template <typename NUMBER>
        static uint32
        Convert(
            const char* Start,
            const uint32 MaxLength,
            NUMBER& Value,
            const NumberBase Type,
            const TemplateIntToType<false>& /* For compile time diffrentiation */)
        {
            /* Do the conversion from string to the proper number. */
            bool Success = true;
            const char* Text = Start;
            NumberBase Base = Type;
            NUMBER Max = NUMBER_MAX_UNSIGNED(NUMBER);
            uint32 ItemsLeft = MaxLength;

            // We start at 0
            Value = 0;

            // Convert the number until we reach the 0 character.
            while ((ItemsLeft != 0) && (Success == true) && (*Text != '\0')) {
                if ((Value == 0) && (*Text == '0') && (Base == BASE_UNKNOWN)) {
                    // Base change, move over to an OCTAL conversion
                    Base = BASE_OCTAL;
                }
                else if ((Value == 0) && (toupper(*Text) == 'X') && ((Base == BASE_OCTAL) || (Base == BASE_HEXADECIMAL))) {
                    // Base change, move over to an HEXADECIMAL conversion
                    Base = BASE_HEXADECIMAL;
                }
                else if ((Value == 0) && ((*Text == '+') || (*Text == ' ') || (*Text == '\t') || (*Text == '0'))) {
                    // Skip all shit and other white spaces
                }
                else {
                    if (Base == BASE_UNKNOWN) {
                        Base = BASE_DECIMAL;
                    }

                    if ((*Text >= '0') && (*Text <= '7')) {
                        uint8 Digit = (*Text - '0');

                        if ((Value <= (Max / Base)) && (Digit <= (Max - (Value * Base)))) {
                            Value = (Value * static_cast<uint8>(Base)) + Digit;
                        }
                        else {
                            Success = false;
                        }
                    }
                    else if ((*Text >= '8') && (*Text <= '9') && (Base != BASE_OCTAL)) {
                        uint8 Digit = (*Text - '0');

                        if ((Value <= (Max / Base)) && (Digit <= (Max - (Value * Base)))) {
                            Value = (Value * static_cast<uint8>(Base)) + Digit;
                        }
                        else {
                            Success = false;
                        }
                    }
                    else if ((toupper(*Text) >= 'A') && (toupper(*Text) <= 'F') && (Base == BASE_HEXADECIMAL)) {
                        uint8 Digit = static_cast<uint8>(toupper(*Text) - 'A' + 10);

                        if ((Value <= (Max / Base)) && (Digit <= (Max - (Value * Base)))) {
                            Value = (Value * static_cast<uint8>(Base)) + Digit;
                        }
                        else {
                            Success = false;
                        }
                    }
                    else {
                        Success = false;
                    }
                }

                // Get the next character in line.
                if (Success) {
                    Text++;
                    ItemsLeft--;
                }
            }

            return (MaxLength - ItemsLeft);
        }
        inline const TYPE TypedAbs(const TemplateIntToType<true>& /* For compile time diffrentiation */) const
        {
            return (m_Value < 0 ? -m_Value : m_Value);
        }
        inline const TYPE TypedAbs(const TemplateIntToType<false>& /* For compile time diffrentiation */) const
        {
            return (m_Value);
        }
        inline bool TypedNegative(const TemplateIntToType<true>& /* For compile time diffrentiation */) const
        {
            return (m_Value < 0);
        }
        inline bool TypedNegative(const TemplateIntToType<false>& /* For compile time diffrentiation */) const
        {
            return (false);
        }
        static const TYPE TypedMin(const TemplateIntToType<false>& /* For compile time diffrentiation */)
        {
            return (NUMBER_MIN_UNSIGNED(TYPE));
        }
        static const TYPE TypedMax(const TemplateIntToType<false>& /* For compile time diffrentiation */)
        {
            return (NUMBER_MAX_UNSIGNED(TYPE));
        }
        static const TYPE TypedMin(const TemplateIntToType<true>& /* For compile time diffrentiation */)
        {
            return (NUMBER_MIN_SIGNED(TYPE));
        }
        static const TYPE TypedMax(const TemplateIntToType<true>& /* For compile time diffrentiation */)
        {
            return (NUMBER_MAX_SIGNED(TYPE));
        }

    private:
        TYPE m_Value;
    };

    class EXTERNAL Fractional {
    public:
        Fractional();
        Fractional(const sint32& integer, const uint32& remainder = 0);
        Fractional(const Fractional& copy);
        virtual ~Fractional();

        Fractional& operator=(const Fractional& RHS);

    public:
        template <typename FLOATINGPOINTTYPE>
        FLOATINGPOINTTYPE Composit() const
        {
            uint16 count = static_cast<uint16>(log10((float)m_Remainder)) + 1;
            uint32 base = static_cast<uint32>(pow((float)10, count));

            return (static_cast<FLOATINGPOINTTYPE>(m_Integer) + static_cast<FLOATINGPOINTTYPE>(m_Remainder / base));
        }

        inline sint32 Integer() const
        {
            return (m_Integer);
        }

        inline uint32 Remainder() const
        {
            return (m_Remainder);
        }

        bool operator==(const Fractional& RHS) const;
        bool operator!=(const Fractional& RHS) const;
        bool operator>=(const Fractional& RHS) const;
        bool operator<=(const Fractional& RHS) const;
        bool operator>(const Fractional& RHS) const;
        bool operator<(const Fractional& RHS) const;
        string Text(const uint8 decimalPlaces) const;

    private:
        sint32 m_Integer;
        uint32 m_Remainder;
    };

    template <bool SIGNED>
    class NumberType<Fractional, SIGNED> {
    public:
        static uint32 Convert(
            const TCHAR* value,
            const uint32 length,
            Fractional& number,
            const NumberBase formatting)
        {
            sint32 integer;
            uint32 result = NumberType<sint32, true>::Convert(value, length, integer, formatting);
            number = Fractional(integer, 0);

            return (result);
        }

        static const Fractional Min()
        {
            return (TypedMin(TemplateIntToType<SIGNED>()));
        }
        static const Fractional Max()
        {
            return (Fractional(NUMBER_MAX_SIGNED(sint32), NUMBER_MAX_UNSIGNED(uint32)));
        }

    private:
        static const Fractional TypedMin(const TemplateIntToType<false>& /* For compile time diffrentiation */)
        {
            return (Fractional(0, 0));
        }
        static const Fractional TypedMin(const TemplateIntToType<true>& /* For compile time diffrentiation */)
        {
            return (Fractional(NUMBER_MIN_SIGNED(sint32), NUMBER_MAX_UNSIGNED(uint32)));
        }
    };

    typedef NumberType<uint8, false> Unsigned8;
    typedef NumberType<sint8, true> Signed8;
    typedef NumberType<uint16, false> Unsigned16;
    typedef NumberType<sint16, true> Signed16;
    typedef NumberType<uint32, false> Unsigned32;
    typedef NumberType<sint32, true> Signed32;
    typedef NumberType<uint64, false> Unsigned64;
    typedef NumberType<sint64, true> Signed64;
}
} // namespace Core

#endif // __NUMBER_H
