#ifndef __CYCLICBUFFER_H
#define __CYCLICBUFFER_H

// ---- Include system wide include files ----
#include <atomic>

// ---- Include local include files ----
#include "Module.h"
#include "DataElementFile.h"

#ifndef __WIN32__
#include <semaphore.h>
#endif

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

namespace WPEFramework {

namespace Core {
    // Rationale:
    // This class allows to share data over process boundaries. Private access can be arranged by taking a lock.
    // The lock is also Process Wide.
    // Whoever holds the lock, can privately read or write from the buffer.

    class EXTERNAL CyclicBuffer {
    private:
        CyclicBuffer() = delete;
        CyclicBuffer(const CyclicBuffer&) = delete;
        CyclicBuffer& operator=(const CyclicBuffer&) = delete;

    public:
        CyclicBuffer(const string& fileName, const uint32 bufferSize = 0, const bool overwrite = false);
        virtual ~CyclicBuffer();

    protected:
        class Cursor {
        public:
        	Cursor(CyclicBuffer& parent, uint32 tail, uint32 requiredSize)
        		: _Parent(parent)
				, _Tail(tail)
        		, _Size(requiredSize)
        		, _Offset(0)
			{
        	}

        	template <typename ArgType>
        	void Peek(ArgType& buffer) const
        	{
        		uint32 startIndex = _Tail & _Parent._administration->_tailIndexMask;
        		startIndex += _Offset;
        		startIndex %= _Parent._maxSize;

        		uint8 * bytePtr = reinterpret_cast<uint8 *>(&buffer);

        		for (uint32 i = 0; i < sizeof(buffer); i++) {
        			uint32 index = (startIndex + i) % _Parent._maxSize;
        			bytePtr[i] = _Parent._realBuffer[index];
        		}
        	}

        	uint32 Size() const
        	{
        		return _Size;
        	}

        	uint32 Offset() const
        	{
        	    return _Offset;
        	}

        	void Forward(uint32 byteCount)
        	{
        		_Offset += byteCount;
        	}

            uint32 GetCompleteTail(uint32 offset) const
            {
                uint32 oldTail = _Tail;
                uint32 roundCount = oldTail / (1 + _Parent._administration->_tailIndexMask);
                oldTail &= _Parent._administration->_tailIndexMask;

                uint32 completeTail = oldTail + offset;
                completeTail %= _Parent._maxSize;
                if (completeTail < oldTail) {
                    // Add one round, but prevent overflow.
                    roundCount = (roundCount + 1) % _Parent._administration->_roundCountModulo;
                }

                completeTail |= roundCount * (1 + _Parent._administration->_tailIndexMask);
                return completeTail;
            }

        private:
        	uint32 GetCurrentTail() const
        	{
        		return GetCompleteTail(_Offset);
        	}

        	uint32 GetRemainingRequired() const
        	{
        		return (_Size - _Offset);
        	}

        	CyclicBuffer& _Parent;
        	uint32 _Tail;

        	uint32 _Size;
        	uint32 _Offset;
        };

        inline uint32 Used(uint32 head, uint32 tail) const
        {
        	uint32 output = (head >= tail ? head - tail : _maxSize - (tail - head));
        	return output;
        }

        inline uint32 Free(uint32 head, uint32 tail) const
        {
            uint32 result = (head >= tail ? _maxSize - (head - tail) : tail - head);
            return result;
        }

    public:
		inline void Flush()
		{
			std::atomic_store_explicit(&(_administration->_tail), (std::atomic_load(&(_administration->_head))), std::memory_order_relaxed);
		}
        inline bool Overwritten() const
        {
            bool overwritten((std::atomic_load(&(_administration->_state)) & OVERWRITTEN) == OVERWRITTEN);

            // Now clear the flag.
            std::atomic_fetch_and(&(_administration->_state), static_cast<uint16>(~OVERWRITTEN));

            return (overwritten);
        }
        inline uint32 ErrorCode() const
        {
            return (_buffer.ErrorCode());
        }
        inline const string& Name() const
        {
            return (_buffer.Storage().Name());
        }
        inline bool IsLocked() const
        {
            return ((std::atomic_load(&(_administration->_state)) & LOCKED) == LOCKED);
        }
        inline uint32 LockPid() const
        {
            return (_administration->_lockPID);
        }
        inline bool IsOverwrite() const
        {
            return ((std::atomic_load(&(_administration->_state)) & OVERWRITE) == OVERWRITE);
        }
        inline bool IsValid() const
        {
            return (_buffer.IsValid());
        }
        inline const File& Storage() const
        {
            return (_buffer.Storage());
        }
        inline uint32 Used() const
        {
            uint32 head(_administration->_head);
            uint32 tail(_administration->_tail & _administration->_tailIndexMask);

            return Used(head, tail);
        }
        inline uint32 Free() const
        {
            uint32 head(_administration->_head);
            uint32 tail(_administration->_tail & _administration->_tailIndexMask);

            return Free(head, tail);
        }
        inline uint32 Size() const
        {
            return (_maxSize);
        }

        // THREAD SAFE
        // If there are thread blocked in the Lock, they can be relinquised by
        // calling this method. This method, will un-block, all blocking calls
        // on the lock.
        void Alert();

        // THREAD SAFE
        // Give system wide, so over process boundaries, access to the cyclic buffer.
        // This is a blocking call. The call is blocked untill  no other requestor has
        // the lock, or there is DataPresent (if dataPresent parameter is set to true).
        uint32 Lock(bool dataPresent = false, const uint32 waitTime = Core::infinite);
        uint32 Unlock();

        // Extract data from the cyclic buffer. Peek, is nondestructive. The cyclic
        // tail pointer is not progressed.
        uint32 Peek(uint8 buffer[], const uint32 length) const;
        // Extract data from the cyclic buffer. Read, is destructive. The cyclic tail
        // pointer is progressed by the amount of data being inserted.
        uint32 Read(uint8 buffer[], const uint32 length);

        // Insert data into the cyclic buffer. By definition the head pointer is
        // progressed after the write.
        uint32 Write(const uint8 buffer[], const uint32 length);

        // Move the tail out of the way so at least we can write "length" bytes.
        // The head will only be moved once all data is written.
        // This allows for writes of partial buffers without worrying about
        //    readers seeing incomplete data.
        uint32 Reserve(const uint32 length);

	virtual void DataAvailable();

    private:
        // If the write occures, this method is called to determine the amount of spaces
        // that should be cleared out. The returned number of bytes must be equal to, or
        // larger than the minimumBytesToOverwrite. This method allows for skipping frames
        // if they are prefixed by a size, for example.
		virtual uint32 GetOverwriteSize(Cursor& cursor);
		virtual uint32 GetReadSize(Cursor& cursor);

        // Makes sure "required" is available. If not, tail is moved in a smart way.
        void AssureFreeSpace(const uint32 required);

        void AdminLock();
        void AdminUnlock();
        void Reevaluate();
        uint32 SignalLock(const uint32 waitTime);

    private:
        enum state {
            UNLOCKED = 0x00,
            LOCKED = 0x01,
            OVERWRITE = 0x02,
            OVERWRITTEN = 0x04
        };

        Core::DataElementFile _buffer;
        uint32 _maxSize;
        uint8* _realBuffer;
        bool _alert;

// Synchronisation over Process boundaries
#ifdef __WIN32__
        HANDLE _mutex;
        HANDLE _signal;
		HANDLE _event;
#endif

    public:
        // Shared data over the processes...
        struct control {
#ifndef __WIN32__
            pthread_mutex_t _mutex;
            pthread_cond_t _signal;

#endif

            std::atomic<uint32> _head;
            std::atomic<uint32> _tail;
            uint32 _tailIndexMask; // Bitmask of index in buffer, rest is round count.
            uint32 _roundCountModulo; // Value with which to mod round count to prevent overflow.
            std::atomic<uint32> _agents;
            std::atomic<uint16> _state;
            uint32 _size;
            uint32 _lockPID;

            // Keeps track of how much has been reserved for writing and by whom.
            uint32 _reserved; // How much reserved in total.
            uint32 _reservedWritten; // How much has already been written.
#ifndef __WIN32__
			std::atomic<pid_t> _reservedPID; // What process made the reservation.
#else
			std::atomic<DWORD> _reservedPID; // What process made the reservation.
#endif

        } * _administration;
    };
}
} // Core

#endif // __CYCLICBUFFER_H
