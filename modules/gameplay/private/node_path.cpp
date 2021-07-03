#include "node_path.hpp"
#include <sstream>
#include <cassert>

namespace ng
{

node_path::node_path() noexcept
: names_()
, absolute_{false}
, trailing_delimiter_{false}
{

}

node_path::node_path(std::string_view str)
: node_path{parse(str)}
{

}

node_path node_path::normalize() const
{
    node_path normalized_path = *this;

    // 1. Remove empty names
    {
        auto it = std::remove(normalized_path.names_.begin(), normalized_path.names_.end(), safe_name{});
        normalized_path.names_.erase(it, normalized_path.names_.end());
    }


    // 2. Resolve dots
    {
        auto it = std::remove(normalized_path.names_.begin(), normalized_path.names_.end(), safe_name{"."});
        normalized_path.names_.erase(it, normalized_path.names_.end());
    }

    // /hello/../world

    static const safe_name parent_node{".."};

    // 3. Resolve dot-dot
    for(int64_t i = normalized_path.names_.size() - 2; i >= 0; --i)
    {
        const std::size_t size = normalized_path.names_.size();

        if(normalized_path.names_[i + 1] == parent_node && normalized_path.names_[i] != parent_node)
        {
            std::swap(normalized_path.names_[i + 1], normalized_path.names_[size - 1]);
            std::swap(normalized_path.names_[i], normalized_path.names_[size - 2]);

            normalized_path.names_.pop_back();
            normalized_path.names_.pop_back();
            --i;
        }
    }

    return normalized_path;
}

node_path node_path::append(const node_path& path) const
{
    if(path.absolute() || empty())
    {
        return path;
    }
    else if(path.empty())
    {
        return *this;
    }
    else
    {
        node_path new_path = *this;
        for(const safe_name& name : path.names_)
        {
            new_path.names_.push_back(name);
        }
        new_path.trailing_delimiter_ = path.trailing_delimiter_;

        return new_path;
    }
}

node_path node_path::concat(const node_path& path) const
{
    if(empty())
    {
        return path;
    }
    else if(path.empty())
    {
        return *this;
    }
    else
    {
        return node_path(string() + path.string());
    }
}

node_path node_path::operator/(const node_path& path) const
{
    return append(path);
}

node_path node_path::operator+(const node_path& path) const
{
    return concat(path);
}

bool node_path::absolute() const noexcept
{
    // Check if path starts with /
    return absolute_;
}

bool node_path::empty() const noexcept
{
    return names_.empty() && !absolute_;
}

void node_path::clear()
{
    names_.clear();
    absolute_ = false;
    trailing_delimiter_ = false;
}

std::string node_path::string() const
{
    std::ostringstream stream;

    if(absolute_)
    {
        stream << delimiter_char;
    }

    for(const safe_name& name : names_)
    {
        stream << name.c_str() << delimiter_char;
    }

    const std::string string = stream.str();

    // Remove last trailing delimiter if there was none
    if(!trailing_delimiter_ && !names_.empty())
    {
        return string.substr(0, string.size() - 1);
    }

    return string;
}

bool node_path::operator==(const node_path& other) const noexcept
{
    return absolute_ == other.absolute_
        && std::equal(names_.begin(), names_.end(), other.names_.begin(), other.names_.end())
        && trailing_delimiter_ == other.trailing_delimiter_;
}

bool node_path::operator!=(const node_path& other) const noexcept
{
    return absolute_ != other.absolute_
        || !std::equal(names_.begin(), names_.end(), other.names_.begin(), other.names_.end())
        || trailing_delimiter_ != other.trailing_delimiter_;
}

node_path_iterator node_path::begin() const
{
    return node_path_iterator{*this, 0};
}

node_path_iterator node_path::end() const
{
    return node_path_iterator{*this, names_.size()};
}

node_path node_path::parse(std::string_view str)
{
    node_path new_path;

    if(!str.empty())
    {
        std::size_t offset = 0;

        if(str[0] == '/')
        {
            new_path.absolute_ = true;
            offset = 1;
        }

        std::string buffer;
        // Iterate over each character in string
        for(std::size_t i = offset; i < str.size(); ++i)
        {
            if(str[i] == delimiter_char)
            {
                new_path.names_.emplace_back(buffer);
                buffer.clear();
            }
            else
            {
                buffer.push_back(str[i]);
            }
        }

        if(!buffer.empty())
        {
            new_path.names_.emplace_back(buffer);
        }
        else if(!new_path.names_.empty())
        {
            new_path.trailing_delimiter_ = true;
        }
    }

    return new_path;
}

std::ostream& operator<<(std::ostream& out, const node_path& path)
{
    return out << '"' << path.string() << '"';
}

//
// ITERATOR implementation
//

node_path_iterator::node_path_iterator()
: owner_{nullptr}
, index_{0}
{

}

node_path_iterator& node_path_iterator::operator++()
{
    assert(owner_);
    assert(index_ < owner_->names_.size());

    ++index_;

    return *this;
}

node_path_iterator node_path_iterator::operator++(int)
{
    node_path_iterator tmp{*this};

    ++(*this);

    return tmp;
}

node_path_iterator& node_path_iterator::operator--()
{
    assert(owner_);
    assert(index_ > 0);

    --index_;

    return *this;
}

node_path_iterator node_path_iterator::operator--(int)
{
    node_path_iterator tmp{*this};

    --(*this);

    return tmp;
}

bool node_path_iterator::operator==(const node_path_iterator& other) const noexcept
{
    return owner_ == other.owner_
        && index_ == other.index_;
}

bool node_path_iterator::operator!=(const node_path_iterator& other) const noexcept
{
    return owner_ != other.owner_
        || index_ != other.index_;
}

const safe_name& node_path_iterator::operator*() const noexcept
{
    assert(owner_);
    assert(index_ < owner_->names_.size());

    return owner_->names_[index_];
}

node_path_iterator::pointer node_path_iterator::operator->() const noexcept
{
    assert(owner_);
    assert(index_ < owner_->names_.size());

    return &owner_->names_[index_];
}

node_path_iterator::node_path_iterator(const node_path& path, std::size_t index)
: owner_{&path}
, index_{index}
{

}
}