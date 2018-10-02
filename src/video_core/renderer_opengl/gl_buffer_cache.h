// Copyright 2018 yuzu Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <cstddef>
#include <memory>
#include <tuple>

#include "common/common_types.h"
#include "video_core/rasterizer_cache.h"
#include "video_core/renderer_opengl/gl_resource_manager.h"
#include "video_core/renderer_opengl/gl_stream_buffer.h"

namespace OpenGL {

struct CachedBufferEntry final {
    VAddr GetAddr() const {
        return addr;
    }

    std::size_t GetSizeInBytes() const {
        return size;
    }

    VAddr addr;
    std::size_t size;
    GLintptr offset;
    std::size_t alignment;
};

class OGLBufferCache final : public RasterizerCache<std::shared_ptr<CachedBufferEntry>> {
public:
    explicit OGLBufferCache(std::size_t size);

    /// Uploads data from a guest GPU address. Returns host's buffer offset where it's been
    /// allocated.
    GLintptr UploadMemory(Tegra::GPUVAddr gpu_addr, std::size_t size, std::size_t alignment = 4,
                          bool cache = true);

    /// Uploads from a host memory. Returns host's buffer offset where it's been allocated.
    GLintptr UploadHostMemory(const void* raw_pointer, std::size_t size, std::size_t alignment = 4);

    /// Reserves memory to be used by host's CPU. Returns mapped address and offset.
    std::tuple<u8*, GLintptr> ReserveMemory(std::size_t size, std::size_t alignment = 4);

    void Map(std::size_t max_size);
    void Unmap();

    GLuint GetHandle() const;

protected:
    void AlignBuffer(std::size_t alignment);

private:
    OGLStreamBuffer stream_buffer;

    u8* buffer_ptr = nullptr;
    GLintptr buffer_offset = 0;
    GLintptr buffer_offset_base = 0;
};

} // namespace OpenGL
