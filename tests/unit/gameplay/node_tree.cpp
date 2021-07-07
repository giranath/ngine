#include <catch.hpp>
#include <ng/gameplay/node_tree.hpp>
#include <ng/gameplay/node.hpp>
#include <ng/gameplay/node_path.hpp>

using namespace ng::literals;

TEST_CASE("A node tree has a root node", "[node_tree]")
{
    ng::node_tree tree;

    auto root = tree.make_node<ng::node>("root"_name);
    auto child = tree.make_node<ng::node>("child"_name, root);
    auto child2 = tree.make_node<ng::node>("child2"_name, root);

    tree.set_root(root);

    REQUIRE(tree.root() == root);
}

TEST_CASE("Specific node inside a tree can be found from a node path", "[node_tree]")
{
    ng::node_tree tree;

    auto root = tree.make_node<ng::node>("root"_name);
    auto child = tree.make_node<ng::node>("child"_name, root);
    auto child2 = tree.make_node<ng::node>("child2"_name, root);
    auto child3 = tree.make_node<ng::node>("child3"_name, root);
    auto child4 = tree.make_node<ng::node>("child4"_name, child3);
    auto child5 = tree.make_node<ng::node>("child5"_name, child3);
    auto child6 = tree.make_node<ng::node>("child6"_name, child5);

    tree.set_root(root);

    SECTION("can search for a node with a relative path")
    {
        ng::node* found_node = tree.find("root/child3/child5/child6"_node);
        REQUIRE(found_node == child6);
    }

    SECTION("can search for a node with an absolute path")
    {
        ng::node* found_node = tree.find("/child3/child5/child6"_node);
        REQUIRE(found_node == child6);
    }

    SECTION("can search for an non-existing node")
    {
        ng::node* found_node = tree.find("/child3/child5/child7"_node);
        REQUIRE(found_node == nullptr);
    }
}

TEST_CASE("Nodes inside a node tree can be iterated", "[node_tree]")
{
    ng::node_tree tree;

    auto root = tree.make_node<ng::node>("root"_name);
    auto child = tree.make_node<ng::node>("child"_name, root);
    auto child2 = tree.make_node<ng::node>("child2"_name, root);
    auto child3 = tree.make_node<ng::node>("child3"_name, root);
    auto child4 = tree.make_node<ng::node>("child4"_name, child3);
    auto child5 = tree.make_node<ng::node>("child5"_name, child3);
    auto child6 = tree.make_node<ng::node>("child6"_name, child5);

    tree.set_root(root);

    auto it = tree.begin();
    REQUIRE(&(*it) == root);

    ++it;
    REQUIRE(&(*it) == child);

    ++it;
    REQUIRE(&(*it) == child2);

    ++it;
    REQUIRE(&(*it) == child3);

    ++it;
    REQUIRE(&(*it) == child4);

    ++it;
    REQUIRE(&(*it) == child5);

    ++it;
    REQUIRE(&(*it) == child6);

    ++it;
    REQUIRE(it == tree.end());
}