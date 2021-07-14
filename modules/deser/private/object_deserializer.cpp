#include "object_deserializer.hpp"
#include <cassert>

namespace ng
{

std::optional<uint8_t> object_deserializer::deserialize_uint8(const ng::string_name& name, uint8_t def) const
{
    const auto temp_value = deserialize_uint64(name, def);

    if(!temp_value)
    {
        return std::nullopt;
    }

    assert(temp_value <= std::numeric_limits<uint8_t>::max());
    return static_cast<uint8_t>(*temp_value);
}

std::optional<uint16_t> object_deserializer::deserialize_uint16(const ng::string_name& name, uint16_t def) const
{
    const auto temp_value = deserialize_uint64(name, def);

    if(!temp_value)
    {
        return std::nullopt;
    }

    assert(temp_value <= std::numeric_limits<uint16_t>::max());
    return static_cast<uint16_t>(*temp_value);
}


std::optional<uint32_t> object_deserializer::deserialize_uint32(const ng::string_name& name, uint32_t def) const
{
    const auto temp_value = deserialize_uint64(name, def);

    if(!temp_value)
    {
        return std::nullopt;
    }

    assert(temp_value <= std::numeric_limits<uint32_t>::max());
    return static_cast<uint32_t>(*temp_value);
}

std::optional<int8_t> object_deserializer::deserialize_int8(const ng::string_name& name, int8_t def) const
{
    const auto temp_value = deserialize_int64(name, def);

    if(!temp_value)
    {
        return std::nullopt;
    }

    assert(temp_value <= std::numeric_limits<int8_t>::max());
    assert(temp_value >= std::numeric_limits<int8_t>::min());
    return static_cast<int8_t>(*temp_value);
}

std::optional<int16_t> object_deserializer::deserialize_int16(const ng::string_name& name, int16_t def) const
{
    const auto temp_value = deserialize_int64(name, def);

    if(!temp_value)
    {
        return std::nullopt;
    }

    assert(temp_value <= std::numeric_limits<int16_t>::max());
    assert(temp_value >= std::numeric_limits<int16_t>::min());
    return static_cast<int16_t>(*temp_value);
}


std::optional<int32_t> object_deserializer::deserialize_int32(const ng::string_name& name, int32_t def) const
{
    const auto temp_value = deserialize_int64(name, def);

    if(!temp_value)
    {
        return std::nullopt;
    }

    assert(temp_value <= std::numeric_limits<uint32_t>::max());
    assert(temp_value >= std::numeric_limits<int32_t>::min());
    return static_cast<int32_t>(*temp_value);
}

std::optional<float> object_deserializer::deserialize_float(const ng::string_name& name, float def) const
{
    const auto temp_value = deserialize_double(name, def);

    if(!temp_value)
    {
        return std::nullopt;
    }

    assert(temp_value <= std::numeric_limits<float>::max());
    assert(temp_value >= std::numeric_limits<float>::min());
    return static_cast<float>(*temp_value);
}

}