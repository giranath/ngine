#include "gpu_program.hpp"

#include <algorithm>
#include <utility>

#include <glad/glad.h>

namespace ng
{


gpu_program::gpu_program() noexcept
: program_(0)
{

}

gpu_program::gpu_program(uint32_t program) noexcept
: program_(program)
{

}

gpu_program::~gpu_program() noexcept
{
    if(program_)
    {
        glDeleteProgram(program_);
    }
}

gpu_program::gpu_program(gpu_program&& other) noexcept
: program_(std::exchange(other.program_, 0))
{

}

gpu_program& gpu_program::operator=(gpu_program&& other) noexcept
{
    if(this != &other)
    {
        if(program_)
        {
            glDeleteProgram(program_);
        }

        program_ = std::exchange(other.program_, 0);
    }

    return *this;
}

void gpu_program::swap(gpu_program& other) noexcept
{
    std::swap(program_, other.program_);
}

void gpu_program::reset(uint32_t new_program) noexcept
{
    if(program_)
    {
        glDeleteProgram(program_);
    }

    program_ = new_program;
}

bool gpu_program::bindable() const noexcept
{
    return program_ != 0;
}

}