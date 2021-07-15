#ifndef NGINE_ASSET_ASSET_HPP
#define NGINE_ASSET_ASSET_HPP

#include <ng/core/string_name.hpp>

namespace ng
{

class asset_package_istream;
class asset_package_ostream;
class asset_package;

/**
 * Base class every assets must override
 * An asset is a unique data container that is serialized on persistent storage.
 * Ngine will load at most one instance of every requested asset so if two different
 * systems requires an asset to be loaded, it will be loaded only once
 */
class asset
{
    // The package owning this asset
    const asset_package* package_ = nullptr;
    int64_t entry_index_ = -1;

public:
    virtual ~asset() = default;

    /**
     * Returns the path to this asset
     * @return The path to this asset
     */
    [[nodiscard]] std::string path() const;

protected:
    /**
     * Load the asset from its package
     * @param stream The stream where to load this asset
     * @param reason The reason the asset is being loaded
     */
    virtual void on_load(asset_package_istream& stream) = 0;

    /**
     * Save the asset into its package
     * @param stream The stream where to save this asset
     */
    virtual void on_save(asset_package_ostream& stream) const = 0;
};

}

#endif
