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
    std::string toString() const {
        return antecedent.toString() + " -> " + consequent.toString();
    }

    // Custom hash function
    std::size_t hash() const {
        return std::hash<std::string>()(toString());
    }

    // Find sequences where the rule applies
    void findSequencesWithRule(const std::vector<std::vector<int>>& sdb) const {
        if (antecedent.getOccurrences().empty()) {
            antecedent.computeOccurrences(sdb);
        }
        if (consequent.getOccurrences().empty()) {
            consequent.computeOccurrences(sdb);
        }
        sequences_with_rule.clear();
        for (const auto& [k, v] : antecedent.getOccurrences()) {
            if (consequent.getOccurrences().count(k) > 0 &&
                v.first < consequent.getOccurrences().at(k).second) {
                sequences_with_rule.push_back(k);
            }
        }
    }

    // Compute support
    void computeSupport(const std::vector<std::vector<int>>& sdb) const {
        if (sequences_with_rule.empty()) {
            findSequencesWithRule(sdb);
        }
        support = static_cast<double>(sequences_with_rule.size()) / sdb.size();
    }

    // Compute confidence
    void computeConfidence(const std::vector<std::vector<int>>& sdb) const {
        if (support < 0) {
            computeSupport(sdb);
        }
        confidence = support * sdb.size() / antecedent.getOccurrences().size();
    }

    // Check if the rule is frequent
    bool isFrequent(const std::vector<std::vector<int>>& sdb, double minsup) const {
        if (support < 0) {
            computeSupport(sdb);
        }
        return support >= minsup;
    }

    // Check if the rule is valid
    bool isValid(const std::vector<std::vector<int>>& sdb, double minsup, double minconf) const {
        if (!isFrequent(sdb, minsup)) {
            return false;
        }
        if (confidence < 0) {
            computeConfidence(sdb);
        }
        return confidence >= minconf;
    }

    // Getters for support and confidence
    double getSupport() const { return support; }
    double getConfidence() const { return confidence; }

    // Print rule (for debugging)
    void printRule() const {
        std::cout << toString() << " [support: " << support
                  << ", confidence: " << confidence << "]" << std::endl;
    }

    std::vector<int> getSequencesWithRule() const {
        return sequences_with_rule;
    }

    const Itemset& getAntecedent() const {
        return antecedent;
    }

    const Itemset& getConsequent() const {
        return consequent;
    }

    bool operator<(const Rule& other) const {
        // Compare the antecedents first, and if they are equal, compare the consequents
        if (antecedent < other.antecedent) return true;
        if (other.antecedent < antecedent) return false;
        return consequent < other.consequent;
    }
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
