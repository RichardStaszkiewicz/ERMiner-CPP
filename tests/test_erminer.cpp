#include <gtest/gtest.h>
#include "erminer.hpp"
#include "itemset.hpp"
#include "rules.hpp"

// Test Fixture for ERMiner
class ERMinerTest : public ::testing::Test {
protected:
    // Sample sequence database
    std::vector<std::vector<int>> sdb = {
        {1, 2, 3, 4, 1},
        {2, 3, 4, 1, 2},
        {3, 1, 4, 2}
    };
};

// Test: Static cooccurrence computation
TEST_F(ERMinerTest, CooccurrenceTest) {
    Itemset a({1});
    Itemset b({2});

    double cooccurrence = ERMiner::cooccurs(a, b, sdb);

    EXPECT_NEAR(cooccurrence, 1.0, 1e-6); // Expect full co-occurrence
}

// Test: Find left equivalence classes
TEST_F(ERMinerTest, LeftEquivalenceClassesTest) {
    ERMiner miner(0.5, 0.5); // minsup = 0.5, minconf = 0.5

    // Define initial rules
    std::set<Rule> rules = {
        Rule(Itemset({1}), Itemset({2})),
        Rule(Itemset({1}), Itemset({3})),
        Rule(Itemset({2}), Itemset({3}))
    };

    auto leq = miner._findLeftEquivalenceClasses(1, rules, sdb);

    // Verify left equivalence classes
    EXPECT_EQ(leq.size(), 2); // Two equivalence classes (antecedents {1} and {2})
    EXPECT_TRUE(leq.count(Itemset({1})) > 0);
    EXPECT_TRUE(leq.count(Itemset({2})) > 0);
}

// Test: Find right equivalence classes
TEST_F(ERMinerTest, RightEquivalenceClassesTest) {
    ERMiner miner(0.5, 0.5);

    // Define initial rules
    std::set<Rule> rules = {
        Rule(Itemset({1}), Itemset({2})),
        Rule(Itemset({1}), Itemset({3})),
        Rule(Itemset({2}), Itemset({3}))
    };

    auto req = miner._findRightEquivalenceClasses(1, rules, sdb);

    // Verify right equivalence classes
    EXPECT_EQ(req.size(), 2); // Two equivalence classes (consequents {2} and {3})
    EXPECT_TRUE(req.count(Itemset({2})) > 0);
    EXPECT_TRUE(req.count(Itemset({3})) > 0);
}

// Test: First scan
TEST_F(ERMinerTest, FirstScanTest) {
    ERMiner miner(0.5, 0.5); // minsup = 0.5, minconf = 0.5

    auto [leq, req] = miner._firstScan(sdb);

    // Verify left and right equivalence classes
    EXPECT_GT(leq.size(), 0); // Should have some left equivalence classes
    EXPECT_GT(req.size(), 0); // Should have some right equivalence classes
}

// Test: Fit the model
TEST_F(ERMinerTest, FitTest) {
    ERMiner miner(0.5, 0.5); // minsup = 0.5, minconf = 0.5

    miner.fit(sdb);

    // Verify rules were generated
    EXPECT_GT(miner.getValidRules().size(), 0); // At least some rules should be valid
}

// Test: Save rules to CSV
TEST_F(ERMinerTest, SaveRulesToCsvTest) {
    ERMiner miner(0.5, 0.5); // minsup = 0.5, minconf = 0.5

    miner.fit(sdb);

    // Save rules to a CSV file
    std::string csvFile = "build/rules.csv";
    miner.rulesToCsv(csvFile);

    // Verify the file was created and has content
    std::ifstream file(csvFile);
    ASSERT_TRUE(file.is_open());
    std::string line;
    std::getline(file, line);
    EXPECT_EQ(line, "antecedent,consequent,support,confidence"); // Check header
    EXPECT_TRUE(std::getline(file, line));                      // Check for at least one rule
    file.close();
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
