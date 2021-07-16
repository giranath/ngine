#include "asset_view.hpp"

namespace ng
{

asset_view::asset_view(const asset_package* owner, string_name name, string_name type, uint8_t version, const uint8_t* data, uint64_t size)
: owner_{owner}
, ptr_{data}
, name_{std::move(name)}
, type_{std::move(type)}
, size_{size}
, version_{version}
{

}

const uint8_t* asset_view::begin() const noexcept
{
    return ptr_;
}

const uint8_t* asset_view::end() const noexcept
{
    return ptr_ + size_;
}

bool asset_view::empty() const noexcept
{
    return size_ == 0;
}

std::size_t asset_view::size() const noexcept
{
    return size_;
}

uint8_t asset_view::version() const noexcept
{
    return version_;
}

const string_name& asset_view::name() const noexcept
{
    return name_;
}

const string_name& asset_view::type() const noexcept
{
    return type_;
}

const asset_package* asset_view::package() const noexcept
{
    return owner_;
}

}