#include "node.hpp"
#include <algorithm>

#include <cassert>

namespace ng
{

void node::set_owner(node_tree* owner)
{
    owner_ = owner;

    for(auto& child : children_)
    {
        child->set_owner(owner);
    }
}

node::node(string_name name, node* parent) noexcept
: name_{std::move(name)}
// A node is owned by the same tree as its parent
, owner_{nullptr}
, parent_{nullptr}
, children_()
, decorators_()
{
    if(parent)
    {
        parent->attach_child(this);
    }
}

const string_name& node::name() const noexcept
{
    return name_;
}

void node::add_decorator(std::unique_ptr<node_decorator> decorator)
{
    decorators_.push_back(std::move(decorator));
}

void node::clear_decorators()
{
    decorators_.clear();
}

void node::attach_to(node* parent)
{
    assert(parent);

    parent->attach_child(this);
}

void node::detach_from_parent()
{
    if(parent_)
    {
        parent_->detach_child(this);
    }
}

void node::detach_child(node* child)
{
    assert(child);

    for(std::size_t i = 0; i < children_.size(); ++i)
    {
        if(children_[i] == child)
        {
            // Make sure the children doesn't reference this node
            child->parent_ = nullptr;

            std::swap(children_[i], children_.back());
            children_.pop_back();
        }
    }
}

void node::attach_child(node* child)
{
    assert(child);

    // If this node already has a child with the same name as the new node to add
    // we cannot proceed further
    if(std::any_of(children_.begin(), children_.end(), [child](const node* child_node) { return child_node->name() == child->name(); }))
    {
        throw invalid_node_name{child->name()};
    }

    // Make sure to detach child from it's parent before attaching it to a new node
    if(node* current_parent = child->parent_)
    {
        // Check if the child is not already attached to this node
        if(current_parent != this)
        {
            current_parent->detach_child(child);
        }
        // If it is, stop right here, nothing needs to be done
        else
        {
            return;
        }
    }

    // At this point, child is ready to be attached to this
    child->parent_ = this;
    children_.push_back(child);
}

bool node::has_child(const node* child) const noexcept
{
    assert(child);

    return std::any_of(children_.begin(), children_.end(), [child](const node* child_strong_ptr)
    {
        return child_strong_ptr == child;
    });
}

node* node::find_child(const string_name& name) noexcept
{
    auto it = std::find_if(children_.begin(), children_.end(), [&name](const node* child_strong_ptr)
    {
       return child_strong_ptr->name() == name;
    });

    if(it == children_.end())
    {
        return nullptr;
    }
    else
    {
        return *it;
    }
}

const node* node::find_child(const string_name& name) const noexcept
{
    auto it = std::find_if(children_.begin(), children_.end(), [&name](const node* child_strong_ptr)
    {
        return child_strong_ptr->name() == name;
    });

    if(it == children_.end())
    {
        return nullptr;
    }
    else
    {
        return *it;
    }
}

bool node::in_hierarchy(const node* parent) const noexcept
{
    assert(parent);

    node* parent_it = parent_;
    while(parent_it)
    {
        if(parent_it == parent)
        {
            return true;
        }
        else
        {
            parent_it = parent_it->parent_;
        }
    }

    return false;
}

bool node::root() const noexcept
{
    return parent_ == nullptr;
}

bool node::leaf() const noexcept
{
    return children_.empty();
}

primary_node_types node::primary_node_type() const noexcept
{
    return primary_node_types::node;
}

node* node::parent() noexcept
{
    return parent_;
}

const node* node::parent() const noexcept
{
    return parent_;
}

node* node::first_child() noexcept
{
    if(!children_.empty())
    {
        return children_.front();
    }

    return nullptr;
}

const node* node::first_child() const noexcept
{
    if(!children_.empty())
    {
        return children_.front();
    }

    return nullptr;
}

node* node::last_child() noexcept
{
    if(!children_.empty())
    {
        return children_.back();
    }

    return nullptr;
}

const node* node::last_child() const noexcept
{
    if(!children_.empty())
    {
        return children_.back();
    }

    return nullptr;
}

node* node::next_sibling() noexcept
{
    if(parent_)
    {
        auto it = std::find(parent_->children_.begin(), parent_->children_.end(), this);

        if(it != parent_->children_.end())
        {
            const std::size_t index = std::distance(parent_->children_.begin(), it);

            if(index < parent_->children_.size() - 1)
            {
                return parent_->children_[index + 1];
            }
        }
    }

    return nullptr;
}

const node* node::next_sibling() const noexcept
{
    if(parent_)
    {
        auto it = std::find(parent_->children_.begin(), parent_->children_.end(), this);

        if(it != parent_->children_.end())
        {
            const std::size_t index = std::distance(parent_->children_.begin(), it);

            if(index < parent_->children_.size() - 1)
            {
                return parent_->children_[index + 1];
            }
        }
    }

    return nullptr;
}

node* node::previous_sibling() noexcept
{
    if(parent_)
    {
        auto it = std::find(parent_->children_.begin(), parent_->children_.end(), this);

        if(it != parent_->children_.end())
        {
            const std::size_t index = std::distance(parent_->children_.begin(), it);

            if(index > 0)
            {
                return parent_->children_[index - 1];
            }
        }
    }

    return nullptr;
}

const node* node::previous_sibling() const noexcept
{
    if(parent_)
    {
        auto it = std::find(parent_->children_.begin(), parent_->children_.end(), this);

        if(it != parent_->children_.end())
        {
            const std::size_t index = std::distance(parent_->children_.begin(), it);

            if(index > 0)
            {
                return parent_->children_[index - 1];
            }
        }
    }

    return nullptr;
}

}