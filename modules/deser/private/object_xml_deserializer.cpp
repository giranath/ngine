#include "object_xml_deserializer.hpp"
#include <cassert>
#include <limits>
#include <numeric>

namespace ng
{

object_xml_deserializer::state::state(loading_modes mode, pugi::xml_node node, uint8_t version)
: node{node}
, index{0}
, mode{mode}
, version{version}
{

}

object_xml_deserializer::state& object_xml_deserializer::current_state() noexcept
{
    assert(!states_.empty());

    return states_.back();
}

const object_xml_deserializer::state& object_xml_deserializer::current_state() const noexcept
{
    assert(!states_.empty());

    return states_.back();
}

pugi::xml_node object_xml_deserializer::find_property_node(const ng::string_name& name) const
{
    const state& s = current_state();

    pugi::xml_node next_node;
    switch(s.mode)
    {
    case loading_modes::object:
        if(name == string_name::none)
        {
            return find_nth_child_node(s.index, "field");
        }
        else
        {
            return find_node_with_name(name.c_str(), "name", "field");
        }
    case loading_modes::array:
        return find_nth_child_node(s.index, "entry");
    case loading_modes::dictionary:
        if(name == string_name::none)
        {
            return find_nth_child_node(s.index, "entry");
        }
        else
        {
            return find_node_with_name(name.c_str(), "key", "entry");
        }
    default:
        assert(false);
        break;
    }

    return next_node;
}

pugi::xml_node object_xml_deserializer::find_nth_child_node(std::size_t index, std::string_view name) const
{
    const state& s = current_state();

    pugi::xml_node next_node;
    std::size_t node_index = 0;
    for (pugi::xml_node node = s.node.first_child(); node && !next_node; node = node.next_sibling())
    {
        if (std::strcmp(node.name(), name.data()) == 0)
        {
            if (node_index == index)
            {
                next_node = node;
            }

            ++node_index;
        }
    }

    return next_node;
}

pugi::xml_node object_xml_deserializer::find_node_with_name(std::string_view name_to_find, std::string_view name_attribute_str, std::string_view node_name) const
{
    const state& s = current_state();

    return s.node.find_child([name_attribute_str, node_name, name_to_find](const pugi::xml_node& node)
    {
        pugi::xml_attribute name_attribute = node.attribute(name_attribute_str.data());

        return name_attribute
             && std::strcmp(node.name(), node_name.data()) == 0
             && std::strcmp(name_to_find.data(), name_attribute.as_string()) == 0;
    });
}

object_xml_deserializer::object_xml_deserializer(std::istream& stream)
: states_{}
, doc_{}
{
    pugi::xml_parse_result result = doc_.load(stream);

    if(!result)
    {
        throw std::runtime_error{"invalid xml"};
    }

    pugi::xml_node document_node = doc_.document_element();

    if(std::strcmp(document_node.name(), "object") != 0)
    {
        throw std::runtime_error{"invalid xml root element for object archive"};
    }

    // Check the version attribute of the object node
    pugi::xml_attribute version_attribute = document_node.attribute("version");

    // Add a state for the root node
    states_.emplace_back(loading_modes::object, doc_.document_element(), version_attribute.as_uint(0));
}

object_xml_deserializer::~object_xml_deserializer()
{
    // We forgot to end subobject or subarrays
    assert(states_.size() == 1);
}

std::size_t object_xml_deserializer::size() const
{
    const state& s = current_state();

    switch(s.mode)
    {
    case loading_modes::object:
        return std::accumulate(s.node.begin(), s.node.end(), 0ull, [](std::size_t count, const pugi::xml_node& node)
        {
            if(node.type() == pugi::xml_node_type::node_element
            && std::strcmp(node.name(), "field") == 0)
            {
                return count + 1;
            }

            return count;
        });
        break;
    case loading_modes::array:
    case loading_modes::dictionary:
        // Count field children
        return std::accumulate(s.node.begin(), s.node.end(), 0ull, [](std::size_t count, const pugi::xml_node& node)
        {
            if(node.type() == pugi::xml_node_type::node_element
               && std::strcmp(node.name(), "entry") == 0)
            {
                return count + 1;
            }

            return count;
        });
    default:
        assert(false);
        break;
    }

    return 0;
}

void object_xml_deserializer::set_index(std::size_t new_index)
{
    state& s = current_state();
    s.index = new_index;
}

std::size_t object_xml_deserializer::index() const
{
    const state& s = current_state();

    return s.index;
}

string_name object_xml_deserializer::name() const
{
    const state& s = current_state();

    pugi::xml_node nth_child;
    pugi::xml_attribute name_attribute;
    switch(s.mode)
    {
    case loading_modes::object:
        nth_child = find_nth_child_node(s.index, "field");
        name_attribute = nth_child.attribute("name");

        return ng::string_name{name_attribute.as_string()};
    case loading_modes::dictionary:
        nth_child = find_nth_child_node(s.index, "entry");
        name_attribute = nth_child.attribute("key");

        return ng::string_name{name_attribute.as_string()};
    default:
        return string_name::none;
    }
}

uint8_t object_xml_deserializer::version() const
{
    const state& s = current_state();

    return s.version;
}

std::optional<uint64_t> object_xml_deserializer::deserialize_uint64(const ng::string_name& name, uint64_t def) const
{
    pugi::xml_node next_node = find_property_node(name);

    // Read the value attribute
    pugi::xml_attribute value_attribute = next_node.attribute("value");
    if(value_attribute)
    {
        return value_attribute.as_ullong(def);
    }

    return std::nullopt;
}

std::optional<int64_t> object_xml_deserializer::deserialize_int64(const ng::string_name& name, int64_t def) const
{
    pugi::xml_node next_node = find_property_node(name);

    // Read the value attribute
    pugi::xml_attribute value_attribute = next_node.attribute("value");
    if(value_attribute)
    {
        return value_attribute.as_llong(def);
    }

    return std::nullopt;
}

std::optional<bool> object_xml_deserializer::deserialize_bool(const ng::string_name& name, bool def) const
{
    pugi::xml_node next_node = find_property_node(name);

    // Read the value attribute
    pugi::xml_attribute value_attribute = next_node.attribute("value");
    if(value_attribute)
    {
        return value_attribute.as_bool(def);
    }

    return std::nullopt;
}

std::optional<double> object_xml_deserializer::deserialize_double(const ng::string_name& name, double def) const
{
    pugi::xml_node next_node = find_property_node(name);

    // Read the value attribute
    pugi::xml_attribute value_attribute = next_node.attribute("value");
    if(value_attribute)
    {
        return value_attribute.as_double(def);
    }

    return std::nullopt;
}

std::optional<std::string> object_xml_deserializer::deserialize_string(const ng::string_name& name, std::string_view def) const
{
    pugi::xml_node next_node = find_property_node(name);

    // Read the value attribute
    pugi::xml_attribute value_attribute = next_node.attribute("value");
    if(value_attribute)
    {
        return value_attribute.as_string(def.data());
    }
    // String can be written as text under node
    else
    {
        return next_node.text().as_string(def.data());
    }
}

void object_xml_deserializer::begin_object(const ng::string_name& name)
{
    pugi::xml_node next_node = find_property_node(name);

    pugi::xml_attribute version_attribute = next_node.attribute("version");
    states_.emplace_back(loading_modes::object, next_node, version_attribute.as_uint(0));
}

void object_xml_deserializer::end_object()
{
    assert(states_.back().mode == loading_modes::object);
    states_.pop_back();
}

void object_xml_deserializer::begin_array(const ng::string_name& name)
{
    pugi::xml_node next_node = find_property_node(name);
    states_.emplace_back(loading_modes::array, next_node, current_state().version);
}

void object_xml_deserializer::end_array()
{
    assert(states_.back().mode == loading_modes::array);
    states_.pop_back();
}

void object_xml_deserializer::begin_dictionary(const ng::string_name& name)
{
    pugi::xml_node next_node = find_property_node(name);
    states_.emplace_back(loading_modes::dictionary, next_node, current_state().version);
}

void object_xml_deserializer::end_dictionary()
{
    assert(states_.back().mode == loading_modes::dictionary);
    states_.pop_back();
}

std::unique_ptr<object_deserializer> make_object_xml_deserializer(std::istream& stream)
{
    return std::make_unique<object_xml_deserializer>(stream);
}

}