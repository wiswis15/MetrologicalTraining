#ifndef __TEXTFRAGMENT_H
#define __TEXTFRAGMENT_H

#include "Module.h"
#include "Portability.h"
#include "Trace.h"

namespace WPEFramework {
namespace Core {
    class Fractional;

    class EXTERNAL TextFragment {
    protected:
        class EXTERNAL Index {
        public:
            inline Index(uint32 begin, uint32 length)
                : m_Begin(begin)
                , m_Length(length)
            {
            }
            inline Index(const Index& copy, uint32 offset, uint32 length)
                : m_Begin(copy.m_Begin + offset)
            {
                if (length == NUMBER_MAX_UNSIGNED(uint32)) {
                    ASSERT(offset <= copy.m_Length);

                    m_Length = copy.m_Length - offset;
                }
                else {
                    ASSERT((length + offset) <= copy.m_Length);
                    m_Length = length;
                }
            }
            inline Index(const Index& copy)
                : m_Begin(copy.m_Begin)
                , m_Length(copy.m_Length)
            {
            }
            inline ~Index()
            {
            }

            inline Index& operator=(const Index& RHS)
            {
                m_Begin = RHS.m_Begin;
                m_Length = RHS.m_Length;

                return (*this);
            }

            inline uint32 Begin() const
            {
                return (m_Begin);
            }

            inline uint32 End() const
            {
                return (m_Begin + m_Length);
            }

            inline uint32 Length() const
            {
                return (m_Length);
            }

            inline void Increment(const uint32 offset)
            {
                if (offset > m_Length) {
                    m_Begin += m_Length;
                    m_Length = 0;
                }
                else {
                    m_Begin += offset;
                    m_Length -= offset;
                }
            }

            inline void Decrement(const uint32 offset)
            {
                if (offset > m_Begin) {
                    m_Length += m_Begin;
                    m_Begin = 0;
                }
                else {
                    m_Begin -= offset;
                    m_Length += offset;
                }
            }
            inline void SetIndexInfo(const uint32 offset, const uint32 length)
            {
                m_Begin = offset;
                m_Length = length;
            }

        private:
            uint32 m_Begin;
            uint32 m_Length;
        };

    private:
        inline void SetIndexInfo(const uint32 offset, const uint32 length)
        {
            m_Index.SetIndexInfo(offset, length);
        }

    public:
        TextFragment()
            : m_Index(0, 0)
            , m_Start(NULL)
            , m_Buffer()
        {
        }
        explicit TextFragment(const TCHAR text[])
            : m_Index(0, static_cast<uint32>(_tcslen(text)))
            , m_Start(text)
            , m_Buffer()
        {
        }
        explicit TextFragment(const TCHAR text[], const uint32 length)
            : m_Index(0, length)
            , m_Start(text)
            , m_Buffer()
        {
        }
        TextFragment(const TCHAR text[], const uint32 offset, const uint32 length)
            : m_Index(offset, length)
            , m_Start(text)
            , m_Buffer()
        {
            ASSERT(m_Index.End() <= _tcslen(text));
        }
        explicit TextFragment(const string& text)
            : m_Index(0, static_cast<uint32>(text.length()))
            , m_Start(NULL)
            , m_Buffer(text)
        {
        }
        TextFragment(const string& text, const uint32 offset, const uint32 length)
            : m_Index(offset, length)
            , m_Start(NULL)
            , m_Buffer(text)
        {
            ASSERT(m_Index.End() <= text.length());
        }
        TextFragment(const TextFragment& base, const uint32 offset, const uint32 length)
            : m_Index(base.m_Index, offset, length)
            , m_Start(base.m_Start)
            , m_Buffer(base.m_Buffer)
        {
        }
        TextFragment(const TextFragment& copy)
            : m_Index(copy.m_Index)
            , m_Start(copy.m_Start)
            , m_Buffer(copy.m_Buffer)
        {
        }
        ~TextFragment()
        {
        }

        TextFragment& operator=(const TextFragment& RHS)
        {
            m_Index = RHS.m_Index;
            m_Start = RHS.m_Start;
            m_Buffer = RHS.m_Buffer;

            return (*this);
        }
        inline void Clear()
        {
            m_Index.SetIndexInfo(0, 0);
            m_Start = NULL,
            m_Buffer.clear();
        }
        inline const TCHAR& operator[](const uint32 index) const
        {
            ASSERT(index < m_Index.Length());

            return (m_Start == NULL ? m_Buffer[m_Index.Begin() + index] : m_Start[m_Index.Begin() + index]);
        }

        inline bool operator==(const TextFragment& RHS) const
        {
            return (equal_case_sensitive(RHS));
        }

        inline bool operator==(const TCHAR RHS[]) const
        {
            return (equal_case_sensitive(TextFragment(RHS, 0, static_cast<uint32>(_tcslen(RHS)))));
        }

        inline bool operator==(const string& RHS) const
        {
            return (equal_case_sensitive(TextFragment(RHS)));
        }

        inline bool operator!=(const TextFragment& RHS) const
        {
            return (!operator==(RHS));
        }

        inline bool operator!=(const string& RHS) const
        {
            return (!operator==(RHS));
        }

        inline bool operator!=(const TCHAR RHS[]) const
        {
            return (!operator==(RHS));
        }

        inline bool OnMarker(const TCHAR characters[]) const
        {
            return (on_given_character(0, characters));
        }

        inline bool IsEmpty() const
        {
            return (m_Index.Length() == 0);
        }

        inline uint32 Length() const
        {
            return (m_Index.Length());
        }

        inline const TCHAR* Data() const
        {
            return (m_Start == NULL ? &(m_Buffer[m_Index.Begin()]) : &m_Start[m_Index.Begin()]);
        }

        inline const string Text() const
        {
            return (m_Start == NULL ? (((m_Index.Begin() == 0) && (m_Index.Length() == m_Buffer.length())) ? m_Buffer : m_Buffer.substr(m_Index.Begin(), m_Index.Length())) : string(&(m_Start[m_Index.Begin()]), m_Index.Length()));
        }

        inline bool EqualText(const TextFragment& RHS, const bool caseSensitive = false) const
        {
            return (caseSensitive == true ? equal_case_sensitive(RHS) : equal_case_insensitive(RHS));
        }

        inline bool EqualText(const TCHAR* compare, const uint32 offset VARIABLE_IS_NOT_USED = 0, const uint32 length = 0, const bool caseSensitive = true) const
        {
            uint32 size = (length != 0 ? length : static_cast<uint32>(_tcslen(compare)));

            return (caseSensitive == true ? equal_case_sensitive(TextFragment(compare, size)) : equal_case_insensitive(TextFragment(compare, size)));
        }

        inline uint32 ForwardFind(const TCHAR delimiter, const uint32 offset = 0) const
        {
            return (find_first_of(offset, delimiter));
        }

        inline uint32 ForwardFind(const TCHAR delimiter[], const uint32 offset = 0) const
        {
            return (find_first_of(offset, delimiter));
        }

        inline uint32 ForwardSkip(const TCHAR delimiter[], const uint32 offset = 0) const
        {
            return (find_first_not_of(offset, delimiter));
        }

        inline uint32 ReverseFind(const TCHAR delimiter, const uint32 offset = 0) const
        {
            return (find_last_of(offset, delimiter));
        }

        inline uint32 ReverseFind(const TCHAR delimiter[], const uint32 offset = 0) const
        {
            return (find_last_of(offset, delimiter));
        }

        inline uint32 ReverseSkip(const TCHAR delimiter[], const uint32 offset = 0) const
        {
            return (find_last_not_of(offset, delimiter));
        }

    protected:
        inline void Forward(const uint32 forward)
        {
            m_Index.Increment(forward);
        }

        inline void Reverse(const uint32 reverse)
        {
            m_Index.Decrement(reverse);
        }

        inline const Index& PartIndex() const
        {
            return (m_Index);
        }

    private:
        bool on_given_character(const uint32 offset, const TCHAR characters[]) const
        {
            bool equal = false;

            if (offset < m_Index.Length()) {
                if (m_Start == NULL) {
                    equal = (_tcschr(characters, m_Buffer[m_Index.Begin() + offset]) != NULL);
                }
                else {
                    equal = (_tcschr(characters, m_Start[m_Index.Begin() + offset]) != NULL);
                }
            }

            return (equal);
        }

        bool equal_case_sensitive(const TextFragment& RHS) const
        {
            bool equal = false;

            if (RHS.m_Index.Length() == m_Index.Length()) {
                if (m_Start != NULL) {
                    if (RHS.m_Start == NULL) {
                        equal = (RHS.m_Buffer.compare(RHS.m_Index.Begin(), RHS.m_Index.Length(), &(m_Start[m_Index.Begin()]), m_Index.Length()) == 0);
                    }
                    else {
                        equal = (_tcsncmp(&(RHS.m_Start[RHS.m_Index.Begin()]), &(m_Start[m_Index.Begin()]), RHS.m_Index.Length()) == 0);
                    }
                }
                else {
                    if (RHS.m_Start == NULL) {
                        equal = (m_Buffer.compare(m_Index.Begin(), m_Index.Length(), RHS.m_Buffer, RHS.m_Index.Begin(), RHS.m_Index.Length()) == 0);
                    }
                    else {
                        equal = (m_Buffer.compare(m_Index.Begin(), m_Index.Length(), &(RHS.m_Start[RHS.m_Index.Begin()]), RHS.m_Index.Length()) == 0);
                    }
                }
            }

            return (equal);
        }

        bool equal_case_insensitive(const TextFragment& RHS) const
        {
            bool equal = false;

            if (RHS.m_Index.Length() == m_Index.Length()) {
                if (m_Start != NULL) {
                    if (RHS.m_Start == NULL) {
                        equal = (_tcsnicmp(&(RHS.m_Buffer[RHS.m_Index.Begin()]), &(m_Start[m_Index.Begin()]), m_Index.Length()) == 0);
                    }
                    else {
                        equal = (_tcsnicmp(&(RHS.m_Start[RHS.m_Index.Begin()]), &(m_Start[m_Index.Begin()]), m_Index.Length()) == 0);
                    }
                }
                else {
                    if (RHS.m_Start == NULL) {
                        equal = (_tcsnicmp(&(RHS.m_Buffer[RHS.m_Index.Begin()]), &(m_Buffer[m_Index.Begin()]), m_Index.Length()) == 0);
                    }
                    else {
                        equal = (_tcsnicmp(&(RHS.m_Start[RHS.m_Index.Begin()]), &(m_Buffer[m_Index.Begin()]), m_Index.Length()) == 0);
                    }
                }
            }

            return (equal);
        }

        uint32 find_first_of(const uint32 offset, const TCHAR delimiter) const
        {
            // If we do not find it, we end up at the end.
            uint32 index = m_Index.Length();

            if (offset < m_Index.Length()) {
                if (m_Start != NULL) {
                    const TCHAR* start = &m_Start[m_Index.Begin()];
                    const TCHAR* entry = _tcschr(&start[offset], delimiter);

                    if (entry != NULL) {
                        index = static_cast<uint32>(entry - start);

                        ASSERT((offset + index) <= m_Index.Length());
                    }
                }
                else {
                    uint32 found = static_cast<uint32>(m_Buffer.find_first_of(delimiter, m_Index.Begin() + offset));

                    if (found <= m_Index.End()) {
                        index = found - m_Index.Begin();
                    }
                }
            }

            return (index);
        }

        uint32 find_last_of(const uint32 offset, const TCHAR delimiter) const
        {
            // If we do not find it, we end up at the end.
            uint32 index = NUMBER_MAX_UNSIGNED(uint32);

            if (offset < m_Index.Length()) {
                if (m_Start != NULL) {
                    const TCHAR* start = &m_Start[m_Index.Begin()];
                    const TCHAR* entry = _tcsrchr(&start[offset], delimiter);

                    if (entry != NULL) {
                        index = static_cast<uint32>(entry - start);

                        ASSERT((offset + index) <= m_Index.Length());
                    }
                }
                else {
                    uint32 found = static_cast<uint32>(m_Buffer.find_last_of(delimiter, m_Index.End()));

                    if ((found != m_Index.End()) && (found >= (m_Index.Begin() + offset))) {
                        index = found - m_Index.Begin();
                    }
                }
            }

            return (index);
        }

        uint32 find_first_of(const uint32 offset, const TCHAR delimiter[]) const
        {
            // If we do not find it, we end up at the end.
            uint32 index = m_Index.Length();

            if (offset < m_Index.Length()) {
                if (m_Start != NULL) {
                    const TCHAR* pointer = &(Data()[offset]);
                    uint32 length = m_Index.Length() - offset;

                    while ((length != 0) && ((_tcschr(delimiter, *pointer) == NULL))) {
                        pointer++;
                        length--;
                    }

                    index = m_Index.Length() - length;
                }
                else {
                    uint32 found = static_cast<uint32>(m_Buffer.find_first_of(delimiter, m_Index.Begin() + offset));

                    if (found <= m_Index.End()) {
                        index = found - m_Index.Begin();
                    }
                }
            }

            return (index);
        }

        uint32 find_last_of(const uint32 offset, const TCHAR delimiter[]) const
        {
            // If we do not find it, we end up at the end.
            uint32 index = NUMBER_MAX_UNSIGNED(uint32);

            if (offset < m_Index.Length()) {
                if (m_Start != NULL) {
                    const TCHAR* pointer = &(Data()[m_Index.Length()]);
                    uint32 count = m_Index.Length() - offset;

                    while ((count != 0) && ((_tcschr(delimiter, *(--pointer)) == NULL))) {
                        count--;
                    }
                    if (count != 0) {
                        index = count - 1;
                    }
                }
                else {
                    uint32 found = static_cast<uint32>(m_Buffer.find_last_of(delimiter, m_Index.End()));

                    if ((found != m_Index.End()) && (found >= (m_Index.Begin() + offset))) {
                        index = found - m_Index.Begin();
                    }
                }
            }

            return (index);
        }

        uint32 find_first_not_of(const uint32 offset, const TCHAR delimiter[]) const
        {
            // If we do not find it, we end up at the end.
            uint32 index = m_Index.Length();

            if (offset < m_Index.Length()) {
                if (m_Start != NULL) {
                    const TCHAR* pointer = &(Data()[offset]);
                    uint32 length = m_Index.Length() - offset;

                    while ((length != 0) && ((_tcschr(delimiter, *pointer) != NULL))) {
                        pointer++;
                        length--;
                    }

                    index = m_Index.Length() - length;
                }
                else {
                    uint32 found = static_cast<uint32>(m_Buffer.find_first_not_of(delimiter, m_Index.Begin() + offset));

                    if (found <= m_Index.End()) {
                        index = found - m_Index.Begin();
                    }
                }
            }

            return (index);
        }

        uint32 find_last_not_of(const uint32 offset, const TCHAR delimiter[]) const
        {
            // If we do not find it, we end up at the end.
            uint32 index = NUMBER_MAX_UNSIGNED(uint32);

            if (offset < m_Index.Length()) {
                if (m_Start != NULL) {
                    const TCHAR* pointer = &(Data()[m_Index.Length()]);
                    uint32 count = m_Index.Length() - offset;

                    while ((count != 0) && ((_tcschr(delimiter, *(--pointer)) != NULL))) {
                        count--;
                    }
                    if (count != 0) {
                        index = count - 1;
                    }
                }
                else {
                    uint32 found = static_cast<uint32>(m_Buffer.find_last_not_of(delimiter, m_Index.End()));

                    if ((found >= (m_Index.Begin() + offset)) && (found != m_Index.End())) {
                        index = found - m_Index.Begin();
                    }
                }
            }

            return (index);
        }

    private:
        Index m_Index;
        const TCHAR* m_Start;
        string m_Buffer;
    };

    class EXTERNAL TextSegmentIterator {
    public:
        TextSegmentIterator()
            : _delimiter(0)
            , _delimiters()
            , _index(~0)
            , _current()
            , _source()
            , _suppressEmpty(false)
        {
        }
        TextSegmentIterator(const TextFragment& text, const bool suppressEmpty, const TCHAR splitter)
            : _delimiter(splitter)
            , _delimiters()
            , _index(~0)
            , _current()
            , _source(text)
            , _suppressEmpty(suppressEmpty)
        {
        }
        TextSegmentIterator(const TextFragment& text, const bool suppressEmpty, const TCHAR splitters[])
            : _delimiter(0)
            , _delimiters(splitters)
            , _index(~0)
            , _current()
            , _source(text)
            , _suppressEmpty(suppressEmpty)
        {
        }
        TextSegmentIterator(const TextSegmentIterator& copy, const bool fixatFromCurrentPosition = false)
            : _delimiter(copy._delimiter)
            , _delimiters(copy._delimiters)
            , _index(fixatFromCurrentPosition ? (copy._index < copy._source.Length() ? 0 : 1) : copy._index)
            , _current(copy._current)
            , _source(fixatFromCurrentPosition ? (copy._index < copy._source.Length() ? TextFragment(copy._source, copy._index, copy._source.Length() - copy._index) : TextFragment()) : copy._source)
            , _suppressEmpty(copy._suppressEmpty)
        {
        }
        ~TextSegmentIterator()
        {
        }

        TextSegmentIterator& operator=(const TextSegmentIterator& RHS)
        {
            _delimiter = RHS._delimiter;
            _delimiters = RHS._delimiters;
            _index = RHS._index;
            _current = RHS._current;
            _source = RHS._source;
            _suppressEmpty = RHS._suppressEmpty;

            return (*this);
        }

    public:
        inline bool IsValid() const
        {
            return (_index <= _source.Length());
        }
        inline void Reset()
        {
            _index = ~0;
        }
        bool Next()
        {
            bool valid = false;
            uint32 start = (_index == static_cast<uint32>(~0) ? 0 : _index + 1);

            while ((start < _source.Length()) && (valid == false)) {
                _index = (_delimiter == 0 ? _source.ForwardFind(_delimiters.c_str(), start) : _source.ForwardFind(_delimiter, start));

                if ((_suppressEmpty == false) || (start <= _index)) {
                    valid = true;

                    _current = TextFragment(_source, start, _index - start);
                }
                else {
                    start = _index + 1;
                }
            }

            return (valid);
        }
        inline TextFragment Current()
        {
            ASSERT(IsValid());

            return (_current);
        }
        inline const TextFragment& Current() const
        {
            ASSERT(IsValid());

            return (_current);
        }
        inline const TextFragment Remainder() const
        {
            ASSERT(IsValid());

            uint32 currentStart = _index - _current.Length();

            return (TextFragment(_source, currentStart, (_source.Length() - currentStart)));
        }

    private:
        TCHAR _delimiter;
        string _delimiters;
        uint32 _index;
        TextFragment _current;
        TextFragment _source;
        bool _suppressEmpty;
    };
}
} // namespace Core

#endif // __TEXTFRAGMENT_H
