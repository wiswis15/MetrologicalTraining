#include <iostream>
#include <endian.h>
#include <sstream>

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

#pragma pack(push)
#pragma pack(1)
struct TraceHeader
{
	uint16 _Length;
	uint64 _ClockTicks;
	uint32 _LineNumber;
};
#pragma pack(pop)

struct TraceData
{
	TraceHeader _Header;
	string _File;
	string _Module;
	string _Category;
	string _Class;
	string _Text;

	string ToString()
	{
		stringstream output;
		output << _File << "(" << _Header._LineNumber << "): " << _Text;
		return output.str();
	}
};

class ServerCyclicBuffer01 : public Core::CyclicBuffer
{
public:
	ServerCyclicBuffer01(const string& fileName)
		: CyclicBuffer(fileName, 0, true)
	{
	}

	virtual uint32 GetReadSize(Cursor& cursor) override
	{
        uint16 entrySize = 0;
        cursor.Peek(entrySize);
        return entrySize;
	}
};

bool ReadTraceString(const uint8 buffer[], uint32 length, uint32& offset, string& output)
{
	output = "";

	const char * charBuffer = reinterpret_cast<const char *>(buffer);

	while(true) {
		char c = charBuffer[offset];

		if (c == '\0') {
			// Found the end
			offset++;
			return true;
		}

		output += string(1, c);
		offset++;

		if (offset >= length) {
			// Buffer overrun
			return false;
		}
	}

	return true;
}

bool ParseTraceData(const uint8 buffer[], uint32 length, uint32& offset, TraceData& traceData)
{
	uint32 startOffset = offset;

	const TraceHeader * header = reinterpret_cast<const TraceHeader *>(buffer + offset);
	offset += sizeof(TraceHeader);

	if (offset > length) {
		cerr << "Offset " << offset << " is larger than length " << length << endl;
		return false;
	}

	traceData._Header = *header;
	uint16 entrySize = traceData._Header._Length;
	ASSERT(entrySize <= TRACE_CYCLIC_BUFFER_SIZE);

	if (!ReadTraceString(buffer, length, offset, traceData._File)) {
		cerr << "Failed to read file name" << endl;
		return false;
	}

	if (!ReadTraceString(buffer, length, offset, traceData._Module)) {
		cerr << "Failed to read module name" << endl;
		return false;
	}

	if (!ReadTraceString(buffer, length, offset, traceData._Category)) {
		cerr << "Failed to read category" << endl;
		return false;
	}

	if (!ReadTraceString(buffer, length, offset, traceData._Class)) {
		cerr << "Failed to read class name" << endl;
		return false;
	}

	uint16 totalHeaderLength = offset - startOffset;
	uint16 textLength = entrySize - totalHeaderLength;
	uint16 textBufferLength = textLength + 1;
	char textBuffer[textBufferLength];

	memcpy(textBuffer, buffer + offset, textLength);
	textBuffer[textLength] = '\0';
	traceData._Text = string(textBuffer);

	offset += textLength;

	ASSERT(offset == (startOffset + entrySize));

	return true;
}

string GetCyclicBufferName()
{
	Core::Directory dir(g_tracingPathName.c_str());

	uint32 bufferCount = 0;
	string bufferPath;

	while(dir.Next()) {
		string triedPath = dir.Current();

		// Skip "." and ".."
		if(triedPath[triedPath.size() - 1] != '.') {
			bufferCount++;
			bufferPath = triedPath;
		}
	}

	if (bufferCount != 1) {
		cerr << "Expected to find exactly one buffer, but found: " << bufferCount << endl;
		exit(1);
	}

	return bufferPath;
}

void DebugCheckIfConsistent(const uint8 * buffer, int length, Core::CyclicBuffer& cycBuffer)
{
	uint entryCount = 0;

	uint32 index = 0;
	while (index < length) {
		uint16 entrySize = 0;
		entrySize += static_cast<uint16>(buffer[index]);
		index++;
		entrySize += static_cast<uint16>(buffer[index]) << 8;

		ASSERT(entrySize < TRACE_CYCLIC_BUFFER_SIZE);
		index += entrySize - 1;

		entryCount++;
	}

	ASSERT(index == length);
}

int main(int argc, const char * argv[])
{
    SingletonTrigger st;

	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " [start-time]" << endl;
		return 1;
	}

    g_startTime = atol(argv[1]);

    waitUntilAfterStart(2);
    string cycBufferName = GetCyclicBufferName();

    Core::DoorBell doorBell("tracebuffer");
    ServerCyclicBuffer01 cycBuffer(cycBufferName);

    fprintf(stderr, "Cyclic buffer size: %d\n", cycBuffer.Size());

    // TODO: maximum running time?
    while (doorBell.Wait(Core::infinite) == Core::ERROR_NONE) {
    	doorBell.Acknowledge();

    	uint32 tailLocation = cycBuffer._administration->_tail.load();

    	uint32 bufferLength = 200 + rand() % 500;
    	uint8 buffer[bufferLength];
    	uint32 actuallyRead = cycBuffer.Read(buffer, sizeof(buffer));

    	ASSERT(actuallyRead < cycBuffer.Size());

    	DebugCheckIfConsistent(buffer, actuallyRead, cycBuffer);

    	uint32 offset = 0;
    	int traceCount = 0;
    	while(offset < actuallyRead) {
        	TraceData traceData;
        	if (!ParseTraceData(buffer, actuallyRead, offset, traceData)) {
        		cerr << "Failed to parse trace data" << endl;
        		return 1;
        	}

        	traceCount++;
    	}

    	//fprintf(stderr, "Server processed %d traces.\n", traceCount);
    }

    cerr << "6. Server is done" << endl;
}
