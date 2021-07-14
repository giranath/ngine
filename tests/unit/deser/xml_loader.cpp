#include <catch.hpp>
#include <ng/deser/object_deserializer.hpp>
#include <sstream>
#include <map>

using namespace ng::literals;

TEST_CASE("An object can be loaded from an XML source", "[xml_loader]")
{
    SECTION("An invalid XML source will throw an exception")
    {
        const std::string xml_source = R"(<object>
    <field
</object>)";

        std::istringstream source_stream{xml_source};
        std::unique_ptr<ng::object_deserializer> deserializer;
        REQUIRE_THROWS_AS(deserializer = ng::make_object_xml_deserializer(source_stream), std::runtime_error);
    }

    SECTION("A valid XML source should contains a serialized object")
    {
        const std::string xml_source = R"(<object>
    <field name="test" value="10"/>
    <field name="long text">This is a very long text with multiple spaces. It makes more sens to store this text inside a textnode instead of an attribute</field>
    <field name="dictionary">
        <entry key="test" value="hello" />
        <entry key="hello" value="world" />
    </field>
    <field name="array">
        <entry value="10" />
        <entry value="13" />
    </field>
    <field name="object" version="1">
        <field name="value" value="666" />
    </field>
</object>)";

        std::istringstream source_stream{xml_source};
        auto archiver = ng::make_object_xml_deserializer(source_stream);

        REQUIRE(archiver->version() == 0);

        // Load integer
        const int test = ng::deserialize_object_property<int>(*archiver, "test"_name).value();
        REQUIRE(test == 10);

        // Load long string
        const auto long_text = archiver->deserialize_string("long text"_name);
        REQUIRE(*long_text == "This is a very long text with multiple spaces. It makes more sens to store this text inside a textnode instead of an attribute");

        // Load dictionary
        std::map<std::string, std::string> dictionary;
        archiver->begin_dictionary("dictionary"_name);
        REQUIRE(archiver->version() == 0);
        {
            const std::size_t size = archiver->size();
            REQUIRE(size == 2);

            for(std::size_t i = 0; i < size; ++i)
            {
                archiver->set_index(i);
                const ng::string_name key = archiver->name();

                const auto value = archiver->deserialize_string(ng::string_name::none);

                dictionary[key.string()] = *value;
            }
        }

        // Fetch specific key
        const auto test_entry_value = archiver->deserialize_string("test"_name);
        REQUIRE(test_entry_value == "hello");

        const auto test_entry_invalid_type = archiver->deserialize_int16("test"_name);
        REQUIRE(*test_entry_invalid_type == 0);

        archiver->end_dictionary();

        REQUIRE(dictionary["test"] == "hello");
        REQUIRE(dictionary["hello"] == "world");

        // Load array
        std::vector<int> array;
        archiver->begin_array("array"_name);
        REQUIRE(archiver->version() == 0);
        {
            const std::size_t size = archiver->size();
            REQUIRE(size == 2);

            array.resize(size);
            for (std::size_t i = 0; i < size; ++i)
            {
                archiver->set_index(i);
                array[i] = *archiver->deserialize_int32(ng::string_name::none);
            }

            const std::vector<int> expected_array{10, 13};
            REQUIRE(array == expected_array);
        }
        archiver->end_array();

        // Load subobject
        REQUIRE(archiver->version() == 0);
        archiver->begin_object("object"_name);
        {
            REQUIRE(archiver->version() == 1);
            const auto value = archiver->deserialize_int32("value"_name);
            REQUIRE(*value == 666);
        }
        archiver->end_object();

        // Trying to get an unexisting field will return nullopt
        REQUIRE(archiver->deserialize_int8("unexisting"_name) == std::nullopt);
    }
}