#include <gtest/gtest.h>
#include "itemset.hpp"

// Test Fixture for Itemset
class ItemsetTest : public ::testing::Test {
protected:
    std::vector<std::vector<int>> createSampleSDB() {
        return {
            {1, 2, 3, 4, 1},
            {2, 3, 4, 1, 2},
            {3, 1, 4, 2}
        };
    }
};

//Test: Sanity Test
TEST_F(ItemsetTest, SanityTest) {
    EXPECT_EQ(true, true);
}

// Test: Initialization
TEST_F(ItemsetTest, InitializationTest) {
    std::vector<int> elements = {1, 2};
    Itemset itemset(elements);

    EXPECT_EQ(itemset.size(), 2);
    EXPECT_TRUE(itemset.find(1) != itemset.end());
    EXPECT_TRUE(itemset.find(2) != itemset.end());
}

// Test: Hash Function
TEST_F(ItemsetTest, HashFunctionTest) {
    std::vector<int> elements = {2, 1};
    Itemset itemset(elements);

    std::size_t hash1 = itemset.hash();
    
    // Create another itemset with same elements but different order
    std::vector<int> elements_reordered = {1, 2};
    Itemset reordered_itemset(elements_reordered);

    std::size_t hash2 = reordered_itemset.hash();

    // Hashes must be equal
    EXPECT_EQ(hash1, hash2);
}

// Test: Compute Occurrences
TEST_F(ItemsetTest, ComputeOccurrencesTest) {
    std::vector<int> elements = {1, 2};
    Itemset itemset(elements);

    std::vector<std::vector<int>> sdb = createSampleSDB();
    itemset.computeOccurrences(sdb);

    // Verify occurrences
    const auto& occurrences = itemset.getOccurrences();
    ASSERT_EQ(occurrences.size(), 3);

    EXPECT_EQ(occurrences.at(0), std::make_pair(1, 1));
    EXPECT_EQ(occurrences.at(1), std::make_pair(3, 3));
    EXPECT_EQ(occurrences.at(2), std::make_pair(3, 1));
}

// Test: Update Occurrences
TEST_F(ItemsetTest, UpdateOccurrencesTest) {
    std::vector<int> elements = {1};
    Itemset itemset(elements);

    std::vector<std::vector<int>> sdb = createSampleSDB();
    itemset.computeOccurrences(sdb);

    // Verify occurrences after adding a new item
    const auto& occurrences = itemset.getOccurrences();
    ASSERT_EQ(occurrences.size(), 3); // Item {1} occurs in 3 sequences

    const auto& updatedOccurrences = itemset.updateOccurrences(2, sdb);
    ASSERT_EQ(updatedOccurrences.size(), 3); // Itemset {1, 2} occurs in 2 sequences

    EXPECT_EQ(updatedOccurrences.at(0), std::make_pair(1, 1)); // Sequence 0: indices [0, 4]
    EXPECT_EQ(updatedOccurrences.at(1), std::make_pair(3, 3)); // Sequence 1: indices [3, 4]
    EXPECT_EQ(updatedOccurrences.at(2), std::make_pair(3, 1)); // Sequence 1: indices [3, 4]
}

// Test: Empty Itemset
TEST_F(ItemsetTest, EmptyItemsetTest) {
    Itemset itemset;
    EXPECT_THROW(itemset.computeOccurrences(createSampleSDB()), std::logic_error);
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
