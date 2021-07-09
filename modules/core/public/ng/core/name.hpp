#ifndef NGINE_CORE_NAME_HPP
#define NGINE_CORE_NAME_HPP

#include <string_view>

namespace ng
{

class name_table_entry;

/**
 * Represent a name, a string optimized for comparison
 * Names should be used to identify stuff with a string because they comparing names together is a O(1) operation
 */
class name
{
    name_table_entry* entry_;
public:
    constexpr name() noexcept
    : entry_{nullptr}
    {

    }

    explicit name(std::string_view string);
    name(const name& other);
    name(name&& other) noexcept;
    ~name();

    /**
     * Copy a name into this name
     * @param other The name to copy into this
     * @return a reference to this
     */
    name& operator=(const name& other);
    name& operator=(name&& other) noexcept;

    /**
     * Swap the name with another
     * @param other The other name
     */
    void swap(name& other) noexcept;

    /**
     * Clear the name
     */
    void clear();

    /**
     * Check if this name is empty
     * @return true when the name is empty or false otherwise
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * Returns the c string associated with this name
     * @return The c string associated with this name
     */
    [[nodiscard]] const char* c_str() const noexcept;

    /**
     * Returns the string associated with this name
     * @return the string associated with this name
     */
    [[nodiscard]] std::string string() const noexcept;

    bool operator==(const name& other) const noexcept;
    bool operator!=(const name& other) const noexcept;
};

void swap(name& a, name& b) noexcept;

namespace literals
{

inline name operator""_name(const char* str, std::size_t len)
{
    return name{std::string_view{str, len}};
}

}

using safe_name = name;
using unsafe_name = name;

}

#endif
