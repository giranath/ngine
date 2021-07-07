#ifndef NGINE_GAMEPLAY_NODE_HPP
#define NGINE_GAMEPLAY_NODE_HPP

#include <ng/core/name.hpp>

#include <memory>
#include <vector>
#include <stdexcept>

namespace ng
{

class node;

class node_decorator;
using node_decorator_ptr = std::unique_ptr<node_decorator>;

class node_tree;

class invalid_node_name : public std::runtime_error
{
    safe_name name_;

public:
    explicit invalid_node_name(safe_name name)
    : std::runtime_error("invalid node name")
    , name_{name}
    {

    }

    [[nodiscard]] inline safe_name name() const noexcept
    {
        return name_;
    }
};

/**
 * Decorator that can be attached to a node to change it's behaviour
 * They will process events from the tree before it's owning node so it could mutate the event or block events
 * from traversing further
 */
class node_decorator : std::enable_shared_from_this<node>
{
    node* node_;
public:
    virtual ~node_decorator() = default;
};

/**
 * Enumerates all supported primary node types
 */
enum class primary_node_types : uint8_t
{
    /// All nodes are by default node
    node,

    /// All nodes under hierarchy of node2d are node2d
    node2d
};

/**
 * Base class every nodes must implement
 * A scene becomes a tree of nodes with events traversing the tree
 */
class node : std::enable_shared_from_this<node>
{
    friend node_tree;

    // The name of the node
    safe_name name_;

    // The tree owning this node
    node_tree* owner_;

    // The node parent to this one
    node* parent_;

    // List of child nodes
    std::vector<node*> children_;

    // List of decorators attached to this node to update it's behaviour
    std::vector<node_decorator_ptr> decorators_;

    void set_owner(node_tree* owner);

public:
    explicit node(safe_name name, node* parent = nullptr) noexcept;
    virtual ~node() = default;

    /**
     * Returns the name of this node
     * @return The name of this node
     */
    const safe_name& name() const noexcept;

    /**
     * Add a decorator to the node
     * @param decorator The decorator to add
     */
    void add_decorator(std::unique_ptr<node_decorator> decorator);

    /**
     * Create a new decorator inside this node
     * @tparam DecoratorType The type of the decorator to create
     * @tparam Args The decorator's constructor arguments
     * @param args The arguments to pass to the constructor of the decorator
     * @return a reference to the created decorator
     */
    template<typename DecoratorType, typename... Args>
    DecoratorType& emplace_decorator(Args&&... args)
    {
        decorators_.push_back(std::make_unique<DecoratorType>(std::forward<Args>(args)...));

        return *static_cast<DecoratorType*>(decorators_.back().get());
    }

    /**
     * Clear all decorators from this node
     */
    void clear_decorators();

    /**
     * Attach this node to a parent
     * @param parent The parent this node should have after calling this function
     */
    void attach_to(node* parent);

    /**
     * Detach this node from it's parent
     */
    void detach_from_parent();

    /**
     * Detach a child node from this node
     * @param child The node to detach from this
     */
    void detach_child(node* child);

    /**
     * Attach a child node to this one
     * @param child The node to attach to this node
     */
    void attach_child(node* child);

    /**
     * Check if this node has a specific node as a direct child
     * @param child The node to check
     * @return true when child is a direct child node of this node
     */
    [[nodiscard]] bool has_child(const node* child) const noexcept;

    /**
     * Find a direct child node by it's name
     * @param name The name of the child node
     * @return the found node or nullptr if it was not found
     */
    [[nodiscard]] node* find_child(const safe_name& name) noexcept;
    [[nodiscard]] const node* find_child(const safe_name& name) const noexcept;

    /**
     * Check if this node is under the hierarchy of a parent node
     * @param parent The node to check if this node is under its hierarchy
     * @return true when this node is under parent at some point in it's hierarchy
     */
    [[nodiscard]] bool in_hierarchy(const node* parent) const noexcept;

    /**
     * Check if this node is a root
     * @return true when this node is a root or false otherwise
     */
    [[nodiscard]] bool root() const noexcept;

    /**
     * Check if this node is a leaf
     * @return true when this node as no children or false otherwise
     */
    [[nodiscard]] bool leaf() const noexcept;

    /**
     * Returns the primary node type identifier
     * @return an identifier that indicate the primary type of a node
     *
     */
    [[nodiscard]] virtual primary_node_types primary_node_type() const noexcept;

    /**
     * Returns the parent node
     * @return The parent node
     */
    [[nodiscard]] node* parent() noexcept;
    [[nodiscard]] const node* parent() const noexcept;

    [[nodiscard]] node* first_child() noexcept;
    [[nodiscard]] const node* first_child() const noexcept;
    [[nodiscard]] node* last_child() noexcept;
    [[nodiscard]] const node* last_child() const noexcept;

    /**
     * Get the next sibling
     * @return The next sibling
     */
    [[nodiscard]] node* next_sibling() noexcept;
    [[nodiscard]] const node* next_sibling() const noexcept;

    /**
     * Returns the previous sibling
     * @return the previous sibling
     */
    [[nodiscard]] node* previous_sibling() noexcept;
    [[nodiscard]] const node* previous_sibling() const noexcept;
};

}

#endif
