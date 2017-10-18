#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../generics/generics.h"

using namespace WPEFramework;

#define PAGE_SZ (1 << 12)

typedef struct {
    unsigned long size, resident, share, text, lib, data, dt;
} memory_stats;

static void ReadMemoryStatus(memory_stats& result)
{
    const char* statm_path = "/proc/self/statm";

    FILE* f = fopen(statm_path, "r");
    if (!f) {
        perror(statm_path);
        abort();
    }
    if (7 != fscanf(f, "%ld %ld %ld %ld %ld %ld %ld",
                 &result.size, &result.resident, &result.share, &result.text, &result.lib, &result.data, &result.dt)) {
        perror(statm_path);
        abort();
    }
    fclose(f);
}

static int startSize;
static int startResident;
memory_stats memoryStats;

static void ListMemoryUsage()
{
    static memory_stats previous;
    static int deltaSize, deltaResident;
    static int previousSize, previousResident;
    static uint8 measured = 0;
    static int countSize;
    static int countResident;

    // print memory usage
    ReadMemoryStatus(memoryStats);

    if (measured == 0) {
        measured++;
        previous = memoryStats;
        printf("##  Memory   %8lu Kb   %8lu Kb\n", (memoryStats.size * PAGE_SZ) / 1024, (memoryStats.resident * PAGE_SZ) / 1024);
    }
    else if (measured == 1) {
        measured++;
        deltaSize = static_cast<int>(memoryStats.size - previous.size);
        previousSize = 0;
        deltaResident = static_cast<int>(memoryStats.size - previous.size);
        previousResident = 0;
        previous = memoryStats;
        startSize = static_cast<int>(memoryStats.size);
        startResident = static_cast<int>(memoryStats.resident);

        printf("##  Memory   %8lu Kb   %8lu Kb\n", (memoryStats.size * PAGE_SZ) / 1024, (memoryStats.resident * PAGE_SZ) / 1024);
    }
    else {
        deltaSize = static_cast<int>(memoryStats.size - previous.size);
        deltaResident = static_cast<int>(memoryStats.resident - previous.resident);
        previous = memoryStats;

        printf("##  Memory   %8lu Kb   %8lu Kb", (memoryStats.size * PAGE_SZ) / 1024, (memoryStats.resident * PAGE_SZ) / 1024);

        if (previousSize != deltaSize) {
            previousSize = deltaSize;
            printf("  %8d Kb [%3d]", (deltaSize * PAGE_SZ) / 1024, countSize);
            countSize = 0;
        }
        else {
            printf("                ");
            countSize++;
        }
        if (previousResident != deltaResident) {
            previousResident = deltaResident;
            printf("  %8d Kb [%3d]", (deltaResident * PAGE_SZ) / 1024, countResident);
            countResident = 0;
        }
        else {
            countResident++;
        }
        printf("\n");
    }

    //printf("##  Memory Share:    %lu\n", memoryStats.share);
}

//static void PreventOOMKill()
//{
//    FILE* fp;
//    int no_oom = -17;
//
//    fp = fopen("/proc/self/oom_adj", "w");
//    if (fp != NULL) {
//        fprintf(fp, "%d", no_oom);
//        fclose(fp);
//    }
//}
//
//static void LockAllMemory()
//{
//    mlockall(MCL_CURRENT | MCL_FUTURE);
//}

int main(int argc, char** argv)
{

    unsigned int allocateUpTo = 0;

    if (argc > 1) {
        if ((argc > 2) && (strcmp(argv[1], "-size") == 0)) {
            allocateUpTo = static_cast<unsigned int>(atoi(argv[2]));
        }
        else {
            Core::ProcessInfo::Iterator index(argv[1], false);

            if (index.Next() == true) {
                ASSERT(index.IsValid() == true);
                printf("Found the process, it has ID: %d, lst size: %d\n", index.Current().Id(), index.Count());
            }
            else {
                printf("Did not find process %s\n", argv[1]);
            }
        }
    }
    if ((argc == 1) || (allocateUpTo > 0)) {
        int i;
        int runs = 412144; // memory to consume in GB
        int blockSize = (8 * PAGE_SZ) - 32;

        if (allocateUpTo > 0) {
            runs = (allocateUpTo * 1024) / ((8 * PAGE_SZ) / 1024);
        }

        printf("Pre-allocate, first run\n");
        ListMemoryUsage();
        for (i = 0; i < runs; ++i) {
            void* m = malloc(static_cast<size_t>(blockSize));
            if (!m) {
                printf("Failed allocation !!!\n");
                break;
            }
            for (int index = 0; index < blockSize; index++) {
                static_cast<uint8*>(m)[index] = static_cast<uint8>(rand() & 0xFF);
            }
            //memset(m, 0, blockSize);
            // memset(m, 0, 1);
            ListMemoryUsage();
        }
        int runSize = static_cast<int>(memoryStats.size - startSize);
        int runResident = static_cast<int>(memoryStats.resident - startResident);
        printf("Allocated %lu KB\n", ((unsigned long)i * blockSize) >> 10);
        printf("Size:     %lu KB\n", ((unsigned long)runSize * PAGE_SZ) >> 10);
        printf("Resident: %lu KB\n", ((unsigned long)runResident * PAGE_SZ) >> 10);

        int fd;
        TCHAR buffer[128];
//        int Share = 0;

        snprintf(buffer, sizeof(buffer), "/proc/self/statm");
        if ((fd = open(buffer, O_RDONLY)) > 0) {
            if (read(fd, buffer, sizeof(buffer)) > 0) {
                printf(" --> %s\n", buffer);
            }
            close(fd);
        }

        WPEFramework::Core::ProcessInfo process;

        printf("--------------------------------\n");
        printf("Size:     %llu KB\n", (process.Allocated()) >> 10);
        printf("Resident: %llu KB\n", (process.Resident()) >> 10);
        printf("Shared:   %llu KB\n", (process.Shared()) >> 10);

        char keyPress;

        do {
            keyPress = static_cast<char>(toupper(getchar()));

            if (keyPress != 'Q') {
                std::cout << "Press Q to quit the server." << std::endl;
            }
        } while (keyPress != 'Q');
    }

    WPEFramework::Core::Singleton::Dispose();
    return 0;
}
