#ifndef NGINE_GPU_PIXEL_FORMATS_HPP
#define NGINE_GPU_PIXEL_FORMATS_HPP

#include <cstdint>

namespace ng
{

enum class gpu_source_pixel_formats : uint8_t
{
    r8,
    r8a8,
    r8g8b8,
    r8g8b8a8
};

/**
 * Enumerates all supported texture pixel formats
 */
enum class gpu_pixel_formats : uint8_t
{
    /// RGBA (8 bits/component)
    r8g8b8a8,

    /// RGB (8 bits/component)
    r8g8b8,

    /// Depth texture
    depth,

    /// Depth + stencil texture
    depth_stencil,
};


}

#endif
