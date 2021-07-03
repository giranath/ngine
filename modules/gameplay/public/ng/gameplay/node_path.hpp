#ifndef NGINE_GAMEPLAY_NODE_PATH_HPP
#define NGINE_GAMEPLAY_NODE_PATH_HPP

#include <ng/core/name.hpp>

#include <string_view>
#include <string>
#include <vector>
#include <ostream>

namespace ng
{

class node_path;

/**
 * Iterate over elements of a node path
 */
class node_path_iterator
{
    friend node_path;

    const node_path* owner_;
    std::size_t index_;
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = safe_name;
    using pointer           = const safe_name*;  // or also value_type*
    using reference         = const safe_name&;  // or also value_type&

    node_path_iterator();

    node_path_iterator& operator++();
    node_path_iterator operator++(int);
    node_path_iterator& operator--();
    node_path_iterator operator--(int);

    /**
     * Check if this iterator points on the same element as another iterator
     * @param other The other iterator
     * @return true when both iterators point on the same element
     */
    bool operator==(const node_path_iterator& other) const noexcept;

    /**
     * Check if this iterator points on the same element as another iterator
     * @param other the other iterator
     * @return true when both iterators point on the same element
     */
    bool operator!=(const node_path_iterator& other) const noexcept;

    /**
     * Returns a reference to the referenced element of the path
     * @return the referenced element of the path
     */
    reference operator*() const noexcept;

    /**
     * Returns a pointer to the referenced element of the path
     * @return The referenced element of the path
     */
    pointer  operator->() const noexcept;

private:
    node_path_iterator(const node_path& path, std::size_t index);
};

/**
 * Holds a path to a node
 */
class node_path
{
    friend node_path_iterator;
    std::vector<safe_name> names_;
    bool absolute_;
    bool trailing_delimiter_;
public:
    static const char delimiter_char = '/';

    node_path() noexcept;

    /**
     * Parse a string to get path
     * @param str The string to parse
     */
    explicit node_path(std::string_view str);

    /**
     * Normalize a path
     * @return The normalized path
     * @note Multiple element separators are combined and ./ are resolved when possible
     */
    [[nodiscard]] node_path normalize() const;

    [[nodiscard]] node_path append(const node_path& path) const;
    [[nodiscard]] node_path concat(const node_path& path) const;

    node_path operator/(const node_path& path) const;
    node_path operator+(const node_path& path) const;

    /**
     * Check if the path is absolute
     * @return true when the path is absolute or false otherwise
     */
    [[nodiscard]] bool absolute() const noexcept;

    /**
     * Check if the path is empty
     * @return true when empty or false otherwise
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * Clear the path
     */
    void clear();

    /**
     * Returns the string representation of the path
     * @return
     */
    [[nodiscard]] std::string string() const;

    bool operator==(const node_path& path) const noexcept;
    bool operator!=(const node_path& path) const noexcept;

    node_path_iterator begin() const;
    node_path_iterator end() const;

    static node_path parse(std::string_view str);
};

std::ostream& operator<<(std::ostream& out, const node_path& path);

namespace literals
{

[[nodiscard]] inline node_path operator ""_node(const char* str, std::size_t length)
{
    return node_path{std::string_view{str, length}};
}

}

}


#endif
