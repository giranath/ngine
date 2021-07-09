#ifndef NGINE_CLASS_ARCHIVER_HPP
#define NGINE_CLASS_ARCHIVER_HPP

#include "object_archive_traits.hpp"

#include <ng/core/name.hpp>

#include <memory>
#include <cstdint>
#include <istream>
#include <optional>
#include <type_traits>
#include <string>
#include <string_view>

namespace ng
{

/**
 * Interface to archive an object
 */
struct object_deserializer
{
public:
    virtual ~object_deserializer() = default;

    /**
     * Returns the size of the current element
     * @return the size of the current element
     */
    [[nodiscard]] virtual std::size_t size() const = 0;

    /**
     * Set the index for the field inside current element
     * @param new_index The new index
     */
    virtual void set_index(std::size_t new_index) = 0;

    /**
     * Returns the index of the current element
     * @return The index of the current element
     */
    [[nodiscard]] virtual std::size_t index() const = 0;

    /**
     * Returns the name of the current field
     * @return The name of the current field
     */
    [[nodiscard]] virtual ng::name name() const = 0;

    /**
     * Returns the serialization version of the current object
     * @return the current serialization version of the current object
     */
    [[nodiscard]] virtual uint8_t version() const = 0;

    /**
     * Deserialize a uint8_t property
     * @param name The name of the property
     * @param def The default value to use when parsing fails
     * @return An optional with the deserialized property value
     */
    [[nodiscard]] virtual std::optional<uint8_t> deserialize_uint8(const ng::name& name, uint8_t def = uint8_t{}) const;

    /**
     * Deserialize a uint16_t property
     * @param name The name of the property
     * @param def The default value to use when parsing fails
     * @return An optional with the deserialized property value
     */
    [[nodiscard]] virtual std::optional<uint16_t> deserialize_uint16(const ng::name& name, uint16_t def = uint16_t{}) const;

    /**
     * Deserialize a uint32_t property
     * @param name The name of the property
     * @param def The default value to use when parsing fails
     * @return An optional with the deserialized property value
     */
    [[nodiscard]] virtual std::optional<uint32_t> deserialize_uint32(const ng::name& name, uint32_t def = uint32_t{}) const;

    /**
     * Deserialize a uint64_t property
     * @param name The name of the property
     * @param def The default value to use when parsing fails
     * @return An optional with the deserialized property value
     */
    [[nodiscard]] virtual std::optional<uint64_t> deserialize_uint64(const ng::name& name, uint64_t def = uint64_t{}) const = 0;

    /**
     * Deserialize a int8_t property
     * @param name The name of the property
     * @param def The default value to use when parsing fails
     * @return An optional with the deserialized property value
     */
    [[nodiscard]] virtual std::optional<int8_t> deserialize_int8(const ng::name& name, int8_t def = int8_t{}) const;

    /**
     * Deserialize a int16_t property
     * @param name The name of the property
     * @param def The default value to use when parsing fails
     * @return An optional with the deserialized property value
     */
    [[nodiscard]] virtual std::optional<int16_t> deserialize_int16(const ng::name& name, int16_t def = int16_t{}) const;

    /**
     * Deserialize a int32_t property
     * @param name The name of the property
     * @param def The default value to use when parsing fails
     * @return An optional with the deserialized property value
     */
    [[nodiscard]] virtual std::optional<int32_t> deserialize_int32(const ng::name& name, int32_t def = int32_t{}) const;

    /**
     * Deserialize a int64_t property
     * @param name The name of the property
     * @param def The default value to use when parsing fails
     * @return An optional with the deserialized property value
     */
    [[nodiscard]] virtual std::optional<int64_t> deserialize_int64(const ng::name& name, int64_t def = int64_t{}) const = 0;

    /**
     * Deserialize a boolean property
     * @param name The name of the property
     * @param def The default value to use when parsing fails
     * @return An optional with the deserialized property value
     */
    [[nodiscard]] virtual std::optional<bool> deserialize_bool(const ng::name& name, bool def = bool{}) const = 0;

    /**
     * Deserialize a float property
     * @param name The name of the property
     * @param def The default value to use when parsing fails
     * @return An optional with the deserialized property value
     */
    [[nodiscard]] virtual std::optional<float> deserialize_float(const ng::name& name, float def = float{}) const;

    /**
     * Deserialize a double property
     * @param name The name of the property
     * @param def The default value to use when parsing fails
     * @return An optional with the deserialized property value
     */
    [[nodiscard]] virtual std::optional<double> deserialize_double(const ng::name& name, double def = double{}) const = 0;

    /**
     * Deserialize a string property
     * @param name The name of the property
     * @param def The default value to use when parsing fails
     * @return An optional with the deserialized property value
     */
    [[nodiscard]] virtual std::optional<std::string> deserialize_string(const ng::name& name, std::string_view def = std::string_view{}) const = 0;

    /**
     * Start deserializing a subobject property
     * @param name The name of the property
     * @note you must call end_object when the subobject is fully deserialized
     */
    virtual void begin_object(const ng::name& name) = 0;

    /**
     * End deserializing a subobject
     */
    virtual void end_object() = 0;

    /**
     * Start deserializing an array property
     * @param name The name of the array property
     * @note you must call end_array when the array is fully deserialized
     */
    virtual void begin_array(const ng::name& name) = 0;

    /**
     * End deserializing an array property
     */
    virtual void end_array() = 0;

    /**
     * Start deserializing a dictionary property
     * @param name The name of the dictionary property
     * @note You must call end_dictionary when the dictionary is fully deserialized
     */
    virtual void begin_dictionary(const ng::name& name) = 0;

    /**
     * End deserializing a dictionary property
     */
    virtual void end_dictionary() = 0;
};

/**
 * Load an object using XML
 * @param stream The stream containing the XML document
 * @return the object archiver
 */
[[nodiscard]] std::unique_ptr<object_deserializer> make_object_xml_deserializer(std::istream& stream);

/**
 * Helper function to deserialize a single property of an object
 * @tparam T The type of the property to deserialize
 * @param archive The archive to deserialize the property from
 * @param name The name of the property to deserialize
 * @return The deserialized property or nullopt if the property was not found
 */
template<typename T, typename = std::enable_if_t<is_object_property_type_v<T>>>
[[nodiscard]] inline std::optional<T> deserialize_object_property(const object_deserializer& archive, const name& name);

template<>
[[nodiscard]] inline std::optional<uint8_t> deserialize_object_property<uint8_t>(const object_deserializer& archive, const name& name)
{
    return archive.deserialize_uint8(name);
}

template<>
[[nodiscard]] inline std::optional<uint16_t> deserialize_object_property<uint16_t>(const object_deserializer& archive, const name& name)
{
    return archive.deserialize_uint16(name);
}

template<>
[[nodiscard]] inline std::optional<uint32_t> deserialize_object_property<uint32_t>(const object_deserializer& archive, const name& name)
{
    return archive.deserialize_uint32(name);
}

template<>
[[nodiscard]] inline std::optional<uint64_t> deserialize_object_property<uint64_t>(const object_deserializer& archive, const name& name)
{
    return archive.deserialize_uint64(name);
}

template<>
[[nodiscard]] inline std::optional<int8_t> deserialize_object_property<int8_t>(const object_deserializer& archive, const name& name)
{
    return archive.deserialize_int8(name);
}

template<>
[[nodiscard]] inline std::optional<int16_t> deserialize_object_property<int16_t>(const object_deserializer& archive, const name& name)
{
    return archive.deserialize_int16(name);
}

template<>
[[nodiscard]] inline std::optional<int32_t> deserialize_object_property<int32_t>(const object_deserializer& archive, const name& name)
{
    return archive.deserialize_int32(name);
}

template<>
[[nodiscard]] inline std::optional<int64_t> deserialize_object_property<int64_t>(const object_deserializer& archive, const name& name)
{
    return archive.deserialize_int64(name);
}

template<>
[[nodiscard]] inline std::optional<float> deserialize_object_property<float>(const object_deserializer& archive, const name& name)
{
    return archive.deserialize_float(name);
}

template<>
[[nodiscard]] inline std::optional<double> deserialize_object_property<double>(const object_deserializer& archive, const name& name)
{
    return archive.deserialize_double(name);
}

template<>
[[nodiscard]] inline std::optional<bool> deserialize_object_property<bool>(const object_deserializer& archive, const name& name)
{
    return archive.deserialize_bool(name);
}

template<>
[[nodiscard]] inline std::optional<std::string> deserialize_object_property<std::string>(const object_deserializer& archive, const name& name)
{
    return archive.deserialize_string(name);
}

}

#endif
