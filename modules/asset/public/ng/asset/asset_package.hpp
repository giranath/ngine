#ifndef NGINE_ASSET_PACKAGE_HPP
#define NGINE_ASSET_PACKAGE_HPP

#include <ng/core/string_name.hpp>

#include <cstdint>
#include <vector>

namespace ng
{

class asset_registry;

/**
 * Represents a view on a stored asset
 */
class asset_view
{
    friend class asset_package;

    const asset_package* owner_ = nullptr;
    const uint8_t* ptr_ = nullptr;
    string_name name_;
    string_name type_;
    uint64_t size_ = 0;
    uint8_t version_ = 0;

    asset_view(const asset_package* owner, string_name name, string_name type, uint8_t version, const uint8_t* data, uint64_t size);

public:
    asset_view() = default;

    /**
     * Returns a pointer to the start of the data for this asset
     * @return A pointer to the first byte of memory for this asset
     */
    [[nodiscard]] const uint8_t* begin() const noexcept;

    /**
     * Returns a pointer after the last byte of data for this asset
     * @return A pointer after the last byte of memory for this asset
     */
    [[nodiscard]] const uint8_t* end() const noexcept;

    /**
     * Check if the asset is empty
     * @return true when no data is associated with the asset
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * Returns the size in bytes for this asset
     * @return the size in bytes for this asset
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * Returns the serialization version for this asset
     * @return The serialization version for this asset
     */
    [[nodiscard]] uint8_t version() const noexcept;

    /**
     * Returns the name of the asset
     * @return The name of this asset
     */
    [[nodiscard]] const string_name& name() const noexcept;

    /**
     * Returns the type of the stored asset
     * @return The type of the stored asset
     */
    [[nodiscard]] const string_name& type() const noexcept;

    /**
     * Returns the pointer to the package owning this view
     * @return The package owning this view
     */
    [[nodiscard]] const asset_package* package() const noexcept;
};

/**
 * Collection of assets serialized on disk
 */
class asset_package
{
    /**
     * Represent an entry from the package
     */
    struct entry
    {
        // The name of the asset
        string_name name;

        // The globally unique type of the asset
        string_name type;

        // The size of the asset in bytes
        uint64_t size;

        // The offset of the asset inside the package
        uint64_t offset;

        // The serialization version used by the asset
        uint8_t version;
    };

    // The registry owning this package
    asset_registry* owner_;

    // The entries inside the package
    std::vector<entry> entries_;

    // The memory address where assets are stored
    std::vector<uint8_t> data_;

    // The serialization version of the package
    uint8_t package_version_;

public:
    /**
     * The path to this package
     * @return The path to this package
     */
    [[nodiscard]] std::string path() const;

    /**
     * Save the package to a stream
     * @param stream The stream where to save the package
     */
    void save(std::ostream& stream) const;

    /**
     * Load the package from a stream
     * @param stream The stream to load the package from
     */
    void load(std::istream& stream);

    /**
     * Check if this package contains a specific asset
     * @param name The name of the asset
     * @return true when the asset is contained or false if it doesn't exist
     */
    [[nodiscard]] bool contains(const string_name& name) const noexcept;

    /**
     * Find the index of an asset
     * @param name The name of the asset to retrieve the index from
     * @return The index of the asset
     * @note May return an index larger than size() if the name doesn't exist
     */
    [[nodiscard]] std::size_t index_of(const string_name& name) const noexcept;

    /**
     * Returns how many assets are stored inside this package
     * @return The number of assets stored inside this package
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * Returns true when this package doesn't contain any assets
     * @return true when the package is empty or false otherwise
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * Returns a view on the stored asset
     * @param name The name of the asset
     * @return the view onto the asset
     */
    asset_view operator[](const string_name& name) const noexcept;

    /**
     * Returns a view on the stored asset
     * @param index The index of the asset
     * @return the view onto the asset
     */
    asset_view operator[](std::size_t index) const noexcept;

    /**
     * Returns a view on a stored asset
     * @param name The name of the asset to retrieve
     * @return the view onto the asset
     * @note Will throw an exception if referenced asset doesn't exist
     */
    [[nodiscard]] asset_view at(const string_name& name) const;

    /**
     * Returns a view on a stored asset
     * @param index The index of the asset to retrieve
     * @return the view onto the asset
     * @note Will throw an exception if referenced asset doesn't exist
     */
    [[nodiscard]] asset_view at(std::size_t index) const;

    [[nodiscard]] asset_registry* owner() const noexcept;
    [[nodiscard]] asset_registry* owner() noexcept;

private:
    void save_header(std::ostream& stream) const;
    void load_header(std::istream& stream);

    void save_entries(std::ostream& stream) const;
    void load_entries(std::istream& stream);

    void save_entry(std::ostream& stream, const entry& e) const;
    void load_entry(std::istream& stream, entry& e);

    void save_data(std::ostream& stream) const;
    void load_data(std::istream& stream);
};

}

#endif
