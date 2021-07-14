#include "node_tree.hpp"
#include "node.hpp"
#include "node_path.hpp"
#include <cassert>
#include <algorithm>

namespace ng
{

node_tree_iterator::node_tree_iterator() noexcept
: current_node_{nullptr}
{

}

node_tree_iterator::node_tree_iterator(node* current_node)
: current_node_{current_node}
{

}

node_tree_iterator::pointer node_tree_iterator::operator->() noexcept
{
    return current_node_;
}

node& node_tree_iterator::operator*() noexcept
{
    assert(current_node_);

    return *current_node_;
}

node_tree_iterator& node_tree_iterator::operator++()
{
    assert(current_node_);

    // Check if current node has a sibling, if it is the case, iterate over the sibling
    auto next_sibling = current_node_->next_sibling();
    if(next_sibling != nullptr)
    {
        current_node_ = next_sibling;
    }
    // We have iterated on every sibling nodes
    // Now we need to move into the children
    else if(!current_node_->root())
    {
        node* parent_node = current_node_->parent();
        node* first_sibling_with_children = parent_node->first_child();

        // Reset current node, so we can handle the case where no sibling has children
        current_node_ = nullptr;

        while(first_sibling_with_children)
        {
            if(first_sibling_with_children->leaf())
            {
                first_sibling_with_children = first_sibling_with_children->next_sibling();
            }
            else
            {
                current_node_ = first_sibling_with_children->first_child();

                // Stop iteration here
                first_sibling_with_children = nullptr;
            }
        }
    }
    // Otherwise, we reach the end
    else
    {
        current_node_ = current_node_->first_child();
    }

    return *this;
}

node_tree_iterator node_tree_iterator::operator++(int)
{
    node_tree_iterator tmp = *this;

    ++(*this);

    return tmp;
}

bool node_tree_iterator::operator==(const node_tree_iterator& other) const noexcept
{
    return current_node_ == other.current_node_;
}

bool node_tree_iterator::operator!=(const node_tree_iterator& other) const noexcept
{
    return current_node_ != other.current_node_;
}

const_node_tree_iterator::const_node_tree_iterator() noexcept
: current_node_{nullptr}
{

}

const_node_tree_iterator::const_node_tree_iterator(node* current_node)
: current_node_{current_node}
{

}

const_node_tree_iterator::pointer const_node_tree_iterator::operator->() noexcept
{
    return current_node_;
}

const_node_tree_iterator::reference const_node_tree_iterator::operator*() noexcept
{
    assert(current_node_);

    return *current_node_;
}

const_node_tree_iterator& const_node_tree_iterator::operator++()
{
    assert(current_node_);

    // Check if current node has a sibling, if it is the case, iterate over the sibling
    auto next_sibling = current_node_->next_sibling();
    if(next_sibling != nullptr)
    {
        current_node_ = next_sibling;
    }
        // We have iterated on every sibling nodes
        // Now we need to move into the children
    else if(!current_node_->root())
    {
        node* parent_node = current_node_->parent();
        node* first_sibling_with_children = parent_node->first_child();

        // Reset current node, so we can handle the case where no sibling has children
        current_node_ = nullptr;

        while(first_sibling_with_children)
        {
            if(first_sibling_with_children->leaf())
            {
                first_sibling_with_children = first_sibling_with_children->next_sibling();
            }
            else
            {
                current_node_ = first_sibling_with_children->first_child();

                // Stop iteration here
                first_sibling_with_children = nullptr;
            }
        }
    }
        // Otherwise, we reach the end
    else
    {
        current_node_ = current_node_->first_child();
    }

    return *this;
}

const_node_tree_iterator const_node_tree_iterator::operator++(int)
{
    const_node_tree_iterator tmp = *this;

    ++(*this);

    return tmp;
}

bool const_node_tree_iterator::operator==(const const_node_tree_iterator& other) const noexcept
{
    return current_node_ == other.current_node_;
}

bool const_node_tree_iterator::operator!=(const const_node_tree_iterator& other) const noexcept
{
    return current_node_ != other.current_node_;
}

void node_tree::set_root(node* root) noexcept
{
    assert(root);
    assert(root->owner_ == this);

    root_ = root;
}

const node* node_tree::root() const noexcept
{
    return root_;
}

node* node_tree::root() noexcept
{
    return root_;
}

bool node_tree::contains(const node* n) const noexcept
{
    return n->owner_ == this;
}

bool node_tree::reachable(const node* n) const noexcept
{
    return std::any_of(begin(), end(),
                       [n](const node& reachable_node) { return &reachable_node == n; });
}

node* node_tree::find(const node_path& path) const noexcept
{
    node* current = nullptr;

    // When using an absolute path, search from the root
    if(path.absolute())
    {
        current = root_;
    }

    // We iterate over each parts of the name
    for(const string_name& name : path)
    {
        if(current == nullptr && root_->name() == name)
        {
            current = root_;
            continue;
        }

        // We found a child with the required name, start searching its children for the rest of the path
        if(node* found_child = current->find_child(name))
        {
            current = found_child;
        }
        // No children were found for this path, return nullptr
        else
        {
            return nullptr;
        }
    }

    return current;
}

void node_tree::free_unreachable_nodes()
{
    for(std::size_t i = nodes_.size(); i > 0; --i)
    {
        // If the node is not reachable
        if(!reachable(nodes_[i].get()))
        {
            std::swap(nodes_[i], nodes_.back());
            nodes_.pop_back();
        }
    }
}

node_tree::iterator node_tree::begin() noexcept
{
    return node_tree::iterator{root_};
}

node_tree::iterator node_tree::end() noexcept
{
    return node_tree::iterator{};
}

node_tree::const_iterator node_tree::begin() const noexcept
{
    return node_tree::const_iterator{root_};
}

node_tree::const_iterator node_tree::end() const noexcept
{
    return node_tree::const_iterator{};
}

node_tree::const_iterator node_tree::cbegin() const noexcept
{
    return node_tree::const_iterator{root_};
}

node_tree::const_iterator node_tree::cend() const noexcept
{
    return node_tree::const_iterator{};
}

}