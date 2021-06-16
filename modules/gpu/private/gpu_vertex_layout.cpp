#include "gpu_vertex_layout.hpp"

#include <algorithm>
#include <utility>
#include <glad/glad.h>

namespace ng
{

gpu_vertex_layout::gpu_vertex_layout() noexcept
: vao_{0}
{

}

gpu_vertex_layout::gpu_vertex_layout(uint32_t vao) noexcept
: vao_{vao}
{

}

gpu_vertex_layout::~gpu_vertex_layout()
{
    if(vao_)
    {
        glDeleteVertexArrays(1, &vao_);
    }
}

gpu_vertex_layout::gpu_vertex_layout(gpu_vertex_layout&& other) noexcept
: vao_(std::exchange(other.vao_, 0))
{

}

gpu_vertex_layout& gpu_vertex_layout::operator=(gpu_vertex_layout&& other) noexcept
{
    if(&other != this)
    {
        if(vao_)
        {
            glDeleteVertexArrays(1, &vao_);
        }

        vao_ = std::exchange(other.vao_, 0);
    }

    return *this;
}

void gpu_vertex_layout::reset(uint32_t new_vao) noexcept
{
    if(vao_)
    {
        glDeleteVertexArrays(1, &vao_);
    }

    vao_ = new_vao;
}

void gpu_vertex_layout::swap(gpu_vertex_layout& other) noexcept
{
    std::swap(vao_, other.vao_);
}

bool gpu_vertex_layout::bindable() const noexcept
{
    return vao_ != 0;
}

}