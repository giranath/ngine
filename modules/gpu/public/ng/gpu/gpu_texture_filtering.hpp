#ifndef NGINE_GPU_TEXTURE_FILTERING_HPP
#define NGINE_GPU_TEXTURE_FILTERING_HPP

#include <cstdint>

namespace ng
{

/**
 * Enumerates all supported texture filtering modes
 */
enum class gpu_texture_filtering : uint8_t
{
    /**
     * No filtering, the texture will look pixelated when near of it
     */
    point,

    /**
     * Linear filtering, linear interpolation of texel will be performed to render pixel
     */
    bilinear,

    /**
     * Linear filtering across mipmaps, same a linear but interpolation between mipmaps will also be performed
     */
    trilinear
};

}

#endif
