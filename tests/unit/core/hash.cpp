#include "catch.hpp"
#include <ng/core/hash.hpp>

using namespace ng::literals;

TEST_CASE("A hash created at compile time should yield the same value as a hash created at runtime if both have the same value", "[hash]" ) {
    constexpr uint64_t compile_time_hash = "hello world"_h;

    const char* hello_world_ctr = "hello world";

    const uint64_t runtime_hash = ng::hash(hello_world_ctr, std::strlen(hello_world_ctr));

    REQUIRE(compile_time_hash == runtime_hash);
}

TEST_CASE("Two different values being hashed should yield two different hashes", "[hash]")
{
    REQUIRE_FALSE("hello"_h == "world"_h);
}

TEST_CASE("The hash function should return the same value as a reference implementation of FNV1A64", "[hash]")
{
    const uint64_t expected_hash_value = 0x779a65e7023cd2e7;

    REQUIRE("hello world"_h == expected_hash_value);
}

TEST_CASE("The hash function should yield the same result everytime the same value is used", "[hash]")
{
    const uint64_t first_hash_value = ng::hash("hello world");
    const uint64_t second_hash_value = ng::hash("hello world");

    REQUIRE(first_hash_value == second_hash_value);
}