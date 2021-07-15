#include "asset_package_stream.hpp"
#include "asset_package.hpp"
#include "asset.hpp"
#include "asset_registry.hpp"
#include <cassert>

namespace ng
{

uint8_t asset_package_istream::version() const noexcept
{
    return view_->version();
}

asset_registry& asset_package_istream::registry() noexcept
{
    assert(view_->package());

    const asset_package* package = view_->package();
    assert(package->owner());

    return *package->owner();
}

void asset_package_ostream::set_version(uint8_t new_version)
{
    assert(false);
    // TODO: Change version on current asset
}

asset_package_ostream& operator<<(asset_package_ostream& stream, const std::shared_ptr<asset>& a)
{
    if(a)
    {
        stream << std::quoted(a->path());
    }

    return stream;
}

asset_package_istream& operator>>(asset_package_istream& stream, std::shared_ptr<asset>& a)
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
            a = registry.load(path);
        }
    }

    return stream;
}

}