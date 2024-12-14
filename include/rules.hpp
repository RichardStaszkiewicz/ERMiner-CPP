#ifndef RULES
#define RULES

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <numeric>
#include "itemset.hpp"

class Rule {
private:
    mutable Itemset antecedent;
    mutable Itemset consequent;
    mutable double support = -1.0;           // Allow modification in const methods
    mutable double confidence = -1.0;       // Allow modification in const methods
    mutable std::vector<int> sequences_with_rule; // Allow modification in const methods

public:
    // Constructor
    Rule(const Itemset& ant, const Itemset& cons)
        : antecedent(ant), consequent(cons) {}

    // String representation
    std::string toString() const;

    // Custom hash function
    std::size_t hash() const;

    // Find sequences where the rule applies
    void findSequencesWithRule(const std::vector<std::vector<int>>& sdb) const;

    // Compute support
    void computeSupport(const std::vector<std::vector<int>>& sdb) const;

    // Compute confidence
    void computeConfidence(const std::vector<std::vector<int>>& sdb) const;

    // Check if the rule is frequent
    bool isFrequent(const std::vector<std::vector<int>>& sdb, double minsup) const;

    // Check if the rule is valid
    bool isValid(const std::vector<std::vector<int>>& sdb, double minsup, double minconf) const;

    // Print rule (for debugging)
    void printRule() const;
    
    // Getters for support and confidence
    double getSupport() const;
    double getConfidence() const;
    std::vector<int> getSequencesWithRule() const;
    const Itemset& getAntecedent() const;
    const Itemset& getConsequent() const;
    bool operator<(const Rule& other) const;
};

namespace std {
    template <>
    struct hash<Rule> {
        size_t operator()(const Rule& rule) const {
            return rule.hash();  // Use the hash function defined in Rule
        }
    };
}


#endif
