/**
 * @file erminer.hpp
 * @brief Declaration of the ERMiner class for extracting association rules from sequence databases.
 * @details The ERMiner algorithm is an efficient method for mining association rules, based on the approach
 *          described in ERMiner: sequential rule mining using equivalence classes by Fournier-Viger et. al. It identifies frequent and valid rules by leveraging
 *          equivalence class search strategies.
 * @version 1.0.0
 * @date 2024-12-14
 * @authors Richard Staszkiewicz
 * @note This class is based on the article:
 * ```
 * Fournier-Viger, P., Gueniche, T., Zida, S., & Tseng, V. S. (2014). ERMiner: sequential rule mining using equivalence classes. In Advances in Intelligent Data Analysis XIII : 13th International Symposium, IDA 2014, Leuven, Belgium, October 30–November 1, 2014. Proceedings 13 (pp. 108-119). Springer International Publishing.
 * ```
 */

#ifndef ERMINER
#define ERMINER

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <numeric>
#include "itemset.hpp"
#include "rules.hpp"

/**
 * @class ERMiner
 * @brief Implements the ERMiner algorithm for efficient rule extraction.
 * @details The ERMiner class uses equivalence class-based search to mine association rules from
 *          sequence databases, with customizable support and confidence thresholds.
 */
class ERMiner {
private:
    /**
     * @brief Minimum support threshold.
     */
    double minsup;

    /**
     * @brief Minimum confidence threshold.
     */
    double minconf;

    /**
     * @brief Whether to generate rules with a single consequent item.
     */
    bool single_consequent;

    /**
     * @brief Debug flag enabeling logging from within the class
     */
    bool debug;

    /**
     * @brief Left store for managing equivalence classes during the left search.
     * @details Maps itemsets to their corresponding rules.
     */
    std::unordered_map<Itemset, std::set<Rule>> _left_store;

    /**
     * @brief Set of valid rules discovered by the algorithm.
     */
    std::set<Rule> valid_rules;

    /**
     * @brief Support count map for item pairs.
     * @details Stores the co-occurrence support of pairs of items.
     */
    std::map<std::pair<int, int>, double> _SCM;

public:
    /**
     * @brief Constructor for the ERMiner class.
     * @param minsup Minimum support threshold.
     * @param minconf Minimum confidence threshold.
     * @param single_consequent Whether to generate rules with a single consequent (default: false).
     */
    ERMiner(double minsup, double minconf, bool single_consequent = false, bool debug = false)
        : minsup(minsup), minconf(minconf), single_consequent(single_consequent), debug(debug) {}

    /**
     * @brief Computes the co-occurrence of two itemsets in a sequence database.
     * @param a The first itemset.
     * @param b The second itemset.
     * @param sdb The sequence database.
     * @return The co-occurrence support value.
     */
    static double cooccurs(const Itemset& a, const Itemset& b, const std::vector<std::vector<int>>& sdb) {
        int count = 0;
        for (const auto& s : sdb) {
            if (a.isSubsetOf(s) && b.isSubsetOf(s)) {
                count++;
            }
        }
        return static_cast<double>(count) / sdb.size();
    }

    /**
     * @brief Finds left equivalence classes for a given rule size.
     * @param i The size of the antecedent.
     * @param rules The set of rules to process.
     * @param sdb The sequence database.
     * @return A map of itemsets to their corresponding left equivalence class rules.
     */
    std::map<Itemset, std::set<Rule>> _findLeftEquivalenceClasses(
        int i, const std::set<Rule>& rules, const std::vector<std::vector<int>>& sdb);

    /**
     * @brief Finds right equivalence classes for a given rule size.
     * @param i The size of the consequent.
     * @param rules The set of rules to process.
     * @param sdb The sequence database.
     * @return A map of itemsets to their corresponding right equivalence class rules.
     */
    std::map<Itemset, std::set<Rule>> _findRightEquivalenceClasses(
        int i, const std::set<Rule>& rules, const std::vector<std::vector<int>>& sdb);

    /**
     * @brief Performs the first scan of the sequence database to initialize equivalence classes.
     * @param sdb The sequence database.
     * @return A pair of maps containing left and right equivalence classes.
     */
    std::pair<std::map<Itemset, std::set<Rule>>, std::map<Itemset, std::set<Rule>>> _firstScan(
        const std::vector<std::vector<int>>& sdb);

    /**
     * @brief Conducts a left equivalence class search.
     * @param leq The left equivalence class rules to process.
     * @param sdb The sequence database.
     */
    void _leftSearch(const std::set<Rule>& leq, const std::vector<std::vector<int>>& sdb);

    /**
     * @brief Conducts a right equivalence class search.
     * @param req The right equivalence class rules to process.
     * @param sdb The sequence database.
     */
    void _rightSearch(const std::set<Rule>& req, const std::vector<std::vector<int>>& sdb);

    /**
     * @brief Fits the ERMiner model to a sequence database.
     * @param sdb The sequence database.
     */
    void fit(const std::vector<std::vector<int>>& sdb);

    /**
     * @brief Exports the valid rules to a CSV file.
     * @param csvFile The path to the output CSV file.
     */
    void rulesToCsv(const std::string& csvFile) const;

    /**
     * @brief Retrieves the set of valid rules.
     * @return A set of valid rules.
     */
    std::set<Rule> getValidRules() const;

    /**
     * @brief Checks whether the single consequent mode is enabled.
     * @return `true` if single consequent mode is enabled; otherwise, `false`.
     */
    bool getSingleConsequent() const;
};

#endif
