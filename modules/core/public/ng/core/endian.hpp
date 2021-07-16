#ifndef NGINE_CORE_ENDIAN_HPP
#define NGINE_CORE_ENDIAN_HPP

#include <cstdint>
#include <type_traits>

namespace ng
{

namespace internal
{

template<typename T, std::size_t... N>
constexpr T swap_bytes_impl(T i, std::index_sequence<N...>) noexcept
{
    return ((((i >> (N * CHAR_BIT)) & (T)(unsigned char)(-1)) <<
            ((sizeof(T) - 1 - N) * CHAR_BIT)) | ...);
};

}

/**
 * Swap bytes inside an unsigned integer
 * @tparam T The unsigned integer type to swap bytes
 * @tparam U The resulting unsigned integer
 * @param value The value to swap bytes
 * @return The value with swapped bytes
 */
template<class T, class U = typename std::make_unsigned_t<T>>
constexpr U swap_bytes(T value) noexcept
{
    return internal::swap_bytes_impl<U>(value, std::make_index_sequence<sizeof(T)>{});
}

// Convert value to big endianness
//======================================================================================================================
static constexpr bool is_little_endian = true;

/**
 * Convert an integer to its big endian representation on this system
 * @tparam T The type of the value to convert
 * @param value The value to convert
 * @return the converted value
 * @note On a big endian host, this function does nothing
 */
template<typename T>
constexpr auto big_endian_cast(T value) noexcept
{
    if constexpr(is_little_endian)
    {
        return swap_bytes(value);
    }
    else
    {
        return value;
    }
}

/**
 * Convert an integer to its little endian representation on this system
 * @tparam T The type of the value to convert
 * @param value The value to convert
 * @return the converted value
 * @note On a little endian host, this function does nothing
 */
template<typename T>
constexpr auto little_endian_cast(T value) noexcept
{
    if constexpr(!is_little_endian)
    {
        return swap_bytes(value);
    }
    else
    {
        return value;
    }
}

/**
 * Convert an unsigned integer to its network representation
 * @tparam T The type of the unsigned integer to convert
 * @param value The value to convert
 * @return the converted value
 * @note equivalent of big_endian_cast
 */
template<typename T>
constexpr auto network_endian_cast(T value) noexcept
{
    return big_endian_cast(value);
}

}

#endif
