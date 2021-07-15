#ifndef NGINE_ASSET_ASSET_REGISTRY_HPP
#define NGINE_ASSET_ASSET_REGISTRY_HPP

#include "asset_factory.hpp"
#include "asset_package.hpp"
#include "asset.hpp"
#include <memory>

namespace ng
{

/**
 * Registry responsible to associate an asset path to a loaded asset
 */
class asset_registry
{
    // The factory for every assets
    asset_factory factory_;

    // Contains all the loaded packages
    std::vector<asset_package> loaded_packages_;

    // Contains a list of all loaded assets
    std::vector<std::weak_ptr<asset>> loaded_assets_;

public:
    /**
     * Request the registry to load an asset
     * @param asset_path The path to the asset to load
     * @return the loaded asset or nullptr in case the asset could not be loaded
     */
    [[nodiscard]] std::shared_ptr<asset> load(std::string_view asset_path);

    /**
     * Request the registry to load a particular asset subtype
     * @tparam AssetType The type of the asset to load
     * @param asset_path The path to the asset to load
     * @return the loaded asset or nullptr in case the asset could not be loaded
     */
    template<typename AssetType>
    [[nodiscard]] std::shared_ptr<AssetType> load_checked(std::string_view asset_path)
    {
        std::shared_ptr<asset> new_asset = load(asset_path);

        if(new_asset)
        {
            // TODO: Validate that new_asset is of valid type
            //asset_traits<AssetType>::compatible_with_type();

            return std::static_pointer_cast<AssetType>(new_asset);
        }

        return nullptr;
    }
};

}

#endif
