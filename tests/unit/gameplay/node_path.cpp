#include <catch.hpp>
#include <ng/gameplay/node_path.hpp>

using namespace ng::literals;

TEST_CASE("A path can be normalized", "[nodepath]")
{
    SECTION("A normalized path will be unchanged")
    {
        REQUIRE("/hello/world"_node.normalize() == "/hello/world"_node);
    }

    SECTION("A final delimiter will be removed")
    {
        REQUIRE("/hello/world/"_node.normalize() == "/hello/world/"_node);
    }

    SECTION("Multiple delimiters will be combined into a single delimiter")
    {
        REQUIRE("//hello////////World//"_node.normalize() == "/hello/World/"_node);
    }

    SECTION("Normalizing an empty path will return an empty path")
    {
        REQUIRE(""_node.normalize() == ""_node);
    }

    SECTION("Normalizing paths with dots should remove them")
    {
        REQUIRE("./hello/../././hello/./world/../world"_node.normalize() == "hello/world"_node);
        REQUIRE("/hello/../././hello/./world/../world/.."_node.normalize() == "/hello"_node);
        REQUIRE("../../.."_node.normalize() == "../../.."_node);
        REQUIRE("hello/../../"_node.normalize() == "../"_node);
    }
}

TEST_CASE("A path can be concatenated to another path", "[nodepath]")
{
    SECTION("concatenating an absolute path with another absolute path returns the concatenated path")
    {
        REQUIRE("/hello/world/"_node + "/new"_node == "/hello/world//new"_node);
    }

    SECTION("concatenating an empty path with another empty path results into an empty path")
    {
        REQUIRE(""_node + ""_node == ""_node);
    }
}

TEST_CASE("A path can be appended to another path", "[nodepath]")
{
    SECTION("appending an absolute path will replace the original path")
    {
        REQUIRE("/hello/"_node / "/world"_node == "/world"_node);
    }

    SECTION("appending an empty path will not change the original path")
    {
        REQUIRE("/hello/"_node / ""_node == "/hello/"_node);
    }

    SECTION("appending to an empty path will return the appended path")
    {
        REQUIRE(""_node / "pouet pouet"_node == "pouet pouet"_node);
    }

    SECTION("appending two relative paths will form a relative path")
    {
        REQUIRE("hello/world/patate"_node / "pouet/hurry"_node == "hello/world/patate/pouet/hurry"_node);
    }
}