#include "gpu_texture2d.hpp"
#include <algorithm>
#include <utility>

#include <glad/glad.h>

namespace ng
{

gpu_texture2d::gpu_texture2d() noexcept
: texture_(0)
{

}

gpu_texture2d::gpu_texture2d(uint32_t texture) noexcept
: texture_(texture)
{

}

gpu_texture2d::~gpu_texture2d() noexcept
{
    if(texture_ != 0)
    {
        glDeleteTextures(1, &texture_);
    }
}

gpu_texture2d::gpu_texture2d(gpu_texture2d&& other) noexcept
: texture_(std::exchange(other.texture_, 0))
{
}

gpu_texture2d& gpu_texture2d::operator=(gpu_texture2d&& other) noexcept
{
    if(this != &other)
    {
        if(texture_ != 0)
        {
            glDeleteTextures(1, &texture_);
        }

        texture_ = std::exchange(other.texture_, 0);
    }

    return *this;
}

void gpu_texture2d::swap(gpu_texture2d& other) noexcept
{
    std::swap(texture_, other.texture_);
}

void gpu_texture2d::reset(uint32_t texture) noexcept
{
    if(texture_ != 0)
    {
        glDeleteTextures(1, &texture_);
    }

    texture_ = texture;
}

bool gpu_texture2d::bindable() const noexcept
{
    return texture_ != 0;
}

void swap(gpu_texture2d& a, gpu_texture2d& b) noexcept
{
    a.swap(b);
}

}