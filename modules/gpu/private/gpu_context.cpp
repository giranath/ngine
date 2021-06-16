#include "gpu_context.hpp"
#include "opengl_helpers.hpp"
#include <stdexcept>

#include <glad/glad.h>

namespace ng
{

void gpu_context::load_driver()
{
    if(!gladLoadGL())
    {
        throw std::runtime_error{"failed to load OpenGL 4.5 core functions"};
    }
}

gpu_shader gpu_context::compile_shader(gpu_shader_types shader_type, std::string_view source) const
{
    GLuint shader_object = glCreateShader(gl::shader_type(shader_type));

    // Compile the shader
    const char* source_cstr = source.data();
    glShaderSource(shader_object, 1, &source_cstr, nullptr);
    glCompileShader(shader_object);

    GLint compilation_status = 0;
    glGetShaderiv(shader_object, GL_COMPILE_STATUS, &compilation_status);

    // If compilation failed
    if(compilation_status == GL_FALSE)
    {
        // Make sure to release shader object, otherwise it will leak
        glDeleteShader(shader_object);

        // Compilation log will be outputed by OpenGL debug messages
        throw std::runtime_error{"shader compilation failed"};
    }

    return gpu_shader{shader_object};
}

gpu_program gpu_context::link_shaders(const gpu_shader* shaders, std::size_t size) const
{
    GLuint program_object = glCreateProgram();

    // Attach all specified shaders to the program
    for(std::size_t i = 0; i < size; ++i)
    {
        const gpu_shader* shader = shaders + i;

        glAttachShader(program_object, shader->shader_);
    }

    glLinkProgram(program_object);

    for(std::size_t i = 0; i < size; ++i)
    {
        const gpu_shader* shader = shaders + i;

        glDetachShader(program_object, shader->shader_);
    }

    GLint link_status = 0;
    glGetProgramiv(program_object, GL_LINK_STATUS, &link_status);

    if(link_status == GL_FALSE)
    {
        glDeleteProgram(program_object);

        throw std::runtime_error{"failed to link shader program"};
    }

    return gpu_program{program_object};
}

gpu_buffer gpu_context::allocate_static(std::size_t size, const void* data) const
{
    GLuint buffer_object;
    glCreateBuffers(1, &buffer_object);
    glNamedBufferStorage(buffer_object, size, data, 0);

    // TODO: Find a way to detect that the storage could not be allocated and throw an exception

    return gpu_buffer{buffer_object};
}

gpu_buffer gpu_context::allocate_gpu(std::size_t size) const
{
    GLuint buffer_object;
    glCreateBuffers(1, &buffer_object);
    glNamedBufferStorage(buffer_object, size, nullptr, 0);

    // TODO: Find a way to detect that the storage could not be allocated and throw an exception

    return gpu_buffer{buffer_object};
}

gpu_buffer gpu_context::allocate_read(std::size_t size) const
{
    GLuint buffer_object;
    glCreateBuffers(1, &buffer_object);
    glNamedBufferStorage(buffer_object, size, nullptr, GL_MAP_READ_BIT);

    // TODO: Find a way to detect that the storage could not be allocated and throw an exception

    return gpu_buffer{buffer_object};
}

gpu_buffer gpu_context::allocate_write(std::size_t size) const
{
    GLuint buffer_object;
    glCreateBuffers(1, &buffer_object);
    glNamedBufferStorage(buffer_object, size, nullptr, GL_MAP_WRITE_BIT);

    // TODO: Find a way to detect that the storage could not be allocated and throw an exception

    return gpu_buffer{buffer_object};
}

gpu_vertex_layout gpu_context::make_vertex_layout(const gpu_vertex_attribute* attributes, std::size_t size) const
{
    GLuint vao;
    glCreateVertexArrays(1, &vao);

    GLuint offset = 0;
    for(std::size_t i = 0; i < size; ++i)
    {
        const gpu_vertex_attribute& attribute = attributes[i];

        glEnableVertexArrayAttrib(vao, static_cast<GLuint>(i));
        glVertexArrayAttribFormat(vao, static_cast<GLuint>(i),
                                  gl::attribute_type_size(attribute.type), gl::attribute_type(attribute.type),
                                  gl::boolean_value(attribute.normalized),
                                  offset);
        glVertexArrayAttribBinding(vao, 0, static_cast<GLuint>(i));

        offset += gl::attribute_type_offset(attribute.type);
    }

    // TODO: Should save stride into layout

    return gpu_vertex_layout{vao};
}

gpu_texture2d gpu_context::make_texture2d(const gpu_texture2d_definition& definition, int width, int height, gpu_source_pixel_formats pixel_format, const void* pixels)
{
    GLuint new_texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &new_texture);

    // set the texture wrapping/filtering options on the texture
    glTextureParameteri(new_texture, GL_TEXTURE_WRAP_S, gl::texture_wrapping_mode(definition.horizontal_wrapping));
    glTextureParameteri(new_texture, GL_TEXTURE_WRAP_T, gl::texture_wrapping_mode(definition.vertical_wrapping));
    glTextureParameteri(new_texture, GL_TEXTURE_MIN_FILTER, gl::texture_min_filtering_mode(definition.filtering));
    glTextureParameteri(new_texture, GL_TEXTURE_MAG_FILTER, gl::texture_mag_filtering_mode(definition.filtering));

    glTextureStorage2D(new_texture, 0, gl::texture_pixel_format(definition.format), width, height);

    // Fill texture
    glTextureSubImage2D(new_texture, 0,
                        0, 0, width, height,
                        // At the moment we only support 8bits/component pixels
                        gl::image_pixel_format(pixel_format), GL_UNSIGNED_BYTE,
                        pixels);

    if(definition.generate_mipmaps)
    {
        glGenerateTextureMipmap(new_texture);
    }

    return gpu_texture2d{new_texture};
}

}