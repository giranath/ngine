#include "hash.hpp"

namespace ng
{

uint64_t hash(const void* memory, std::size_t size) noexcept
{
    const uint8_t* byte_memory = reinterpret_cast<const uint8_t*>(memory);

    return hash(byte_memory, byte_memory + size);
}

}