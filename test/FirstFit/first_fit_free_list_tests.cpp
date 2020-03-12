#include <array>
#include <cstddef>

#include <gtest/gtest.h>

#include "FirstFit/first_fit_free_list.h"

using FLNode = FirstFitFreeList::DLLNode;

TEST(AddNode, OnlyNode)
{
    FirstFitFreeList fl;

    FLNode node;
    node.value = 0;

    fl.add_node(&node);

    EXPECT_EQ(node.prev, nullptr);
    EXPECT_EQ(node.next, nullptr);
    EXPECT_EQ(fl.count(), 1);
}

TEST(AddNode, EarlierNode)
{
    FirstFitFreeList fl;

    std::array<std::uint8_t, 256> arr;
    std::uint8_t* mem = arr.data();

    FLNode* node1 = reinterpret_cast<FLNode*>(mem+50);
    node1->value = 0;
    fl.add_node(node1);

    FLNode* node2 = reinterpret_cast<FLNode*>(mem);
    node2->value = 1;
    fl.add_node(node2);

    EXPECT_EQ(node2->prev, nullptr);
    EXPECT_EQ(node2->next, node1);
    EXPECT_EQ(fl.count(), 2);
}

TEST(AddNode, LaterNode)
{
    FirstFitFreeList fl;

    std::array<std::uint8_t, 256> arr;
    std::uint8_t* mem = arr.data();

    FLNode* node1 = reinterpret_cast<FLNode*>(mem);
    node1->value = 0;
    fl.add_node(node1);

    FLNode* node2 = reinterpret_cast<FLNode*>(mem+50);
    node2->value = 1;
    fl.add_node(node2);

    EXPECT_EQ(node2->prev, node1);
    EXPECT_EQ(node2->next, nullptr);
    EXPECT_EQ(fl.count(), 2);
}

TEST(AddNode, MiddleNode)
{
    FirstFitFreeList fl;

    std::array<std::uint8_t, 256> arr;
    std::uint8_t* mem = arr.data();

    FLNode* node1 = reinterpret_cast<FLNode*>(mem);
    node1->value = 0;
    fl.add_node(node1);

    FLNode* node2 = reinterpret_cast<FLNode*>(mem+100);
    node2->value = 1;
    fl.add_node(node2);

    FLNode* node3 = reinterpret_cast<FLNode*>(mem+50);
    node3->value = 2;
    fl.add_node(node3);

    EXPECT_EQ(node3->prev, node1);
    EXPECT_EQ(node3->next, node2);
    EXPECT_EQ(fl.count(), 3);
}

TEST(RemoveNode, OnlyNode)
{
    FirstFitFreeList fl;

    FLNode node;
    node.value = 0;
    fl.add_node(&node);

    fl.remove_node(&node);

    EXPECT_EQ(fl.count(), 0);
}

TEST(RemoveNode, SmallestNode)
{
    FirstFitFreeList fl;

    std::array<std::uint8_t, 256> arr;
    std::uint8_t* mem = arr.data();

    FLNode* node1 = reinterpret_cast<FLNode*>(mem);
    node1->value = 0;
    fl.add_node(node1);

    FLNode* node2 = reinterpret_cast<FLNode*>(mem+50);
    node2->value = 1;
    fl.add_node(node2);

    fl.remove_node(node1);

    EXPECT_EQ(node2->prev, nullptr);
    EXPECT_EQ(node2->next, nullptr);
    EXPECT_EQ(fl.count(), 1);
}

TEST(RemoveNode, LargestNode)
{
    FirstFitFreeList fl;

    std::array<std::uint8_t, 256> arr;
    std::uint8_t* mem = arr.data();

    FLNode* node1 = reinterpret_cast<FLNode*>(mem+50);
    node1->value = 0;
    fl.add_node(node1);

    FLNode* node2 = reinterpret_cast<FLNode*>(mem);
    node2->value = 1;
    fl.add_node(node2);

    fl.remove_node(node1);

    EXPECT_EQ(node2->prev, nullptr);
    EXPECT_EQ(node2->next, nullptr);
    EXPECT_EQ(fl.count(), 1);
}

TEST(RemoveNode, MiddleNode)
{
    FirstFitFreeList fl;

    std::array<std::uint8_t, 256> arr;
    std::uint8_t* mem = arr.data();

    FLNode* node1 = reinterpret_cast<FLNode*>(mem);
    node1->value = 0;
    fl.add_node(node1);

    FLNode* node2 = reinterpret_cast<FLNode*>(mem+100);
    node2->value = 1;
    fl.add_node(node2);

    FLNode* node3 = reinterpret_cast<FLNode*>(mem+50);
    node3->value = 2;
    fl.add_node(node3);

    fl.remove_node(node3);

    EXPECT_EQ(node2->prev, node1);
    EXPECT_EQ(node1->next, node2);
    EXPECT_EQ(fl.count(), 2);
}

TEST(HeadNode, EmptyList)
{
    FirstFitFreeList fl;

    EXPECT_EQ(fl.head(), nullptr);
}

TEST(HeadNode, OnlyNode)
{
    FirstFitFreeList fl;

    FLNode node;
    node.value = 0;

    fl.add_node(&node);

    EXPECT_EQ(fl.head(), &node);
}

TEST(HeadNode, AddLaterNode)
{
    FirstFitFreeList fl;

    std::array<std::uint8_t, 256> arr;
    std::uint8_t* mem = arr.data();

    FLNode* node1 = reinterpret_cast<FLNode*>(mem);
    node1->value = 0;
    fl.add_node(node1);

    FLNode* node2 = reinterpret_cast<FLNode*>(mem+50);
    node2->value = 1;
    fl.add_node(node2);

    EXPECT_EQ(fl.head(), node1);
}

TEST(HeadNode, AddEarlierNode)
{
    FirstFitFreeList fl;

    std::array<std::uint8_t, 256> arr;
    std::uint8_t* mem = arr.data();

    FLNode* node1 = reinterpret_cast<FLNode*>(mem+50);
    node1->value = 0;
    fl.add_node(node1);

    FLNode* node2 = reinterpret_cast<FLNode*>(mem);
    node2->value = 1;
    fl.add_node(node2);

    EXPECT_EQ(fl.head(), node2);
}

TEST(HeadNode, RemoveSmallestNode)
{
    FirstFitFreeList fl;

    std::array<std::uint8_t, 256> arr;
    std::uint8_t* mem = arr.data();

    FLNode* node1 = reinterpret_cast<FLNode*>(mem+50);
    node1->value = 0;
    fl.add_node(node1);

    FLNode* node2 = reinterpret_cast<FLNode*>(mem);
    node2->value = 1;
    fl.add_node(node2);

    fl.remove_node(node2);

    EXPECT_EQ(fl.head(), node1);
}