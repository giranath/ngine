#ifndef NGINE_ASSET_ASSET_PACKAGE_STREAM_HPP
#define NGINE_ASSET_ASSET_PACKAGE_STREAM_HPP

#include "asset_registry.hpp"
#include <iostream>
#include <iomanip>

namespace ng
{

class asset_view;
class asset;
class asset_registry;

/**
 * Base class for input asset package stream
 * @note Use this stream to read from an asset package
 */
class asset_package_istream : public std::istream
{
    asset_view* view_;

public:
    /**
     * Returns the serialization version used by this asset when it was saved
     * @return
     */
    [[nodiscard]] uint8_t version() const noexcept;

    /**
     * Returns the asset registry owning the package where this asset is being deserialized from
     * @return The asset registry owning this asset
     */
    [[nodiscard]] asset_registry& registry() noexcept;
};

/**
 * Base class for output asset package stream
 * @note Use this stream to write into an asset package
 */
class asset_package_ostream : public std::ostream
{
public:
    /**
     * Change the serialization version for this stream
     * @param new_version The new version to use when serializing this asset
     * @note By default, the version will be 0
     */
    void set_version(uint8_t new_version);

};

/**
 * Manage asset pointers from asset packages
 * @param stream The stream to write the asset to
 * @param a The pointer containing the reference to the asset
 * @return the stream passed in parameter
 */
asset_package_ostream& operator<<(asset_package_ostream& stream, const std::shared_ptr<asset>& a);

/**
 * Manage asset pointers from asset package
 * @param stream The stream to read the referenced asset from
 * @param a The pointer where to store the reference to the asset
 * @return the stream passed in parameter
 */
asset_package_istream& operator>>(asset_package_istream& stream, std::shared_ptr<asset>& a);

/**
 * Manage asset pointers from asset package
 * @tparam AssetType The type of the asset to load
 * @param stream The stream to read the referenced asset from
 * @param a The pointer where to store the reference to the asset
 * @return the stream passed in parameter
 */
template<typename AssetType>
asset_package_istream& operator>>(std::enable_if_t<std::is_base_of_v<asset, AssetType>, asset_package_istream&> stream, std::shared_ptr<AssetType>& a)
{
    if(std::istream::sentry s(stream); s)
    {
        // First, an asset strong pointer is always serialized as a path to the asset data
        // So we extract that path from the stream
        std::string path;
        if(stream >> std::quoted(path))
        {
            // At this point, we have a path that should leads us to the asset to load
            asset_registry& registry = stream.registry();

            // So we request the asset registry to load it for us
            // If the asset is already in memory, it will be returned directly, otherwise it will be fetched from package
            a = registry.load_checked<AssetType>(path);
        }
    }

    return stream;
}

}

#endif
