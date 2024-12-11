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
#include "itemset.hpp"
#include "rules.hpp"

class ERMiner 
{
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
        int i, const std::set<Rule>& rules, const std::vector<std::vector<int>>& sdb) {
        std::map<Itemset, std::set<Rule>> leq;
        for (const auto& rule : rules) {
            if (rule.isFrequent(sdb, minsup) && rule.getAntecedent().size() == i) {
                leq[rule.getAntecedent()].insert(rule);
            }
        }
        return leq;
    }

    // Find right equivalence classes
    std::map<Itemset, std::set<Rule>> _findRightEquivalenceClasses(
        int i, const std::set<Rule>& rules, const std::vector<std::vector<int>>& sdb) {
        std::map<Itemset, std::set<Rule>> req;
        for (const auto& rule : rules) {
            if (rule.isFrequent(sdb, minsup) && rule.getConsequent().size() == i) {
                req[rule.getConsequent()].insert(rule);
            }
        }
        return req;
    }

    // First scan
    std::pair<std::map<Itemset, std::set<Rule>>, std::map<Itemset, std::set<Rule>>>
    _firstScan(const std::vector<std::vector<int>>& sdb) {
        std::set<int> itemset;
        for (const auto& s : sdb) {
            itemset.insert(s.begin(), s.end());
        }

        // Compute pair cooccurrences
        for (auto it1 = itemset.begin(); it1 != itemset.end(); ++it1) {
            for (auto it2 = std::next(it1); it2 != itemset.end(); ++it2) {
                _SCM[std::make_pair(*it1, *it2)] = cooccurs(Itemset({*it1}), Itemset({*it2}), sdb);
            }
        }

        // Generate initial rules
        std::set<Rule> rules11;
        for (auto it1 = itemset.begin(); it1 != itemset.end(); ++it1) {
            for (auto it2 = itemset.begin(); it2 != itemset.end(); ++it2) {
                if (*it1 != *it2) {
                    rules11.insert(Rule(Itemset({*it1}), Itemset({*it2})));
                }
            }
        }

        // Filter frequent and valid rules
        std::set<Rule> frequent_rules11;
        for (const auto& rule : rules11) {
            if (rule.isFrequent(sdb, minsup)) {
                frequent_rules11.insert(rule);
                if (rule.isValid(sdb, minsup, minconf)) {
                    valid_rules.insert(rule);
                }
            }
        }

        // Find equivalence classes
        auto leq = _findLeftEquivalenceClasses(1, frequent_rules11, sdb);
        auto req = _findRightEquivalenceClasses(1, frequent_rules11, sdb);

        return {leq, req};
    }

    // Left search
    void _leftSearch(const std::set<Rule>& leq, const std::vector<std::vector<int>>& sdb) {
        std::set<Rule> leq1;
        for (auto it1 = leq.begin(); it1 != leq.end(); ++it1) {
            for (auto it2 = std::next(it1); it2 != leq.end(); ++it2) {
                const auto& r = *it1;
                const auto& s = *it2;

                auto yr = r.getConsequent().toSortedVector();
                auto ys = s.getConsequent().toSortedVector();

                if (std::equal(yr.begin(), yr.end() - 1, ys.begin())) {
                    int c = yr.back();
                    int d = ys.back();
                    if (_SCM[{std::min(c, d), std::max(c, d)}] >= minsup) {
                        Itemset yrud(yr);
                        yrud.insert(d);
                        Rule t(r.getAntecedent(), yrud);
                        if (t.isFrequent(sdb, minsup)) {
                            leq1.insert(t);
                            if (t.isValid(sdb, minsup, minconf)) {
                                valid_rules.insert(t);
                            }
                        }
                    }
                }
            }
        }
        if (!leq1.empty()) {
            _leftSearch(leq1, sdb);
        }
    }

    // Fit the model
    void fit(const std::vector<std::vector<int>>& sdb) {
        auto [leq, req] = _firstScan(sdb);
        if (!single_consequent) {
            for (const auto& [_, H] : leq) {
                _leftSearch(H, sdb);
            }
        }
    }

    // Save rules to a CSV file
    void rulesToCsv(const std::string& csvFile) const {
        std::ofstream file(csvFile);
        file << "antecedent,consequent,support,confidence\n";
        for (const auto& rule : valid_rules) {
            file << "\"" << rule.getAntecedent().toString() << "\","
                 << "\"" << rule.getConsequent().toString() << "\","
                 << std::fixed << std::setprecision(6) << rule.getSupport() << ","
                 << rule.getConfidence() << "\n";
        }
        file.close();
    }

    std::set<Rule> getValidRules() const {
        return valid_rules;
    }
};

#endif
