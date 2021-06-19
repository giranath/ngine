#include "node_tree.hpp"

namespace ng
{

const node* node_tree::root() const noexcept
{
    return root_.get();
}

node* node_tree::root() noexcept
{
    return root_.get();
}

bool node_tree::contains(const node* n) const noexcept
{
    return n->owner_ == this;
}

}