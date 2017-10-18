#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../../generics/generics.h"

namespace WPEFramework {

namespace Test {

    bool ParseOptions(int argc, char** argv, string& fileName, bool& server)
    {
        int index = 1;
        bool showHelp = false;

        fileName.clear();
        server = true;

        while ((index < argc) && (showHelp == false)) {
            if (strcmp(argv[index], "-client") == 0) {
                server = false;
            }
            else if (strcmp(argv[index], "-file") == 0) {
                if (((index + 1) < argc) && (argv[index + 1][0] != '-')) {
                    index++;
                    fileName = argv[index];
                }
            }
            else {
                showHelp = true;
            }
            index++;
        }

        if (fileName.empty() == true) {
            showHelp = true;
        }

        if (showHelp == true) {
            printf("CyclicBufferTest [-client] -file <filename of the CyclicBuffer>\n");
            printf("    -client: Operate in client mode, read the cyclic buffer.\n");
            printf("    -file:   Filename of the cyclic buffer.\n");
        }

        return (showHelp == false);
    }
}
} // namespace WPEFramework::Test

using namespace WPEFramework;

const char defaultText[] = "the lazy dog jumped over the quick brown fox!";

void help()
{
    printf("1) Write default text to the buffer.\n");
    printf("2) Read the cyclic buffer.\n");
    printf("3) Aquire lock if there is data (10S).\n");
    printf("4) Unlock.\n");
    printf("5) Statistics.\n\n");
}

int main(int argc, char** argv)
{
    // If there are no arguments, we act as a server, otherwise we will be the client :-)
    bool server;
    string fileName;

    if (WPEFramework::Test::ParseOptions(argc, argv, fileName, server) == true) {

        char element;
        Core::CyclicBuffer buffer(fileName, (server == true ? 256 : 0), true);

        printf("Ready for action. Running as: [%s]\n\n", (server ? "Server" : "Client"));

        if (buffer.IsValid() == false) {
            printf("Failed to open Memory Mapped File. Error: %d\n\n", buffer.ErrorCode());
        }

        help();

        do {
            element = toupper(getchar());

            switch (element) {
            case '?': {
                help();
                break;
            }
            case '1': {

                buffer.Write(reinterpret_cast<const uint8*>(defaultText), sizeof(defaultText));
                break;
            }
            case '2': {
                uint8 loadBuffer[128];
                uint32 result = buffer.Read(loadBuffer, sizeof(loadBuffer) - 1);
                if (result > 0) {
                    unsigned int index = 0;
                    unsigned int line = 0;
                    loadBuffer[result] = '\0';
                    printf("Read %d bytes:\n", result);
                    while (index < result) {
                        line++;
                        printf("%02d => [%s]\n", line, &(loadBuffer[index]));
                        index += (strlen(reinterpret_cast<char*>(&(loadBuffer[index]))) + 1);
                    }
                }
                else {
                    printf("Nothing to read.\n");
                }

                break;
            }
            case '3': {
                uint32 result = buffer.Lock(true, 10000);

                printf("Lock resulted int %d.\n", result);
                break;
            }
            case '4': {
                buffer.Unlock();
                break;
            }
            case '5': {
                printf("Free:      %d\n", buffer.Free());
                printf("Used:      %d\n", buffer.Used());
                printf("Size:      %d\n", buffer.Size());
                printf("Locked:    %s\n", (buffer.IsLocked() ? "true" : "false"));
                printf("Lock PID:  %d\n", buffer.LockPid());
                printf("Overwrite: %s\n", (buffer.Overwritten() ? "true" : "false"));
                printf("Mode:      %s\n", (buffer.IsOverwrite() ? "overwrite" : "hold"));
                break;
            }
            }

        } while (element != 'Q');
    }

    printf("Leaving app.\n");

    Core::Singleton::Dispose();

    return (0);
}
