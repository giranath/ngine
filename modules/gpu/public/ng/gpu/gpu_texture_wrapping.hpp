#ifndef NGINE_GPU_TEXTURE_WRAPPING_HPP
#define NGINE_GPU_TEXTURE_WRAPPING_HPP

#include <cstdint>

namespace ng
{

/**
 * Enumerates every supported texture wrapping modes
 */
enum class gpu_texture_wrapping : uint8_t
{
    /**
     * The texture will repeat itself
     */
    repeat,

    /**
     * The texture will repeat itself with a mirrored effect
     */
    mirrored,

    /**
     * The texture will not repeat itself, a uv over the limit of the texture will be clamped back to the texture
     */
    clamp
};

}

#endif
