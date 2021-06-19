#include <catch.hpp>
#include <ng/gameplay/node2d.hpp>

using namespace ng::literals;

TEST_CASE("A node2d combines it's local transform with it's parent world transform", "[node2d]")
{
    SECTION("A node2d without any parent will have its local transform equals to its world transform")
    {
        ng::node2d node("temp"_name, ng::transform2d{glm::vec2{10.f, 10.f}, glm::radians(25.f), glm::vec2{1.2f, 1.2f}});

        REQUIRE(node.world_transform() == node.local_transform());
    }

    SECTION("A node2d with a parent will have its world transform affected by its parent")
    {
        const ng::transform2d child_transform{glm::vec2{5.f, 10.f}, glm::radians(10.f)};
        ng::node2d parent_node("parent"_name, ng::transform2d{glm::vec2{10.f, 10.f}, glm::radians(25.f), glm::vec2{1.2f, 1.2f}});
        ng::node2d child_node("child"_name, child_transform, &parent_node);

        REQUIRE(child_node.local_transform() == child_transform);
        REQUIRE(child_node.world_transform() == parent_node.world_transform() * child_node.local_transform());
    }

    SECTION("A node2d with a node parent will not have it's world transform affected by its parent")
    {
        const ng::transform2d child_transform{glm::vec2{5.f, 10.f}, glm::radians(10.f)};

        ng::node parent("parent"_name);
        ng::node2d child_node{"child"_name, child_transform, &parent};

        REQUIRE(child_node.world_transform() == child_node.local_transform());
        REQUIRE(child_node.local_transform() == child_transform);
    }
}

TEST_CASE("The world transform of a node2d can be set", "[node2d]")
{
    const ng::transform2d parent_transform{glm::vec2{10.f, 10.f},
                                           glm::radians(25.f),
                                           glm::vec2{1.2f, 1.2f}};

    const ng::transform2d child_transform{glm::vec2{5.f, 10.f},
                                          glm::radians(10.f)};

    ng::node2d parent_node("parent"_name, parent_transform);
    ng::node2d child_node("child"_name, child_transform, &parent_node);

    SECTION("changing world transform of child to the world transform of parent means the child has the identity transform")
    {
        // If we move child node on parent transform
        child_node.set_world_transform(parent_transform);

        // Than it means that child transform is the identity
        REQUIRE(child_node.local_transform().similar(ng::transform2d{}, 0.01f));
    }

    SECTION("changing world transform of child to a specific value correctly update its local transform")
    {
        const ng::transform2d new_transform{
            glm::vec2{100.f, 0.f},
            glm::radians(15.f),
            glm::vec2{1.5f, 1.3f}
        };

        // If we move child node on parent transform
        child_node.set_world_transform(new_transform);

        const glm::mat3 expected_transform_matrix = parent_node.world_transform().matrix() * child_node.local_transform().matrix();
        const ng::transform2d expected_transform(expected_transform_matrix);

        // Than it means that child transform is the identity
        REQUIRE(child_node.world_transform() == new_transform);
        REQUIRE(expected_transform.similar(child_node.world_transform(), 0.1f));
    }
}