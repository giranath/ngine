#ifndef NGINE_OBJECT_XML_DESERIALIZER_HPP
#define NGINE_OBJECT_XML_DESERIALIZER_HPP

#include "object_deserializer.hpp"
#include <pugixml.hpp>

#include <vector>
#include <istream>

namespace ng
{

/**
 * Object archiver that load xml
 */
class object_xml_deserializer final : public object_deserializer
{
    /**
     * Enumerates all supported loading modes
     */
    enum class loading_modes : uint8_t
    {
        object,
        array,
        dictionary
    };

    /**
     * The current state of the loader
     */
    struct state
    {
        pugi::xml_node node;
        std::size_t index;
        loading_modes mode;
        uint8_t version;

        state(loading_modes mode, pugi::xml_node node, uint8_t version);
    };

    // A stack of state
    std::vector<state> states_;
    pugi::xml_document doc_;

private:
    [[nodiscard]] pugi::xml_node find_property_node(const ng::string_name& name) const;
    [[nodiscard]] pugi::xml_node find_nth_child_node(std::size_t index, std::string_view name) const;
    [[nodiscard]] pugi::xml_node find_node_with_name(std::string_view name_to_find, std::string_view name_attribute, std::string_view node_name) const;

protected:
    [[nodiscard]] state& current_state() noexcept;
    [[nodiscard]] const state& current_state() const noexcept;

public:
    object_xml_deserializer(std::istream& stream);
    virtual ~object_xml_deserializer();

    [[nodiscard]] std::size_t size() const override final;
    void set_index(std::size_t new_index) override final;
    [[nodiscard]] std::size_t index() const override final;
    [[nodiscard]] ng::string_name name() const override final;
    [[nodiscard]] uint8_t version() const override final;

    [[nodiscard]] std::optional<uint64_t> deserialize_uint64(const ng::string_name& name, uint64_t def) const override final;
    [[nodiscard]] std::optional<int64_t> deserialize_int64(const ng::string_name& name, int64_t def) const override final;
    [[nodiscard]] std::optional<bool> deserialize_bool(const ng::string_name& name, bool def) const override final;
    [[nodiscard]] std::optional<double> deserialize_double(const ng::string_name& name, double def) const override final;
    [[nodiscard]] std::optional<std::string> deserialize_string(const ng::string_name& name, std::string_view def) const override final;

    void begin_object(const ng::string_name& name) override final;
    void end_object() override final;

    void begin_array(const ng::string_name& name) override final;
    void end_array() override final;

    void begin_dictionary(const ng::string_name& name) override final;
    void end_dictionary() override final;
};

}

#endif
