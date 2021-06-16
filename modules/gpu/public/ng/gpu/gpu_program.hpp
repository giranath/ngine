#ifndef NGINE_GPU_PROGRAM_HPP
#define NGINE_GPU_PROGRAM_HPP

#include <cstdint>

namespace ng
{

/**
 * RAII class wrapping a shader program
 */
class gpu_program
{
    friend class gpu_context;

    uint32_t program_;

    explicit gpu_program(uint32_t program) noexcept;
public:
    gpu_program(const gpu_program&) = delete;
    gpu_program& operator=(const gpu_program&) = delete;

    gpu_program() noexcept;
    ~gpu_program() noexcept;

    gpu_program(gpu_program&& other) noexcept;
    gpu_program& operator=(gpu_program&& other) noexcept;

    void swap(gpu_program& other) noexcept;
    void reset(uint32_t new_program = 0) noexcept;

    [[nodiscard]] bool bindable() const noexcept;
};

void swap(gpu_program& a, gpu_program& b) noexcept;

}

#endif
