#ifndef NGINE_ASSET_ASSET_VIEW_HPP
#define NGINE_ASSET_ASSET_VIEW_HPP

#include <ng/core/string_name.hpp>

namespace ng
{

class asset_registry;
class asset_package;

/**
 * Represents a view on a stored asset
 */
class asset_view
{
    friend class asset_package;

    const asset_package* owner_ = nullptr;
    const uint8_t* ptr_ = nullptr;
    string_name name_{};
    string_name type_{};
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

}

#endif
