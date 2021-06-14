#ifndef NG_CORE_HASH_HPP
#define NG_CORE_HASH_HPP

#include <cstdint>
#include <iterator>
#include <type_traits>
#include <string_view>

namespace ng
{

/**
 * Hash a range of bytes
 * @tparam It The iterator on the range of byte
 * @param begin The first iterator on the range
 * @param end The iterator after the last on the range
 * @return The hash value of the range
 * @note FNV-1a implementation
 */
template<typename It>
constexpr uint64_t hash(It begin, It end) noexcept
{
    using iterator_value_type = typename std::iterator_traits<It>::value_type;
    static_assert(std::is_convertible_v<iterator_value_type, uint8_t>, "Iterator must be on bytes");

    constexpr const uint64_t fnv_offset_basis = 0xcbf29ce484222325;
    constexpr const uint64_t fnv_prime = 0x00000100000001B3;

    uint64_t h = fnv_offset_basis;

    for (; begin != end; ++begin)
    {
        h ^= static_cast<uint8_t>(*begin);
        h *= fnv_prime;
    }

    return h;
}

/**
 * Hash a string
 * @param str The string to hash
 * @return The hash value of the string
 */
constexpr uint64_t hash(std::string_view str) noexcept
{
    return hash(str.begin(), str.end());
}

/**
 * Hash a memory block
 * @param memory The memory block to hash
 * @param size The size of the memory block
 * @return The hash value for this memory block
 */
uint64_t hash(const void* memory, std::size_t size) noexcept;

namespace literals
{

/**
 * String literals to create compile time hash
 * @param str The string to hash
 * @param length The length of the string to hash
 * @return The hashed value of the string
 */
constexpr uint64_t operator ""_h(const char* str, std::size_t length)
{
    return hash(std::string_view{str, length});
}

}

}

#endif
