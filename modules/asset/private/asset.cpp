#include "asset.hpp"
#include "asset_package.hpp"

namespace ng
{

std::string asset::path() const
{
    if(package_)
    {
        if(entry_index_ >= 0)
        {
            return package_->path() + "." + (*package_)[entry_index_].name().string();
        }
        else
        {
            return package_->path();
        }
    }

    return std::string{};
}

}