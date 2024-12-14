/**
 * @file rules.hpp
 * @brief Declaration of the Rule class for association rule representation and evaluation.
 * @details The Rule class models an association rule comprising an antecedent and a consequent,
 *          with methods to compute support, confidence, and validate the rule based on minimum thresholds.
 * @version 1.0.0
 * @date 2024-12-14
 * @author Richard Staszkiewicz
 */

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

/**
 * @class Rule
 * @brief Represents an association rule with methods for support, confidence, and validation.
 * @details The Rule class manages an antecedent and a consequent (both `Itemset` objects), 
 *          with functions to compute the rule's validity in a sequence database.
 */
class Rule {
private:
    /**
     * @brief The antecedent of the rule.
     */
    mutable Itemset antecedent;

    /**
     * @brief The consequent of the rule.
     */
    mutable Itemset consequent;

    /**
     * @brief The support value of the rule.
     * @details Stored as a mutable double to allow lazy computation.
     */
    mutable double support = -1.0;

    /**
     * @brief The confidence value of the rule.
     * @details Stored as a mutable double to allow lazy computation.
     */
    mutable double confidence = -1.0;

    /**
     * @brief List of sequence IDs where the rule applies.
     */
    mutable std::vector<int> sequences_with_rule;

public:
    /**
     * @brief Constructor to initialize a rule with antecedent and consequent.
     * @param ant The antecedent of the rule.
     * @param cons The consequent of the rule.
     */
    Rule(const Itemset& ant, const Itemset& cons)
        : antecedent(ant), consequent(cons) {}

    /**
     * @brief Generates a string representation of the rule.
     * @return A string in the format `antecedent -> consequent`.
     */
    std::string toString() const;

    /**
     * @brief Computes a hash value for the rule.
     * @return The hash value.
     */
    std::size_t hash() const;

    /**
     * @brief Finds the sequences in which the rule is applicable.
     * @param sdb A sequence database represented as a vector of integer vectors.
     */
    void findSequencesWithRule(const std::vector<std::vector<int>>& sdb) const;

    /**
     * @brief Computes the support of the rule.
     * @param sdb A sequence database represented as a vector of integer vectors.
     * @details The support is the proportion of sequences in the database that satisfy the rule.
     */
    void computeSupport(const std::vector<std::vector<int>>& sdb) const;

    /**
     * @brief Computes the confidence of the rule.
     * @param sdb A sequence database represented as a vector of integer vectors.
     * @details The confidence is the ratio of sequences containing both the antecedent and consequent
     *          to sequences containing the antecedent.
     */
    void computeConfidence(const std::vector<std::vector<int>>& sdb) const;

    /**
     * @brief Checks if the rule is frequent based on a minimum support threshold.
     * @param sdb A sequence database represented as a vector of integer vectors.
     * @param minsup The minimum support threshold.
     * @return `true` if the rule's support is greater than or equal to the threshold; otherwise, `false`.
     */
    bool isFrequent(const std::vector<std::vector<int>>& sdb, double minsup) const;

    /**
     * @brief Validates the rule based on minimum support and confidence thresholds.
     * @param sdb A sequence database represented as a vector of integer vectors.
     * @param minsup The minimum support threshold.
     * @param minconf The minimum confidence threshold.
     * @return `true` if the rule meets both thresholds; otherwise, `false`.
     */
    bool isValid(const std::vector<std::vector<int>>& sdb, double minsup, double minconf) const;

    /**
     * @brief Prints the rule and its metrics (support and confidence) for debugging.
     */
    void printRule() const;

    /**
     * @brief Retrieves the support value of the rule.
     * @return The support value.
     */
    double getSupport() const;

    /**
     * @brief Retrieves the confidence value of the rule.
     * @return The confidence value.
     */
    double getConfidence() const;

    /**
     * @brief Retrieves the list of sequence IDs where the rule applies.
     * @return A vector of sequence IDs.
     */
    std::vector<int> getSequencesWithRule() const;

    /**
     * @brief Retrieves the antecedent of the rule.
     * @return A const reference to the antecedent.
     */
    const Itemset& getAntecedent() const;

    /**
     * @brief Retrieves the consequent of the rule.
     * @return A const reference to the consequent.
     */
    const Itemset& getConsequent() const;

    /**
     * @brief Compares two rules for ordering.
     * @param other The rule to compare against.
     * @return `true` if this rule is less than the other; otherwise, `false`.
     * @details Comparison is based on lexicographical ordering of antecedent and consequent.
     */
    bool operator<(const Rule& other) const;
};

/**
 * @namespace std
 * @brief Provides a specialization of the `std::hash` template for the Rule class.
 */
namespace std {
    template <>
    struct hash<Rule> {
        /**
         * @brief Computes the hash value of a Rule.
         * @param rule The rule to hash.
         * @return The computed hash value.
         */
        size_t operator()(const Rule& rule) const {
            return rule.hash();
        }
    };
}

#endif
