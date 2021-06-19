#ifndef NGINE_CORE_NAME_HPP
#define NGINE_CORE_NAME_HPP

#include "hash.hpp"
#include <cstddef>
#include <string_view>
#include <string>
#include <cassert>
#include <algorithm>

namespace ng
{

struct hash_collision_safe{};
struct hash_collision_unsafe{};

template<typename T>
struct is_hash_collision_safety_hint : std::false_type {};

template<>
struct is_hash_collision_safety_hint<hash_collision_safe> : std::true_type {};

template<>
struct is_hash_collision_safety_hint<hash_collision_unsafe> : std::true_type {};

template<typename T>
constexpr bool is_hash_collision_safety_hint_v = is_hash_collision_safety_hint<T>::value;

/**
 * A name with a size limitation
 * @tparam Size The size limitation of the name
 * @tparam HashCollisionSafetyHint A hint to indicate if we want this name to be safe about hash collisions
 * Names are strings that are more efficient to compare, if you want to identify something with a string, you should
 * try to use a name because checking if two names are different is a O(1) operation instead of a O(N).
 * The internal string is only compared to another when the hashes of the two names are identical.
 * We still perform the string comparison to safely handle hash collisions but if you do not care about collision
 * you can disable it
 */
template<std::size_t Size, typename HashCollisionSafetyHint = hash_collision_safe>
class inline_name
{
    static_assert(is_hash_collision_safety_hint_v<HashCollisionSafetyHint>, "Expecting valid hash collision safety hint");

    uint64_t hash_;
    char str_[Size];

public:
    constexpr inline_name() noexcept
    : hash_{0}
    , str_{0}
    {

    }

    /**
     * Construct a name from a string
     * @param str The string to construct the name from
     */
    constexpr explicit inline_name(std::string_view str) noexcept
    : hash_{0}
    , str_{0}
    {
        assert(str.size() < Size);

        // Handle case where string is too long
        if(str.size() > Size - 1)
        {
            std::copy(str.begin(), std::next(str.begin(), Size - 1), str_);
            str_[Size - 1] = 0;
        }
        else
        {
            std::copy(str.begin(), str.end(), str_);
            str_[str.size()] = 0;
        }

        hash_ = ng::hash(str_);
    }

    /**
     * Returns the hash of the name
     * @return The hash of the name
     */
    [[nodiscard]] constexpr uint64_t hash() const noexcept
    {
        return hash_;
    }

    /**
     * Returns a pointer to the internal name
     * @return a pointer to the internal name
     * @note Pointer will be invalidated if the name moves in memory or if it is destroyed
     *       therefore you should not cache this pointer
     */
    [[nodiscard]] const char* c_str() const noexcept
    {
        return str_;
    }

    /**
     * Returns a string on the internal name
     * @return A string on the internal name
     * @note Safest way to store a name
     */
    [[nodiscard]] std::string string() const noexcept
    {
        return str_;
    }

    /**
     * Returns a string view to the name
     * @return a string view on the name
     * @note The name must be kept alive for the view to be valid and not move in memory
     */
    [[nodiscard]] std::string_view string_view() const noexcept
    {
        return str_;
    }

    /**
     * Check if this name is identical to another name
     * @tparam OtherSize The size of the other name
     * @param other The other name
     * @return true when both names are identical
     */
    template<std::size_t OtherSize, typename OtherHashCollisionSafetyHint>
    constexpr bool operator==(const inline_name<OtherSize, OtherHashCollisionSafetyHint>& other) const noexcept
    {
        // Safe about hash collisions
        if constexpr(std::is_same_v<HashCollisionSafetyHint, hash_collision_safe>)
        {
            return Size == OtherSize
                && hash() == other.hash()
                && std::strcmp(c_str(), other.c_str()) == 0;
        }
        // Unsafe about hash collisions
        else
        {
            return Size == OtherSize
                && hash() == other.hash();
        }
    }

    /**
     * Check if this name is different from another name
     * @tparam OtherSize The size of the other name
     * @param other The other name to compare this one with
     * @return true when both names are different or false otherwise
     */
    template<std::size_t OtherSize, typename OtherHashCollisionSafetyHint>
    constexpr bool operator!=(const inline_name<OtherSize, OtherHashCollisionSafetyHint>& other) const noexcept
    {
        if constexpr(std::is_same_v<HashCollisionSafetyHint, hash_collision_safe>)
        {
            return Size != OtherSize
                || hash() != other.hash()
                || std::strcmp(str_, other.c_str()) != 0;
        }
        else
        {
            return Size != OtherSize
                || hash() != other.hash();
        }
    }
};

/**
 * Name with not size limitation
 */
template<typename HashCollisionSafetyHint = hash_collision_safe>
class name
{
    std::string str_;
    uint64_t hash_;
public:
    constexpr name() noexcept
    : str_{}
    , hash_{0}
    {

    }

    constexpr explicit name(std::string_view name)
    : str_{name}
    , hash_{ng::hash(str_)}
    {

    }

    /**
     * Returns the hash of the name
     * @return The hash of the name
     */
    [[nodiscard]] constexpr uint64_t hash() const noexcept
    {
        return hash_;
    }

    /**
     * Returns a pointer to the internal name
     * @return a pointer to the internal name
     * @note Pointer will be invalidated if the name moves in memory or if it is destroyed
     *       therefore you should not cache this pointer
     */
    [[nodiscard]] const char* c_str() const noexcept
    {
        return str_.c_str();
    }

    /**
     * Returns a string on the internal name
     * @return A string on the internal name
     * @note Safest way to store a name
     */
    [[nodiscard]] const std::string& string() const noexcept
    {
        return str_;
    }

    /**
     * Returns a string view to the name
     * @return a string view on the name
     * @note The name must be kept alive for the view to be valid and not move in memory
     */
    [[nodiscard]] std::string_view string_view() const noexcept
    {
        return str_;
    }

    /**
     * Check if this name is identical to another name
     * @param other The other name
     * @return true when both names are identical
     */
    template<typename OtherHashCollisionSafetyHint>
    constexpr bool operator==(const name<OtherHashCollisionSafetyHint>& other) const noexcept
    {
        // Safe about hash collisions
        if constexpr(std::is_same_v<HashCollisionSafetyHint, hash_collision_safe>)
        {
            return string().size() == other.string().size()
                && hash() == other.hash()
                && string() == other.string();
        }
        // Unsafe about hash collisions
        else
        {
            return string().size() == other.string().size()
                && hash() == other.hash();
        }
    }

    /**
     * Check if this name is different from another name
     * @param other The other name to compare this one with
     * @return true when both names are different or false otherwise
     */
    template<typename OtherHashCollisionSafetyHint>
    constexpr bool operator!=(const name<OtherHashCollisionSafetyHint>& other) const noexcept
    {
        // Safe about hash collisions
        if constexpr(std::is_same_v<HashCollisionSafetyHint, hash_collision_safe>)
        {
            return string().size() != other.string().size()
                || hash() != other.hash()
                || string() != other.string();
        }
        else
        {
            return string().size() != other.string().size()
                || hash() != other.hash();
        }
    }
};

using safe_name = name<hash_collision_safe>;
using unsafe_name = name<hash_collision_unsafe>;

namespace literals
{

/**
 * String literals to create compile time hash
 * @param str The string to hash
 * @param length The length of the string to hash
 * @return The hashed value of the string
 */
[[nodiscard]] inline safe_name operator ""_sname(const char* str, std::size_t length)
{
    return safe_name(std::string_view{str, length});
}

[[nodiscard]] inline unsafe_name operator ""_usname(const char* str, std::size_t length)
{
    return unsafe_name(std::string_view{str, length});
}


}

}

#endif
