#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <linux/limits.h>
#include <unistd.h>
#include <dirent.h>
#include <memory.h>
#include <stdio.h>
#include <sys/stat.h> 
#include <fcntl.h>

using namespace std;

uint32_t g_pageSize = static_cast<uint32_t>(sysconf(_SC_PAGESIZE));

// Used to parse /proc/PID/maps
struct MemRange
{
    uintptr_t m_start;
    uintptr_t m_end;

    explicit MemRange(const string & mapsLine)
        : m_start(0)
        , m_end(0)
    {
        // TODO: sscanf seems to be perfect here
        size_t spaceIndex = mapsLine.find(' ');
        string rangeStr = mapsLine.substr(0, spaceIndex);
        size_t dashIndex = rangeStr.find('-');
        string startStr = rangeStr.substr(0, dashIndex);
        string endStr = rangeStr.substr(dashIndex + 1);

        std::istringstream issStart(startStr);
        issStart >> std::hex >> m_start;
        std::istringstream issEnd(endStr);
        issEnd >> std::hex >> m_end;
    }
};

// pagemap file is documented here:
//   https://www.kernel.org/doc/Documentation/vm/pagemap.txt
void DumpInfo(pid_t pid)
{
    //uint32_t pageSize = static_cast<uint32_t>(sysconf(_SC_PAGESIZE));

    char mapsPath[PATH_MAX];
    sprintf(mapsPath, "/proc/%u/maps", pid);
    std::ifstream is01(mapsPath);

    char pagemapPath[PATH_MAX];
    sprintf(pagemapPath, "/proc/%u/pagemap", pid);

    FILE * pagemapFile = fopen(pagemapPath, "rb");
    while (!is01.eof()) {
        string readLine;
        getline(is01, readLine);
        if (readLine.empty())
            continue;

        MemRange range(readLine);
        uint64_t pageCount = (range.m_end - range.m_start) / g_pageSize;
        cout << "   " << readLine << " (" << pageCount << ")" << endl;
        uint64_t pageMapOffset = (range.m_start / g_pageSize) * sizeof(uint64_t);
        int fseekStatus = fseek(pagemapFile, pageMapOffset, SEEK_SET);
        if (fseekStatus != 0) {
            cerr << "      Failed to seek in " << pagemapPath << endl;
            continue;
        }

        for (uint64_t i = 0; i < pageCount; i++) {
            uint64_t pageData = 0;
            size_t readCount = fread(&pageData, sizeof(uint64_t), 1, pagemapFile);
            if (readCount != 1) {
                cerr << "      Failed to read pageInfo from " << pagemapPath << endl;
                continue;
            }

            cout << "      " << i << ": " << flush;

            // Skip pages that are swapped out.
            bool isSwapped = ((pageData >> 62) & 1) != 0;
            if (isSwapped) {
                cout << "SWAPPED" << endl;
                continue;
            }

            // Skip pages that aren't present.
            bool isPresent = ((pageData >> 63) & 1) != 0;
            if (!isPresent) {
                cout << "NOT PRESENT" << endl;
                continue;
            }

            // Skip pages mapped to files.
            bool isFilePage = ((pageData >> 61) & 1) != 0;
            if (isFilePage) {
                cout << "FILE" << endl;
                continue;
            }

            // Lower 54 bits contain actual page frame number (PFN).
            uint64_t filter = (static_cast<uint64_t>(1) << 55) - 1;
            uint32_t pageFrameNumber = static_cast<uint32_t>(pageData & filter);
            cout << pageFrameNumber << endl;
        }
    }

    fclose(pagemapFile);
}

int main()
{
    DIR* dp;
    struct dirent* ep;

    dp = opendir("/proc");
    if (dp != nullptr) {
        while (nullptr != (ep = readdir(dp))) {
            int pid;
            char* endptr;

            pid = strtol(ep->d_name, &endptr, 10);

            if ('\0' == endptr[0]) {
                // We have a valid PID, Find, the parent of this process..
                char buffer[512];
                memset(buffer, 0, sizeof(buffer));
                int fd;
                cout << "Process: " << pid << endl;
                DumpInfo(pid);
            }
        }

        (void)closedir(dp);
    }


    return 0;
}
