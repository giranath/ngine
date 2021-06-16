#ifndef NGINE_RENDERING_RENDERING_CONTEXT_HPP
#define NGINE_RENDERING_RENDERING_CONTEXT_HPP

#include "gpu_buffer.hpp"
#include "gpu_shader.hpp"
#include "gpu_program.hpp"
#include "gpu_texture2d.hpp"
#include "gpu_texture_filtering.hpp"
#include "gpu_texture_wrapping.hpp"
#include "gpu_pixel_formats.hpp"
#include "gpu_vertex_layout.hpp"
#include "gpu_vertex_attribute.hpp"

#include <string_view>

namespace ng
{

/**
 * Abstraction over the GPU
 */
class gpu_context
{
public:
    /**
     * Load GPU functions from driver
     * @note At the moment, will load OpenGL 4.5 core profile functions
     */
    static void load_driver();

    /**
     * Compile a shader from source
     * @param shader_type The type of the shader
     * @param source The source code to compile
     * @return the compiled shader
     */
    [[nodiscard]] gpu_shader compile_shader(gpu_shader_types shader_type, std::string_view source) const;

    /**
     * Link multiple shaders together to create a shader program
     * @param shaders A pointer to the first shader to link
     * @param size The number of shaders to attach starting from shaders pointer
     * @return the linked program
     */
    [[nodiscard]] gpu_program link_shaders(const gpu_shader* shaders, std::size_t size) const;

    /**
     * Allocate a static block of memory that won't be changed
     * @param size The size to allocate on the GPU
     * @param data The initial data to copy into the buffer
     * @return the allocated buffer
     * @note You won't be able to modify the content of the allocated buffer
     */
    [[nodiscard]] gpu_buffer allocate_static(std::size_t size, const void* data) const;

    /**
     * Allocate a block of memory that will only be used by the GPU
     * @param size The number of bytes to allocate
     * @return the allocated buffer
     * @note As a client, you won't be able to write nor read from this buffer
     */
    [[nodiscard]] gpu_buffer allocate_gpu(std::size_t size) const;

    /**
     * Allocate a block of memory that can be read from the client
     * @param size The size of the buffer
     * @return The allocated buffer
     * @note As a client, you can map this buffer to read data from it. It allows you to communicate with the GPU
     */
    [[nodiscard]] gpu_buffer allocate_read(std::size_t size) const;

    /**
     * Allocate a block memory that can be written from the client
     * @param size The size of the buffer
     * @return The allocated buffer
     * @note As a client, you can map this buffer to write data to it. It allows you to communicate with the GPU
     */
    [[nodiscard]] gpu_buffer allocate_write(std::size_t size) const;

    /**
     * Make a new vertex layout object
     * @return The created vertex layout
     */
    [[nodiscard]] gpu_vertex_layout make_vertex_layout(const gpu_vertex_attribute* attributes, std::size_t size) const;

    /**
     * Make a new texture2d
     * @param definition The definition of the texture
     * @param width The width of the texture
     * @param height The height of the texture
     * @param pixel_format The format of the pixels in pixels
     * @param pixels A pointer to the pixels to store into the texture
     * @return The created texture
     */
    [[nodiscard]] gpu_texture2d make_texture2d(const gpu_texture2d_definition& definition,
                                               int width, int height,
                                               gpu_source_pixel_formats pixel_format, const void* pixels);
};

}

#endif
