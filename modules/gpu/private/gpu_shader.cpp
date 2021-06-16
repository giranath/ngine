#include "gpu_shader.hpp"

#include <algorithm>
#include <utility>

#include <glad/glad.h>

namespace ng
{

gpu_shader::gpu_shader() noexcept
: shader_(0)
{

}

gpu_shader::gpu_shader(uint32_t shader) noexcept
: shader_(shader)
{

}

gpu_shader::~gpu_shader() noexcept
{
    if(shader_ != 0)
    {
        glDeleteShader(shader_);
    }
}

gpu_shader::gpu_shader(gpu_shader&& other) noexcept
: shader_(std::exchange(other.shader_, 0))
{

}

gpu_shader& gpu_shader::operator=(gpu_shader&& other) noexcept
{
    if(this != &other)
    {
        if(shader_ != 0)
        {
            glDeleteShader(shader_);
        }

        shader_ = std::exchange(other.shader_, 0);
    }

    return *this;
}

void gpu_shader::swap(gpu_shader& other) noexcept
{
    std::swap(shader_, other.shader_);
}

void gpu_shader::reset(uint32_t new_shader) noexcept
{
    if(shader_ != 0)
    {
        glDeleteShader(shader_);
    }

    shader_ = new_shader;
}

bool gpu_shader::attachable() const noexcept
{
    return shader_ != 0;
}

void swap(gpu_shader& a, gpu_shader& b) noexcept
{
    a.swap(b);
}

}