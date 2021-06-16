#ifndef NGINE_GPU_BUFFER_HPP
#define NGINE_GPU_BUFFER_HPP

#include <cstdint>

namespace ng
{

/**
 * RAII wrapper on top of a GPU memory buffer
 */
class gpu_buffer
{
    friend class gpu_context;

    uint32_t buffer_;

    explicit gpu_buffer(uint32_t buffer) noexcept;
public:
    // Copy is disabled
    gpu_buffer(const gpu_buffer&) = delete;
    gpu_buffer& operator=(const gpu_buffer&) = delete;

    gpu_buffer() noexcept;
    ~gpu_buffer() noexcept;
    gpu_buffer(gpu_buffer&& other) noexcept;
    gpu_buffer& operator=(gpu_buffer&& other) noexcept;

    void swap(gpu_buffer& other) noexcept;
    void reset(uint32_t new_buffer = 0) noexcept;

    [[nodiscard]] bool bindable() const noexcept;
};

/**
 * Swap two buffers together
 * @param a The first buffer to swap with b
 * @param b The second buffer to swap with a
 */
void swap(gpu_buffer& a, gpu_buffer& b) noexcept;

}

#endif
