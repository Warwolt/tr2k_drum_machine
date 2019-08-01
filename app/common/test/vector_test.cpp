/*
****************************************************************************************************
* brief : Unit tests for r2k::vector<> container template.
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "r2k/vector.h"

class R2KVectorTest : public ::testing::Test
{
public:
	//
};

TEST_F(R2KVectorTest, Values_can_be_accessed_using_bracket_operator)
{
	r2k::vector<u8, 2> vec;
	r2k::ivector<u8>& ivec = vec;

	ivec[0] = 12;
	ivec[1] = 23;

	EXPECT_EQ(ivec[0], 12);
	EXPECT_EQ(ivec[1], 23);
}

TEST_F(R2KVectorTest, Values_can_be_assigned_using_initializer_list)
{
	r2k::vector<u8, 3> vec = {10, 20, 30};
	r2k::ivector<u8>& ivec = vec;

	EXPECT_EQ(ivec[0], 10);
	EXPECT_EQ(ivec[1], 20);
	EXPECT_EQ(ivec[2], 30);
}

TEST_F(R2KVectorTest, Vector_size_can_be_queried)
{
	r2k::vector<u8, 3> vec = {1, 2};
	r2k::ivector<u8>& ivec = vec;
	EXPECT_EQ(ivec.size(), 2u);
}

TEST_F(R2KVectorTest, Vector_capacity_can_be_queried)
{
	r2k::vector<u8, 3> vec = {1, 2, 3};
	r2k::ivector<u8>& ivec = vec;
	EXPECT_EQ(ivec.size(), 3u);
}

TEST_F(R2KVectorTest, Values_can_be_accessed_using_iterator)
{
	size_t i = 0;
	r2k::vector<u8, 3> vec = {11, 22, 33};
	r2k::ivector<u8>& ivec = vec;
	for(const auto& num : ivec)
	{
		EXPECT_EQ(num, vec[i++]);
	}
}
