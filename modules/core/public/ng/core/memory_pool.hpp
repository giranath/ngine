#ifndef NGINE_CORE_MEMORY_POOL_HPP
#define NGINE_CORE_MEMORY_POOL_HPP

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <memory>
#include <vector>

namespace ng
{

/**
 * Check if an address is correctly aligned
 * @param ptr The pointer to check for alignment
 * @param alignment The alignment the pointer must have
 * @return true when the pointer is correctly aligned or false otherwise
 */
inline bool is_address_aligned(const void* ptr, std::size_t alignment) noexcept
{
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(ptr);

    return (address % alignment) == 0;
}

/**
 * A pool of memory
 * @tparam ObjectSize The size of individual elements inside the pool
 * @tparam ObjectAlignment The alignment individual elements inside the pool must have
 * @note The pool doesn't own the memory is manages,
 */
template<std::size_t ObjectSize, std::size_t ObjectAlignment>
class memory_pool_view
{
    //using storage_type = typename std::aligned_storage<ObjectSize, ObjectAlignment>::type;

    struct object_cell
    {
        object_cell* next;

        constexpr explicit object_cell(object_cell* next) noexcept
        : next{next}
        {

        }
    };

    object_cell* free_;
    std::size_t count_;

public:
    // View cannot be copied
    memory_pool_view(const memory_pool_view& other) = delete;
    memory_pool_view& operator=(const memory_pool_view&) = delete;

    memory_pool_view() noexcept
    : free_{nullptr}
    , count_{0}
    {

    }

    memory_pool_view(uint8_t* memory, std::size_t capacity) noexcept
    : free_{nullptr}
    , count_{0}
    {
        const std::size_t block_size = std::max(sizeof(object_cell), ObjectSize);
        const std::size_t block_alignment = std::max(alignof(object_cell), ObjectAlignment);

        void* aligned_memory_address = memory;
        std::size_t aligned_capacity = capacity;
        aligned_memory_address = std::align(block_alignment, capacity, aligned_memory_address, aligned_capacity);

        uint8_t* aligned_memory = reinterpret_cast<uint8_t*>(aligned_memory_address);

        // Now we initialize every blocks
        object_cell* previous_cell = nullptr;
        const std::size_t block_count = aligned_capacity / block_size;
        for(std::size_t block_index = 0; block_index < block_count; ++block_index)
        {
            // TODO: Check if padding must be added if object alignment is bigger than the cell's
            previous_cell = new(aligned_memory + block_index * block_size) object_cell{previous_cell};

            assert(is_address_aligned(previous_cell, alignof(object_cell)));
        }

        free_ = previous_cell;
        count_ = block_count;
    }

    ~memory_pool_view() noexcept
    {
        assert(empty());
    }

    /**
     * Allocate a block of memory
     * @return The allocated memory or throw an exception if not enough memory is available
     */
    [[nodiscard]] void* allocate()
    {
        if(free_)
        {
            object_cell* allocated_cell = free_;
            free_ = free_->next;

            // allocated_address is an unitialized memory block
            uint8_t* allocated_address = reinterpret_cast<uint8_t*>(allocated_cell);

            // Call destructor on allocated cell to unitialize memory
            allocated_cell->~object_cell();

            // TODO: Check if padding must be added for object alignment
            return allocated_address;
        }
        else
        {
            throw std::bad_alloc{};
        }
    }

    /**
     * Allocate a block of memory
     * @return the allocated memory or nullptr if not enough memory is available
     */
    [[nodiscard]] void* allocate(std::nothrow_t) noexcept
    {
        if(free_)
        {
            object_cell* allocated_cell = free_;
            free_ = free_->next;

            // allocated_address is an unitialized memory block
            uint8_t* allocated_address = reinterpret_cast<uint8_t*>(allocated_cell);

            // Call destructor on allocated cell to unitialize memory
            allocated_cell->~object_cell();

            // TODO: Check if padding must be added for object alignment
            return allocated_address;
        }

        return nullptr;
    }

    /**
     * Free a block of memory from the pool
     * @param memory The memory to free
     */
    void free(void* memory) noexcept
    {
        uint8_t* memory_address = reinterpret_cast<uint8_t*>(memory);

        // TODO: Check if padding must be added to convert back to object_cell address

        // At this point we assume that memory is unitialized
        object_cell* new_freed_cell = new(memory) object_cell{free_};
        assert(is_address_aligned(new_freed_cell, alignof(object_cell)));

        free_ = new_freed_cell;
    }

    /**
     * Returns the number of elements that can be stored inside this pool
     * @return The number of elements that can be stored inside this pool
     */
    [[nodiscard]] std::size_t capacity() const noexcept
    {
        return count_;
    }

    /**
     * Returns the number of allocated elements
     * @return The number of allocated elements
     */
    [[nodiscard]] std::size_t size() const noexcept
    {
        std::size_t free_count = 0;

        object_cell* cell_it = free_;
        while(cell_it)
        {
            ++free_count;
            cell_it = cell_it->next;
        }

        return count_ - free_count;
    }

    /**
     * Check if this pool has allocated no elements
     * @return true when no elements were allocated by this pool
     */
    [[nodiscard]] bool empty() const noexcept
    {
        return size() == 0;
    }
};

template<typename T>
using type_memory_pool_view = memory_pool_view<sizeof(T), alignof(T)>;

}

#endif
