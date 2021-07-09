#include "catch.hpp"
#include <ng/core/hash.hpp>
#include <ng/core/name.hpp>

static const char* known_hash_collisions[2] = {
        "8yn0iYCKYHlIj4-BwPqk",
        "GReLUrM4wMqfg9yzV3KQ"
};

TEST_CASE("Two string that are expected to collide should collide", "[name]")
{
    // First validate that the two strings does collide
    REQUIRE(ng::hash(known_hash_collisions[0]) == ng::hash(known_hash_collisions[1]));
}

TEST_CASE("A name should safely handle hash collisions", "[name]" ) {
    const ng::name name_1{known_hash_collisions[0]};
    const ng::name name_2{known_hash_collisions[1]};

    REQUIRE_FALSE(name_1 == name_2);
}

TEST_CASE("An inline name should safely handle hash collisions", "[name]" ) {
    const ng::name name_1{known_hash_collisions[0]};
    const ng::name name_2{known_hash_collisions[1]};

    REQUIRE_FALSE(name_1 == name_2);
}