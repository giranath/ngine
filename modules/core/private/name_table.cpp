#include "name_table.hpp"
#include "hash.hpp"
#include <cassert>
#include <algorithm>
#include <numeric>

namespace ng
{

name_table_entry::name_table_entry(std::string_view str, uint64_t hash, name_table_entry* next, std::size_t table_index)
: next_{next}
, prev_{next_ ? next_->prev_ : nullptr}
, string_{str}
, hash_{hash}
, refcount_{1}
, table_index_{table_index}
{

}

name_table_entry::~name_table_entry()
{
    // We assume that the table entry was correctly released
    assert(refcount_ == 0);
}

void name_table_entry::addref()
{
    ++refcount_;
}

bool name_table_entry::release()
{
    --refcount_;

    return refcount_ == 0;
}

const char* name_table_entry::c_str() const noexcept
{
    return string_.c_str();
}

std::string name_table_entry::string() const noexcept
{
    return string_;
}

name_table::name_table()
: entries_{nullptr}
, mutex_{}
{

}

name_table::~name_table()
{
    std::unique_lock lock(mutex_);

    for(std::size_t i = 0; i < entry_count; ++i)
    {
        // We assume that the table was correctly released
        assert(entries_[i] == nullptr);

        while(entries_[i])
        {
            name_table_entry* entry = entries_[i];
            entries_[i] = entry->next_;

            delete entry;
        }
    }
}

name_table_entry* name_table::find_or_add(std::string_view str)
{
    std::unique_lock lock(mutex_);

    const uint64_t str_hash = hash(str);
    const std::size_t table_index = str_hash & mask;

    // Search for an existing entry
    for(name_table_entry* entry = entries_[table_index]; entry; entry = entry->next_)
    {
        // Check if the entry contains the same string
        if(entry->hash_ == str_hash && entry->string_ == str)
        {
            entry->addref();
            return entry;
        }
    }

    // Create a new entry
    name_table_entry* new_entry = new name_table_entry{str, str_hash, entries_[table_index], table_index};
    entries_[table_index] = new_entry;

    return new_entry;
}

name_table_entry* name_table::find(std::string_view str) const
{
    std::shared_lock lock(mutex_);

    // Get the hash of the string
    const uint64_t str_hash = hash(str);
    const std::size_t table_index = str_hash & mask;

    for(name_table_entry* entry = entries_[table_index]; entry; entry = entry->next_)
    {
        // Check if the entry contains the same string
        if(entry->hash_ == str_hash && entry->string_ == str)
        {
            return entry;
        }
    }

    return nullptr;
}

void name_table::release(name_table_entry* entry)
{
    std::unique_lock lock(mutex_);

    if(entry && entry->release())
    {
        // Remove entry from table

        // If entry was not the root entry from it's table
        if(entry->prev_)
        {
            entry->prev_->next_ = entry->next_;
        }
        // If the entry is the root, we need to make it's next entry the new root
        else
        {
            entries_[entry->table_index_] = entry->next_;
        }

        if(entry->next_)
        {
            entry->next_->prev_ = entry->prev_;
        }

        // Now we can free memory for this entry
        delete entry;
    }
}

bool name_table::empty() const noexcept
{
    std::shared_lock lock(mutex_);

    return std::all_of(std::begin(entries_), std::end(entries_), [](const name_table_entry* entry) { return entry == nullptr; });
}

std::size_t name_table::size() const noexcept
{
    std::shared_lock lock(mutex_);

    return std::accumulate(std::begin(entries_), std::end(entries_), 0ull, [](std::size_t count, const name_table_entry* entry)
    {
        std::size_t linked_list_size = 0;
        for(const name_table_entry* it = entry; it; it = it->next_)
        {
            ++linked_list_size;
        }

        return count + linked_list_size;
    });
}

name_table& name_table::get()
{
    static name_table instance;

    return instance;
}

}