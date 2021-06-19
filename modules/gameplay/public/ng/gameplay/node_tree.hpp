#ifndef NGINE_GAMEPLAY_NODE_TREE_HPP
#define NGINE_GAMEPLAY_NODE_TREE_HPP

#include "node.hpp"

namespace ng
{

/**
 * Represents a tree of nodes
 */
class node_tree
{
    strong_node_ptr root_;

public:

    /**
     * Returns a reference to the root node
     * @return The root node
     */
    [[nodiscard]] const node* root() const noexcept;

    /**
     * Returns a reference to the root node
     * @return The root node
     */
    [[nodiscard]] node* root() noexcept;

    /**
     * Check if this tree contains a specific node
     * @param n The node to check
     * @return true if the tree contains the node
     */
    [[nodiscard]] bool contains(const node* n) const noexcept;
};

}

#endif
