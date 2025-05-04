#pragma once
#include <string>
#include <cstdint>
#include <linux/fb.h>

namespace PixelForge{
    class FBContext {
        public:
            FBContext(const std::string& device_path = "/dev/fb0");
            ~FBContext();

            bool init(); // Gets resolution, color depth etc...
            void release();
            bool initFromBuffer(void*, uint32_t, uint32_t, uint32_t, uint32_t);

            uint8_t* getBuffer() const; // Framebuffer pointer
            void flush(); // Sync

            uint32_t width() const;
            uint32_t height() const;
            uint32_t bpp() const; // Bits per pixel
            uint32_t pitch() const; // Byte per string
        private:
            int fbfd_ = -1;               // File descrypter
            uint8_t* buffer_ = nullptr;   // Buffer pointer
            uint32_t width_ = 0;          // Screen width
            uint32_t height_ = 0;         // Screen height
            uint32_t bpp_ = 0;            // Bits per pixel
            uint32_t pitch_ = 0;          // Byte per string
            std::string device_path_;     // Path to framebuffer device

            FBContext(const FBContext&) = delete;
            FBContext& operator=(const FBContext&) = delete;
    };
}
