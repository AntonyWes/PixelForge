#include "../../include/PixelForge/core/fb_context.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <cstring>
#include <stdexcept>

namespace PixelForge {
    FBContext::FBContext(const std::string& device_path)
    : device_path_(device_path) {}

    FBContext::~FBContext() {
        release();
    }
    bool FBContext::init() {
        fbfd_ = open(device_path_.c_str(), O_RDWR);
        if (fbfd_ == -1) return false;

        struct fb_var_screeninfo var_info;
        if (ioctl(fbfd_, FBIOGET_VSCREENINFO, &var_info) == -1) {
            close(fbfd_);
            return false;
        }

        struct fb_fix_screeninfo fix_info;
        if (ioctl(fbfd_, FBIOGET_FSCREENINFO, &fix_info) == -1) {
            close(fbfd_);
            return false;
        }

        width_ = var_info.xres;
        height_ = var_info.yres;
        bpp_ = var_info.bits_per_pixel;
        pitch_ = fix_info.line_length;
        size_t buffer_size = pitch_ * height_;
        buffer_ = static_cast<uint8_t*>(
            mmap(nullptr, buffer_size, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd_, 0)
        );

        if (buffer_ == MAP_FAILED) {
            close(fbfd_);
            return false;
        }

        return true;
    }
    bool FBContext::initFromBuffer(void* buffer, uint32_t width, uint32_t height, uint32_t pitch, uint32_t bpp) {
        buffer_ = static_cast<uint8_t*>(buffer);
        width_ = width;
        height_ = height;
        pitch_ = pitch;
        bpp_ = bpp;
        return true;
    }
    void FBContext::release() {
        if (buffer_ != nullptr) {
            size_t buffer_size = pitch_ * height_;
            munmap(buffer_, buffer_size);
            buffer_ = nullptr;
        }

        if (fbfd_ != -1) {
            close(fbfd_);
            fbfd_ = -1;
        }
    }
    uint8_t* FBContext::getBuffer() const {
        return buffer_;
    }

    void FBContext::flush() {
        if (buffer_ != nullptr) {
            size_t buffer_size = pitch_ * height_;
            msync(buffer_, buffer_size, MS_SYNC);
        }
    }
    uint32_t FBContext::width() const {
        return width_;
    }

    uint32_t FBContext::height() const {
        return height_;
    }

    uint32_t FBContext::bpp() const {
        return bpp_;
    }

    uint32_t FBContext::pitch() const {
        return pitch_;
    }

}
