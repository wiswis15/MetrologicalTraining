#ifndef __PROCESS_H__
#define __PROCESS_H__

/// https://msdn.microsoft.com/nl-nl/library/windows/desktop/ms682499(v=vs.85).aspx

#include "Module.h"
#include "Portability.h"

namespace WPEFramework {
namespace Core {

    class Process {
    public:
        class Options {
        private:
            Options() = delete;
            Options& operator=(const Options&) = delete;

            typedef std::map<const string, string> Map;

        public:
            typedef Core::IteratorMapType<const Map, const string&, const string&, Map::const_iterator> Iterator;

        public:
            Options(const string& command)
                : _command(command)
                , _options()
            {
                ASSERT(command.empty() == false);
            }
            Options(const string& command, const Iterator& options)
                : _command(command)
                , _options()
            {
                ASSERT(command.empty() == false);
                Set(options);
            }
            Options(const Options& copy)
                : _command(copy._command)
                , _options(copy._options)
            {
            }
            ~Options()
            {
            }

        public:
            inline const string& Command() const
            {
                return (_command);
            }
            inline void Erase(const string& key)
            {
                _options.erase(key);
            }
            inline string& operator[](const TCHAR key[])
            {
                return (_options[string(key)]);
            }
            inline string& operator[](const string& key)
            {
                return (_options[string(key)]);
            }
            inline void Clear()
            {
                _options.clear();
            }
            void Set(const string& key, const string value)
            {
                _options.insert(std::pair<const string, const string>(key, value));
            }
            void Set(const Iterator& options)
            {
                Iterator index(options);

                _options.clear();

                while (index.Next() == true) {
                    _options.insert(std::pair<const string, const string>(index.Key(), *index));
                }
            }
            Iterator Get() const
            {
                return (Iterator(_options));
            }
            uint16 LineSize() const
            {
                uint16 size = (static_cast<uint16>(_command.length()));
                Iterator index(_options);

                // First option needs to be the application start name
                while (index.Next() == true) {
                    const string& value(*index);

                    size += 2 + static_cast<uint16>(index.Key().length());

                    if ((value.empty() != true) && (value[0] != '\0')) {
                        size += 1 + static_cast<uint16>(value.length());
                    }
                }
                size++; // closing '\0'

                return (size);
            }
            uint16 BlockSize() const
            {
                // Time to build the argument array !!!
                uint16 argCount = 2; // at least a command and the closing nullptr;
                uint16 size = static_cast<uint16>(_command.length()) + 1;
                Iterator index(_options);

                // First option needs to be the application start name
                while (index.Next() == true) {
                    string value(*index);

                    argCount++;
                    size += 1 + static_cast<uint16>(index.Key().length()) + 1;

                    if (value.empty() != true) {
                        argCount++;
                        size += static_cast<uint16>(value.length()) + 1;
                    }
                }

                return ((sizeof(char*) * argCount) + (size * sizeof(char)));
            }
            uint16 Line(void* result, const uint16 length) const
            {
                uint16 commandCount(1);
                uint16 data(length);
                char* destination(reinterpret_cast<char*>(result));

                ASSERT(result != nullptr);

                if (data > _command.length()) {
                    ::strncpy(destination, _command.c_str(), _command.length());
                    data -= static_cast<uint16>(_command.length());
                    destination = &destination[_command.length()];
                }
                Iterator index(_options);

                // First option needs to be the application start name
                while ((data > 2) && (index.Next() == true)) {
                    const string& value(*index);

                    commandCount++;
                    *destination++ = ' ';
                    *destination++ = '-';
                    data -= 2;

                    ::strncpy(destination, index.Key().c_str(), data);
                    destination = &destination[index.Key().length()];
                    data -= static_cast<uint16>(index.Key().length() > data ? data : index.Key().length());

                    if ((data > 0) && (value.empty() != true)) {
                        commandCount++;
                        *destination++ = ' ';
                        data--;
                        ::strncpy(destination, value.c_str(), data);
                        destination = &destination[value.length()];
                        data -= static_cast<uint16>(value.length() > data ? data : value.length());
                    }
                }

                if (data > 0) {
                    *destination = '\0';
                }

                return (commandCount);
            }
            uint16 Block(void* result, const uint16 length) const
            {
                // Time to build the argument array !!!
                uint16 data(length);
                uint16 commandCount = 1 + static_cast<uint16>(_options.size()) + 1;
                Iterator index(_options);

                // Calculate the number of "not empty" values, they will trigger another argument
                while (index.Next() == true) {
                    if ((*index).empty() == false) {
                        commandCount++;
                    }
                }
                ASSERT(result != nullptr);

                if ((result == nullptr) || (data <= (sizeof(char*) * commandCount))) {
                    commandCount = 0;
                }
                else {
                    // Alright, create and fill the arrays.
                    char** indexer = reinterpret_cast<char**>(result);
                    char* destination = reinterpret_cast<char*>(&(static_cast<uint8*>(result)[(sizeof(char*) * commandCount)]));

                    data -= (sizeof(char*) * commandCount);
                    commandCount = 1;

                    // Set the first parameter, the command.
                    *indexer++ = destination;

                    // Copy the commad line in
                    ::strncpy(destination, _command.c_str(), data);
                    destination = &destination[_command.length() + 1];
                    data -= static_cast<uint16>((_command.length() + 1) > data ? data : (_command.length() + 1));
                    index.Reset(0);

                    // First option needs to be the application start name
                    while ((data > 2) && (index.Next() == true)) {
                        const string& value(*index);

                        commandCount++;
                        *indexer++ = destination;
                        *destination++ = '-';
                        data -= 1;

                        ::strncpy(destination, index.Key().c_str(), data);
                        destination = &destination[index.Key().length() + 1];
                        data -= static_cast<uint16>((index.Key().length() + 1) > data ? data : (index.Key().length() + 1));

                        if ((data > 0) && (value.empty() != true)) {
                            commandCount++;
                            *indexer++ = destination;
                            ::strncpy(destination, value.c_str(), data);
                            destination = &destination[value.length() + 1];
                            data -= static_cast<uint16>((value.length() + 1) > data ? data : (value.length() + 1));
                        }
                    }

                    *indexer = nullptr;
                }

                return (commandCount);
            }

        private:
            const string _command;
            Map _options;
        };

    private:
        Process(const Process&) = delete;
        Process& operator=(const Process&) = delete;

    public:
        Process(const bool capture)
            : _argc(0)
            , _parameters(nullptr)
            , _exitCode(static_cast<uint32>(~0))
#ifndef __WIN32__
            , _stdin(capture ? -1 : 0)
            , _stdout(capture ? -1 : 0)
            , _stderr(capture ? -1 : 0)
            , _PID(0)
#else
            , _stdin(capture ? reinterpret_cast<HANDLE>(~0) : nullptr)
            , _stdout(capture ? reinterpret_cast<HANDLE>(~0) : nullptr)
            , _stderr(capture ? reinterpret_cast<HANDLE>(~0) : nullptr)
#endif
        {
#ifdef __WIN32__
            ::memset(&_info, 0, sizeof(_info));
#endif
        }
        ~Process()
        {
#ifdef __WIN32__
            if (_info.hProcess != 0) {
                //// Close process and thread handles.
                CloseHandle(_info.hProcess);
                CloseHandle(_info.hThread);
            }
#endif

            if (_parameters != nullptr) {
                ::free(_parameters);
            }
        }

    public:
        inline uint32 Id() const
        {
#ifdef __WIN32__
            return (_info.dwProcessId);
#else
            return (_PID);
#endif
        }

        inline bool IsActive() const
        {
#ifdef __WIN32__
            DWORD exitCode = 0;
            if ((_info.hProcess != 0) && (_exitCode != static_cast<uint32>(~0)) && (GetExitCodeProcess(_info.hProcess, &exitCode) != 0) && (exitCode == STILL_ACTIVE)) {
                return (true);
            }
            else {
                _exitCode = static_cast<uint32>(exitCode);
            }
#else
            if ((_PID == 0) || (_exitCode != static_cast<uint32>(~0))) {
                return (false);
            }
            else if (::kill(_PID, 0) == 0) {
                int status;
                int result = waitpid(_PID, &status, WNOHANG);

                if (result == static_cast<int>(_PID)) {
                    if (WIFEXITED(status) == true) {
                        _exitCode = (WEXITSTATUS(status) & 0xFF);
                    }
                    else {
                        // Turn on highest bit to signal a SIGKILL was received.
                        _exitCode = 0x80000000;
                    }
                }

                return (_exitCode == static_cast<uint32>(~0));
            }
#endif

            return (false);
        }
        inline bool HasConnector() const
        {
#ifdef __WIN32__
            return ((_stdin != reinterpret_cast<HANDLE>(~0)) && (_stdin != nullptr));
#else
            return ((_stdin != -1) && (_stdin != 0));
#endif
        }
        inline HANDLE Input() const
        {
            return (_stdin);
        }
        inline HANDLE Output() const
        {
            return (_stdout);
        }
        inline HANDLE Error() const
        {
            return (_stderr);
        }
        inline bool IsKilled() const
        {
            return ((_exitCode != static_cast<uint32>(~0)) && ((_exitCode & 0x80000000) != 0));
        }
        inline bool IsTerminated() const
        {
            return ((_exitCode != static_cast<uint32>(~0)) && ((_exitCode & 0x80000000) == 0));
        }
#ifdef __WIN32__
        inline uint16 Input(const uint8 data[], const uint16 length)
        {

            DWORD bytesWritten;

            BOOL success = WriteFile(_stdin, data, length, &bytesWritten, NULL);

            return (success ? static_cast<uint16>(bytesWritten) : 0);
        }
        inline uint16 Output(uint8 data[], const uint16 length) const
        {

            DWORD bytesRead;

            BOOL success = ReadFile(_stdout, data, length, &bytesRead, NULL);

            return (success ? static_cast<uint16>(bytesRead) : 0);
        }
        inline uint16 Error(uint8 data[], const uint16 length) const
        {

            DWORD bytesRead;

            BOOL success = ReadFile(_stdout, data, length, &bytesRead, NULL);

            return (success ? static_cast<uint16>(bytesRead) : 0);
        }
#else
        inline uint16 Input(const uint8 data[], const uint16 length)
        {
            return (static_cast<uint16>(write(_stdin, data, length)));
        }
        inline uint16 Output(uint8 data[], const uint16 length) const
        {
            return (static_cast<uint16>(read(_stdout, data, length)));
        }
        inline uint16 Error(uint8 data[], const uint16 length) const
        {
            return (static_cast<uint16>(read(_stderr, data, length)));
        }
#endif
        uint32 Launch(const Process::Options& parameters, uint32* pid)
        {
            uint32 error = Core::ERROR_NONE;

            if (IsActive() == false) {
                if (_parameters != nullptr) {
                    // Destruct what we got, we sare starting again!!!
                    ::free(_parameters);
                }

#ifdef __WIN32__
                STARTUPINFO si;
                ZeroMemory(&si, sizeof(si));
                si.cb = sizeof(si);
                ZeroMemory(&_info, sizeof(_info));
                BOOL inheritance = FALSE;

                HANDLE stdinfd[2], stdoutfd[2], stderrfd[2];

                stdinfd[0] = nullptr;
                stdinfd[1] = nullptr;
                stdoutfd[0] = nullptr;
                stdoutfd[1] = nullptr;
                stderrfd[0] = nullptr;
                stderrfd[1] = nullptr;

                /* Create the pipe and set non-blocking on the readable end. */
                if (_stdin == reinterpret_cast<HANDLE>(~0)) {
                    SECURITY_ATTRIBUTES saAttr;

                    // Set the bInheritHandle flag so pipe handles are inherited.
                    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
                    saAttr.bInheritHandle = TRUE;
                    saAttr.lpSecurityDescriptor = NULL;

                    // Create a pipes for the child process.
                    if ((CreatePipe(&stdoutfd[0], &stdoutfd[1], &saAttr, 0) == 0) && (CreatePipe(&stderrfd[0], &stderrfd[1], &saAttr, 0) == 0) && (CreatePipe(&stdinfd[0], &stdinfd[1], &saAttr, 0) == 0) &&
                        // Ensure the write/read handle to the pipe for STDIN/STDOUT,STDERR are not inherited.
                        (SetHandleInformation(stdoutfd[0], HANDLE_FLAG_INHERIT, 0) == 0) && (SetHandleInformation(stderrfd[0], HANDLE_FLAG_INHERIT, 0) == 0) && (SetHandleInformation(stdinfd[1], HANDLE_FLAG_INHERIT, 0) == 0)) {

                        si.hStdError = stderrfd[1];
                        si.hStdOutput = stdoutfd[1];
                        si.hStdInput = stdinfd[0];
                        _stdout = stdoutfd[0];
                        _stderr = stderrfd[0];
                        _stdin = stdinfd[1];
                        si.dwFlags |= STARTF_USESTDHANDLES;
                        inheritance = TRUE;
                    }
                    else {
                        _stdin = 0;
                    }
                }

                uint16 size = parameters.LineSize();
                _parameters = ::malloc(size);
                _argc = parameters.Line(_parameters, size);

                if (!CreateProcess(parameters.Command().c_str(), reinterpret_cast<char*>(_parameters), NULL, NULL, inheritance, CREATE_NEW_CONSOLE, NULL, NULL, &si, &_info)) {
                    int status = GetLastError();
                    TRACE_L1("Failed to start a process, Error <%d>.", status);
                    error = (status == 2 ? Core::ERROR_UNAVAILABLE : Core::ERROR_GENERAL);
                }
				else {
					*pid = _info.dwProcessId;
				}

                if (_stdin == 0) {
                    //* check descriptors if they need to be closed
                    if (stdinfd[0] != nullptr) {
                        CloseHandle(stdinfd[0]);
                        CloseHandle(stdinfd[1]);
                    }
                    if (stdoutfd[0] != nullptr) {
                        CloseHandle(stdoutfd[0]);
                        CloseHandle(stdoutfd[1]);
                    }
                    if (stderrfd[0] != nullptr) {
                        CloseHandle(stderrfd[0]);
                        CloseHandle(stderrfd[1]);
                    }
                }
#endif

#ifdef __LINUX__
                uint16 size = parameters.BlockSize();
                _parameters = ::malloc(size);
                _argc = parameters.Block(_parameters, size);
                int stdinfd[2], stdoutfd[2], stderrfd[2];

                stdinfd[0] = -1;
                stdinfd[1] = -1;
                stdoutfd[0] = -1;
                stdoutfd[1] = -1;
                stderrfd[0] = -1;
                stderrfd[1] = -1;

                /* Create the pipe and set non-blocking on the readable end. */
                if ((_stdin == -1) && (pipe(stdinfd) == 0) && (pipe(stdoutfd) == 0) && (pipe(stderrfd) == 0)) {
                    // int flags = ( fcntl(p[0], F_GETFL, 0) & (~O_NONBLOCK) );
                    int input = (fcntl(stdinfd[1], F_GETFL, 0) | O_NONBLOCK);
                    int output = (fcntl(stdoutfd[0], F_GETFL, 0) | O_NONBLOCK);

                    if ((fcntl(stdinfd[1], F_SETFL, input) != 0) || (fcntl(stdoutfd[0], F_SETFL, output) != 0) || (fcntl(stderrfd[0], F_SETFL, output) != 0)) {
                        _stdin = 0;
                        _stdout = 0;
                        _stderr = 0;
                    }
                }

                if ((*pid = fork()) == 0) {
                    char** parameters = reinterpret_cast<char**>(_parameters);
                    if (_stdin == -1) {
                        /* Close STDIN and STDOUT as we will redirect them. */
                        close(0);
                        close(1);
                        close(2);

                        /* Close master end of pipe */
                        close(stdinfd[1]);
                        close(stdoutfd[0]);
                        close(stderrfd[0]);

                        /* Make stdin into a readable end */
                        dup2(stdinfd[0], 0);

                        /* Make stdout into writable end */
                        dup2(stdoutfd[1], 1);

                        /* Make stdout into writable end */
                        dup2(stderrfd[1], 2);
                    }
                    /* fork a child process           */
                    if (execvp(*parameters, parameters) < 0) {
                        // TRACE_L1("Failed to start process: %s.", explain_execvp(*parameters, parameters));
                        TRACE_L1("Failed to start process: %d.", errno);
                    }
                }
                else if (static_cast<int>(_PID) != -1) {
                    /* Parent process... */
                    if (_stdin == -1) {
                        close(stdinfd[0]);
                        _stdin = stdinfd[1];
                    }
                    if (_stdout == -1) {
                        close(stdoutfd[1]);
                        _stdout = stdoutfd[0];
                    }
                    if (_stderr == -1) {
                        close(stderrfd[1]);
                        _stderr = stderrfd[0];
                    }
                }
				_PID = *pid;
                if (_stdin == 0) {
                    //* check descriptors if they need to be closed
                    if (stdinfd[0] != -1) {
                        close(stdinfd[0]);
                        close(stdinfd[1]);
                    }
                    if (stdoutfd[0] != -1) {
                        close(stdoutfd[0]);
                        close(stdoutfd[1]);
                    }
                    if (stderrfd[0] != -1) {
                        close(stderrfd[0]);
                        close(stderrfd[1]);
                    }
                }
#endif
            }

            return (error);
        }

        void Kill(const bool hardKill)
        {
#ifdef __WIN32__
            if (hardKill == true) {
                TerminateProcess(_info.hProcess, 1234);
            }
#else
            ::kill(_PID, (hardKill ? SIGKILL : SIGTERM));
#endif
        }

        uint32 WaitProcessCompleted(const uint32 waitTime)
        {
#ifdef __WIN32__
            if (WaitForSingleObject(_info.hProcess, waitTime) == 0) {
                return (Core::ERROR_NONE);
            }
            return (Core::ERROR_TIMEDOUT);
#else
            uint32 timeLeft(waitTime);
            while ((IsActive() == true) && (timeLeft > 0)) {
                if (timeLeft == Core::infinite) {
                    SleepMs(500);
                }
                else {
                    uint32 sleepTime = (timeLeft > 500 ? 500 : timeLeft);

                    SleepMs(sleepTime);

                    timeLeft -= sleepTime;
                }
            }

            return (Core::ERROR_TIMEDOUT);
#endif
        }

        inline uint32 ExitCode() const
        {
            return (_exitCode);
        }

        Process(uint32 pid)
           : _argc(0)
           , _parameters(nullptr)
           , _exitCode(static_cast<uint32>(~0))
#ifndef __WIN32__
           , _stdin(0)
           , _stdout(0)
           , _stderr(0)
           , _PID(pid)
#else
           , _stdin(nullptr)
           , _stdout(nullptr)
           , _stderr(nullptr)
#endif
        {
#ifdef __WIN32__
            ::memset(&_info, 0, sizeof(_info));
#endif
        }

    private:
        uint16 _argc;
        void* _parameters;
        mutable uint32 _exitCode;
#ifdef __WIN32__
        HANDLE _stdin;
        HANDLE _stdout;
        HANDLE _stderr;
        PROCESS_INFORMATION _info;
#else
        int _stdin;
        int _stdout;
        int _stderr;
        uint32 _PID;
#endif
    };
}
}

#endif // __PROCESS_H__