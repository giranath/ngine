#ifndef NGINE_NAME_TABLE_HPP
#define NGINE_NAME_TABLE_HPP

#include <cstdint>
#include <cstddef>
#include <string>
#include <shared_mutex>
#include <string_view>

namespace ng
{

/**
 * Represent an entry inside the name table
 */
class name_table_entry
{
    friend class name_table;

    name_table_entry* next_;
    name_table_entry* prev_;
    std::string string_;
    uint64_t hash_;
    uint64_t refcount_;
    std::size_t table_index_;

public:
    name_table_entry(std::string_view str, uint64_t hash, name_table_entry* next, std::size_t table_index);
    ~name_table_entry();

    /**
     * Increase refcount
     */
    void addref();

    /**
     * Decrease refcount
     * @return true when the entry is no longer referenced or false if something still references it
     * @note You must destroy the entry when it returns true
     */
    [[nodiscard]] bool release();

    /**
     * Returns the c string
     * @return The c string associated with entry
     */
    [[nodiscard]] const char* c_str() const noexcept;

    /**
     * Returns the string representation of this name
     * @return The string representation
     */
    [[nodiscard]] std::string string() const noexcept;
};

/**
 * Holds all existing name entries
 */
class name_table
{
    static constexpr std::size_t entry_count = 4096;
    static constexpr std::size_t mask = entry_count - 1;

    // We split names into multiple entries to simplify searching names
    name_table_entry* entries_[entry_count];
    mutable std::shared_mutex mutex_;

    name_table();

public:
    ~name_table();

    /**
     * Find or add a new entry into the table
     * @param str The string to search for in the table
     * @return the entry containing the string
     */
    [[nodiscard]] name_table_entry* find_or_add(std::string_view str);

    /**
     * Find an existing entry in the table
     * @param str The string to find
     * @return the entry
     */
    [[nodiscard]] name_table_entry* find(std::string_view str) const;

    /**
     * Release a table entry
     * @param entry The entry to release
     */
    void release(name_table_entry* entry);

    /**
     * Check if the table is empty
     * @return true when empty or false otherwise
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * Returns the number of names the table contains
     * @return the number of names the table contains
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * Get singleton instance
     * @return the singleton instance of this name table
     */
    static name_table& get();
};

}

#endif
