#include "main.h"
#include <gtest/gtest.h>
#include <cstring>

class XorListTest : public ::testing::Test 
{
protected:
    XorList* list;

    void SetUp() override {
        list = xor_list_create();
    }

    void TearDown() override {
        xor_list_destroy(list);
    }
};

TEST_F(XorListTest, AddToEmptyList_no1) 
{
    xor_list_push_back(list, "zero");
    EXPECT_NE(list->head, nullptr);
    EXPECT_STREQ(list->head->data, "zero");
}

TEST_F(XorListTest, FindExistingKey_no2) 
{
    xor_list_push_back(list, "one");
    xor_list_push_back(list, "two");
    XorNode* match = xor_list_find(list, "two");
    ASSERT_NE(match, nullptr);
    EXPECT_STREQ(match->data, "two");
}

TEST_F(XorListTest, FindNonExistingKey_no3) 
{
    xor_list_push_back(list, "three");
    EXPECT_EQ(xor_list_find(list, "unknown"), nullptr);
}

TEST_F(XorListTest, RemoveByKeyFromSingleElement_no4)
{
    xor_list_push_back(list, "four");
    int result = xor_list_remove_by_key(list, "four");
    EXPECT_EQ(result, 1);
    EXPECT_EQ(list->head, nullptr);
    EXPECT_EQ(list->tail, nullptr);
}

TEST_F(XorListTest, RemoveMiddleElement_no5) 
{
    xor_list_push_back(list, "five");
    xor_list_push_back(list, "six");
    xor_list_push_back(list, "seven");
    EXPECT_EQ(xor_list_remove_by_key(list, "six"), 1);
    EXPECT_EQ(xor_list_find(list, "six"), nullptr);
    EXPECT_STREQ(list->head->data, "five");
    XorNode* curr = list->head;
    XorNode* previous = nullptr;
    curr = xor_list_next(list, curr, &previous);
    EXPECT_STREQ(curr->data, "seven");
}

TEST_F(XorListTest, RemoveHead_no6) 
{
    xor_list_push_back(list, "eight");
    xor_list_push_back(list, "nine");
    EXPECT_EQ(xor_list_remove_by_key(list, "eight"), 1);
    EXPECT_STREQ(list->head->data, "nine");
}

TEST_F(XorListTest, RemoveTail_no7) 
{
    xor_list_push_back(list, "ten");
    xor_list_push_back(list, "eleven");
    EXPECT_EQ(xor_list_remove_by_key(list, "eleven"), 1);
    EXPECT_EQ(list->tail, list->head);
    EXPECT_STREQ(list->tail->data, "ten");
}

TEST_F(XorListTest, RemoveNonExistingKey_no8) 
{
    xor_list_push_back(list, "twelve");
    EXPECT_EQ(xor_list_remove_by_key(list, "phantom"), 0);
}

TEST_F(XorListTest, IterateEmptyList_no9) 
{
    EXPECT_EQ(xor_list_begin(list), nullptr);
}

TEST_F(XorListTest, RemoveByNode_no10) 
{
    xor_list_push_back(list, "thirteen");
    XorNode* target = xor_list_find(list, "thirteen");
    ASSERT_NE(target, nullptr);
    EXPECT_EQ(xor_list_remove_by_node(list, target), 1);
    EXPECT_EQ(list->head, nullptr);
}

TEST_F(XorListTest, RemoveFromListWithDuplicateKeys_no11)
{
    xor_list_push_back(list, "fourteen");
    xor_list_push_back(list, "fourteen");
    xor_list_push_back(list, "unique");

    EXPECT_EQ(xor_list_remove_by_key(list, "fourteen"), 1);

    XorNode* first = xor_list_begin(list);
    ASSERT_NE(first, nullptr);
    EXPECT_STREQ(first->data, "fourteen");

    XorNode* prev = nullptr;
    XorNode* second = xor_list_next(list, first, &prev);
    ASSERT_NE(second, nullptr);
    EXPECT_STREQ(second->data, "unique");

    EXPECT_EQ(xor_list_find(list, "fourteen"), first);
}

TEST_F(XorListTest, IterateThreeElementsInOrder_no12) 
{
    xor_list_push_back(list, "fifteen");
    xor_list_push_back(list, "sixteen");
    xor_list_push_back(list, "seventeen");

    XorNode* current = xor_list_begin(list);
    XorNode* prev = nullptr;

    ASSERT_NE(current, nullptr);
    EXPECT_STREQ(current->data, "fifteen");

    current = xor_list_next(list, current, &prev);
    ASSERT_NE(current, nullptr);
    EXPECT_STREQ(current->data, "sixteen");

    current = xor_list_next(list, current, &prev);
    ASSERT_NE(current, nullptr);
    EXPECT_STREQ(current->data, "seventeen");

    current = xor_list_next(list, current, &prev);
    EXPECT_EQ(current, nullptr); 
}