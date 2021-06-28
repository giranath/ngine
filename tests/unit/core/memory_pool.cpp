#include <catch.hpp>
#include <ng/core/memory_pool.hpp>

struct mock_fat_type
{
    std::aligned_storage<32, alignof(std::max_align_t)>::type storage;
};

TEST_CASE("A memory pool will notify when it doesn't have enough capacity for allocation", "[memory_pool_view]")
{
    ng::type_memory_pool_view<uint8_t> pool(nullptr, 0);

    SECTION("will throw a bad_alloc exception when no memory can be allocated")
    {
        REQUIRE_THROWS_AS(pool.allocate(), std::bad_alloc);
    }

    SECTION("will return a nullptr when an exception is unwanted")
    {
        REQUIRE(pool.allocate(std::nothrow) == nullptr);
    }
}

TEST_CASE("A memory pool will allocate correctly aligned objects", "[memory_pool_view]")
{
    uint8_t pool_memory[128];

    SECTION("even when object size is small")
    {
        ng::type_memory_pool_view<uint8_t> pool(pool_memory, sizeof(pool_memory));

        void* allocated_memory = pool.allocate();
        REQUIRE(ng::is_address_aligned(allocated_memory, alignof(uint8_t)));

        pool.free(allocated_memory);
    }

    SECTION("even when object is is big")
    {
        ng::type_memory_pool_view<mock_fat_type> pool(pool_memory, sizeof(pool_memory));

        void* allocated_memory = pool.allocate();
        REQUIRE(ng::is_address_aligned(allocated_memory, alignof(mock_fat_type)));

        pool.free(allocated_memory);
    }
}