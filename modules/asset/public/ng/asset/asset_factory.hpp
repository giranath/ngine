#ifndef NGINE_ASSET_ASSET_FACTORY_HPP
#define NGINE_ASSET_ASSET_FACTORY_HPP

#include "asset.hpp"
#include <memory>
#include <vector>

namespace ng
{

class string_name;
class asset;

/**
 * Function pointer for an asset factory
 */
using asset_factory_fn = std::unique_ptr<asset>(*)();

/**
 * Abstract factory for assets
 */
class asset_factory
{
    std::vector<string_name> names_;
    std::vector<string_name> parent_names_;
    std::vector<asset_factory_fn> fns_;

public:

    /**
     * Register a new asset type
     * @tparam T The type of the asset
     * @param asset_type The name of the asset type
     */
    template<typename T>
    void register_asset(const string_name& asset_type)
    {
        register_asset(asset_type, []() { return std::make_unique<T>(); });
    }

    /**
     * Register a new asset factory function
     * @param asset_type The name of the asset type
     * @param factory_fn The factory function to call to create a new asset
     */
    void register_asset(const string_name& asset_type, asset_factory_fn factory_fn);

    /**
     * Unregister an asset type from the factory
     * @param asset_type The type of the asset
     */
    void unregister_asset(const string_name& asset_type);

    /**
     * Make a new instance of an asset
     * @param asset_type The type of the asset to create
     * @return the created asset
     */
    std::unique_ptr<asset> make(std::nothrow_t, const string_name& asset_type) noexcept;

    /**
     * Make a new instance of an asset
     * @param asset_type The type of the asset to create
     * @return the created asset
     * @note Will throw an exception if asset_type is not recognised
     */
    std::unique_ptr<asset> make(const string_name& asset_type);
};

}

#endif
