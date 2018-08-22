#include "NexusServer.h"

#include <nexus_config.h>
#include <nexus_types.h>
#include <nexus_platform.h>
#include <nxclient.h>
#include <nxserverlib.h>
/* #include <nxserverlib_impl.h> */
#include <nexus_display_vbi.h>

/* print_capabilities */
#if NEXUS_HAS_VIDEO_DECODER
#include <nexus_video_decoder.h>
#endif
#include <nexus_display.h>
#include <sys/stat.h>


BDBG_MODULE(NexusServer);

namespace WPEFramework {
namespace Broadcom {
/*
    static unsigned int GetSurfaceId(const nxclient_t client) {
        int rc, clientID;
        NxClient_AllocSettings allocSettings;
        NxClient_AllocResults allocResults;

        NxClient_GetDefaultAllocSettings(&allocSettings);
        allocSettings.surfaceClient = 1;
        rc = NxClient_Alloc(&allocSettings, &allocResults);
        if (rc) {
            printf("Couldnt allocate \n");
            return(0);
        }

        return (allocResults.surfaceClient[0].id);
    }
*/

    /* virtual */ string Platform::Client::Name() const
    {
        return (::std::string(Id()));
    }

    /* virtual */ void Platform::Client::Kill()
    {
        ASSERT(_client != nullptr);

        /* must call nxserver_ipc so it can unwind first. */
        nxserver_ipc_close_client(_client);

        TRACE(Trace::Information, (_T("Kill client %s."), Name().c_str()));

        /* We expect the Disconnect Client to be triggered by the previous call
           TODO: Double check this is true, and if not call it explicitly here. */
    }

    /* virtual */ void Platform::Client::Opacity(const uint32_t value)
    {
        ASSERT(_client != nullptr);

        TRACE(Trace::Information, (_T("Alpha client %s to %d."), Name().c_str(), value));
        nxserverlib_set_server_alpha(_client, value);
    }

    /* virtual */ void Platform::Client::Geometry(const uint32_t X, const uint32_t Y, const uint32_t width, const uint32_t height)
    {
        ASSERT(_client != nullptr);

        TRACE(Trace::Information, (_T("Geometry client %s size=%dx%d position=%dx%d"), X,Y,width,height));

        NEXUS_SurfaceComposition comp;
#if 0
        unsigned int surfaceId = GetSurfaceId(_client);
        // NxClient_GetSurfaceClientComposition(clientID, &comp);
        NxClient_P_GetSurfaceClientComposition(_client, surfaceId , &comp);
        printf("Getting info for %d: %d,%d,%d,%d.\n", surfaceId, comp.position.x, comp.position.y, comp.position.width, comp.position.height);
        comp.virtualDisplay.width  = 0;
        comp.virtualDisplay.height = 0;
        comp.position.width  = width;
        comp.position.height = height;
        comp.position.x = X;
        comp.position.y = Y;
        NxClient_P_SetSurfaceClientComposition(_client, surfaceId, &comp);
        // NxClient_P_Config_SetSurfaceClientComposition(_client, nullptr, surfaceHandle, &comp);
        TRACE(Trace::Information, (_T("Geometry enter3")));
#endif

    }

    /* virtual */ void Platform::Client::Visible(const bool visible)
    {
        // TODO
    }

    /* virtual */ void Platform::Client::SetTop()
    {
        ASSERT(_client != nullptr);

        /* the definition of "focus" is variable. this is one impl. */
        NxClient_ConnectList list;
        struct nxclient_status status;

        nxclient_get_status(_client, &status);
        NxClient_P_Config_GetConnectList(_client, status.handle, &list);

        /* only refresh first connect */
    }

    /* virtual */ void Platform::Client::SetInput() {
    }

    class Config : public Core::JSON::Container {
    private:
        Config(const Config&) = delete;
        Config& operator=(const Config&) = delete;

    public:
        enum svptype {
            NONE = nxserverlib_svp_type_none,
            VIDEO = nxserverlib_svp_type_cdb,   /* Force CRR for video CDB */
            ALL = nxserverlib_svp_type_cdb_urr  /* Force CRR for video CDB + default to all secure only buffers */
        };

        class MemoryInfo : public Core::JSON::Container {
        private:
            MemoryInfo(const MemoryInfo&) = delete;
            MemoryInfo& operator= (const MemoryInfo&) = delete;

        public:
            MemoryInfo ()
                : GFX()
                , GFX2()
                , Restricted()
                , Main()
                , Export()
                , Client()
                , SecureGFX() {
                Add(_T("gfx"), &GFX);
                Add(_T("gfx2"), &GFX2);
                Add(_T("restricted"), &Restricted);
                Add(_T("main"), &Main);
                Add(_T("export"), &Export);
                Add(_T("client"), &Client);
                Add(_T("secureGFX"), &SecureGFX);
            }
            ~MemoryInfo() {
            }

        public:
            Core::JSON::DecUInt16 GFX;
            Core::JSON::DecUInt16 GFX2;
            Core::JSON::DecUInt16 Restricted;
            Core::JSON::DecUInt16 Main;
            Core::JSON::DecUInt16 Export;
            Core::JSON::DecUInt16 Client;
            Core::JSON::DecUInt16 SecureGFX;
        };

    public:
        Config()
            : Core::JSON::Container()
            , IRMode(NEXUS_IrInputMode_eCirNec)
            , Authentication(true)
            , BoxMode(~0)
            , SagePath()
            , SVPType(NONE)
            , Memory() {
            Add(_T("irmode"), &IRMode);
            Add(_T("authentication"), &Authentication);
            Add(_T("boxmode"), &BoxMode);
            Add(_T("sagepath"), &SagePath);
            Add(_T("svp"), &SVPType);
            Add(_T("memory"), &Memory);
        }
        ~Config()
        {
        }

    public:
        Core::JSON::DecUInt16 IRMode;
        Core::JSON::Boolean Authentication;
        Core::JSON::DecUInt8 BoxMode;
        Core::JSON::String SagePath;
        Core::JSON::EnumType<svptype> SVPType;
        MemoryInfo Memory;
    };


    /* -------------------------------------------------------------------------------------------------------------
     * This is a singleton. Declare all C accessors to this object here
     * ------------------------------------------------------------------------------------------------------------- */
    /* static */ Platform* Platform::_implementation = nullptr;

    static int find_unused_heap(const NEXUS_PlatformSettings& platformSettings)
    {
        for (int i=NEXUS_MAX_HEAPS-1;i<NEXUS_MAX_HEAPS;i--) {
            if (!platformSettings.heap[i].size && !platformSettings.heap[i].heapType) return i;
        }
        return -1;
    }
    /* static */ void Platform::CloseDown()
    {
        // Seems we are destructed.....If we still have a pointer to the implementation, Kill it..
        if (_implementation != nullptr) {
            delete _implementation;
            _implementation = nullptr;
        }
    }

    /* static */ int Platform::ClientConnect(nxclient_t client, const NxClient_JoinSettings* pJoinSettings,
        NEXUS_ClientSettings* pClientSettings)
    {
        BSTD_UNUSED(pClientSettings);

        if (_implementation != nullptr) {

            /* server app has opportunity to reject client altogether, or modify pClientSettings */
            _implementation->Add(client, pJoinSettings);
        }
        return (0);
    }

    /* static */ void Platform::ClientDisconnect(nxclient_t client, const NxClient_JoinSettings* pJoinSettings)
    {
        BSTD_UNUSED(pJoinSettings);

        if (_implementation != nullptr) {

            _implementation->Remove(pJoinSettings->name);
        }
    }

    Platform::~Platform()
    {
        _state = DEINITIALIZING;

        nxserver_ipc_uninit();
        nxserverlib_uninit(_instance);
        BKNI_DestroyMutex(_lock);
        NEXUS_Platform_Uninit();

        NxClient_Uninit();
    }

    Platform::Platform(IStateChange* stateChanges, IClient* clientChanges, const std::string& configuration)
        : _lock()
        , _instance()
        , _serverSettings()
        , _platformSettings()
        , _platformCapabilities()
        , _joinSettings()
        , _state(UNITIALIZED)
        , _clientHandler(clientChanges)
        , _stateHandler(stateChanges)
    {
        NEXUS_Error rc;

        ASSERT(_implementation == nullptr);
        ASSERT(_instance == nullptr);

        _implementation = this;

        // Strange someone already started a NXServer.
        if (_instance == nullptr) {

            // Register an @Exit, in case we are killed, with an incorrect ref count !!
            if (atexit(CloseDown) != 0) {
                TRACE(Trace::Information, (("Could not register @exit handler. Error: %d."), errno));
                exit(EXIT_FAILURE);
            }

            TRACE_L1("Start Nexus server...%d\n", __LINE__);
            Config config; config.FromString(configuration);

            if ((config.SagePath.IsSet() == true) && (config.SagePath.Value().empty() == false)) {
                ::setenv("SAGEBIN_PATH", config.SagePath.Value().c_str(), 1);
            }

            if (config.BoxMode.IsSet()) {
                // Set box mode using env var.
                char stringNumber[10];
                snprintf(stringNumber, sizeof(stringNumber), "%d", config.BoxMode.Value());
                ::setenv("B_REFSW_BOXMODE", stringNumber, 1);
                TRACE_L1("Set BoxMode to %d\n", config.BoxMode.Value());
            }

            NxClient_GetDefaultJoinSettings(&(_joinSettings));
            strcpy(_joinSettings.name, "NexusServerLocal");

            nxserver_get_default_settings(&(_serverSettings));
            NEXUS_Platform_GetDefaultSettings(&(_platformSettings));
            NEXUS_GetDefaultMemoryConfigurationSettings(&(_serverSettings.memConfigSettings));
            NEXUS_GetPlatformCapabilities(&(_platformCapabilities));

            if (config.Memory.IsSet()) {
                const Config::MemoryInfo& memory (config.Memory);

                if (memory.GFX.IsSet()) {
                    int index = nxserver_heap_by_type(&_platformSettings, NEXUS_HEAP_TYPE_GRAPHICS);
                    if (index != -1) {
                        TRACE_L1("Setting gfx heap[%d] to %dMB", index, memory.GFX.Value());
                        _platformSettings.heap[index].size = (memory.GFX.Value() * 1024 * 1024);
                    }
                }
                if (memory.GFX2.IsSet()) {
                    int index = nxserver_heap_by_type(&_platformSettings, NEXUS_HEAP_TYPE_SECONDARY_GRAPHICS);
                    if (index != -1) {
                        TRACE_L1("Setting gfx2 heap[%d] to %dMB", index, memory.GFX2.Value());
                        _platformSettings.heap[index].size = (memory.GFX2.Value() * 1024 * 1024);
                    }
                }
                if (memory.Restricted.IsSet()) {
                    int index = nxserver_heap_by_type(&_platformSettings, NEXUS_HEAP_TYPE_COMPRESSED_RESTRICTED_REGION);
                    if (index != -1) {
                        TRACE_L1("Setting secure video heap[%d] to %dMB", index, memory.Restricted.Value());
                        _platformSettings.heap[index].size = (memory.Restricted.Value() * 1024 * 1024);
                    }
                }
                if (memory.Main.IsSet()) {
                    int index = nxserver_heap_by_type(&_platformSettings, NEXUS_HEAP_TYPE_MAIN);
                    if (index != -1) {
                        TRACE_L1("Setting main heap[%d] to %dMB", index, memory.Main.Value());
                        _platformSettings.heap[index].size = (memory.Main.Value() * 1024 * 1024);
                    }
                }
                if (memory.Export.IsSet()) {
                    int index = nxserver_heap_by_type(&_platformSettings, NEXUS_HEAP_TYPE_EXPORT_REGION);
                    if (index != -1) {
                        TRACE_L1("Setting export heap[%d] to %dMB", index, memory.Export.Value());
                        _platformSettings.heap[index].size = (memory.Export.Value() * 1024 * 1024);
                    }
                }
                if (memory.Client.IsSet()) {
                    /* create a dedicated heap for the client */
                    int mainIndex = nxserver_heap_by_type(&_platformSettings, NEXUS_HEAP_TYPE_GRAPHICS);
                    if (mainIndex != -1) {
                        uint32_t unused = 0;
                        int unused_heap = find_unused_heap(_platformSettings);
                        if (unused_heap != -1) {
                            TRACE_L1("Setting client heap[%d] to %dMB", unused_heap, memory.Client.Value());
                            _serverSettings.heaps.clientFullHeap = unused_heap;
                            _platformSettings.heap[_serverSettings.heaps.clientFullHeap].size = (memory.Client.Value() * 1024 * 1024);
                            _platformSettings.heap[_serverSettings.heaps.clientFullHeap].memcIndex = _platformSettings.heap[mainIndex].memcIndex;
                            _platformSettings.heap[_serverSettings.heaps.clientFullHeap].subIndex = _platformSettings.heap[mainIndex].subIndex;
                            _platformSettings.heap[_serverSettings.heaps.clientFullHeap].guardBanding = false; /* corruptions shouldn't cause server failures */
                            _platformSettings.heap[_serverSettings.heaps.clientFullHeap].alignment = _platformSettings.heap[mainIndex].alignment;
                            _platformSettings.heap[_serverSettings.heaps.clientFullHeap].memoryType = NEXUS_MemoryType_eFull; /* requires for packet blit and playpump */
#if NEXUS_HAS_SAGE
                            /* sage must be told which heap the client's will be using */
                            _platformSettings.sageModuleSettings.clientHeapIndex = _serverSettings.heaps.clientFullHeap;
#endif
                        }
                    }
                }
                if (memory.SecureGFX.IsSet()) {
                    NEXUS_PlatformConfigCapabilities cap;
                    NEXUS_GetPlatformConfigCapabilities(&_platformSettings, &(_serverSettings.memConfigSettings), &cap);
                    if (cap.secureGraphics[0].valid) {
                        TRACE_L1("creating securegfx heap[%d] on MEMC%u for %dMB", cap.secureGraphics[0].heapIndex, cap.secureGraphics[0].memcIndex, memory.SecureGFX.Value());
                        _platformSettings.heap[cap.secureGraphics[0].heapIndex].size = (memory.SecureGFX.Value() * 1024 * 1024);
                        _platformSettings.heap[cap.secureGraphics[0].heapIndex].memcIndex = cap.secureGraphics[0].memcIndex;
                        _platformSettings.heap[cap.secureGraphics[0].heapIndex].heapType = NEXUS_HEAP_TYPE_SECURE_GRAPHICS;
                        _platformSettings.heap[cap.secureGraphics[0].heapIndex].memoryType = NEXUS_MEMORY_TYPE_ONDEMAND_MAPPED|NEXUS_MEMORY_TYPE_MANAGED|NEXUS_MEMORY_TYPE_SECURE;
                        /* TODO: allow securegfx to GFD1. for now, apply "-sd off" */
                        _serverSettings.session[0].output.sd = false;
                        _serverSettings.session[0].output.hd = true;
                        _serverSettings.session[0].output.encode = false;
                    }
                }
            }

            if (config.SVPType.IsSet() == true) {
                _serverSettings.svp = static_cast<nxserverlib_svp_type>(config.SVPType.Value());
            }
	    
            /* display[1] will always be either SD or transcode */
            if (!_platformCapabilities.display[1].supported || _platformCapabilities.display[1].encoder) {
                for (unsigned int i = 0; i < NXCLIENT_MAX_SESSIONS; i++)
                    _serverSettings.session[i].output.sd = false;
            }
            /* display[0] will always be either HD, or system is headless */
            if (!_platformCapabilities.display[0].supported || _platformCapabilities.display[0].encoder) {
                for (unsigned int i = 0; i < NXCLIENT_MAX_SESSIONS; i++)
                    _serverSettings.session[i].output.hd = false;
            }

#if NEXUS_HAS_IR_INPUT
            for (unsigned int i = 0; i < NXCLIENT_MAX_SESSIONS; i++)
#if NEXUS_PLATFORM_VERSION_MAJOR < 16 || (NEXUS_PLATFORM_VERSION_MAJOR == 16 && NEXUS_PLATFORM_VERSION_MINOR < 3)
                _serverSettings.session[i].ir_input_mode = static_cast<NEXUS_IrInputMode>(config.IRMode.Value());
#else
                for (unsigned int irInputIndex = 0; i < NXSERVER_IR_INPUTS; i++) {
                    _serverSettings.session[i].ir_input.mode[irInputIndex] = static_cast<NEXUS_IrInputMode>(config.IRMode.Value());
                    _serverSettings.session[i].keypad = true;
                }
#endif // NEXUS_PLATFORM_VERSION_MAJOR < 17
#endif // NEXUS_HAS_IR_INPUT

            struct nxserver_cmdline_settings cmdline_settings;
            memset(&cmdline_settings, 0, sizeof(cmdline_settings));
            cmdline_settings.frontend = true;
            cmdline_settings.loudnessMode = NEXUS_AudioLoudnessEquivalenceMode_eNone;
            _serverSettings.session[0].dolbyMs = nxserverlib_dolby_ms_type_none;

            rc = nxserver_modify_platform_settings(&_serverSettings,
                &cmdline_settings,
                &_platformSettings,
                &_serverSettings.memConfigSettings);
            if (rc == NEXUS_SUCCESS) {
                rc = NEXUS_Platform_MemConfigInit(&_platformSettings, &_serverSettings.memConfigSettings);

                if (rc == NEXUS_SUCCESS) {
                    BKNI_CreateMutex(&_lock);
                    _serverSettings.lock = _lock;

                    nxserver_set_client_heaps(&_serverSettings, &_platformSettings);

                    _serverSettings.client.connect = Platform::ClientConnect;
                    _serverSettings.client.disconnect = Platform::ClientDisconnect;

#if NEXUS_SERVER_HAS_EXTENDED_INIT
                    _instance = nxserverlib_init_extended(&_serverSettings, config.Authentication.Value());
#else
                    _instance = nxserverlib_init(&_serverSettings);
#endif

                    if (_instance != nullptr) {
                        rc = nxserver_ipc_init(_instance, _lock);

                        if (rc == NEXUS_SUCCESS) {
                            TRACE_L1("Created NexusServer[%p].\n", &_instance);
                        }
                        else {
                            TRACE_L1("nxserver_ipc_init failed [%d]\n", rc);
                        }
                    }
                    else {
                        TRACE_L1("nxserverlib_init_extended failed [%d]\n", rc);
                    }
                }
                else {
                    TRACE_L1("NEXUS_Platform_MemConfigInit failed [%d]\n", rc);
                }
            }
            else {
                TRACE_L1("nxserver_modify_platform_settings failed [%d]\n", rc);
            }
        }

        StateChange(rc == NEXUS_SUCCESS ? OPERATIONAL : FAILURE);

        ASSERT(_state != FAILURE);
    }

    // -------------------------------------------------------------------------------------------------------
    //   private methods
    // -------------------------------------------------------------------------------------------------------
    void Platform::Add(nxclient_t client, const NxClient_JoinSettings* joinSettings)
    {
        TRACE_L1("Nexus client %s connected... \n", joinSettings->name);

        if (_clientHandler != nullptr) {
            Exchange::IComposition::IClient* entry(Platform::Client::Create(client, joinSettings));
            _clientHandler->Attached(entry);
            entry->Release();
        }
    }

    void Platform::Remove(const char clientName[])
    {
        TRACE_L1("Nexus client %s disconnected... \n", clientName);

        if (_clientHandler != nullptr) {
            _clientHandler->Detached(clientName);
        }
    }

    void Platform::StateChange(server_state state)
    {
        TRACE_L1("Nexus server state change [%d -> %d] \n", _state, state);

        _state = state;

        if (_stateHandler != nullptr) {
            _stateHandler->StateChange(_state);
        }
    };

} // Broadcom

ENUM_CONVERSION_BEGIN(Broadcom::Config::svptype)

	{ Broadcom::Config::NONE,  _TXT("None")  },
	{ Broadcom::Config::VIDEO, _TXT("Video") },
	{ Broadcom::Config::ALL,   _TXT("All")   },

ENUM_CONVERSION_END(Broadcom::Config::svptype);

} // oPEFramework
