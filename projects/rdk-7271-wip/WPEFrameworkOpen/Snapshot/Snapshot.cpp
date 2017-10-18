#include <png.h>

#include <bcm_host.h>

#include "Snapshot.h"

namespace WPEFramework {
namespace Plugin {

    const char _snapshotName[] = "/tmp/snapshot.png";

    /* static */ Snapshot::Dispmanx Snapshot::_video;

    Snapshot::Dispmanx::Dispmanx()
    {
        // Init GPU resources.
        bcm_host_init();
    }

    Snapshot::Dispmanx::~Dispmanx()
    {
    }
 
    int Snapshot::Dispmanx::FullscreenSnapshot(Core::File& file)
    {
        DISPMANX_DISPLAY_HANDLE_T display;
        DISPMANX_MODEINFO_T info;
        DISPMANX_RESOURCE_HANDLE_T resource;
        VC_IMAGE_TYPE_T type = VC_IMAGE_RGB888;
        DISPMANX_TRANSFORM_T transform = static_cast<DISPMANX_TRANSFORM_T>(0);
        VC_RECT_T rect;
        uint32_t vc_image_ptr;
        int status = 0;

        display = vc_dispmanx_display_open(0);

        status = vc_dispmanx_display_get_info(display, &info);
        assert(status == 0);

        image_ = new uint8[info.width * 3 * info.height];
        assert(image_ == nullptr);

        resource = vc_dispmanx_resource_create(type,
            info.width,
            info.height,
            &vc_image_ptr);

        vc_dispmanx_snapshot(display, resource, transform);

        status = vc_dispmanx_rect_set(&rect, 0, 0, info.width, info.height);
        assert(status == 0);

        status = vc_dispmanx_resource_read_data(resource, &rect, image_, info.width * 3);
        assert(status == 0);

        status = SavePNGImage(image_, info.width, info.height, file);
        assert(status == 0);

        status = vc_dispmanx_resource_delete(resource);
        assert(status == 0);

        status = vc_dispmanx_display_close(display);
        assert(status == 0);

        delete[] image_;

        return status;
    }

    int Snapshot::Dispmanx::SavePNGImage(uint8_t* image, size_t width, size_t height, Core::File& file)
    {
        png_structp png_ptr = nullptr;
        png_infop info_ptr = nullptr;
        size_t x, y;
        png_byte** row_pointers = nullptr;
        int pixel_size = 3; // RGB
        int depth = 8;
        int status = -1;

        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (png_ptr == nullptr) {

            return status;
        }

        info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == nullptr) {
            png_destroy_write_struct(&png_ptr, &info_ptr);

            return status;
        }

        // Set up error handling.
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);

            return status;
        }

        // Set image attributes.
        png_set_IHDR(png_ptr,
            info_ptr,
            width,
            height,
            depth,
            PNG_COLOR_TYPE_RGB,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT);

        // Initialize rows of PNG.
        row_pointers = static_cast<png_byte**>(png_malloc(png_ptr, height * sizeof(png_byte*)));
        for (y = 0; y < height; ++y) {
            png_byte* row = static_cast<png_byte*>(png_malloc(png_ptr, sizeof(uint8_t) * width * pixel_size));
            row_pointers[y] = row;
            for (x = 0; x < width; ++x) {
                uint8_t* pixel = image + 3 * width * y + 3 * x;
                *row++ = pixel[0];
                *row++ = pixel[1];
                *row++ = pixel[2];
            }
        }

        // Duplicate file descriptor and create File stream based on it.
        FILE *filePointer = static_cast<FILE*>(file);
        if (nullptr != filePointer) {

            // Write the image data to "file".
            png_init_io(png_ptr, filePointer);
            png_set_rows(png_ptr, info_ptr, row_pointers);
            png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);
            // All went well.
            status = 0;
        }
        // Close stream to flush and release allocated buffers
        fclose(filePointer);

        for (y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);

        png_destroy_write_struct(&png_ptr, &info_ptr);

        return status;
    }

    SERVICE_REGISTRATION(Snapshot, 1, 0);

    /* virtual */ const string Snapshot::Initialize(PluginHost::IShell* service)
    {

        // Setup skip URL for right offset.
        _skipURL = service->WebPrefix().length();

        // On succes return "".
        return _T("");
    }

    /* virtual */ void Snapshot::Deinitialize(PluginHost::IShell* service)
    {
    }

    /* virtual */ string Snapshot::Information() const
    {
        // No additional info to report.
        return (string());
    }

    /* virtual */ void Snapshot::Inbound(Web::Request& /* request */)
    {
    }

    /* virtual */ Core::ProxyType<Web::Response> Snapshot::Process(const Web::Request& request)
    {
        ASSERT(_skipURL <= request.Path.length());

        // Proxy object for response type.
        Core::ProxyType<Web::Response> response(PluginHost::Factories::Instance().Response());

        // Default is not allowed.
        response->Message = _T("Method not allowed");
        response->ErrorCode = Web::STATUS_METHOD_NOT_ALLOWED;

        // Decode request path.
        Core::TextSegmentIterator index(Core::TextFragment(request.Path, _skipURL, request.Path.length() - _skipURL), false, '/');

        // Get first plugin verb.
        index.Next();

        // Move to next segment if it exists.
        if (request.Verb == Web::Request::HTTP_GET) {

            if (false == index.Next()) {

                response->Message = _T("Plugin is up and running");
                response->ErrorCode = Web::STATUS_OK;
            }
            // Save screencapture as png-image.
            else if ( (index.Current() == "Capture") ) {

                // Create smart file object (for socket communication).
                Core::ProxyType<Plugin::Snapshot::FileBodyExtended> png_file(FileBodyExtended::Instance(_inProgress, string(_snapshotName)));

                // _inProgress event is signalled, capture screen
                if (true == png_file.IsValid() && _video.FullscreenSnapshot(*png_file) == 0)
                {
                    // Attach to response.
                    response->ContentType = Web::MIMETypes::MIME_IMAGE_PNG;
                    response->Body(png_file);
                    response->Message = _snapshotName;
                    response->ErrorCode = Web::STATUS_ACCEPTED;
                }
                else {
                    response->Message = _T("Plugin is already in progress");
                    response->ErrorCode = Web::STATUS_PRECONDITION_FAILED;
                }
            }
        }

        return (response);
    }

}
}
