#include "gpu_buffer.hpp"
#include <glad/glad.h>
#include <utility>

namespace ng
{

gpu_buffer::gpu_buffer() noexcept
: buffer_{0}
{

}

gpu_buffer::gpu_buffer(uint32_t buffer) noexcept
: buffer_{buffer}
{

}

gpu_buffer::~gpu_buffer() noexcept
{
    if(buffer_ != 0)
    {
        glDeleteBuffers(1, &buffer_);
    }
}

gpu_buffer::gpu_buffer(gpu_buffer&& other) noexcept
: buffer_{std::exchange(other.buffer_, 0)}
{

}

gpu_buffer& gpu_buffer::operator=(gpu_buffer&& other) noexcept
{
    if(this != &other)
    {
        if(buffer_ != 0)
        {
            glDeleteBuffers(1, &buffer_);
        }

        buffer_ = std::exchange(other.buffer_, 0);
    }

    return *this;
}

void gpu_buffer::swap(gpu_buffer& other) noexcept
{
    std::swap(buffer_, other.buffer_);
}

void gpu_buffer::reset(uint32_t new_buffer) noexcept
{
    if(buffer_ != 0)
    {
        glDeleteBuffers(1, &buffer_);
    }

    buffer_ = new_buffer;
}

bool gpu_buffer::bindable() const noexcept
{
    return buffer_ != 0;
}

void swap(gpu_buffer& a, gpu_buffer& b) noexcept
{
    a.swap(b);
}

}