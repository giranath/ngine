#include "node2d.hpp"

namespace ng
{

transform2d node2d::parent_transform() const noexcept
{
    if(const node* parent_node = parent();
       parent_node && parent_node->primary_node_type() == primary_node_types::node2d)
    {
        const node2d* parent_node2d = static_cast<const node2d*>(parent_node);

        return parent_node2d->world_transform();
    }

    return transform2d{};
}

node2d::node2d(string_name name, node* parent) noexcept
: node{std::move(name), parent}
, local_()
, world_()
{

}

node2d::node2d(string_name name, transform2d transform, node* parent) noexcept
: node2d(std::move(name), parent)
{
    set_local_transform(transform);
}

const transform2d& node2d::world_transform() const noexcept
{
    return world_;
}

const transform2d& node2d::local_transform() const noexcept
{
    return local_;
}

void node2d::set_world_transform(const transform2d& world_transform) noexcept
{
    world_ = world_transform;
    local_ = transform2d{glm::inverse(parent_transform().matrix()) * world_.matrix()};
}

void node2d::set_local_transform(const transform2d& transform) noexcept
{
    local_ = transform;
    world_ = parent_transform() * local_;
}

primary_node_types node2d::primary_node_type() const noexcept
{
    return primary_node_types::node2d;
}

}