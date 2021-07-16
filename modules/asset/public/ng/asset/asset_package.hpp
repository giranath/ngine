#ifndef NGINE_ASSET_PACKAGE_HPP
#define NGINE_ASSET_PACKAGE_HPP

#include "asset_view.hpp"
#include <ng/core/string_name.hpp>

#include <cstdint>
#include <vector>
#include <optional>

namespace ng
{

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
    asset_registry* owner_ = nullptr;

    // The entries inside the package
    std::vector<entry> entries_ {};

    // The memory address where assets are stored
    std::vector<uint8_t> data_ {};

    // The serialization version of the package
    uint8_t package_version_ = 0;

public:
    asset_package() = default;

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
    bool try_load(std::istream& stream);

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

    /**
     * Returns a reference to the asset registry owning this package
     * @return The asset registry owning this package
     */
    [[nodiscard]] asset_registry* owner() const noexcept;

    /**
     * Returns a reference to the asset registry owning this package
     * @return The asset registry owning this package
     */
    [[nodiscard]] asset_registry* owner() noexcept;

private:
    void save_header(std::ostream& stream) const;
    bool try_load_header(std::istream& stream);

    void save_entries(std::ostream& stream) const;
    bool try_load_entries(std::istream& stream);

    void save_entry(std::ostream& stream, const entry& e) const;
    bool try_load_entry(std::istream& stream, entry& e);

    void save_data(std::ostream& stream) const;
    bool try_load_data(std::istream& stream);
};

}

#endif
