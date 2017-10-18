#ifndef __SNAPSHOT_H
#define __SNAPSHOT_H

#include "Module.h"

namespace WPEFramework {
namespace Plugin {

    class Snapshot : public PluginHost::IPlugin, public PluginHost::IWeb {
    private:
        Snapshot(const Snapshot&) = delete;
        Snapshot& operator=(const Snapshot&) = delete;

        class FileBodyExtended : public Web::FileBody {
        private:
            FileBodyExtended() = delete;
            FileBodyExtended(const FileBodyExtended&) = delete;
            FileBodyExtended& operator=(const FileBodyExtended&) = delete;

        public:
            FileBodyExtended(Core::BinairySemaphore *semLock, const string& path)
                    : Web::FileBody(path, false)
                    , _semLock(*semLock)
            {
                // Already exist, due to unexpected termination!
                if (true == Core::File::Exists())
                {
                    // Close and remove file
                    Core::File::Destroy();
                }

                Core::File::Create();
            }

            virtual ~FileBodyExtended()
            {
                Core::File::Destroy();

                // Signal, It is ready for new capture
                _semLock.Unlock();
            }

        public:
            static Core::ProxyType<FileBodyExtended> Instance(Core::BinairySemaphore& semLock, const string& fileName)
            {
                Core::ProxyType<FileBodyExtended> result;

                if (semLock.Lock(0) == Core::ERROR_NONE)
                {
                    // We got the lock, forward it to the filebody
                    result = Core::ProxyType<FileBodyExtended>::Create(&semLock, fileName);
                }

                return (result);
            }

        private:
            Core::BinairySemaphore& _semLock;
        };

        class Dispmanx {
        private:
            Dispmanx(const Dispmanx&) = delete;
            Dispmanx& operator=(const Dispmanx&) = delete;

        public:
            //! C'tors.
            Dispmanx();
            ~Dispmanx();

        public:
            //! Fullscreen snapshot.
            int FullscreenSnapshot(Core::File& file);

        private:
            //! Handle to image-buffer.
            uint8* image_;

            int SavePNGImage(uint8_t* image, size_t width, size_t height, Core::File& file);
        };

    public:
        Snapshot()
            : _skipURL(0)
            , _inProgress(false)
        {
        }
        virtual ~Snapshot()
        {
        }

        BEGIN_INTERFACE_MAP(Snapshot)
            INTERFACE_ENTRY(PluginHost::IPlugin)
            INTERFACE_ENTRY(PluginHost::IWeb)
        END_INTERFACE_MAP

    public:
        //   IPlugin methods
        // -------------------------------------------------------------------------------------------------------
        virtual const string Initialize(PluginHost::IShell* service);
        virtual void Deinitialize(PluginHost::IShell* service);
        virtual string Information() const;

        //	IWeb methods
        // -------------------------------------------------------------------------------------------------------
        virtual void Inbound(Web::Request& request);
        virtual Core::ProxyType<Web::Response> Process(const Web::Request& request);

    private:
        uint8 _skipURL;
        Core::BinairySemaphore _inProgress;

        //! video.
        static Dispmanx _video;
    };

} // Namespace Plugin.
}

#endif // __SNAPSHOT_H
