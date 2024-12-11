#include <gtest/gtest.h>
#include "itemset.hpp"
#include "rules.hpp"

// Test Fixture for Rule class
class RuleTest : public ::testing::Test {
protected:
    // Sample sequence database
    std::vector<std::vector<int>> sdb = {
        {1, 2, 3, 4, 1}, // Sequence 0
        {2, 3, 4, 1, 2}, // Sequence 1
        {3, 1, 4, 2}     // Sequence 2
    };
};

//Test: Sanity Test
TEST_F(RuleTest, SanityTest) {
    EXPECT_EQ(true, true);
}

// Test: Initialization
TEST_F(RuleTest, InitializationTest) {
    Itemset antecedent({1, 2});
    Itemset consequent({3});
    Rule rule(antecedent, consequent);

    EXPECT_EQ(rule.getSupport(), -1);   // Default support
    EXPECT_EQ(rule.getConfidence(), -1); // Default confidence
}

// Test: Finding Sequences with Rule
TEST_F(RuleTest, FindSequencesWithRuleTest) {
    Itemset antecedent({1, 2});
    Itemset consequent({3});
    Rule rule(antecedent, consequent);

    rule.findSequencesWithRule(sdb);

    // Verify sequences with the rule
    const auto& sequences = rule.getSequencesWithRule();
    ASSERT_EQ(sequences.size(), 1);
    EXPECT_EQ(sequences[0], 0);
}

// Test: Compute Support
TEST_F(RuleTest, ComputeSupportTest) {
    Itemset antecedent({1, 2});
    Itemset consequent({3});
    Rule rule(antecedent, consequent);

    rule.computeSupport(sdb);

    // Verify support calculation
    EXPECT_NEAR(rule.getSupport(), 1.0 / 3.0, 1e-6); // Support = 1/3
}

// Test: Compute Confidence
TEST_F(RuleTest, ComputeConfidenceTest) {
    Itemset antecedent({1, 2});
    Itemset consequent({3});
    Rule rule(antecedent, consequent);

    rule.computeConfidence(sdb);

    // Verify confidence calculation
    double expectedConfidence = (1.0 / 3.0) * sdb.size() / rule.getAntecedent().getOccurrences().size();
    EXPECT_NEAR(rule.getConfidence(), expectedConfidence, 1e-6);
}

// Test: Rule Frequency
TEST_F(RuleTest, IsFrequentTest) {
    Itemset antecedent({1, 2});
    Itemset consequent({3});
    Rule rule(antecedent, consequent);

    double minsup = 0.5; // Minimum support threshold
    bool isFrequent = rule.isFrequent(sdb, minsup);

    // Verify rule frequency
    EXPECT_FALSE(isFrequent); // Support = 1/3, which is <= 0.5
}

// Test: Rule Validity
TEST_F(RuleTest, IsValidTest) {
    Itemset antecedent({1, 2});
    Itemset consequent({3});
    Rule rule(antecedent, consequent);

    double minsup = 0.5;  // Minimum support threshold
    double minconf = 0.6; // Minimum confidence threshold
    bool isValid = rule.isValid(sdb, minsup, minconf);

    // Verify rule validity
    EXPECT_FALSE(isValid);
}

// Test: Rule Validity (Low tresholds)
TEST_F(RuleTest, IsValidTestLowTresholdTests) {
    Itemset antecedent({1, 2});
    Itemset consequent({3});
    Rule rule(antecedent, consequent);

    double minsup = 0.2;  // Minimum support threshold
    double minconf = 0.2; // Minimum confidence threshold
    bool isValid = rule.isValid(sdb, minsup, minconf);

    // Verify rule validity
    EXPECT_TRUE(isValid); // The rule satisfies both support and confidence thresholds
}

// Test: Invalid Rule (Low Support)
TEST_F(RuleTest, InvalidRuleLowSupportTest) {
    Itemset antecedent({4, 5}); // Antecedent not in database
    Itemset consequent({6});   // Consequent not in database
    Rule rule(antecedent, consequent);

    double minsup = 0.2; // Minimum support threshold
    bool isFrequent = rule.isFrequent(sdb, minsup);

    // Verify invalidity due to low support
    EXPECT_FALSE(isFrequent); // Support = 0, which is < 0.2
}

// Test: Invalid Rule (Low Confidence)
TEST_F(RuleTest, InvalidRuleLowConfidenceTest) {
    Itemset antecedent({1, 2});
    Itemset consequent({4});
    Rule rule(antecedent, consequent);

    double minsup = 0.2;  // Minimum support threshold
    double minconf = 0.9; // High minimum confidence threshold
    bool isValid = rule.isValid(sdb, minsup, minconf);

    // Verify invalidity due to low confidence
    EXPECT_FALSE(isValid); // Confidence is below 0.9
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
