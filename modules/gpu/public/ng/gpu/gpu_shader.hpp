#ifndef NGINE_GPU_SHADER_HPP
#define NGINE_GPU_SHADER_HPP

#include <cstdint>

namespace ng
{

/**
 * Enumerates all supported shader types
 */
enum class gpu_shader_types : uint8_t
{
    vertex,
    fragment,
    geometry,
    compute
};

/**
 * RAII class wrapping a compiled shader
 * @note Wraps a compiled shader, you must compile shader before sending it to this wrapper
 */
class gpu_shader
{
    friend class gpu_context;

    uint32_t shader_;

    explicit gpu_shader(uint32_t shader) noexcept;
public:
    gpu_shader(const gpu_shader&) = delete;
    gpu_shader& operator=(const gpu_shader&) = delete;

    gpu_shader() noexcept;
    ~gpu_shader() noexcept;

    gpu_shader(gpu_shader&& other) noexcept;
    gpu_shader& operator=(gpu_shader&& other) noexcept;

    void swap(gpu_shader& other) noexcept;
    void reset(uint32_t new_shader = 0) noexcept;

    /**
     * Check if shader can be attached to a program
     * @return true when can be attached or false otherwise
     */
    [[nodiscard]] bool attachable() const noexcept;
};

void swap(gpu_shader& a, gpu_shader& b) noexcept;

}

#endif
