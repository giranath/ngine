#include "asset_factory.hpp"
#include <ng/core/string_name.hpp>
#include <algorithm>
#include <stdexcept>

namespace ng
{

void asset_factory::register_asset(const string_name& asset_type, asset_factory_fn factory_fn)
{
    auto it = std::find(names_.begin(), names_.end(), asset_type);

    if(it != names_.end())
    {
        throw std::runtime_error{"asset type already registered"};
    }

    names_.push_back(asset_type);
    fns_.push_back(factory_fn);
}

void asset_factory::unregister_asset(const string_name& asset_type)
{
    auto it = std::find(names_.begin(), names_.end(), asset_type);

    if(it != names_.end())
    {
        const std::size_t index = std::distance(names_.begin(), it);

        using std::swap;
        swap(*it, names_.back());
        swap(fns_[index], fns_.back());

        names_.pop_back();
        fns_.pop_back();
    }
}

std::unique_ptr<asset> asset_factory::make(std::nothrow_t, const string_name& asset_type) noexcept
{
    auto it = std::find(names_.begin(), names_.end(), asset_type);

    if(it != names_.end())
    {
        const std::size_t index = std::distance(names_.begin(), it);

        return fns_[index]();
    }
    else
    {
        return nullptr;
    }
}

std::unique_ptr<asset> asset_factory::make(const string_name& asset_type)
{
    auto it = std::find(names_.begin(), names_.end(), asset_type);

    if(it != names_.end())
    {
        const std::size_t index = std::distance(names_.begin(), it);

        return fns_[index]();
    }
    else
    {
        throw std::runtime_error{"invalid asset type"};
    }
}

}