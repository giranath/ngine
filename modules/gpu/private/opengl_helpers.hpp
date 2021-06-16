#ifndef NGINE_GPU_OPENGL_HELPERS_HPP
#define NGINE_GPU_OPENGL_HELPERS_HPP

#include "gpu_shader.hpp"
#include "gpu_vertex_attribute.hpp"
#include "gpu_pixel_formats.hpp"
#include "gpu_texture_filtering.hpp"
#include "gpu_texture_wrapping.hpp"

#include <cstdint>
#include <cassert>

#include <glad/glad.h>

namespace ng::gl
{

/**
 * Converts a gpu shader types to it's opengl enum equivalent
 * @param type The type of the shader
 * @return The equivalent OpenGL enum value for type
 */
constexpr GLenum shader_type(gpu_shader_types type) noexcept
{
    switch(type)
    {
    case gpu_shader_types::vertex:
        return GL_VERTEX_SHADER;
    case gpu_shader_types::fragment:
        return GL_FRAGMENT_SHADER;
    case gpu_shader_types::geometry:
        return GL_GEOMETRY_SHADER;
    case gpu_shader_types::compute:
        return GL_COMPUTE_SHADER;
    default:
        assert(false);
        return GL_VERTEX_SHADER;
    }
}

/**
 * Returns the OpenGL equivalent of a boolean value
 * @param value The boolean value to convert into OpenGL
 * @return the converted boolean value
 */
constexpr GLboolean boolean_value(bool value) noexcept
{
    return value ? GL_TRUE : GL_FALSE;
}

/**
 * Returns the OpenGL equivalent of an attribute
 * @param type The type of the attribute
 * @return the opengl equivalent enum value
 */
constexpr GLenum attribute_type(gpu_vertex_attribute_types type) noexcept
{
    switch(type)
    {
    case gpu_vertex_attribute_types::float1:
    case gpu_vertex_attribute_types::float2:
    case gpu_vertex_attribute_types::float3:
    case gpu_vertex_attribute_types::float4:
        return GL_FLOAT;
    case gpu_vertex_attribute_types::int1:
    case gpu_vertex_attribute_types::int2:
    case gpu_vertex_attribute_types::int3:
    case gpu_vertex_attribute_types::int4:
        return GL_INT;
    default:
        assert(false);
        return GL_FLOAT;
    }
}

/**
 * Returns the number of component a specific attribute type has
 * @param type The type of the attribute
 * @return the number of components in the type
 */
constexpr GLint attribute_type_size(gpu_vertex_attribute_types type) noexcept
{
    switch(type)
    {
    case gpu_vertex_attribute_types::float1:
    case gpu_vertex_attribute_types::int1:
        return 1;
    case gpu_vertex_attribute_types::float2:
    case gpu_vertex_attribute_types::int2:
        return 2;
    case gpu_vertex_attribute_types::float3:
    case gpu_vertex_attribute_types::int3:
        return 3;
    case gpu_vertex_attribute_types::float4:
    case gpu_vertex_attribute_types::int4:
        return 4;
    default:
        assert(false);
        return 0;
    }
}

constexpr GLuint attribute_type_offset(gpu_vertex_attribute_types type) noexcept
{
    switch(type)
    {
    case gpu_vertex_attribute_types::float1:
        return sizeof(float);
    case gpu_vertex_attribute_types::float2:
        return sizeof(float) * 2;
    case gpu_vertex_attribute_types::float3:
        return sizeof(float) * 3;
    case gpu_vertex_attribute_types::float4:
        return sizeof(float) * 4;
    case gpu_vertex_attribute_types::int1:
        return sizeof(int);
    case gpu_vertex_attribute_types::int2:
        return sizeof(int) * 2;
    case gpu_vertex_attribute_types::int3:
        return sizeof(int) * 3;
    case gpu_vertex_attribute_types::int4:
        return sizeof(int) * 4;
    default:
        assert(false);
        return 0;
    }
}

static constexpr GLenum image_pixel_format(gpu_source_pixel_formats format)
{
    switch(format)
    {
    case gpu_source_pixel_formats::r8:
        return GL_RED;
    case gpu_source_pixel_formats::r8a8:
        return GL_RG;
    case gpu_source_pixel_formats::r8g8b8:
        return GL_RGB;
    case gpu_source_pixel_formats::r8g8b8a8:
        return GL_RGBA;
    default:
        assert(false);
        return GL_RGB;
    }
}

static constexpr GLenum texture_pixel_format(gpu_pixel_formats format)
{
    switch(format)
    {
    case gpu_pixel_formats::r8g8b8:
        return GL_RGB8;
    case gpu_pixel_formats::r8g8b8a8:
        return GL_RGBA8;
    case gpu_pixel_formats::depth:
        return GL_DEPTH_COMPONENT24;
    case gpu_pixel_formats::depth_stencil:
        return GL_DEPTH24_STENCIL8;
    default:
        return GL_RGB8;
    }
}

static constexpr GLenum texture_wrapping_mode(gpu_texture_wrapping mode)
{
    switch(mode)
    {
    case gpu_texture_wrapping::repeat:
        return GL_REPEAT;
    case gpu_texture_wrapping::mirrored:
        return GL_MIRRORED_REPEAT;
    case gpu_texture_wrapping::clamp:
        return GL_CLAMP_TO_EDGE;
    default:
        assert(false);
        return GL_REPEAT;
    }
}

static constexpr GLenum texture_min_filtering_mode(gpu_texture_filtering mode)
{
    switch(mode)
    {
    case gpu_texture_filtering::point:
        return GL_NEAREST;
    case gpu_texture_filtering::bilinear:
        return GL_LINEAR;
    case gpu_texture_filtering::trilinear:
        return GL_LINEAR_MIPMAP_LINEAR;
    default:
        assert(false);
        return GL_NEAREST;
    }
}

static constexpr GLenum texture_mag_filtering_mode(gpu_texture_filtering mode)
{
    switch(mode)
    {
    case gpu_texture_filtering::point:
        return GL_NEAREST;
    case gpu_texture_filtering::bilinear:
        return GL_LINEAR;
    case gpu_texture_filtering::trilinear:
        return GL_LINEAR;
    default:
        assert(false);
        return GL_NEAREST;
    }
}

}

#endif
