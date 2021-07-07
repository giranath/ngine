#ifndef NGINE_GAMEPLAY_NODE_TREE_HPP
#define NGINE_GAMEPLAY_NODE_TREE_HPP

#include <memory>
#include <vector>
#include <iterator>

namespace ng
{

class node_path;
class node;

/**
 * Iterate over nodes inside the tree
 */
class node_tree_iterator
{
    friend class node_tree;

    node* current_node_;

    node_tree_iterator(node* current_node);
public:
    using iterator_category = std::forward_iterator_tag;
    using pointer = node*;
    using reference = node&;
    using difference_type = std::ptrdiff_t;

    node_tree_iterator() noexcept;

    /**
     * Get a pointer to the iterated node
     * @return A pointer to the iterated node
     */
    pointer operator->() noexcept;

    /**
     * Get a reference to the iterated node
     * @return A reference to the iterated node
     */
    reference operator*() noexcept;

    /**
     * Move this iterator to the next node
     * @return A reference to this iterator
     */
    node_tree_iterator& operator++();

    /**
     * Move this iterator to the next node
     * @return The previous iterator
     */
    node_tree_iterator operator++(int);

    /**
     * Check if both iterators are pointing to the same node
     * @param other The other iterator
     * @return true when both iterators are pointing to the same node
     */
    bool operator==(const node_tree_iterator& other) const noexcept;

    /**
     * Check if both iterators are not pointing to the same node
     * @param other The other iterator
     * @return true when both iterators are pointing to the same node
     */
    bool operator!=(const node_tree_iterator& other) const noexcept;
};

/**
 * Iterate over nodes inside the tree
 */
class const_node_tree_iterator
{
    friend class node_tree;

    node* current_node_;

    const_node_tree_iterator(node* current_node);
public:
    using iterator_category = std::forward_iterator_tag;
    using pointer = const node*;
    using reference = const node&;
    using difference_type = std::ptrdiff_t;

    const_node_tree_iterator() noexcept;

    /**
     * Get a pointer to the iterated node
     * @return A pointer to the iterated node
     */
    pointer operator->() noexcept;

    /**
     * Get a reference to the iterated node
     * @return A reference to the iterated node
     */
    reference operator*() noexcept;

    /**
     * Move this iterator to the next node
     * @return A reference to this iterator
     */
    const_node_tree_iterator& operator++();

    /**
     * Move this iterator to the next node
     * @return The previous iterator
     */
    const_node_tree_iterator operator++(int);

    /**
     * Check if both iterators are pointing to the same node
     * @param other The other iterator
     * @return true when both iterators are pointing to the same node
     */
    bool operator==(const const_node_tree_iterator& other) const noexcept;

    /**
     * Check if both iterators are not pointing to the same node
     * @param other The other iterator
     * @return true when both iterators are pointing to the same node
     */
    bool operator!=(const const_node_tree_iterator& other) const noexcept;
};

/**
 * Represents a tree of nodes
 */
class node_tree
{
    std::vector<std::unique_ptr<node>> nodes_;

    node* root_;

public:
    using iterator = node_tree_iterator;
    using const_iterator = const_node_tree_iterator;

    /**
     * Set the root node for this tree
     * @param root The new root node for this tree
     */
    void set_root(node* root) noexcept;

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

    /**
     * Check if a node is reachable
     * @param n the node to check
     * @return true when reachable or false otherwise
     */
    [[nodiscard]] bool reachable(const node* n) const noexcept;

    /**
     * Find a node by it's path
     * @param path The path to get the node from
     * @return the found node
     */
    [[nodiscard]] node* find(const node_path& path) const noexcept;

    /**
     * Free all nodes owned by this tree that doesn't exist in the tree
     */
    void free_unreachable_nodes();

    /**
     * Create a new node owned by this tree
     * @tparam NodeType The type of the node to construct
     * @tparam Args The types of the arguments to pass to the node's constructor
     * @param args The arguments to pass to the node's constructor
     * @return the constructed node
     */
    template<typename NodeType, typename... Args>
    [[nodiscard]] NodeType* make_node(Args&&... args)
    {
        static_assert(std::is_base_of_v<node, NodeType>, "Expecting valid node type");

        std::unique_ptr<NodeType> new_node = std::make_unique<NodeType>(std::forward<Args>(args)...);
        // Here we set the owner of the node to be this tree
        new_node->owner_ = this;

        NodeType* node_ptr = new_node.get();

        nodes_.push_back(std::move(new_node));

        return node_ptr;
    }

    /**
     * Returns an iterator to the root node
     * @return An iterator to the root node
     */
    [[nodiscard]] iterator begin() noexcept;

    /**
     * Returns an iterator after the last node
     * @return An iterator passed the last node
     */
    [[nodiscard]] iterator end() noexcept;

    /**
     * Returns an iterator to the root node
     * @return An iterator to the root node
     */
    [[nodiscard]] const_iterator begin() const noexcept;

    /**
     * Returns an iterator after the last node
     * @return An iterator passed the last node
     */
    [[nodiscard]] const_iterator end() const noexcept;

    /**
     * Returns an iterator to the root node
     * @return An iterator to the root node
     */
    [[nodiscard]] const_iterator cbegin() const noexcept;

    /**
     * Returns an iterator after the last node
     * @return An iterator passed the last node
     */
    [[nodiscard]] const_iterator cend() const noexcept;
};

}

#endif
