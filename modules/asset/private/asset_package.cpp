#include "asset_package.hpp"
#include <iostream>
#include <algorithm>
#include <cassert>

namespace ng
{

// TODO: Handle endianess here

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

// Asset package starts with the magic header NGPKG
static const char ASSET_MAGIC_HEADER[] = { 'N', 'G', 'P', 'K', 'G' };
static const uint8_t CURRENT_PACKAGE_VERSION = 0;

static std::ostream& save_name(std::ostream& stream, const string_name& name)
{
    const char* name_cstr = name.c_str();
    const uint64_t name_length = std::strlen(name_cstr);

    stream.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length));
    stream.write(name_cstr, name_length);

    return stream;
}

std::istream& load_name(std::istream& stream, string_name& name)
{
    uint64_t name_length = 0;

    stream.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));

    std::vector<char> name_buffer(name_length + 1);
    stream.read(name_buffer.data(), name_length);

    // Create a name using the deserialized string
    name = string_name{name_buffer.data()};

    return stream;
}

std::string asset_package::path() const
{
    // TODO: Implement this
    return "";
}

void asset_package::save(std::ostream& stream) const
{
    if(stream)
    {
        save_header(stream);
    }

    if(stream)
    {
        save_entries(stream);
    }

    if(stream)
    {
        save_data(stream);
    }
}

void asset_package::load(std::istream& stream)
{
    if(stream)
    {
        load_header(stream);
    }

    if(stream)
    {
        load_entries(stream);
    }

    if(stream)
    {
        load_data(stream);
    }
}

void asset_package::save_header(std::ostream& stream) const
{
    // Starts by writing magic header
    if(!stream.write(ASSET_MAGIC_HEADER, sizeof(ASSET_MAGIC_HEADER)))
    {
        // Failed to write package header
        return;
    }

    // Then write version
    if(!stream.write(reinterpret_cast<const char*>(&CURRENT_PACKAGE_VERSION), sizeof(CURRENT_PACKAGE_VERSION)))
    {
        // Failed to write package version
        return;
    }

    // Then write every entries
    const uint64_t entries_count = entries_.size();
    if(!stream.write(reinterpret_cast<const char*>(&entries_count), sizeof(entries_count)))
    {
        // Failed to write entries count
        return;
    }
}

void asset_package::load_header(std::istream& stream)
{
    char magic_header[5] = {0};

    // Starts by read magic header
    if(!stream.read(magic_header, 5))
    {
        // Failed to write package header
        return;
    }

    // Now validate that the header is the expected one, otherwise, this stream is not an asset package
    if(!std::equal(std::begin(magic_header), std::end(magic_header), std::begin(ASSET_MAGIC_HEADER)))
    {
        // Invalid magic header
        // Notify stream that it is corrupted and we should not continue loading this package
        stream.setstate(std::ios::failbit);
        return;
    }

    // Then read version
    if(!stream.read(reinterpret_cast<char*>(&package_version_), sizeof(package_version_)))
    {
        // Failed to write package version
        return;
    }
}

void asset_package::save_entries(std::ostream& stream) const
{
    // Then write every entries
    const uint64_t entries_count = entries_.size();
    if(!stream.write(reinterpret_cast<const char*>(&entries_count), sizeof(entries_count)))
    {
        // Failed to write entries count
        return;
    }

    for(const entry& e : entries_)
    {
        save_entry(stream, e);
    }
}

void asset_package::load_entries(std::istream& stream)
{
    // Then write every entries
    uint64_t entries_count = 0;
    if(!stream.read(reinterpret_cast<char*>(&entries_count), sizeof(entries_count)))
    {
        // Failed to write entries count
        return;
    }

    for(uint64_t i = 0; i < entries_count; ++i)
    {
        entries_.emplace_back();
        load_entry(stream, entries_.back());
    }
}

void asset_package::save_entry(std::ostream& stream, const entry& e) const
{
    const char* name_cstr = e.name.c_str();
    const uint64_t name_length = std::strlen(name_cstr);

    if(!save_name(stream, e.name))
    {
        // Failed to save name
        return;
    }

    if(!save_name(stream, e.type))
    {
        // Failed to save type's name
        return;
    }

    if(!stream.write(reinterpret_cast<const char*>(&e.version), sizeof(e.version)))
    {
        // Failed to write asset serialization version
        return;
    }

    if(!stream.write(reinterpret_cast<const char*>(&e.size), sizeof(e.size)))
    {
        // Failed to write asset's size
        return;
    }

    if(!stream.write(reinterpret_cast<const char*>(&e.offset), sizeof(e.offset)))
    {
        // Failed to write asset's offset
        return;
    }
}

void asset_package::load_entry(std::istream& stream, entry& e)
{
    if(!load_name(stream, e.name))
    {
        // Failed to load name
        return;
    }

    if(!load_name(stream, e.type))
    {
        // Failed to load type
        return;
    }

    if(!stream.read(reinterpret_cast<char*>(&e.version), sizeof(e.version)))
    {
        // Failed to write asset serialization version
        return;
    }

    if(!stream.read(reinterpret_cast<char*>(&e.size), sizeof(e.size)))
    {
        // Failed to write asset's size
        return;
    }

    if(!stream.read(reinterpret_cast<char*>(&e.offset), sizeof(e.offset)))
    {
        // Failed to write asset's offset
        return;
    }
}

void asset_package::save_data(std::ostream& stream) const
{
    const uint64_t data_size = data_.size();
    if(!stream.write(reinterpret_cast<const char*>(&data_size), sizeof(data_size)))
    {
        // Failed to write size of data storage
        return;
    }

    if(!stream.write(reinterpret_cast<const char*>(data_.data()), data_size))
    {
        // Failed to write data storage
        return;
    }
}

void asset_package::load_data(std::istream& stream)
{
    uint64_t data_size = 0;
    if(!stream.read(reinterpret_cast<char*>(&data_size), sizeof(data_size)))
    {
        // Failed to write size of data storage
        return;
    }

    data_.resize(data_size);

    if(!stream.read(reinterpret_cast<char*>(data_.data()), data_size))
    {
        // Failed to write data storage
        return;
    }
}

bool asset_package::contains(const string_name& name) const noexcept
{
    return index_of(name) < entries_.size();
}

std::size_t asset_package::index_of(const string_name& name) const noexcept
{
    auto it = std::find_if(entries_.begin(), entries_.end(), [&name](const entry& e) { return e.name == name; });

    return std::distance(entries_.begin(), it);
}

std::size_t asset_package::size() const noexcept
{
    return entries_.size();
}

bool asset_package::empty() const noexcept
{
    return entries_.empty();
}

asset_view asset_package::operator[](const string_name& name) const noexcept
{
    return operator[](index_of(name));
}

asset_view asset_package::operator[](std::size_t index) const noexcept
{
    assert(index < entries_.size());

    const uint8_t* data = data_.data() + entries_[index].offset;

    return asset_view{this, entries_[index].name, entries_[index].type, entries_[index].version, data, entries_[index].size};
}

asset_view asset_package::at(const string_name& name) const
{
    return at(index_of(name));
}

asset_view asset_package::at(std::size_t index) const
{
    if(index >= entries_.size())
    {
        throw std::out_of_range{"invalid asset package"};
    }

    const uint8_t* data = data_.data() + entries_[index].offset;

    return asset_view{this, entries_[index].name, entries_[index].type, entries_[index].version, data, entries_[index].size};
}

asset_registry* asset_package::owner() const noexcept
{
    return owner_;
}

asset_registry* asset_package::owner() noexcept
{
    return owner_;
}

}