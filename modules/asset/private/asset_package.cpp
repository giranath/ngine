#include "asset_package.hpp"
#include <ng/core/endian.hpp>
#include <iostream>
#include <algorithm>
#include <cassert>

namespace ng
{

// A package should always be written to disk using little endian
// This allows a big endian host to correctly handle an asset package because we know that some endianess conversions are needed
// on this particular host.
// At some point, I think it would be better to indicate inside the package which endianness was used when writing the file

// Asset package starts with the magic header NGPKG
static const char ASSET_MAGIC_HEADER[] = { 'N', 'G', 'P', 'K', 'G' };
static const uint8_t CURRENT_PACKAGE_VERSION = 0;

static std::ostream& write_uint(std::ostream& stream, uint8_t value)
{
    const uint8_t little_endian_value = little_endian_cast(value);
    stream.write(reinterpret_cast<const char*>(&little_endian_value), sizeof(little_endian_value));

    return stream;
}

static std::ostream& write_uint(std::ostream& stream, uint16_t value)
{
    const uint16_t little_endian_value = little_endian_cast(value);
    stream.write(reinterpret_cast<const char*>(&little_endian_value), sizeof(little_endian_value));

    return stream;
}

static std::ostream& write_uint(std::ostream& stream, uint32_t value)
{
    const uint32_t little_endian_value = little_endian_cast(value);
    stream.write(reinterpret_cast<const char*>(&little_endian_value), sizeof(little_endian_value));

    return stream;
}

static std::ostream& write_uint(std::ostream& stream, uint64_t value)
{
    const uint64_t little_endian_value = little_endian_cast(value);
    stream.write(reinterpret_cast<const char*>(&little_endian_value), sizeof(little_endian_value));

    return stream;
}

static std::istream& read_uint(std::istream& stream, uint8_t& out_value)
{
    uint8_t little_endian_value;
    stream.read(reinterpret_cast<char*>(&little_endian_value), sizeof(little_endian_value));

    if constexpr(!is_little_endian)
    {
        out_value = big_endian_cast(little_endian_value);
    }
    else
    {
        out_value = little_endian_value;
    }

    return stream;
}

static std::istream& read_uint(std::istream& stream, uint16_t& out_value)
{
    uint16_t little_endian_value;
    stream.read(reinterpret_cast<char*>(&little_endian_value), sizeof(little_endian_value));

    if constexpr(!is_little_endian)
    {
        out_value = big_endian_cast(little_endian_value);
    }
    else
    {
        out_value = little_endian_value;
    }

    return stream;
}

static std::istream& read_uint(std::istream& stream, uint32_t& out_value)
{
    uint32_t little_endian_value;
    stream.read(reinterpret_cast<char*>(&little_endian_value), sizeof(little_endian_value));

    if constexpr(!is_little_endian)
    {
        out_value = big_endian_cast(little_endian_value);
    }
    else
    {
        out_value = little_endian_value;
    }

    return stream;
}

static std::istream& read_uint(std::istream& stream, uint64_t& out_value)
{
    uint64_t little_endian_value;
    stream.read(reinterpret_cast<char*>(&little_endian_value), sizeof(little_endian_value));

    if constexpr(!is_little_endian)
    {
        out_value = big_endian_cast(little_endian_value);
    }
    else
    {
        out_value = little_endian_value;
    }

    return stream;
}


static std::ostream& save_name(std::ostream& stream, const string_name& name)
{
    const char* name_cstr = name.c_str();
    const uint64_t name_length = std::strlen(name_cstr);

    write_uint(stream, name_length);
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

bool asset_package::try_load(std::istream& stream)
{
    if(stream)
    {
        if(!try_load_header(stream))
        {
            return false;
        }
    }

    if(stream)
    {
        if(!try_load_entries(stream))
        {
            return false;
        }
    }

    if(stream)
    {
        if(!try_load_data(stream))
        {
            return false;
        }
    }

    return true;
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
    if(!write_uint(stream, CURRENT_PACKAGE_VERSION))
    {
        // Failed to write package version
        return;
    }

    // Then write every entries
    const uint64_t entries_count = entries_.size();
    if(!write_uint(stream, entries_count))
    {
        // Failed to write entries count
        return;
    }
}

bool asset_package::try_load_header(std::istream& stream)
{
    char magic_header[5] = {0};

    // Starts by read magic header
    if(!stream.read(magic_header, 5))
    {
        // Failed to write package header
        return false;
    }

    // Now validate that the header is the expected one, otherwise, this stream is not an asset package
    if(!std::equal(std::begin(magic_header), std::end(magic_header), std::begin(ASSET_MAGIC_HEADER)))
    {
        // Invalid magic header
        // Notify stream that it is corrupted and we should not continue loading this package
        stream.setstate(std::ios::failbit);
        return false;
    }

    // Then read version
    if(!read_uint(stream, package_version_))
    {
        // Failed to write package version
        return false;
    }

    return true;
}

void asset_package::save_entries(std::ostream& stream) const
{
    // Then write every entries
    const uint64_t entries_count = entries_.size();
    if(!write_uint(stream, entries_count))
    {
        // Failed to write entries count
        return;
    }

    for(const entry& e : entries_)
    {
        save_entry(stream, e);
    }
}

bool asset_package::try_load_entries(std::istream& stream)
{
    // Then write every entries
    uint64_t entries_count = 0;
    if(!read_uint(stream, entries_count))
    {
        // Failed to write entries count
        return false;
    }

    for(uint64_t i = 0; i < entries_count; ++i)
    {
        entries_.emplace_back();

        if(!try_load_entry(stream, entries_.back()))
        {
            return false;
        }
    }

    return true;
}

void asset_package::save_entry(std::ostream& stream, const entry& e) const
{
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

    if(!write_uint(stream, e.version))
    {
        // Failed to write asset serialization version
        return;
    }

    if(!write_uint(stream, e.size))
    {
        // Failed to write asset's size
        return;
    }

    if(!write_uint(stream, e.offset))
    {
        // Failed to write asset's offset
        return;
    }
}

bool asset_package::try_load_entry(std::istream& stream, entry& e)
{
    if(!load_name(stream, e.name))
    {
        // Failed to load name
        return false;
    }

    if(!load_name(stream, e.type))
    {
        // Failed to load type
        return false;
    }

    if(!read_uint(stream, e.version))
    {
        // Failed to write asset serialization version
        return false;
    }

    if(!read_uint(stream, e.size))
    {
        // Failed to write asset's size
        return false;
    }

    if(!read_uint(stream, e.offset))
    {
        // Failed to write asset's offset
        return false;
    }

    return true;
}

void asset_package::save_data(std::ostream& stream) const
{
    const uint64_t data_size = data_.size();
    if(!write_uint(stream, data_size))
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

bool asset_package::try_load_data(std::istream& stream)
{
    uint64_t data_size = 0;
    if(!read_uint(stream, data_size))
    {
        // Failed to write size of data storage
        return false;
    }

    data_.resize(data_size);

    if(!stream.read(reinterpret_cast<char*>(data_.data()), data_size))
    {
        // Failed to write data storage
        return false;
    }

    return true;
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