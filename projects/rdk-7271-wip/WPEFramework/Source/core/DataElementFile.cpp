#include "DataElementFile.h"

#ifdef __LINUX__
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#undef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE nullptr
#endif

#ifdef __WIN32__
#include <sys/types.h>
#include <sys/stat.h>
#endif

namespace WPEFramework {
namespace Core {
    DataElementFile::DataElementFile(File& file, const FileState type)
        : DataElement()
        , m_File(file)
        , m_MemoryMappedFile(INVALID_HANDLE_VALUE)
        , m_Flags(type)
    {
        // What is the use of a file that is not readable nor writable ?
        ASSERT(m_Flags != 0);

        // The file needs to be prepared in the same way as we request the Memoerymapped file...
        ASSERT(m_File.IsOpen() == false);
        ASSERT(m_File.IsShared() == ((type & SHAREABLE) != 0));

        if (IsValid()) {
            OpenMemoryMappedFile(static_cast<uint32>(m_File.Size()));
        }
    }

    DataElementFile::DataElementFile(const string& fileName, const FileState type, const uint32 requestedSize)
        : DataElement()
        , m_File(fileName, ((type & SHAREABLE) != 0))
        , m_MemoryMappedFile(INVALID_HANDLE_VALUE)
        , m_Flags(type)
    {
        // What is the use of a file that is not readable nor writable ?
        ASSERT(m_Flags != 0);

        if ((type & CREATE) != 0) {
            m_File.Create();
        }
        else {
            m_File.Open((type & WRITABLE) == 0);
        }

        if (IsValid()) {
            if ((requestedSize != 0) && (requestedSize > m_File.Size())) {
                m_File.SetSize(requestedSize);
                OpenMemoryMappedFile(requestedSize);
            }
            else {
                OpenMemoryMappedFile(static_cast<uint32>(m_File.Size()));
            }
        }
    }

#ifdef __WIN32__
    void DataElementFile::OpenMemoryMappedFile(uint32 requiredSize)
    {
        DWORD flags = ((m_Flags & WRITABLE) != 0 ? PAGE_READWRITE : PAGE_READONLY);
        SYSTEM_INFO systemInfo;
        ::GetSystemInfo(&systemInfo);
        uint32 mapSize = ((((requiredSize - 1) / systemInfo.dwPageSize) + 1) * systemInfo.dwPageSize);

        // Open the file in MM mode as one element.
        m_MemoryMappedFile = ::CreateFileMapping(m_File, NULL, flags, 0, mapSize, NULL);

        if (m_MemoryMappedFile == NULL) {
            DWORD value = GetLastError();
            m_File.Close();
        }
        else {
            flags = ((m_Flags & READABLE) != 0 ? FILE_MAP_READ : 0) | ((m_Flags & WRITABLE) != 0 ? FILE_MAP_WRITE : 0);

            void* newBuffer = (::MapViewOfFile(m_MemoryMappedFile, flags, 0, 0, mapSize));

            // Seems like everything succeeded. Lets map it.
            UpdateCache(0, static_cast<uint8*>(newBuffer), requiredSize, mapSize);
        }
    }

    /* virtual */ DataElementFile::~DataElementFile()
    {
        if (IsValid()) {
            // Set the last size...
            ::MapViewOfFile(m_MemoryMappedFile, m_Flags, 0, 0, static_cast<SIZE_T>(AllocatedSize()));
            ::FlushViewOfFile(Buffer(), static_cast<SIZE_T>(AllocatedSize()));
            ::CloseHandle(m_MemoryMappedFile);

            m_MemoryMappedFile = INVALID_HANDLE_VALUE;
        }
    }

    /* virtual */ void DataElementFile::Reallocation(const uint64 size)
    {
        if (IsValid()) {
            SYSTEM_INFO systemInfo;
            ::GetSystemInfo(&systemInfo);

            // Allocated a new page.
            uint64 requestedSize = ((size / systemInfo.dwPageSize) * systemInfo.dwPageSize) + systemInfo.dwPageSize;

            void* newBuffer = ::MapViewOfFileEx(m_MemoryMappedFile, m_Flags, 0, 0, static_cast<SIZE_T>(requestedSize), Buffer());

            if (newBuffer == NULL) {
                m_File.Close();

                ::CloseHandle(m_MemoryMappedFile);
                m_MemoryMappedFile = INVALID_HANDLE_VALUE;

                UpdateCache(0, NULL, 0, 0);
            }
            else {
                // Seems we upgraded, set the caches
                UpdateCache(0, static_cast<uint8*>(newBuffer), Size(), requestedSize);
            }
        }
    }

#endif

#ifdef __POSIX__
    void DataElementFile::OpenMemoryMappedFile(uint32 requiredSize)
    {
        int pageSize = getpagesize();
        uint64 mapSize = ((((requiredSize - 1) / pageSize) + 1) * pageSize);
        int flags = (((m_Flags & READABLE) != 0 ? PROT_READ : 0) | ((m_Flags & WRITABLE) != 0 ? PROT_WRITE : 0));

        // Open the file in MM mode as one element.
        m_MemoryMappedFile = mmap(NULL, mapSize, flags, ((m_Flags & SHAREABLE) != 0 ? MAP_SHARED : MAP_PRIVATE), m_File, 0);

        if (m_MemoryMappedFile == MAP_FAILED) {
            m_File.Close();
            m_MemoryMappedFile = NULL;
        }
        else {
            // Seems like everything succeeded. Lets map it.
            UpdateCache(0, static_cast<uint8*>(m_MemoryMappedFile), requiredSize, mapSize);
        }
    }

    /* virtual */ DataElementFile::~DataElementFile()
    {
        if (IsValid()) {
            munmap(m_MemoryMappedFile, AllocatedSize());

            m_MemoryMappedFile = NULL;

            m_File.Close();
        }
    }

    /* virtual */ void DataElementFile::Reallocation(const uint64 size)
    {
        if (IsValid()) {
            int pageSize = getpagesize();

            // Allocated a new page.
            uint64 requestedSize = ((size / pageSize) * pageSize) + pageSize;

            m_MemoryMappedFile = mremap(m_MemoryMappedFile, AllocatedSize(), requestedSize, MREMAP_MAYMOVE);

            if (m_MemoryMappedFile == MAP_FAILED) {
                m_File.Close();
                m_MemoryMappedFile = NULL;

                UpdateCache(0, NULL, 0, 0);
            }
            else {
                // Seems we upgraded, set the caches
                UpdateCache(0, static_cast<uint8*>(m_MemoryMappedFile), Size(), requestedSize);
            }
        }
    }

#endif
}
} // namespace Core
