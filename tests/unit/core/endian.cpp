#include <catch.hpp>
#include <ng/core/endian.hpp>

TEST_CASE("It's possible to swap endianness of an unsigned integer", "[core]")
{
    SECTION("a uint8 is unchanged")
    {
        const uint8_t byte = 0xAB;

        REQUIRE(ng::swap_bytes(byte) == byte);
    }

    SECTION("a uint16 is swapped")
    {
        const uint16_t value = 0x0123;
        const uint16_t expected_value = 0x2301;

        REQUIRE(ng::swap_bytes(value) == expected_value);
    }

    SECTION("a uint32 is swapped")
    {
        const uint32_t value = 0x01234567;
        const uint32_t expected_value = 0x67452301;

        REQUIRE(ng::swap_bytes(value) == expected_value);
    }

    SECTION("a uint64 is swapped")
    {
        const uint64_t value = 0x0123456789ABCDEF;
        const uint64_t expected_value = 0xEFCDAB8967452301;

        REQUIRE(ng::swap_bytes(value) == expected_value);
    }
}

TEST_CASE("It's possible to swap endianness of a signed integer", "[core]")
{
    SECTION("a int8 is unchanged")
    {
        const int8_t byte = -14;
        const int8_t expected_byte = -14;

        const int8_t res = ng::swap_bytes(byte);

        REQUIRE(res == expected_byte);
    }

    SECTION("a int16 is swapped")
    {
        const int16_t value = 0x0123;
        const int16_t expected_value = 0x2301;

        REQUIRE(ng::swap_bytes(value) == expected_value);
    }

    SECTION("a int32 is swapped")
    {
        const int32_t value = 0x01234567;
        const int32_t expected_value = 0x67452301;

        REQUIRE(ng::swap_bytes(value) == expected_value);
    }

    SECTION("a int64 is swapped")
    {
        const int64_t value = 0x0123456789ABCDEF;
        const int64_t expected_value = 0xEFCDAB8967452301;

        REQUIRE(ng::swap_bytes(value) == expected_value);
    }
}