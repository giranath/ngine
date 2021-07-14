#include "string_name.hpp"
#include "name_table.hpp"
#include <utility>

namespace ng
{

const string_name string_name::none{};

string_name::string_name(std::string_view string)
: entry_(string.empty() ? nullptr : name_table::get().find_or_add(string))
{
}

string_name::string_name(const string_name& other)
: entry_{other.entry_}
{
    if(entry_)
    {
        entry_->addref();
    }
}

string_name::string_name(string_name&& other) noexcept
: entry_{std::exchange(other.entry_, nullptr)}
{

}

string_name::~string_name()
{
    name_table::get().release(entry_);
}

string_name& string_name::operator=(const string_name& other)
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

string_name& string_name::operator=(string_name&& other) noexcept
{
    if(&other != this)
    {
        name_table::get().release(entry_);

        entry_ = std::exchange(other.entry_, nullptr);
    }

    return *this;
}

void string_name::swap(string_name& other) noexcept
{
    using std::swap;

    swap(entry_, other.entry_);
}

void string_name::clear()
{
    name_table::get().release(entry_);
    entry_ = nullptr;
}

bool string_name::empty() const noexcept
{
    return entry_ == nullptr;
}

const char* string_name::c_str() const noexcept
{
    return entry_ ? entry_->c_str() : nullptr;
}

std::string string_name::string() const noexcept
{
    return entry_ ? entry_->string() : std::string{};
}

bool string_name::operator==(const string_name& other) const noexcept
{
    return entry_ == other.entry_;
}

bool string_name::operator!=(const string_name& other) const noexcept
{
    return entry_ != other.entry_;
}

void swap(string_name& a, string_name& b) noexcept
{
    a.swap(b);
}

}