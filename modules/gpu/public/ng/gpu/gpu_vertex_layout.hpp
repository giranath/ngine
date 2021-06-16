#ifndef NGINE_GPU_VERTEX_LAYOUT_HPP
#define NGINE_GPU_VERTEX_LAYOUT_HPP

#include <cstdint>

namespace ng
{

/**
 * RAII class on top of the representation of the layout a vertex buffer has
 */
class gpu_vertex_layout
{
    friend class gpu_context;

    uint32_t vao_;

    explicit gpu_vertex_layout(uint32_t vao) noexcept;
public:
    gpu_vertex_layout(const gpu_vertex_layout&) = delete;
    gpu_vertex_layout& operator=(const gpu_vertex_layout&) = delete;

    gpu_vertex_layout() noexcept;
    ~gpu_vertex_layout();
    gpu_vertex_layout(gpu_vertex_layout&& other) noexcept;
    gpu_vertex_layout& operator=(gpu_vertex_layout&& other) noexcept;

    void reset(uint32_t new_vao = 0) noexcept;
    void swap(gpu_vertex_layout& other) noexcept;

    [[nodiscard]] bool bindable() const noexcept;
};

void swap(gpu_vertex_layout& a, gpu_vertex_layout& b) noexcept;

}

#endif
