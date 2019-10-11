/*
****************************************************************************************************
* brief : Unit tests for r2k::vector<> container template.
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "r2k/vector.h"

class r2kVectorTest : public ::testing::Test
{
public:
};

TEST_F(r2kVectorTest, Values_can_be_assigned_using_bracket_operator)
{
	r2k::vector<u8, 2> vec;
	r2k::ivector<u8>& ivec = vec;

	ivec[0] = 12;
	ivec[1] = 23;

	EXPECT_EQ(ivec[0], 12);
	EXPECT_EQ(ivec[1], 23);
}

TEST_F(r2kVectorTest, Values_can_be_assigned_using_initializer_list)
{
	r2k::vector<u8, 3> vec = {10, 20, 30};
	r2k::ivector<u8>& ivec = vec;

	EXPECT_EQ(ivec[0], 10);
	EXPECT_EQ(ivec[1], 20);
	EXPECT_EQ(ivec[2], 30);
}

TEST_F(r2kVectorTest, Values_can_be_assigned_using_push_back)
{
	r2k::vector<u8, 3> vec;
	r2k::ivector<u8>& ivec = vec;

	ivec.push_back(10);
	ivec.push_back(20);
	ivec.push_back(30);

	EXPECT_EQ(ivec[0], 10);
	EXPECT_EQ(ivec[1], 20);
	EXPECT_EQ(ivec[2], 30);
}

TEST_F(r2kVectorTest, Cannot_push_back_elements_beyond_capacity)
{
	r2k::vector<u8, 2> vec; // NOTE: size is not 3!
	r2k::ivector<u8>& ivec = vec;

	ivec.push_back(10);
	ivec.push_back(20);
	ivec.push_back(30); // this should do nothing

	EXPECT_EQ(ivec[0], 10);
	EXPECT_EQ(ivec[1], 20);
	EXPECT_NE(ivec[2], 30);
}

TEST_F(r2kVectorTest, Vector_size_can_be_queried) // generalize this test
{
	r2k::vector<u8, 3> vec = {1, 2};
	r2k::ivector<u8>& ivec = vec;
	EXPECT_EQ(ivec.size(), 2u);
}

TEST_F(r2kVectorTest, Vector_capacity_can_be_queried)
{
	r2k::vector<u8, 3> vec = {1, 2, 3};
	r2k::ivector<u8>& ivec = vec;
	EXPECT_EQ(ivec.size(), 3u);
}

TEST_F(r2kVectorTest, Values_can_be_accessed_using_iterator)
{
	size_t i = 0;
	r2k::vector<u8, 3> vec = {11, 22, 33};
	r2k::ivector<u8>& ivec = vec;
	for(const auto& num : ivec)
	{
		EXPECT_EQ(num, ivec[i++]);
	}
}

TEST_F(r2kVectorTest, Last_value_can_be_accessed_using_back)
{
	r2k::vector<u8, 3> vec = {5, 6, 7};
	r2k::ivector<u8>& ivec = vec;
	EXPECT_EQ(ivec.back(), ivec[2]);
}

TEST_F(r2kVectorTest, Last_element_can_be_removed_with_pop_back)
{
	r2k::vector<u8, 3> vec = {11, 22, 33};
	r2k::ivector<u8>& ivec = vec;

	ivec.pop_back();

	EXPECT_EQ(ivec.back(), ivec[1]);
}

TEST_F(r2kVectorTest, Vector_not_empty_while_values_remain)
{
	r2k::vector<u8, 2> vec = {1, 2};
	r2k::ivector<u8>& ivec = vec;

	ivec.pop_back();

	EXPECT_FALSE(ivec.empty());
}

TEST_F(r2kVectorTest, Vector_empty_after_last_value_popped)
{
	r2k::vector<u8, 2> vec = {1, 2};
	r2k::ivector<u8>& ivec = vec;

	ivec.pop_back();
	ivec.pop_back();

	EXPECT_TRUE(ivec.empty());
}

TEST_F(r2kVectorTest, Vector_can_be_resized_up_to_capacity)
{
	r2k::vector<u8, 3> vec = {21, 22, 23};
	r2k::ivector<u8>& ivec = vec;

	ivec.resize(2);
	EXPECT_EQ(ivec.size(), 2u);

	ivec.resize(4);
	EXPECT_NE(ivec.size(), 4u);
}