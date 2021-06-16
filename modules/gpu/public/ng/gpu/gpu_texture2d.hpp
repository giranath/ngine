#ifndef NGINE_RENDERING_GPU_TEXTURE_2D_HPP
#define NGINE_RENDERING_GPU_TEXTURE_2D_HPP

#include "gpu_texture_filtering.hpp"
#include "gpu_texture_wrapping.hpp"
#include "gpu_pixel_formats.hpp"

#include <cstdint>

namespace ng
{

/**
 * Definition of a texture
 */
struct gpu_texture2d_definition
{
    gpu_texture_wrapping horizontal_wrapping;
    gpu_texture_wrapping vertical_wrapping;
    gpu_texture_filtering filtering;
    gpu_pixel_formats format;
    bool generate_mipmaps;

    constexpr gpu_texture2d_definition() noexcept
    : horizontal_wrapping{gpu_texture_wrapping::repeat}
    , vertical_wrapping{gpu_texture_wrapping::repeat}
    , filtering{gpu_texture_filtering::point}
    , format{gpu_pixel_formats::r8g8b8}
    , generate_mipmaps{true}
    {

    }

    constexpr gpu_texture2d_definition(gpu_texture_wrapping h_wrapping, gpu_texture_wrapping v_wrapping, gpu_texture_filtering filtering, gpu_pixel_formats format, bool generate_mipmaps) noexcept
    : horizontal_wrapping(h_wrapping)
    , vertical_wrapping(v_wrapping)
    , filtering(filtering)
    , format(format)
    , generate_mipmaps(generate_mipmaps)
    {

    }
};

/**
 * RAII class on top of a GPU texture2D resource
 */
class gpu_texture2d
{
    friend class gpu_context;

    uint32_t texture_;

    /**
     * Handles an OpenGL texture handle
     * @param texture The texture object to handle with this instance
     */
    explicit gpu_texture2d(uint32_t texture) noexcept;

public:
    // Copy is disabled
    gpu_texture2d(const gpu_texture2d&) = delete;
    gpu_texture2d& operator=(const gpu_texture2d&) = delete;

    gpu_texture2d() noexcept;

    ~gpu_texture2d() noexcept;
    gpu_texture2d(gpu_texture2d&& other) noexcept;
    gpu_texture2d& operator=(gpu_texture2d&& other) noexcept;

    /**
     * Swap this texture with another one
     * @param other The other texture to swap this one with
     */
    void swap(gpu_texture2d& other) noexcept;

    /**
     * Reset the texture object handled by this object
     * @param texture The new texture object this instance should handle
     * @note If this class was already handling a texture, it will be correctly destroyed
     */
    void reset(uint32_t texture = 0) noexcept;

    /**
     * Check if this texture can be bound
     * @return true when the texture can be bound or false otherwise
     */
    [[nodiscard]] bool bindable() const noexcept;
};

/**
 * Swap a texture with another
 * @param a The first texture to swap with b
 * @param b The second texture to swap with a
 */
void swap(gpu_texture2d& a, gpu_texture2d& b) noexcept;

}

#endif
