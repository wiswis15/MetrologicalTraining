#include "../../../Source/core/core.h"
#include "../../../Source/tracing/tracing.h"

#include "../globals.h"
#include "../utils.h"

using namespace WPEFramework;
using namespace std;

class SingletonTrigger
{
public:
    ~SingletonTrigger() {
        WPEFramework::Core::Singleton::Dispose();
     }
};

int main(int argc, const char * argv[])
{
	SingletonTrigger st;

	//srand(time(nullptr));

	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " [start-time]" << endl;
		return 1;
	}

    g_startTime = atol(argv[1]);

    WPEFramework::Trace::TraceUnit::Instance().Open(g_tracingPathName);
    Core::CyclicBuffer * cycBuffer = WPEFramework::Trace::TraceUnit::Instance().CyclicBuffer();

    WPEFramework::Trace::TraceType<WPEFramework::Trace::Information, &WPEFramework::Core::System::MODULE_NAME>::Enable(true);

    cerr << "Client is about to trace" << endl;

    // Build too long trace statement.
    const int longBufferSize = TRACE_CYCLIC_BUFFER_SIZE * 4 / 3;
    char longBuffer[longBufferSize];
    for (int j = 0; j < (longBufferSize - 1); j++) {
        longBuffer[j] = 'a';
    }
    longBuffer[longBufferSize - 1] = '\0';

    while(true) {
    	int traceCount = rand() % 100 + 50;

		for (int i = 0; i < traceCount; i++) {
			int traceLength = 50 + rand() % 200;
			char buffer[traceLength + 1];
			for (int j = 0; j < traceLength; j++) {
				buffer[j] = static_cast<char>(static_cast<int>('a') + rand() % 26);
			}

			buffer[traceLength] = '\0';

			TRACE_GLOBAL(WPEFramework::Trace::Information, (buffer));
		}

		// One long trace.
		TRACE_GLOBAL(WPEFramework::Trace::Information, (longBuffer));
    }

    sleep(10);
    return 0;
}
