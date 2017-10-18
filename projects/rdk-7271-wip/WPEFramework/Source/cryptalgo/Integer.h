#ifndef __INTEGER_H
#define __INTEGER_H

#include "Module.h"

#include "MultiPrecisionInteger.h"
#include "Random.h"

// In case of dynamic allocated storage, start with 1024 bits for an integer...
#define INTEGER_CHUNKSIZE 1024

namespace WPEFramework {

namespace Crypto {

    class EXTERNAL IntegerData {
    private:
        struct conversionInfo {
            unsigned char Bits;
            unsigned char Bytes;
            unsigned char (*ToByte)(const char element);
            char (*FromByte)(const unsigned char element);
        };

        inline conversionInfo* ConversionInfo(const unsigned short base) const
        {
            switch (base) {
            case 2: {
                static conversionInfo result = { 1, 8, Core::FromDigits, Core::ToDigits };
                return (&result);
            }
            case 4: {
                static conversionInfo result = { 2, 4, Core::FromDigits, Core::ToDigits };
                return (&result);
            }
            case 8: {
                static conversionInfo result = { 3, 3, Core::FromDigits, Core::ToDigits };
                return (&result);
            }
            case 16: {
                static conversionInfo result = { 4, 2, Core::FromHexDigits, Core::ToHexDigits };
                return (&result);
            }
            case 32: {
                static conversionInfo result = { 5, 2, Core::FromHexDigits, Core::ToHexDigits };
                return (&result);
            }
            case 64: {
                static conversionInfo result = { 6, 2, Core::FromBase64, Core::ToBase64 };
                return (&result);
            }
            case 128: {
                static conversionInfo result = { 7, 2, Core::FromDirect, Core::ToDirect };
                return (&result);
            }
            case 256: {
                static conversionInfo result = { 8, 1, Core::FromDirect, Core::ToDirect };
                return (&result);
            }
            default:
                return NULL;
            }
        }

    public:
        // This can all be done compile time. No runtime penalty!!!
        static const unsigned short BytesPerLimb = (sizeof(LimbType));
        static const unsigned short BitsPerLimb = (sizeof(LimbType)) * 8;
        static const LimbType HighestBit = static_cast<LimbType>(1) << (((sizeof(LimbType)) * 8) - 1);

    protected:
        inline explicit IntegerData(const IntegerData& copy VARIABLE_IS_NOT_USED)
            : _buffer(NULL)
        {
        }
        inline LimbType* Pointer()
        {
            return (_buffer);
        }

    public:
        inline IntegerData()
            : _buffer(NULL)
        {
        }
        inline void SetBuffer(LimbType* buffer, unsigned int length, const bool heap)
        {
            _buffer = &buffer[0];
            _bitLength = length;
            _limbLength = BitsToLimbs(length);
            _flags = (heap ? 0x80 : 0x00);
        }
        inline IntegerData(LimbType number[], const unsigned int length)
            : _buffer(&number[0])
            , _bitLength(length)
            , _limbLength(BitsToLimbs(length))
        {
        }
        inline ~IntegerData()
        {
        }

        inline IntegerData& operator=(const IntegerData& RHS)
        {
            if (this != &RHS) {
                Copy(RHS);
            }
            return (*this);
        }
        template <typename SCALAR>
        IntegerData& operator=(const SCALAR RHS)
        {
            CopyValue(RHS);
            return (*this);
        }

    public:
        // -----------------------------------------------------------------------
        // Read-Only Metadata methods
        // -----------------------------------------------------------------------
        inline static unsigned int BitsToLimbs(const unsigned int bits)
        {
            return ((bits + BitsPerLimb - 1) / BitsPerLimb);
        }
        inline bool IsHeap() const
        {
            return ((_flags & 0x80) != 0);
        }
        inline bool IsZero() const
        {
            unsigned int result = 0;

            while ((result < _limbLength) && (_buffer[result] == 0)) {
                result++;
            }

            return (result == _limbLength);
        }
        inline bool GetBit(const unsigned int bit) const
        {
            ASSERT(_buffer != NULL);
            ASSERT(bit < Bits());

            return (_buffer[bit / BitsPerLimb] & (static_cast<LimbType>(1) << (bit % BitsPerLimb))) != 0;
        }
        inline bool IsNegative() const
        {
            return ((_flags & 0x01) != 0);
        }
        inline bool IsEven() const
        {
            return ((_buffer[0] & 0x01) == 0);
        }
        inline bool IsOdd() const
        {
            return (!IsEven());
        }
        inline bool HasOverflow() const
        {
            return ((_flags & 0x02) != 0);
        }
        inline unsigned int Bits() const
        {
            return (_bitLength);
        }
        inline unsigned int Limbs() const
        {
            return (_limbLength);
        }
        inline unsigned int LeastSignificantBit() const
        {
            ASSERT(_buffer != NULL);

            unsigned int result = 0;

            // Find the first identifying digit...
            while ((_buffer[result] == 0) && (result != _limbLength)) {
                result++;
            }

            if (result == _limbLength) {
                return (static_cast<unsigned int>(~0));
            }
            else {
                // Some bits are used, determine the bits in the byte now..
                // Bit first determine how many bits are still left and thus used..
                LimbType bitSource = _buffer[result];
                result *= BitsPerLimb;

                // Now check the number of bits used in the current byte..
                while ((bitSource & 0x01) == 0) {
                    result++;
                    bitSource >>= 1;
                }
            }

            return (result);
        }
        inline unsigned int MostSignificantBit() const
        {
            ASSERT(_buffer != NULL);

            unsigned int result = (_limbLength - 1);

            // Find the first identifying digit...
            while ((_buffer[result] == 0) && (result > 0)) {
                result--;
            }

            if (_buffer[result] == 0) {
                // Ther is no MostSignificantBit, the value is 0!!!
                return (static_cast<unsigned int>(~0));
            }
            else {
                // Some bits are used, determine the bits in the byte now..
                // Bit first determine how many bits are still left and thus used..
                LimbType bitSource = (_buffer[result] >> 1);
                result *= BitsPerLimb;

                // Now check the number of bits used in the current byte..
                while (bitSource != 0) {
                    result++;
                    bitSource >>= 1;
                }
            }

            return (result);
        }
        inline unsigned int UsedLimbs() const
        {
            ASSERT(_buffer != NULL);

            unsigned int result = _limbLength - 1;

            // Find the first identifying digit...
            while ((_buffer[result] == 0) && (result > 0)) {
                result--;
            }

            return (_buffer[result] == 0 ? 0 : (result + 1));
        }
        inline signed int Compare(const IntegerData& RHS) const
        {
            ASSERT(_buffer != NULL);
            ASSERT(RHS._buffer != NULL);

            if (IsNegative() ^ RHS.IsNegative()) {
                return (IsNegative() ? -1 : +1);
            }
            else if (_bitLength > RHS._bitLength) {
                unsigned int MSB = MostSignificantBit();

                if ((MSB != static_cast<unsigned int>(~0)) && (MSB > RHS._bitLength)) {
                    return (IsNegative() ? -1 : +1);
                }
            }
            else if (_bitLength < RHS._bitLength) {
                unsigned int MSB = RHS.MostSignificantBit();

                if ((MSB != static_cast<unsigned int>(~0)) && (MSB > _bitLength)) {
                    return (IsNegative() ? +1 : -1);
                }
            }

            unsigned int index = (_limbLength < RHS._limbLength ? _limbLength : RHS._limbLength) - 1;

            // Compare the parts in common...
            while ((index != 0) && (_buffer[index] == RHS[index])) {
                index--;
            }

            return (_buffer[index] == RHS[index] ? 0 : ((_buffer[index] > RHS[index]) ^ (IsNegative())) ? +1 : -1);
        }
        inline signed int AbsoluteCompare(const IntegerData& RHS) const
        {
            ASSERT(_buffer != NULL);
            ASSERT(RHS._buffer != NULL);

            if (_bitLength > RHS._bitLength) {
                unsigned int MSB = MostSignificantBit();

                if ((MSB != static_cast<unsigned int>(~0)) && (MSB > RHS._bitLength)) {
                    return (+1);
                }
            }
            else if (_bitLength < RHS._bitLength) {
                unsigned int MSB = RHS.MostSignificantBit();

                if ((MSB != static_cast<unsigned int>(~0)) && (MSB > _bitLength)) {
                    return (-1);
                }
            }

            unsigned int index = (_limbLength < RHS._limbLength ? _limbLength : RHS._limbLength) - 1;

            // Compare the parts in common...
            while ((index != 0) && (_buffer[index] == RHS[index])) {
                index--;
            }

            return (_buffer[index] == RHS[index] ? 0 : ((_buffer[index] > RHS[index]) ? +1 : -1));
        }
        // -----------------------------------------------------------------------
        // Mathematical calculations required to deliver MetaData on the Integer
        // -----------------------------------------------------------------------
        inline const LimbType& operator[](const unsigned int index) const
        {
            ASSERT(index < _limbLength);

            return (_buffer[index]);
        }

    public:
        // -----------------------------------------------------------------------
        // Method Data modifiers
        // -----------------------------------------------------------------------
        inline void Clear()
        {
            ASSERT(_buffer != NULL);
            memset(_buffer, 0, (_limbLength * BytesPerLimb));
            _flags = (_flags & 0xF0);
        }
        inline void SetBit(const unsigned int bit)
        {
            ASSERT(_buffer != NULL);
            ASSERT(bit < Bits());

            _buffer[bit / BitsPerLimb] |= (static_cast<LimbType>(1) << (bit % BitsPerLimb));
        }
        inline void ClearBit(const unsigned int bit)
        {
            ASSERT(_buffer != NULL);
            ASSERT(bit < Bits());

            _buffer[bit / BitsPerLimb] &= (static_cast<LimbType>(1) << (bit % BitsPerLimb));
        }
        inline unsigned char Data(const unsigned int index) const
        {
            return static_cast<unsigned char>((operator[](index / BytesPerLimb) >> ((index % BytesPerLimb) * 8)) & 0xFF);
        }
        inline LimbType& operator[](const unsigned int index)
        {
            ASSERT(_buffer != NULL);
            ASSERT(index < _limbLength);

            return (_buffer[index]);
        }
        inline void Negative(const bool value)
        {
            if (value == true) {
                _flags |= 0x01;
            }
            else {
                _flags &= (~0x01);
            }
        }
        inline void Add(const IntegerData& RHS, IntegerData& result) const
        {
            ASSERT(_buffer != NULL);
            ASSERT(RHS._buffer != NULL);

            if (IsNegative() == RHS.IsNegative()) {
                DoAdd(RHS, result);
                result.Negative(IsNegative());
            }
            else if (AbsoluteCompare(RHS) >= 0) {
                DoSub(RHS, result);
                result.Negative(IsNegative());
            }
            else {
                RHS.DoSub(*this, result);
                result.Negative(!IsNegative());
            }
        }
        inline void Sub(const IntegerData& RHS, IntegerData& result) const
        {
            ASSERT(_buffer != NULL);
            ASSERT(RHS._buffer != NULL);

            if (IsNegative() ^ RHS.IsNegative()) {
                DoAdd(RHS, result);
                result.Negative(IsNegative());
            }
            else if (AbsoluteCompare(RHS) >= 0) {
                DoSub(RHS, result);
                result.Negative(IsNegative());
            }
            else {
                RHS.DoSub(*this, result);
                result.Negative(!IsNegative());
            }
        }
        inline void Mul(const IntegerData& RHS, IntegerData& result) const
        {
            unsigned int limbsLHS = UsedLimbs();
            unsigned int limbsRHS = RHS.UsedLimbs();
            unsigned int limbsNeeded = 2 * (limbsLHS > limbsRHS ? limbsLHS : limbsRHS);

            LimbType* stackBuffer = static_cast<LimbType*>(ALLOCA(limbsNeeded * BytesPerLimb));
            IntegerData X;
            X.SetBuffer(stackBuffer, limbsNeeded * BitsPerLimb, false);

            // Off course, we start at 0 :-)
            X.Clear();

            for (; limbsRHS > 0; --limbsRHS) {
                DoMul(limbsLHS, _buffer, &(X._buffer[limbsRHS - 1]), RHS._buffer[limbsRHS - 1]);
            }

            X.Negative(RHS.IsNegative() ^ IsNegative());
            result.Copy(X);
        }
        /*
         * Division by mpi: A = Q * B + R  (HAC 14.20)
         */
        inline void Div(const IntegerData& B, IntegerData& Q, IntegerData& R) const
        {
            unsigned int mostSignificantBit = B.MostSignificantBit(); /* t */

            if ((mostSignificantBit == static_cast<unsigned int>(~0)) || (Compare(B) < 0)) {
                Q.Clear();
                R.Copy(*this);
            }
            else {
                unsigned int limbsNeeded = UsedLimbs() + 2;
                IntegerData X, Y, Z, T1, T2;
                LimbType* stackBuffer = static_cast<LimbType*>(ALLOCA(5 * limbsNeeded * BytesPerLimb));
                X.SetBuffer(&stackBuffer[0], limbsNeeded * BitsPerLimb, false);
                Y.SetBuffer(&stackBuffer[limbsNeeded], limbsNeeded * BitsPerLimb, false);
                Z.SetBuffer(&stackBuffer[2 * limbsNeeded], limbsNeeded * BitsPerLimb, false);
                T1.SetBuffer(&stackBuffer[3 * limbsNeeded], limbsNeeded * BitsPerLimb, false);
                T2.SetBuffer(&stackBuffer[4 * limbsNeeded], limbsNeeded * BitsPerLimb, false);

                X.Copy(*this);
                X.Negative(false);
                Y.Copy(B);
                Y.Negative(false);
                Z.Clear();

                unsigned int adjust = (BitsPerLimb - 1) - ((mostSignificantBit + 1) % BitsPerLimb);

                X.ShiftLeft(adjust);
                Y.ShiftLeft(adjust);

                unsigned int aUsedLimbs = (X.UsedLimbs() - 1);
                unsigned int bUsedLimbs = (Y.UsedLimbs() - 1);

                Y.ShiftLeft(BitsPerLimb * (aUsedLimbs - bUsedLimbs));

                while (X.Compare(Y) >= 0) {
                    Z._buffer[aUsedLimbs - bUsedLimbs]++;
                    X.Sub(Y, X);
                }
                Y.ShiftRight(BitsPerLimb * (aUsedLimbs - bUsedLimbs));

                for (unsigned int index = aUsedLimbs; index > bUsedLimbs; index--) {
                    if (X._buffer[index] >= Y._buffer[bUsedLimbs]) {
                        Z._buffer[index - bUsedLimbs - 1] = ~0;
                    }
                    else {
                        /*
                         * __udiv_qrnnd_c, from gmp/longlong.h
                         */
                        LimbType q0, q1, r0, r1;
                        LimbType d0, d1, d, m;

                        d = Y._buffer[bUsedLimbs];
                        d0 = (d << (BitsPerLimb >> 1)) >> (BitsPerLimb >> 1);
                        d1 = (d >> (BitsPerLimb >> 1));

                        q1 = X._buffer[index] / d1;
                        r1 = X._buffer[index] - d1 * q1;
                        r1 <<= (BitsPerLimb >> 1);
                        r1 |= (X._buffer[index - 1] >> (BitsPerLimb >> 1));

                        m = q1 * d0;
                        if (r1 < m) {
                            q1--, r1 += d;
                            while (r1 >= d && r1 < m) {
                                q1--, r1 += d;
                            }
                        }
                        r1 -= m;

                        q0 = r1 / d1;
                        r0 = r1 - d1 * q0;
                        r0 <<= (BitsPerLimb >> 1);
                        r0 |= (X._buffer[index - 1] << (BitsPerLimb >> 1)) >> (BitsPerLimb >> 1);

                        m = q0 * d0;
                        if (r0 < m) {
                            q0--, r0 += d;
                            while (r0 >= d && r0 < m) {
                                q0--, r0 += d;
                            }
                        }
                        r0 -= m;

                        Z._buffer[index - bUsedLimbs - 1] = (q1 << (BitsPerLimb >> 1)) | q0;
                    }

                    Z._buffer[index - bUsedLimbs - 1]++;

                    do {
                        Z._buffer[index - bUsedLimbs - 1]--;

                        T1.Clear();
                        T1._buffer[0] = (bUsedLimbs < 1) ? 0 : Y._buffer[bUsedLimbs - 1];
                        T1._buffer[1] = Y._buffer[bUsedLimbs];

                        IntegerData temp;
                        temp.SetBuffer(&Z._buffer[index - bUsedLimbs - 1], BitsPerLimb, false);
                        T1.Mul(temp, T1);

                        T2.Clear();
                        T2._buffer[0] = (index < 2) ? 0 : X._buffer[index - 2];
                        T2._buffer[1] = (index < 1) ? 0 : X._buffer[index - 1];
                        T2._buffer[2] = X._buffer[index];

                    } while (T1.Compare(T2) > 0);

                    IntegerData temp;
                    temp.SetBuffer(&Z._buffer[index - bUsedLimbs - 1], BitsPerLimb, false);
                    Y.Mul(temp, T1);
                    T1.ShiftLeft(BitsPerLimb * (index - bUsedLimbs - 1));
                    X.Sub(T1, X);

                    if (X.IsNegative() == true) {
                        T1.Copy(Y);
                        T1.ShiftLeft(BitsPerLimb * (index - bUsedLimbs - 1));
                        X.Add(T1, X);
                        Z._buffer[index - bUsedLimbs - 1]--;
                    }
                }

                Q.Copy(Z);
                Q.Negative(IsNegative() ^ B.IsNegative());

                X.ShiftRight(adjust);
                R.Copy(X);
                R.Negative(IsNegative() && !R.IsZero());
            }
        }
        /* Montgomery multiplication: A = A * B * R^-1 mod N  (HAC 14.36) */
        inline void Montgomery(const IntegerData& B, const IntegerData& N, IntegerData& result) const
        {
            MontgomeryMul(B, N, MontgomeryInit(), result);
        }
        inline bool ShiftBitLeft(const bool bit)
        {
            ASSERT(_buffer != NULL);

            LimbType carryBit(bit == true ? 0x01 : 0x00);
            LimbType value;

            for (unsigned int index = 0; index < _limbLength; ++index) {
                value = (_buffer[index] << 1) | carryBit;
                carryBit = ((_buffer[index] & HighestBit) != 0 ? 0x01 : 0x00);
                _buffer[index] = value;
            }

            return (carryBit != 0);
        }
        inline bool ShiftBitRight(const bool bit)
        {
            ASSERT(_buffer != NULL);

            LimbType carryBit(bit == true ? 1 << (Bits() % BitsPerLimb) : 0x00);
            LimbType value;

            for (unsigned int index = _limbLength; index != 0; --index) {
                value = (_buffer[index - 1] >> 1) | carryBit;
                carryBit = (_buffer[index - 1] & HighestBit);
                _buffer[index - 1] = value;
            }

            return (carryBit != 0);
        }
        inline void ShiftLeft(const unsigned int bitsToShift)
        {
            ASSERT(_buffer != NULL);

            if (bitsToShift != 0) {
                unsigned int v0 = bitsToShift / BitsPerLimb;
                unsigned int t1 = bitsToShift & (BitsPerLimb - 1);
                unsigned int s0 = _limbLength;

                while ((_buffer[s0 - 1] == 0) && (s0 < v0)) {
                    --s0;
                }

                /*
                 * shift by count / limb_size
                 */
                if (v0 > 0) {
                    Overflow(HasOverflow() || ((s0 + v0) <= _limbLength));

                    for (unsigned int index = s0; index > v0; index--) {
                        _buffer[index - 1] = _buffer[index - v0 - 1];
                    }

                    for (unsigned int index = v0; index > 0; index--) {
                        _buffer[index - 1] = 0;
                    }
                }

                /*
                 * shift by count % limb_size
                 */
                if (t1 > 0) {
                    LimbType r0 = 0;
                    s0 = (s0 + v0 + 1 > _limbLength ? _limbLength : s0 + v0 + 1);
                    Overflow(HasOverflow() || ((s0 == _limbLength) && ((_buffer[_limbLength - 1] >> ((BitsPerLimb - 1) - t1)) != 0)));

                    for (unsigned int index = v0; index < s0; index++) {
                        LimbType r1 = _buffer[index] >> (BitsPerLimb - t1);
                        _buffer[index] <<= t1;
                        _buffer[index] |= r0;
                        r0 = r1;
                    }
                }
            }
        }
        inline void ShiftRight(const unsigned int bitsToShift)
        {
            ASSERT(_buffer != NULL);

            if (bitsToShift != 0) {
                unsigned int v0 = bitsToShift / BitsPerLimb;
                unsigned int v1 = bitsToShift & (BitsPerLimb - 1);
                unsigned int s0 = _limbLength;

                while ((_buffer[s0 - 1] == 0) && (s0 < v0)) {
                    --s0;
                }

                /*
                 * shift by count / limb_size
                 */
                if (v0 > 0) {
                    for (unsigned int index = 0; index < (s0 - v0); index++) {
                        _buffer[index] = _buffer[index + v0];
                    }

                    for (unsigned int index = (s0 - v0); index < s0; index++) {
                        _buffer[index] = 0;
                    }
                }

                /*
                 * shift by count % limb_size
                 */
                if (v1 > 0) {
                    LimbType r0 = 0;

                    for (unsigned int index = s0; index > 0; index--) {
                        LimbType r1 = _buffer[index - 1] << (BitsPerLimb - v1);
                        _buffer[index - 1] >>= v1;
                        _buffer[index - 1] |= r0;
                        r0 = r1;
                    }
                }
            }
        }
        inline void And(const IntegerData& RHS, IntegerData& result) const
        {
            ASSERT(_buffer != NULL);
            ASSERT(RHS._buffer != NULL);

            unsigned int length(_limbLength > RHS._limbLength ? RHS._limbLength : _limbLength);

            for (unsigned int index = 0; index < length; ++index) {
                result[index] = (_buffer[index] & RHS[index]);
            }
        }
        inline void Or(const IntegerData& RHS, IntegerData& result) const
        {
            ASSERT(_buffer != NULL);
            ASSERT(RHS._buffer != NULL);

            unsigned int length(_limbLength > RHS._limbLength ? RHS._limbLength : _limbLength);

            for (unsigned int index = 0; index < length; ++index) {
                result[index] = (_buffer[index] | RHS[index]);
            }

            if (RHS._bitLength > _bitLength) {
                unsigned int MSB = RHS.MostSignificantBit();

                if ((MSB != static_cast<unsigned int>(~0)) && (MSB > _bitLength)) {
                    result.Overflow(true);
                }
            }
        }
        inline void Xor(const IntegerData& RHS, IntegerData& result) const
        {
            ASSERT(_buffer != NULL);
            ASSERT(RHS._buffer != NULL);

            unsigned int length(_limbLength > RHS._limbLength ? RHS._limbLength : _limbLength);

            for (unsigned int index = 0; index < length; ++index) {
                result[index] = (_buffer[index] ^ RHS[index]);
            }

            if (RHS._bitLength > _bitLength) {
                unsigned int MSB = RHS.MostSignificantBit();

                if ((MSB != static_cast<unsigned int>(~0)) && (MSB > _bitLength)) {
                    result.Overflow(true);
                }
            }
        }
        inline void Neg(IntegerData& result) const
        {
            ASSERT(_buffer != NULL);

            for (unsigned int index = 0; index < _limbLength; ++index) {
                result[index] = ~(_buffer[index]);
            }

            unsigned int skipBits = ((_limbLength * 8) - _bitLength);
            if (skipBits != 0) {
                // Oops last part neds to be corrected.
                result[_limbLength - 1] = result[_limbLength - 1] & (0xFF >> skipBits);
            }
        }
        void Copy(const IntegerData& copy)
        {
            ASSERT(_buffer != NULL);
            ASSERT(copy._buffer != NULL);

            // Copy Sign and overflow info..
            _flags = (_flags & 0xF0) | (copy._flags & 0x0F);

            // Copy the bulk info
            memcpy(_buffer, &(copy[0]), (_limbLength < copy._limbLength ? _limbLength : copy._limbLength) * BytesPerLimb);

            if (_bitLength < copy._bitLength) {
                // Check for overflow...
                if (copy.HasOverflow() == false) {
                    unsigned int MSB = copy.MostSignificantBit();

                    if ((MSB != static_cast<unsigned int>(~0)) && (MSB > _bitLength)) {
                        // Seems we did not copy significant bits with a value, so we are in overflow.
                        Overflow(true);
                    }
                }
            }
            else if (_limbLength > copy._limbLength) {
                // Clear the part we did not have yet..
                memset(&(_buffer[copy._limbLength]), 0, (_limbLength - copy._limbLength) * BytesPerLimb);
            }
        }
        void Copy(const unsigned char number[], const unsigned int length)
        {
            ASSERT(_buffer != NULL);

            unsigned int source_index = length;
            unsigned int destination_index = 0;

            // CLear the flags..
            _flags &= 0xF0;

            if (source_index > 0) {
                source_index--;
                // Copy the byte array to the propwer location
                while ((destination_index != _limbLength) && (source_index != 0)) {
                    LimbType value = 0;
                    unsigned int delta = (source_index > BytesPerLimb ? BytesPerLimb : source_index);

                    for (unsigned int index = source_index - delta; index < source_index; ++index) {
                        value = (value << 8) | number[index];
                    }
                    source_index -= delta;
                    _buffer[destination_index++] = value;
                }
            }

            if (destination_index < _limbLength) {
                memset(&_buffer[destination_index], 0, (_limbLength - destination_index) * BytesPerLimb);
            }
            else if (source_index != 0) {
                unsigned int MSB = MostSignificantBit();

                if ((MSB != static_cast<unsigned int>(~0)) && (MSB > _bitLength)) {
                    Overflow(true);
                }
                else {
                    while ((source_index != 0) && (number[--source_index] == 0)) {
                        // Intentionally left empty !!!!;
                    }

                    if (source_index != 0) {
                        Overflow(true);
                    }
                }
            }
        }
        void ToString(string& text, const NumberBase type = BASE_HEXADECIMAL) const
        {
            unsigned int MSB = MostSignificantBit();
            conversionInfo* info = ConversionInfo(type);
            text.clear();

            if ((MSB != static_cast<unsigned int>(~0)) && (info != NULL)) {
                unsigned int toHandleBits = ((MSB + info->Bits) / info->Bits) * info->Bits;
                unsigned char mask = (0xFF >> (8 - info->Bits));

                if (IsNegative() == true) {
                    text = '-';
                }

                // Copy the byte array to the propwer location
                while (toHandleBits > 0) {
                    unsigned char value = 0;

                    if ((toHandleBits / BitsPerLimb) == ((toHandleBits - info->Bits) / BitsPerLimb)) {
                        // It is split over two limbs..
                        value = (((_buffer[toHandleBits / BitsPerLimb]) << (info->Bits - (toHandleBits % BitsPerLimb))) & mask);
                    }

                    toHandleBits -= info->Bits;

                    text += info->FromByte(value | (((_buffer[toHandleBits / BitsPerLimb]) >> (toHandleBits % BitsPerLimb)) & mask));
                }
            }
            else {
                // Oops its a terrible format... this means "werk aan de winkel" do this later..
            }
        }
        void CopyValue(const TCHAR text[], const unsigned int length = 0, const NumberBase type = BASE_HEXADECIMAL)
        {
            ASSERT(_buffer != NULL);

            unsigned int source_index = (length == 0 ? strlen(text) : length);
            unsigned int destination_index = 0;

            // CLear the flags..
            _flags &= 0xF0;

            if (source_index > 0) {
                source_index--;
                conversionInfo* info = ConversionInfo(type);

                if (info != NULL) {
                    unsigned int sectionSize = info->Bytes * BytesPerLimb;
                    unsigned char mask = (0xFF >> (8 - info->Bits));
                    unsigned char rollback = ((info->Bits == 1) || (info->Bits == 2) || (info->Bits == 4) || (info->Bits == 8) ? 0 : 1);

                    // Copy the byte array to the propwer location
                    while ((destination_index != _limbLength) && (source_index != 0)) {
                        LimbType value = 0;
                        unsigned int delta = (source_index >= sectionSize ? sectionSize : source_index);

                        for (unsigned int index = source_index - delta; index < source_index; ++index) {
                            value = (value << info->Bits) | (info->ToByte(text[index]) & mask);
                        }

                        // The last needs to be "inserted" out side of the loop as it might overflow the element..
                        source_index -= (delta - rollback);
                        _buffer[destination_index++] = value;
                    }
                }
                else {
                    // Oops its a terrible format... this means "werk aan de winkel" do this later..
                }
            }

            if (destination_index < _limbLength) {
                memset(&_buffer[destination_index], 0, (_limbLength - destination_index) * BytesPerLimb);
            }
            else if (source_index != 0) {
                unsigned int MSB = MostSignificantBit();

                if ((MSB != static_cast<unsigned int>(~0)) && (MSB > _bitLength)) {
                    Overflow(true);
                }
                else {
                    while ((source_index != 0) && (text[--source_index] == '0')) {
                        // Intentionally left empty !!!!;
                    }

                    if (source_index != 0) {
                        Overflow(true);
                    }
                }
            }
        }
        inline void Random()
        {
            Random(Bits());
        }
        IntegerData& Random(const unsigned int bits)
        {
            ASSERT(bits <= Bits());

            unsigned int limbs = bits / BitsPerLimb;
            unsigned int index = 0;

            for (; index < limbs; index++) {
                Crypto::Random(_buffer[index]);
            }

            if ((bits % BitsPerLimb) != 0) {
                LimbType newLimb;
                Crypto::Random(newLimb);

                _buffer[index] = (newLimb >> (BitsPerLimb - (bits % BitsPerLimb)));
            }

            return (*this);
        }
        template <typename SCALAR>
        inline void CopyValue(const TemplateIntToType<true>& /* For compile time diffrentiation */, const SCALAR value)
        {
            SCALAR workingValue;
            ASSERT(_buffer != NULL);

            _flags &= 0xF0;

            if (value < 0) {
                workingValue = static_cast<SCALAR>(-value);
                _flags |= 0x01;
            }
            else {
                workingValue = value;
            }

            unsigned int index;

            if (sizeof(SCALAR) <= sizeof(LimbType)) {
                _buffer[0] = (LimbType)(workingValue);
                index = 1;
            }
            else {
                index = 0;
                LimbType mask = static_cast<LimbType>(~0);

                while ((workingValue != 0) && (index < Limbs())) {
                    _buffer[index++] = (workingValue & mask);
                    workingValue = (workingValue >> BitsPerLimb);
                }
                if (workingValue != 0) {
                    Overflow(true);
                }
            }

            if (index < Limbs()) {
                memset(&(_buffer[index]), 0, (Limbs() - index) * BytesPerLimb);
            }
        }

        template <typename SCALAR>
        inline void CopyValue(const TemplateIntToType<false>& /* For compile time diffrentiation */, const SCALAR value)
        {
            SCALAR workingValue;
            ASSERT(_buffer != NULL);

            _flags &= 0xF0;

            workingValue = value;

            unsigned int index;

            if (sizeof(SCALAR) <= sizeof(LimbType)) {
                _buffer[0] = (LimbType)(workingValue);
                index = 1;
            }
            else {
                index = 0;
                LimbType mask = static_cast<LimbType>(~0);

                while ((workingValue != 0) && (index < Limbs())) {
                    _buffer[index++] = (workingValue & mask);
                    workingValue = (workingValue >> BitsPerLimb);
                }
                if (workingValue != 0) {
                    Overflow(true);
                }
            }

            if (index < Limbs()) {
                memset(&(_buffer[index]), 0, (Limbs() - index) * BytesPerLimb);
            }
        }

        /* Fast Montgomery initialization (thanks to Tom St Denis) */
        LimbType MontgomeryInit() const
        {
            LimbType x((*this)[0]);

            x += (((*this)[0] + 2) & 4) << 1;

            for (unsigned int step = BitsPerLimb; step >= 8; step /= 2) {
                x *= (2 - ((*this)[0] * x));
            }

            return (~x + 1);
        }
        /* Montgomery multiplication: A = A * B * R^-1 mod N  (HAC 14.36) */
        void MontgomeryMul(const IntegerData& B, const IntegerData& N, const LimbType mm, IntegerData& result) const
        {
            unsigned int n = N.UsedLimbs();
            unsigned int m = B.UsedLimbs();
            unsigned int length = (n + 1) * 2 * IntegerData::BytesPerLimb;
            LimbType* destination = reinterpret_cast<LimbType*>(ALLOCA(length));

            if (m > n) {
                m = n;
            }

            memset(destination, 0, length);

            for (unsigned int index = 0; index < n; index++) {
                /* T = (T + u0*B + u1*N) / 2 ^ BitsPerLimb */
                LimbType u0 = _buffer[index];
                LimbType u1 = (destination[0] + u0 * B[0]) * mm;

                DoMul(m, B._buffer, destination, u0);
                DoMul(n, N._buffer, destination, u1);

                *destination++ = u0;
                destination[n + 1] = 0;
            }

            ASSERT(result.Limbs() > n);

            // We are almost done, copy the result...
            memcpy(&(result[0]), destination, (n + 1) * BytesPerLimb);

            if (result.Compare(N) >= 0) {
                result.DoSub(N, result);
            }
            //  else
            //      /* prevent timing attacks */
            //      mpi_sub_hlp( n, A->p, T->p );
        }

        // Helper for multiplication
        static void DoMul(unsigned int i, const LimbType* s, LimbType* d, const LimbType b)
        {
            LimbType c = 0, t = 0;

#if defined(MULADDC_HUIT)
            for (; i >= 8; i -= 8) {
                MULADDC_INIT
                MULADDC_HUIT
                MULADDC_STOP
            }

            for (; i > 0; i--) {
                MULADDC_INIT
                MULADDC_CORE
                MULADDC_STOP
            }
#else /* MULADDC_HUIT */
            for (; i >= 16; i -= 16) {
                MULADDC_INIT
                MULADDC_CORE MULADDC_CORE
                    MULADDC_CORE MULADDC_CORE
                        MULADDC_CORE MULADDC_CORE
                            MULADDC_CORE MULADDC_CORE

                                MULADDC_CORE MULADDC_CORE
                                    MULADDC_CORE MULADDC_CORE
                                        MULADDC_CORE MULADDC_CORE
                                            MULADDC_CORE MULADDC_CORE
                                                MULADDC_STOP
            }

            for (; i >= 8; i -= 8) {
                MULADDC_INIT
                MULADDC_CORE MULADDC_CORE
                    MULADDC_CORE MULADDC_CORE

                        MULADDC_CORE MULADDC_CORE
                            MULADDC_CORE MULADDC_CORE
                                MULADDC_STOP
            }

            for (; i > 0; i--) {
                MULADDC_INIT
                MULADDC_CORE
                MULADDC_STOP
            }
#endif /* MULADDC_HUIT */

            t++;

            do {
                *d += c;
                c = (*d < c);
                d++;
            } while (c != 0);
        }

        // -----------------------------------------------------------------------
        // Write Metadata methods
        // -----------------------------------------------------------------------
        inline void Overflow(const bool value)
        {
            if (value == true) {
                _flags |= 0x02;

                // TODO: Clear the upper overflow bits !!!
            }
            else {
                _flags &= (~0x02);
            }
        }
        inline void DoAdd(const IntegerData& RHS, IntegerData& result) const
        {
            LimbType borrow = 0;
            unsigned int index = 0;

            while ((index < RHS._limbLength) && (index < _limbLength)) {
                // Can not optimize this out by checking, after the addition method
                // if the new value is smaller than the one before, because we would
                // like the result value to be this class (See div). It allows for
                // not having to copy over the value after the addition.
                bool overflow = ((_buffer[index] + RHS._buffer[index]) < _buffer[index]);

                // There is data from both sides to add
                result[index] = _buffer[index] + RHS._buffer[index];

                if (overflow == true) {
                    // We need to borrow..
                    result[index] += borrow;

                    borrow = 1;
                }
                else if (result[index] != static_cast<LimbType>(~0)) {
                    result[index] += borrow;

                    borrow = 0;
                }
                else {
                    result[index] += borrow;
                }
                index++;
            }

            while (index < RHS._limbLength) {
                result[index] = RHS._buffer[index] + borrow;
                borrow = (result[index] < borrow ? 1 : 0);
                index++;
            }

            while (index < _limbLength) {
                result[index] = _buffer[index] + borrow;
                borrow = (result[index] < borrow ? 1 : 0);
                index++;
            }

            ASSERT(borrow == 0);
        }
        inline void DoSub(const IntegerData& RHS, IntegerData& result) const
        {
            LimbType borrow = 0;
            unsigned int index = 0;

            while ((index < RHS._limbLength) && (index < _limbLength)) {
                // Can not optimize this out by checking, after the substract method
                // if the mnew value is bigger than the one before, because we would
                // like the result value to be this class (See div). It allows for
                // not having to copy over the value after the substract.
                bool overflow = (_buffer[index] < RHS._buffer[index]);

                // There is data from both sides to sub
                result[index] = _buffer[index] - RHS._buffer[index];

                if (overflow == true) {
                    // We need to borrow..
                    result[index] -= borrow;

                    borrow = 1;
                }
                else if (result[index] != 0) {
                    result[index] -= borrow;

                    borrow = 0;
                }
                else {
                    result[index] -= borrow;
                }

                index++;
            }

            while (index < RHS._limbLength) {
                result[index] = RHS._buffer[index] - borrow;
                borrow = (result[index] < borrow ? 1 : 0);
                index++;
            }

            while (index < _limbLength) {
                result[index] = _buffer[index] + borrow;
                borrow = (result[index] < borrow ? 1 : 0);
                index++;
            }

            ASSERT(borrow == 0);
        }

    private:
        unsigned char _flags;
        LimbType* _buffer;
        unsigned int _bitLength;
        unsigned int _limbLength;
    };

    static const unsigned short SmallPrime[] = {
        3, 5, 7, 11, 13, 17, 19, 23,
        29, 31, 37, 41, 43, 47, 53, 59,
        61, 67, 71, 73, 79, 83, 89, 97,
        101, 103, 107, 109, 113, 127, 131, 137,
        139, 149, 151, 157, 163, 167, 173, 179,
        181, 191, 193, 197, 199, 211, 223, 227,
        229, 233, 239, 241, 251, 257, 263, 269,
        271, 277, 281, 283, 293, 307, 311, 313,
        317, 331, 337, 347, 349, 353, 359, 367,
        373, 379, 383, 389, 397, 401, 409, 419,
        421, 431, 433, 439, 443, 449, 457, 461,
        463, 467, 479, 487, 491, 499, 503, 509,
        521, 523, 541, 547, 557, 563, 569, 571,
        577, 587, 593, 599, 601, 607, 613, 617,
        619, 631, 641, 643, 647, 653, 659, 661,
        673, 677, 683, 691, 701, 709, 719, 727,
        733, 739, 743, 751, 757, 761, 769, 773,
        787, 797, 809, 811, 821, 823, 827, 829,
        839, 853, 857, 859, 863, 877, 881, 883,
        887, 907, 911, 919, 929, 937, 941, 947,
        953, 967, 971, 977, 983, 991, 997
    };

    template <const unsigned int BITS>
    class Integer : public IntegerData {
    private:
        template <const unsigned int ARRAYSIZE>
        class IntegerDataArray {
        private:
            IntegerDataArray();
            IntegerDataArray(const IntegerDataArray<ARRAYSIZE>&);
            IntegerDataArray<ARRAYSIZE>& operator=(const IntegerDataArray<ARRAYSIZE>&);

        public:
            IntegerDataArray(unsigned int bits, LimbType* sourceArray)
            {
                unsigned int limbLength = ((bits + 7) / ((sizeof(LimbType)) * 8));
                for (unsigned int entry = 0; entry < ARRAYSIZE; ++entry) {
                    _array[entry].SetBuffer(&(sourceArray[entry * limbLength]), bits, true);
                }
            }
            ~IntegerDataArray()
            {
            }

        public:
            IntegerData& operator[](const unsigned int index)
            {
                ASSERT(index < ARRAYSIZE);

                return (_array[index]);
            }
            const IntegerData& operator[](const unsigned int index) const
            {
                ASSERT(index < ARRAYSIZE);

                return (_array[index]);
            }

        private:
            IntegerData _array[ARRAYSIZE];
        };

    private:
        template <typename OBJECT>
        inline signed int CompareTyped(const TemplateIntToType<true>& /* For compile time diffrentiation */, const OBJECT& RHS) const
        {
            // Compare the parts in common...
            return (IntegerData::Compare(RHS) != 0);
        }
        template <typename OBJECT>
        inline signed int CompareTyped(const TemplateIntToType<false>& /* For compile time diffrentiation */, const OBJECT& RHS) const
        {
            PlatformInteger comparator(RHS);

            // Compare the parts in common...
            return (IntegerData::Compare(comparator) != 0);
        }
        template <typename OBJECT>
        inline void CopyConstructor(const TemplateIntToType<true>& /* For compile time diffrentiation */, const OBJECT& copy)
        {
            Construct(copy.Bits());
            IntegerData::Copy(copy);
        }
        template <typename OBJECT>
        inline void CopyConstructor(const TemplateIntToType<false>& /* For compile time diffrentiation */, const OBJECT& copy)
        {
            Construct(((sizeof(OBJECT) / IntegerData::BytesPerLimb) * IntegerData::BitsPerLimb));
            IntegerData::CopyValue(TemplateIntToType<Core::TypeTraits::sign<OBJECT>::Signed != 0>(), copy);
        }
        template <typename OBJECT>
        Integer<BITS>& AssignmentOperator(const TemplateIntToType<true>& /* For compile time diffrentiation */, const OBJECT& RHS)
        {
            if (BITS == 0) {
                unsigned int MSB = RHS.MostSignificantBit();

                if ((MSB != static_cast<unsigned int>(~0)) && (MSB > IntegerData::Bits())) {
                    ASSERT(IntegerData::IsHeap() == true);

                    if (RHS.Limbs() > IntegerData::Limbs()) {
                        // Resize...
                        delete[] IntegerData::Pointer();

                        IntegerData::SetBuffer(new LimbType[RHS.Limbs()], RHS.Bits(), true);
                    }
                }
            }

            IntegerData::Copy(RHS);

            return (*this);
        }
        template <typename OBJECT>
        Integer<BITS>& AssignmentOperator(const TemplateIntToType<false>& /* For compile time diffrentiation */, const OBJECT& RHS)
        {
            IntegerData::CopyValue(TemplateIntToType<Core::TypeTraits::sign<OBJECT>::Signed != 0>(), RHS);

            return (*this);
        }

        // number of Miller-Rabin iterations for an error rate  of less than 2^-80
        // for random 'b'-bit input, b >= 100 (taken from table 4.4 in the Handbook
        // of Applied Cryptography [Menezes, van Oorschot, Vanstone; CRC Press 1996];
        // original paper: Damgaard, Landrock, Pomerance: Average case error estimates
        // for the strong probable prime test. -- Math. Comp. 61 (1993) 177-194) */
        unsigned int PrimeCheckSize(unsigned int b) const
        {
            return ((b) >= 1300 ? 2 : (b) >= 850 ? 3 : (b) >= 650 ? 4 : (b) >= 550 ? 5 : (b) >= 450 ? 6 : (b) >= 400 ? 7 : (b) >= 350 ? 8 : (b) >= 300 ? 9 : (b) >= 250 ? 12 : (b) >= 200 ? 15 : (b) >= 150 ? 18 :
                                                                                                                                                                                                            /* b >= 100 */ 27);
        }
        // Depending on your platform, define the size here
        typedef Integer<64> PlatformInteger;

    public:
        // If we need temporaray storage from stack, this is the
        // constructor that will create a temporary storage.
        // Lifetime equal to the stackframe on which it is
        // allocated. Use with care !!!!
        // template<const unsigned int BITSIZE, typename LimbType>
        // friend class Integer;
        Integer(unsigned int length, LimbType number[])
        {
            IntegerData::SetBuffer(&(number[0]), length, false);
            IntegerData::Clear();
        }
        Integer()
            : IntegerData()
        {
            Construct(BITS);
            IntegerData::Clear();
        }
        Integer(const Integer<BITS>& copy)
            : IntegerData()
        {
            CopyConstructor(TemplateIntToType<true>(), copy);
        }
        Integer(const unsigned char number[], const unsigned int length)
            : IntegerData()
        {
            Construct(length * IntegerData::BitsPerLimb);
            IntegerData::Copy(number, length);
        }
        Integer(const TCHAR number[], const unsigned int length, const NumberBase type = BASE_HEXADECIMAL)
            : IntegerData()
        {
            Construct(length * IntegerData::BitsPerLimb);
            IntegerData::CopyValue(number, length, type);
        }
        template <typename OBJECT>
        explicit Integer(const OBJECT& copy)
            : IntegerData()
        {
            CopyConstructor(TemplateIntToType<Core::TypeTraits::same_or_inherits<IntegerData, OBJECT>::value != 0>(), copy);
        }
        ~Integer()
        {
            if ((BITS == 0) && (IntegerData::IsHeap() == true)) {
                delete[] IntegerData::Pointer();
            }
        }

        inline Integer<BITS>& operator=(const Integer<BITS>& RHS)
        {
            return (AssignmentOperator(TemplateIntToType<true>(), RHS));
        }
        template <typename OBJECT>
        inline Integer<BITS>& operator=(const OBJECT& RHS)
        {
            return (AssignmentOperator(TemplateIntToType<Core::TypeTraits::same_or_inherits<IntegerData, OBJECT>::value != 0>(), RHS));
        }

    public:
        bool SmallPrimeTest() const
        {
            if (IntegerData::Compare(Integer<8>(static_cast<unsigned char>(3))) < 0) {
                return (true);
            }
            else if (((*this)[0] & 0x01) == 0) {
                return (false);
            }

            Integer<0> integer(IntegerData::Limbs() * IntegerData::BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA(IntegerData::Limbs() * IntegerData::BytesPerLimb)));
            Integer<0> remainder(IntegerData::Limbs() * IntegerData::BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA(IntegerData::Limbs() * IntegerData::BytesPerLimb)));

            for (unsigned short index = 0; index < sizeof(SmallPrime); index++) {
                Integer<32> test(SmallPrime[index]);

                IntegerData::Div(test, integer, remainder);

                if (remainder.IsZero() == true) {
                    return (false);
                }
            }
            return (true);
        }
        /*
         * Miller-Rabin pseudo-primality test  (HAC 4.24)
         */
        bool MillerRabin(const unsigned int cycles = 0) const
        {
            unsigned int maxRuns = cycles;

            if (IntegerData::Compare(Integer<8>(static_cast<unsigned char>(3))) < 0) {
                return (true);
            }
            else if (((*this)[0] & 0x01) == 0) {
                return (false);
            }

            if (maxRuns == 0) {
                maxRuns = PrimeCheckSize(IntegerData::Bits());
            }

            // We tackled the obvious, let start determining what might be correct.
            Integer<BITS> reference(*this);
            Integer<BITS> result;
            Integer<BITS> remainder;
            reference.Sub(Integer<8>(static_cast<unsigned char>(1)), reference);
            Integer<BITS> storage(reference);

            // Shift lower '0' bits out
            unsigned int index = 0;
            while (storage[index] == 0) {
                index++;
            }
            LimbType value = storage[index];
            index <<= 3;

            while ((value & 0x01) == 0) {
                value >>= 1;
                index++;
            }
            storage.ShiftRight(index);

            for (unsigned int runs = 0; runs < maxRuns; runs++) {
                Integer<BITS> test;
                Integer<BITS> temp(storage);
                Integer<0> multiply((IntegerData::Limbs() * IntegerData::BitsPerLimb * 2), reinterpret_cast<LimbType*>(ALLOCA(IntegerData::Limbs() * IntegerData::BytesPerLimb * 2)));
                Integer<0> integer((IntegerData::Limbs() * IntegerData::BitsPerLimb * 2), reinterpret_cast<LimbType*>(ALLOCA(IntegerData::Limbs() * IntegerData::BytesPerLimb * 2)));

                // Here is now a theorethical hole, where the test might be '0'.
                // Create a random number, smaller or equal to the number
                // we try to prove as being a prime.
                test.Random(reference.MostSignificantBit());

                Integer<BITS> mod;
                test.PowerMod<6>(temp, *this, mod);

                while ((temp != reference) && (mod != Integer<8>(static_cast<unsigned char>(1))) && (mod != reference)) {
                    mod.Mul(mod, multiply);
                    ASSERT(multiply.HasOverflow() == false);
                    multiply.Div(*this, integer, mod);
                    ASSERT(mod.HasOverflow() == false);
                    temp.ShiftLeft(1);
                }
                if ((mod != reference) && ((temp[0] & 0x01) == 0)) {
                    return (false);
                }
            }
            return (true);
        }
        /*
         * Greatest common divisor: G = gcd(A, B)  (HAC 14.54)
         */
        void GreatestCommonDivisor(const IntegerData& RHS, IntegerData& result) const
        {
            unsigned int skip = LeastSignificantBit();
            unsigned int lsbrhs = RHS.LeastSignificantBit();

            if ((skip == static_cast<unsigned int>(~0)) || (lsbrhs == static_cast<unsigned int>(~0))) {
                result.Clear();
            }
            else {
                unsigned int length = (RHS.Limbs() > IntegerData::Limbs() ? RHS.Limbs() : IntegerData::Limbs());
                Integer<0> rhs(length * IntegerData::BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA(length * IntegerData::BytesPerLimb)));
                Integer<0> lhs(length * IntegerData::BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA(length * IntegerData::BytesPerLimb)));

                lhs.Copy(*this); /* A */
                rhs.Copy(RHS); /* B */

                if (lsbrhs < skip) {
                    skip = lsbrhs;
                }

                lhs.ShiftRight(skip);
                rhs.ShiftRight(skip);
                lhs.Negative(false);
                rhs.Negative(false);

                while (lhs.IsZero() == false) {
                    lhs.ShiftRight(lhs.LeastSignificantBit());
                    rhs.ShiftRight(rhs.LeastSignificantBit());

                    if (lhs.Compare(rhs) >= 0) {
                        lhs.DoSub(rhs, lhs);
                        lhs.ShiftBitRight(false);
                    }
                    else {
                        rhs.DoSub(lhs, rhs);
                        rhs.ShiftBitRight(false);
                    }
                }

                rhs.ShiftLeft(skip);
                result.Copy(rhs);
            }
        }

        /*
         * Modular inverse: X = A^-1 mod N  (HAC 14.61 / 14.64)
         */
        void InverseMod(const IntegerData& N, IntegerData& X) const
        {
            unsigned int nLimbs = N.UsedLimbs();

            if (nLimbs == 0) {
                X.Clear();
            }
            else {
                unsigned int aLimbs = UsedLimbs();
                unsigned int limbsNeeded = (nLimbs > aLimbs ? nLimbs : aLimbs);
                Integer<0> TA(limbsNeeded * BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA(limbsNeeded * IntegerData::BytesPerLimb)));

                GreatestCommonDivisor(N, TA);

                // TA != 1
                if (TA.MostSignificantBit() != 0) {
                    X.Clear();
                }
                else {
                    LimbType* allocation = reinterpret_cast<LimbType*>(ALLOCA(limbsNeeded * 7 * IntegerData::BytesPerLimb));
                    Integer<0> TU(limbsNeeded * BitsPerLimb, &allocation[0]);
                    Integer<0> U1(limbsNeeded * BitsPerLimb, &allocation[limbsNeeded]);
                    Integer<0> U2(limbsNeeded * BitsPerLimb, &allocation[2 * limbsNeeded]);
                    Integer<0> TB(limbsNeeded * BitsPerLimb, &allocation[3 * limbsNeeded]);
                    Integer<0> TV(limbsNeeded * BitsPerLimb, &allocation[4 * limbsNeeded]);
                    Integer<0> V1(limbsNeeded * BitsPerLimb, &allocation[5 * limbsNeeded]);
                    Integer<0> V2(limbsNeeded * BitsPerLimb, &allocation[6 * limbsNeeded]);

                    Div(N, TU, TA);
                    TU.Copy(TA);
                    TB.Copy(N);
                    TV.Copy(N);
                    U1.SetBit(0); // Set U1 = 1
                    V2.SetBit(0); // Set V2 = 1

                    do {
                        while (TU.IsEven()) {
                            TU.ShiftRight(1);

                            if (U1.IsOdd() || U2.IsOdd()) {
                                U1.Add(TB, U1);
                                U2.Sub(TA, U2);
                            }
                            U1.ShiftRight(1);
                            U2.ShiftRight(1);
                        }

                        while (TV.IsEven() == true) {
                            TV.ShiftRight(1);

                            if (V1.IsOdd() || V2.IsOdd()) {
                                V1.Add(TB, V1);
                                V2.Sub(TA, V2);
                            }
                            V1.ShiftRight(1);
                            V2.ShiftRight(1);
                        }

                        if (TU.Compare(TV) >= 0) {
                            TU.Sub(TV, TU);
                            U1.Sub(V1, U1);
                            U2.Sub(V2, U2);
                        }
                        else {
                            TV.Sub(TU, TV);
                            V1.Sub(U1, V1);
                            V2.Sub(U2, V2);
                        }

                    } while (TU.IsZero() == false);

                    while (V1.IsNegative()) {
                        V1.Add(N, V1);
                    }
                    while (V1.Compare(N) >= 0) {
                        V1.Add(N, V1);
                    }

                    X.Copy(V1);
                }
            }
        }
        /*
         * Sliding-window exponentiation: X = A^E mod N  (HAC 14.85)
         */
        template <const unsigned int CACHESIZE>
        inline void PowerMod(const IntegerData& exponent, const IntegerData& mod, IntegerData& result) const
        {
            unsigned int limbsNeeded = mod.UsedLimbs() * 2;
            Integer<0> number((limbsNeeded + 1) * IntegerData::BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA((limbsNeeded + 1) * IntegerData::BytesPerLimb)));
            Integer<0> RR(mod.Bits(), reinterpret_cast<LimbType*>(ALLOCA(mod.Limbs() * IntegerData::BytesPerLimb)));
            Integer<0> integer(mod.Bits(), reinterpret_cast<LimbType*>(ALLOCA(mod.Limbs() * IntegerData::BytesPerLimb)));

            // Turn on the highest bit
            number.SetBit(limbsNeeded * IntegerData::BitsPerLimb);
            number.Div(mod, integer, RR);

            PowerMod<CACHESIZE>(exponent, mod, RR, result);
        }
        template <const unsigned int CACHESIZE>
        void PowerMod(const IntegerData& E, const IntegerData& N, const IntegerData& RR, IntegerData& result) const
        {
            unsigned int bits = E.MostSignificantBit();
            ASSERT(N.Bits() <= BITS);

            if ((N.IsNegative() == false) && (N.IsZero() == false) && (N.IsOdd() == true) && (bits != static_cast<unsigned int>(~0)) && (E.IsNegative() == false)) {
                LimbType mm = N.MontgomeryInit();
                unsigned int length = N.UsedLimbs() + 1;
                unsigned int wsize = (bits > 671) ? CACHESIZE : (bits > 239) ? (CACHESIZE >= 5 ? 5 : CACHESIZE) : (bits > 79) ? (CACHESIZE >= 4 ? 4 : CACHESIZE) : (bits > 23) ? (CACHESIZE >= 3 ? 3 : CACHESIZE) : 1;

                Integer<8> One;
                One.SetBit(0);
                Integer<0> X(length * IntegerData::BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA(length * IntegerData::BytesPerLimb)));
                IntegerDataArray<2 << CACHESIZE> cache(length * IntegerData::BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA((2 << CACHESIZE) * length * IntegerData::BytesPerLimb)));

                // By definition we do not want to be negative..
                if (AbsoluteCompare(N) > 0) {
                    Integer<0> integer(Limbs() * IntegerData::BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA(Limbs() * IntegerData::BytesPerLimb)));
                    Integer<0> remainder(Limbs() * IntegerData::BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA(Limbs() * IntegerData::BytesPerLimb)));
                    Div(N, integer, remainder);

                    cache[1].Copy(remainder);
                }
                else {
                    cache[1].Copy(*this);
                }

                cache[1].Negative(false);
                cache[1].MontgomeryMul(RR, N, mm, cache[1]);
                RR.MontgomeryMul(One, N, mm, X);

                if (wsize > 1) {
                    /* W[1 << (wsize - 1)] = W[1] ^ (wsize - 1) */
                    unsigned int j = 1 << (wsize - 1);
                    cache[j].Copy(cache[1]);

                    for (unsigned int i = 0; i < wsize - 1; i++) {
                        cache[j].MontgomeryMul(cache[j], N, mm, cache[j]);
                    }

                    /* W[i] = W[i - 1] * W[1] */
                    for (unsigned int i = j + 1; i < static_cast<unsigned int>(1 << wsize); i++) {
                        cache[i - 1].MontgomeryMul(cache[1], N, mm, cache[i]);
                    }
                }

                unsigned int nblimbs = E.UsedLimbs();
                unsigned int bufsize = 0;
                unsigned int nbits = 0;
                unsigned int state = 0;
                LimbType wbits = 0;

                while ((bufsize != 0) || (nblimbs != 0)) {
                    if (bufsize == 0) {
                        nblimbs--;

                        bufsize = sizeof(LimbType) << 3;
                    }

                    bufsize--;

                    unsigned int ei = (E[nblimbs] >> bufsize) & 1;

                    /* skip leading 0s */
                    if ((ei != 0) || (state != 0)) {
                        if ((ei == 0) && (state == 1)) {
                            /* out of window, square X */
                            X.MontgomeryMul(X, N, mm, X);
                        }
                        else {
                            /* add ei to current window */
                            state = 2;

                            nbits++;
                            wbits |= (ei << (wsize - nbits));

                            if (nbits == wsize) {
                                /* X = X^wsize R^-1 mod N */
                                for (unsigned int i = 0; i < wsize; i++) {
                                    X.MontgomeryMul(X, N, mm, X);
                                }

                                /* X = X * W[wbits] R^-1 mod N */
                                X.MontgomeryMul(cache[wbits], N, mm, X);

                                state--;
                                nbits = 0;
                                wbits = 0;
                            }
                        }
                    }
                }

                /* process the remaining bits */
                for (unsigned int i = 0; i < nbits; i++) {
                    X.MontgomeryMul(X, N, mm, X);

                    wbits <<= 1;

                    if ((wbits & (1 << wsize)) != 0) {
                        X.MontgomeryMul(cache[1], N, mm, X);
                    }
                }

                /* X = A^E * R * R^-1 mod N = A^E mod N */
                X.MontgomeryMul(One, N, mm, X);

                if (IsNegative()) {
                    X.Negative(true);
                    X.Add(N, X);
                }

                // Finaly, copy what we calculated...
                result.Copy(X);
            }
        }

    public:
        // -----------------------------------------------------------------------
        // Compare method overloading, as if it is a regular integer.
        // -----------------------------------------------------------------------
        template <typename OBJECT>
        inline bool operator==(const OBJECT& RHS) const
        {
            // Compare the parts in common...
            return (CompareTyped(TemplateIntToType<Core::TypeTraits::same_or_inherits<IntegerData, OBJECT>::value != 0>(), RHS) == 0);
        }
        template <typename OBJECT>
        inline bool operator!=(const OBJECT& RHS) const
        {
            // Compare the parts in common...
            return (CompareTyped(TemplateIntToType<Core::TypeTraits::same_or_inherits<IntegerData, OBJECT>::value != 0>(), RHS) != 0);
        }
        template <typename OBJECT>
        inline bool operator<(const OBJECT& RHS) const
        {
            // Compare the parts in common...
            return (CompareTyped(TemplateIntToType<Core::TypeTraits::same_or_inherits<IntegerData, OBJECT>::value != 0>(), RHS) < 0);
        }
        template <typename OBJECT>
        inline bool operator>(const OBJECT& RHS) const
        {
            // Compare the parts in common...
            return (CompareTyped(TemplateIntToType<Core::TypeTraits::same_or_inherits<IntegerData, OBJECT>::value != 0>(), RHS) > 0);
        }
        template <typename OBJECT>
        inline bool operator<=(const OBJECT& RHS) const
        {
            // Compare the parts in common...
            return (CompareTyped(TemplateIntToType<Core::TypeTraits::same_or_inherits<IntegerData, OBJECT>::value != 0>(), RHS) <= 0);
        }
        template <typename OBJECT>
        inline bool operator>=(const OBJECT& RHS) const
        {
            // Compare the parts in common...
            return (CompareTyped(TemplateIntToType<Core::TypeTraits::same_or_inherits<IntegerData, OBJECT>::value != 0>(), RHS) >= 0);
        }

        // -----------------------------------------------------------------------
        // Assignement method overloading, as if it is a regular integer.
        // -----------------------------------------------------------------------
        inline Integer<BITS> operator<<(const unsigned int bitsToShift) const
        {
            Integer<BITS> result;
            result.Copy(*this);
            result.ShiftLeft(bitsToShift);
            return (result);
        }
        inline Integer<BITS>& operator<<=(const unsigned int bitsToShift)
        {
            IntegerData::ShiftLeft(bitsToShift);
            return (*this);
        }
        inline Integer<BITS> operator>>(const unsigned int bitsToShift) const
        {
            Integer<BITS> result;
            result.Copy(*this);
            result.ShiftRight(bitsToShift);
            return (result);
        }
        inline Integer<BITS>& operator>>=(const unsigned int bitsToShift)
        {
            IntegerData::ShiftRight(bitsToShift);
            return (*this);
        }

        template <const unsigned int RHSBITS>
        inline Integer<BITS> operator+(const Integer<RHSBITS>& RHS) const
        {
            Integer<BITS> returnValue;
            IntegerData::Add(RHS, returnValue);
            return (returnValue);
        }
        template <typename SCALAR>
        inline Integer<BITS> operator+(const SCALAR value) const
        {
            return (operator+(PlatformInteger(value)));
        }

        template <const unsigned int RHSBITS>
        inline Integer<BITS>& operator+=(const Integer<RHSBITS>& RHS)
        {
            IntegerData::Add(RHS, *this);
            return (*this);
        }
        template <typename SCALAR>
        inline Integer<BITS>& operator+=(const SCALAR value)
        {
            return (operator+=(PlatformInteger(value)));
        }

        template <const unsigned int RHSBITS>
        inline Integer<BITS> operator-(const Integer<RHSBITS>& RHS) const
        {
            Integer<BITS> returnValue;
            IntegerData::Sub(RHS, returnValue);
            return (returnValue);
        }
        template <typename SCALAR>
        inline Integer<BITS> operator-(const SCALAR value) const
        {
            return (operator-(PlatformInteger(value)));
        }
        template <const unsigned int RHSBITS>
        inline Integer<BITS>& operator-=(const Integer<RHSBITS>& RHS)
        {
            IntegerData::Sub(RHS, *this);
            return (*this);
        }
        template <typename SCALAR>
        inline Integer<BITS>& operator-=(const SCALAR value)
        {
            return (operator-=(PlatformInteger(value)));
        }

        template <const unsigned int RHSBITS>
        inline Integer<BITS> operator*(const Integer<RHSBITS>& RHS) const
        {
            Integer<BITS> returnValue;
            IntegerData::Mul(RHS, returnValue);
            return (returnValue);
        }
        template <typename SCALAR>
        inline Integer<BITS> operator*(const SCALAR value) const
        {
            return (operator*(PlatformInteger(value)));
        }
        template <const unsigned int RHSBITS>
        inline Integer<BITS>& operator*=(const Integer<RHSBITS>& RHS)
        {
            IntegerData::Mul(RHS, *this);
            return (*this);
        }
        template <typename SCALAR>
        inline Integer<BITS>& operator*=(const SCALAR value)
        {
            return (operator*=(PlatformInteger(value)));
        }

        template <const unsigned int RHSBITS>
        inline Integer<BITS> operator/(const Integer<RHSBITS>& RHS) const
        {
            unsigned int length = (IntegerData::Limbs() > RHS.Limbs() ? IntegerData::Limbs() : RHS.Limbs());
            Integer<0> remainder(length * IntegerData::BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA(length * IntegerData::BytesPerLimb)));
            Integer<BITS> integer;
            IntegerData::Div(RHS, integer, remainder);
            return (integer);
        }
        template <typename SCALAR>
        inline Integer<BITS> operator/(const SCALAR value) const
        {
            PlatformInteger RHS(value);
            unsigned int length = (IntegerData::Limbs() > RHS.Limbs() ? IntegerData::Limbs() : RHS.Limbs());
            Integer<0> remainder(length * IntegerData::BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA(length * IntegerData::BytesPerLimb)));
            Integer<BITS> integer;
            IntegerData::Div(RHS, integer, remainder);
            return (integer);
        }
        template <const unsigned int RHSBITS>
        inline Integer<BITS>& operator/=(const Integer<RHSBITS>& RHS)
        {
            unsigned int length = (IntegerData::Limbs() > RHS.Limbs() ? IntegerData::Limbs() : RHS.Limbs());
            Integer<0> remainder(length * IntegerData::BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA(length * IntegerData::BytesPerLimb)));
            Integer<BITS> integer;
            IntegerData::Div(RHS, integer, remainder);
            IntegerData::Copy(integer);
            return (*this);
        }
        template <typename SCALAR>
        inline Integer<BITS>& operator/=(const SCALAR value)
        {
            PlatformInteger RHS(value);
            unsigned int length = (IntegerData::Limbs() > RHS.Limbs() ? IntegerData::Limbs() : RHS.Limbs());
            Integer<0> remainder(length * IntegerData::BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA(length * IntegerData::BytesPerLimb)));
            Integer<BITS> integer;
            IntegerData::Div(RHS, integer, remainder);
            IntegerData::Copy(integer);
            return (*this);
        }

        template <const unsigned int RHSBITS>
        inline Integer<BITS> operator%(const Integer<RHSBITS>& RHS) const
        {
            unsigned int length = (IntegerData::Limbs() > RHS.Limbs() ? IntegerData::Limbs() : RHS.Limbs());
            Integer<0> remainder(length * IntegerData::BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA(length * IntegerData::BytesPerLimb)));
            Integer<BITS> integer;
            IntegerData::Div(RHS, integer, remainder);
            Integer<BITS> returnValue;
            returnValue.Copy(remainder);
            return (returnValue);
        }
        template <const unsigned int RHSBITS>
        inline Integer<BITS>& operator%=(const Integer<RHSBITS>& RHS)
        {
            unsigned int length = (IntegerData::Limbs() > RHS.Limbs() ? IntegerData::Limbs() : RHS.Limbs());
            Integer<0> remainder(length * IntegerData::BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA(length * IntegerData::BytesPerLimb)));
            Integer<BITS> integer;
            IntegerData::Div(RHS, integer, remainder);
            IntegerData::Copy(remainder);
            return (*this);
        }
        template <typename SCALAR>
        inline Integer<BITS> operator%(const SCALAR value) const
        {
            PlatformInteger RHS(value);
            unsigned int length = (IntegerData::Limbs() > RHS.Limbs() ? IntegerData::Limbs() : RHS.Limbs());
            Integer<0> remainder(length * IntegerData::BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA(length * IntegerData::BytesPerLimb)));
            Integer<BITS> integer;
            IntegerData::Div(RHS, integer, remainder);
            Integer<BITS> returnValue;
            returnValue.Copy(remainder);
            return (returnValue);
        }
        template <typename SCALAR>
        inline Integer<BITS>& operator%=(const SCALAR value)
        {
            PlatformInteger RHS(value);
            unsigned int length = (IntegerData::Limbs() > RHS.Limbs() ? IntegerData::Limbs() : RHS.Limbs());
            Integer<0> remainder(length * IntegerData::BitsPerLimb, reinterpret_cast<LimbType*>(ALLOCA(length * IntegerData::BytesPerLimb)));
            Integer<BITS> integer;
            IntegerData::Div(RHS, integer, remainder);
            IntegerData::Copy(remainder);
            return (*this);
        }

        template <const unsigned int RHSBITS>
        inline Integer<BITS> operator|(const Integer<RHSBITS>& RHS) const
        {
            Integer<BITS> result;
            IntegerData::Or(RHS, result);
            return (result);
        }
        template <const unsigned int RHSBITS>
        inline Integer<BITS>& operator|=(const Integer<RHSBITS>& RHS)
        {
            IntegerData::Or(RHS, *this);
            return (*this);
        }

        template <const unsigned int RHSBITS>
        inline Integer<BITS> operator&(const Integer<RHSBITS>& RHS) const
        {
            Integer<BITS> result;
            IntegerData::And(RHS, result);
            return (result);
        }
        template <const unsigned int RHSBITS>
        inline Integer<BITS>& operator&=(const Integer<RHSBITS>& RHS)
        {
            IntegerData::And(RHS, *this);
            return (*this);
        }

        template <const unsigned int RHSBITS>
        inline Integer<BITS> operator^(const Integer<RHSBITS>& RHS) const
        {
            Integer<BITS> result;
            IntegerData::Xor(RHS, result);
            return (result);
        }
        template <const unsigned int RHSBITS>
        inline Integer<BITS>& operator^=(const Integer<RHSBITS>& RHS)
        {
            IntegerData::Xor(RHS, *this);
            return (*this);
        }

        inline Integer<BITS> operator~() const
        {
            Integer<BITS> result;
            IntegerData::Neg(result);
            return (result);
        }
        inline Integer<BITS> operator--()
        {
            IntegerData::Sub(Integer<8>(static_cast<unsigned char>(1)), *this);
            return (*this);
        }
        inline Integer<BITS> operator++()
        {
            IntegerData::Add(Integer<8>(static_cast<unsigned char>(1)), *this);
            return (*this);
        }

    private:
        // -----------------------------------------------------------------------
        // Write Metadata methods
        // -----------------------------------------------------------------------
        inline void Construct(const unsigned int minimumBits)
        {
            if (BITS == 0) {
                unsigned requiredSize = IntegerData::BitsToLimbs(minimumBits > INTEGER_CHUNKSIZE ? minimumBits : INTEGER_CHUNKSIZE);
                IntegerData::SetBuffer(new LimbType[requiredSize], requiredSize * IntegerData::BitsPerLimb, true);
            }
            else {
                IntegerData::SetBuffer(&(_allocation[0]), BITS, false);
            }
        }

    private:
        LimbType _allocation[BITS == 0 ? 1 : (BITS + ((sizeof(LimbType) * 8) - 1) / (sizeof(LimbType) * 8))];
    };
}
} // Namespace Core

#endif // __INTEGER_H
