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

class ERMiner {
private:
    double minsup;
    double minconf;
    bool single_consequent;
    std::unordered_map<Itemset, std::set<Rule>> _left_store;
    std::set<Rule> valid_rules;
    std::map<std::pair<int, int>, double> _SCM; // Support count map for item pairs

public:
    // Constructor
    ERMiner(double minsup, double minconf, bool single_consequent = false)
        : minsup(minsup), minconf(minconf), single_consequent(single_consequent) {}

    // Static cooccurrence computation
    static double cooccurs(const Itemset& a, const Itemset& b, const std::vector<std::vector<int>>& sdb) {
        int count = 0;
        for (const auto& s : sdb) {
            if (a.isSubsetOf(s) && b.isSubsetOf(s)) {
                count++;
            }
        }
        return static_cast<double>(count) / sdb.size();
    }

    // Find left equivalence classes
    std::map<Itemset, std::set<Rule>> _findLeftEquivalenceClasses(
        int i, const std::set<Rule>& rules, const std::vector<std::vector<int>>& sdb);

    // Find right equivalence classes
    std::map<Itemset, std::set<Rule>> _findRightEquivalenceClasses(
        int i, const std::set<Rule>& rules, const std::vector<std::vector<int>>& sdb);

    // First scan
    std::pair<std::map<Itemset, std::set<Rule>>, std::map<Itemset, std::set<Rule>>>
    _firstScan(const std::vector<std::vector<int>>& sdb);

    // Left search
    void _leftSearch(const std::set<Rule>& leq, const std::vector<std::vector<int>>& sdb);

    // Right search
    void _rightSearch(const std::set<Rule>& req, const std::vector<std::vector<int>>& sdb);

    // Fit the model
    void fit(const std::vector<std::vector<int>>& sdb);

    // Save rules to a CSV file
    void rulesToCsv(const std::string& csvFile) const;

    // Getter for valid rules
    std::set<Rule> getValidRules() const;

    bool getSingleConsequent() const;
};

#endif
