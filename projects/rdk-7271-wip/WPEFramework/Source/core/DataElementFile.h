#ifndef __DATAELEMENTFILE_H
#define __DATAELEMENTFILE_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "Portability.h"
#include "DataElement.h"
#include "FileSystem.h"

// ---- Referenced classes and types ----
#ifdef __LINUX__
#include <sys/stat.h>
#endif

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
namespace WPEFramework {
namespace Core {
    // The datapackage is the abstract of a package that needs to be send over the line.
    class EXTERNAL DataElementFile : public DataElement {
    private:
        DataElementFile() = delete;
        DataElementFile(const DataElementFile&) = delete;
        DataElementFile& operator=(const DataElementFile&) = delete;

    public:
        enum FileState {
            READABLE = 0x01,
            WRITABLE = 0x02,
            SHAREABLE = 0x04,
            CREATE = 0x08
        };

        DataElementFile(File& fileName, const FileState type = READABLE);
        DataElementFile(const string& fileName, const FileState type = READABLE, const uint32 requiredSize = 0);
        virtual ~DataElementFile();

    public:
        inline bool IsValid() const
        {
            return (m_File.IsOpen());
        }
        inline const File& Storage() const
        {
            return (m_File);
        }
        inline uint32 ErrorCode() const
        {
            return (m_File.ErrorCode());
        }

    protected:
        virtual void Reallocation(const uint64 size);

    private:
        void OpenMemoryMappedFile(uint32 requiredSize);

    private:
#ifdef __WIN32__
        typedef HANDLE Handle;
#else
        typedef void* Handle;
#endif

        File m_File;
        Handle m_MemoryMappedFile;
        uint32 m_Flags;
    };
}
} // namespace Core

#endif // __DATAELEMENTFILE_H
