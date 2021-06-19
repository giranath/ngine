#include "node.hpp"
#include <algorithm>

#include <cassert>

namespace ng
{

node::node(safe_name name, node* parent, node_tree* owner) noexcept
: name_{std::move(name)}
, owner_{owner}
, parent_{parent}
, children_()
, decorators_()
{

}

const safe_name& node::name() const noexcept
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
        if(children_[i].get() == child)
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

    // Keep a reference on the child, because if we remove it from it's parent,
    // it will be destroyed
    strong_node_ptr strong_child = child->shared_from_this();

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
    strong_child->parent_ = this;
    children_.push_back(strong_child);
}

bool node::has_child(const node* child) const noexcept
{
    assert(child);

    return std::any_of(children_.begin(), children_.end(), [child](const strong_node_ptr& child_strong_ptr)
    {
        return child_strong_ptr.get() == child;
    });
}

node* node::find_child(const safe_name& name) noexcept
{
    auto it = std::find_if(children_.begin(), children_.end(), [&name](const strong_node_ptr& child_strong_ptr)
    {
       return child_strong_ptr->name() == name;
    });

    if(it == children_.end())
    {
        return nullptr;
    }
    else
    {
        return it->get();
    }
}

const node* node::find_child(const safe_name& name) const noexcept
{
    auto it = std::find_if(children_.begin(), children_.end(), [&name](const strong_node_ptr& child_strong_ptr)
    {
        return child_strong_ptr->name() == name;
    });

    if(it == children_.end())
    {
        return nullptr;
    }
    else
    {
        return it->get();
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
    return parent_;
}

bool node::leaf() const noexcept
{
    return children_.empty();
}

primary_node_types node::primary_node_type() const noexcept
{
    return primary_node_types::node;
}

const node* node::parent() const noexcept
{
    return parent_;
}

}