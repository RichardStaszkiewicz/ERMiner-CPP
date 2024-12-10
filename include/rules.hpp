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
    Itemset antecedent;
    Itemset consequent;
    double support = -1.0;
    double confidence = -1.0;
    std::vector<int> sequences_with_rule;

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
    void findSequencesWithRule(const std::vector<std::vector<int>>& sdb) {
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
    void computeSupport(const std::vector<std::vector<int>>& sdb) {
        if (sequences_with_rule.empty()) {
            findSequencesWithRule(sdb);
        }
        support = static_cast<double>(sequences_with_rule.size()) / sdb.size();
    }

    // Compute confidence
    void computeConfidence(const std::vector<std::vector<int>>& sdb) {
        if (support < 0) {
            computeSupport(sdb);
        }
        confidence = support * sdb.size() / antecedent.getOccurrences().size();
    }

    // Check if the rule is frequent
    bool isFrequent(const std::vector<std::vector<int>>& sdb, double minsup) {
        if (support < 0) {
            computeSupport(sdb);
        }
        return support >= minsup;
    }

    // Check if the rule is valid
    bool isValid(const std::vector<std::vector<int>>& sdb, double minsup, double minconf) {
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

    std::vector<int> getSequencesWithRule(){
        return sequences_with_rule;
    }

    Itemset getAntecedent(){
        return antecedent;
    }
};


#endif