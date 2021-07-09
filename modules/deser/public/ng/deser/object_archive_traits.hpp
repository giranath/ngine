#ifndef NGINE_OBJECT_ARCHIVE_TRAITS_HPP
#define NGINE_OBJECT_ARCHIVE_TRAITS_HPP

#include <type_traits>
#include <cstdint>
#include <string>

namespace ng
{

/**
 * Traits to check if a type is a valid object property for serialization
 * @tparam T
 */
template<typename T>
struct is_object_property_type : std::false_type {};

/**
 * Constant to check if a type is a valid object property for serialization
 * @tparam T
 */
template<typename T>
static constexpr bool is_object_property_type_v = is_object_property_type<T>::value;

template<>
struct is_object_property_type<uint8_t> : std::true_type {};


template<>
struct is_object_property_type<uint16_t> : std::true_type {};


template<>
struct is_object_property_type<uint32_t> : std::true_type {};

template<>
struct is_object_property_type<uint64_t> : std::true_type {};

template<>
struct is_object_property_type<int8_t> : std::true_type {};

template<>
struct is_object_property_type<int16_t> : std::true_type {};

template<>
struct is_object_property_type<int32_t> : std::true_type {};

template<>
struct is_object_property_type<int64_t> : std::true_type {};

template<>
struct is_object_property_type<float> : std::true_type {};

template<>
struct is_object_property_type<double> : std::true_type {};

template<>
struct is_object_property_type<bool> : std::true_type {};

template<>
struct is_object_property_type<std::string> : std::true_type {};

}

#endif
