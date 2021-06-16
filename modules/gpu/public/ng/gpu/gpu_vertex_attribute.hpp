#ifndef NGINE_GPU_VERTEX_ATTRIBUTE_HPP
#define NGINE_GPU_VERTEX_ATTRIBUTE_HPP

#include <cstdint>

namespace ng
{

/**
 * Enumerates all supported vertex attribute types
 */
enum class gpu_vertex_attribute_types : uint8_t
{
    /// A float scalar
    float1,

    /// A vec2 of floats
    float2,

    /// A vec3 of floats
    float3,

    /// A vec4 of floats
    float4,

    /// A integer scalar
    int1,

    /// A vec2 of integers
    int2,

    /// A vec3 of integers
    int3,

    /// A vec4 of integers
    int4
};

/**
 * Represents a single vertex attribute
 */
struct gpu_vertex_attribute
{
    /**
     * Is the attribute normalized
     */
    bool normalized;

    /**
     * The type of the attribute
     */
    gpu_vertex_attribute_types type;
};

}

#endif
