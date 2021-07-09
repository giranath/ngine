#include "name.hpp"
#include "name_table.hpp"
#include <utility>

namespace ng
{

name::name(std::string_view string)
: entry_(string.empty() ? nullptr : name_table::get().find_or_add(string))
{
}

name::name(const name& other)
: entry_{other.entry_}
{
    if(entry_)
    {
        entry_->addref();
    }
}

name::name(name&& other) noexcept
: entry_{std::exchange(other.entry_, nullptr)}
{

}

name::~name()
{
    name_table::get().release(entry_);
}

name& name::operator=(const name& other)
{
    if(&other != this)
    {
        name_table::get().release(entry_);

        entry_ = other.entry_;
        if(entry_)
        {
            entry_->addref();
        }
    }

    return *this;
}

name& name::operator=(name&& other) noexcept
{
    if(&other != this)
    {
        name_table::get().release(entry_);

        entry_ = std::exchange(other.entry_, nullptr);
    }

    return *this;
}

void name::swap(name& other) noexcept
{
    using std::swap;

    swap(entry_, other.entry_);
}

void name::clear()
{
    name_table::get().release(entry_);
    entry_ = nullptr;
}

bool name::empty() const noexcept
{
    return entry_ == nullptr;
}

const char* name::c_str() const noexcept
{
    return entry_ ? entry_->c_str() : nullptr;
}

std::string name::string() const noexcept
{
    return entry_ ? entry_->string() : std::string{};
}

bool name::operator==(const name& other) const noexcept
{
    return entry_ == other.entry_;
}

bool name::operator!=(const name& other) const noexcept
{
    return entry_ != other.entry_;
}

void swap(name& a, name& b) noexcept
{
    a.swap(b);
}

}