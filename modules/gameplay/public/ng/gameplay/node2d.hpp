#ifndef NGINE_GAMEPLAY_NODE2D_HPP
#define NGINE_GAMEPLAY_NODE2D_HPP

#include "node.hpp"
#include <ng/core/transform2d.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace ng
{

/**
 * Base class for every node that has a 2D transform
 */
class node2d : public node
{
    transform2d local_;
    transform2d world_;

    [[nodiscard]] transform2d parent_transform() const noexcept;

public:
    explicit node2d(safe_name name, node* parent = nullptr, node_tree* owner = nullptr) noexcept;
    node2d(safe_name name, transform2d transform,  node* parent = nullptr, node_tree* owner = nullptr) noexcept;

    /**
     * Returns the world transform of this node
     * @return the world transform of this node
     */
    [[nodiscard]] const transform2d& world_transform() const noexcept;

    /**
     * Returns the local transform of this node
     * @return the local transform of this node
     */
    [[nodiscard]] const transform2d& local_transform() const noexcept;

    /**
     * Set the world transform this node should have
     * @param world_transform The world transform this node should have
     */
    void set_world_transform(const transform2d& world_transform) noexcept;

    /**
     * Set the local transform this node should have
     * @param local_transform The local transform this node should have
     */
    void set_local_transform(const transform2d& transform) noexcept;

    /**
     * Here we make sure all node2d have the primary node type node2d
     * @return node2d
     * @note This is used to upcast with a static_cast instead of a dynamic cast
     */
    [[nodiscard]] primary_node_types primary_node_type() const noexcept override final;
};

}

#endif
