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
class string_name
{
    name_table_entry* entry_;
public:
    static const string_name none;

    constexpr string_name() noexcept
    : entry_{nullptr}
    {

    }

    explicit string_name(std::string_view string);
    string_name(const string_name& other);
    string_name(string_name&& other) noexcept;
    ~string_name();

    /**
     * Copy a name into this name
     * @param other The name to copy into this
     * @return a reference to this
     */
    string_name& operator=(const string_name& other);
    string_name& operator=(string_name&& other) noexcept;

    /**
     * Swap the name with another
     * @param other The other name
     */
    void swap(string_name& other) noexcept;

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

    bool operator==(const string_name& other) const noexcept;
    bool operator!=(const string_name& other) const noexcept;
};

void swap(string_name& a, string_name& b) noexcept;

namespace literals
{

inline string_name operator""_name(const char* str, std::size_t len)
{
    return string_name{std::string_view{str, len}};
}

}

}

#endif
