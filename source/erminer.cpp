#include "erminer.hpp"


std::map<Itemset, std::set<Rule>> ERMiner::_findLeftEquivalenceClasses(
    int i, const std::set<Rule>& rules, const std::vector<std::vector<int>>& sdb) {
    std::map<Itemset, std::set<Rule>> leq;
    for (const auto& rule : rules) {
        if (rule.isFrequent(sdb, minsup) && rule.getAntecedent().size() == i) {
            leq[rule.getAntecedent()].insert(rule);
        }
    }
    return leq;
}

std::map<Itemset, std::set<Rule>> ERMiner::_findRightEquivalenceClasses(
    int i, const std::set<Rule>& rules, const std::vector<std::vector<int>>& sdb) {
    std::map<Itemset, std::set<Rule>> req;
    for (const auto& rule : rules) {
        if (rule.isFrequent(sdb, minsup) && rule.getConsequent().size() == i) {
            req[rule.getConsequent()].insert(rule);
        }
    }
    return req;
}

std::pair<std::map<Itemset, std::set<Rule>>, std::map<Itemset, std::set<Rule>>>
    ERMiner::_firstScan(const std::vector<std::vector<int>>& sdb) {
    std::set<int> itemset;
    for (const auto& s : sdb) {
        itemset.insert(s.begin(), s.end());
    }
    if(debug) std::cout << "[_first_scan] Itemset loaded with (" << itemset.size() << ") elements..." <<std::endl;
    // Compute pair cooccurrences
    for (auto it1 = itemset.begin(); it1 != itemset.end(); ++it1) {
        for (auto it2 = std::next(it1); it2 != itemset.end(); ++it2) {
            _SCM[std::make_pair(*it1, *it2)] = cooccurs(Itemset({*it1}), Itemset({*it2}), sdb);
        }
    }

    if(debug) std::cout << "[_first_scan] Coocurance pairs computet with (" << _SCM.size() << ") elements..." <<std::endl;
    // Generate initial rules (certainly does not need rules between oneself)
    std::set<Rule> rules11;
    for (auto it1 = itemset.begin(); it1 != itemset.end(); ++it1) {
        for (auto it2 = itemset.begin(); it2 != itemset.end(); ++it2) {
            if (*it1 != *it2) {
                rules11.insert(Rule(Itemset({*it1}), Itemset({*it2})));
            }
        }
    }
    if(debug) std::cout << "[_first_scan] Initial rules loaded with (" << rules11.size() << ") elements..." <<std::endl;

    // Filter frequent and valid rules
    std::set<Rule> frequent_rules11;
    int i = 0;
    for (const auto& rule : rules11) {
        if (rule.isFrequent(sdb, minsup)) {
            frequent_rules11.insert(rule);
            if (rule.isValid(sdb, minsup, minconf)) {
                valid_rules.insert(rule);
            }
        }
        if(debug && i % 100 == 0) std::cout << "[_first_scan] " << i << "/" << rules11.size() <<std::endl;
        i++;
    }
    if(debug) std::cout << "[_first_scan] Frequent rules loaded with (" << frequent_rules11.size() << ") elements..." <<std::endl;

    // Find equivalence classes
    auto leq = _findLeftEquivalenceClasses(1, frequent_rules11, sdb);
    auto req = _findRightEquivalenceClasses(1, frequent_rules11, sdb);

    return {leq, req};
}

void ERMiner::_leftSearch(const std::set<Rule>& leq, const std::vector<std::vector<int>>& sdb) {
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

void ERMiner::_rightSearch(const std::set<Rule>& req, const std::vector<std::vector<int>>& sdb) {
    std::set<Rule> req1;
    for (auto it1 = req.begin(); it1 != req.end(); ++it1) {
        for (auto it2 = std::next(it1); it2 != req.end(); ++it2) {
            const auto& r = *it1;
            const auto& s = *it2;

            auto xr = r.getAntecedent().toSortedVector();
            auto xs = s.getAntecedent().toSortedVector();

            if (std::equal(xr.begin(), xr.end() - 1, xs.begin())) {
                int c = xr.back();
                int d = xs.back();
                if (_SCM[{std::min(c, d), std::max(c, d)}] >= minsup) {
                    Itemset xrud(xr);
                    xrud.insert(d);
                    Rule t(xrud, r.getConsequent());
                    if (t.isFrequent(sdb, minsup)) {
                        req1.insert(t);
                        _left_store[t.getAntecedent()].insert(t);
                        if (t.isValid(sdb, minsup, minconf)) {
                            valid_rules.insert(t);
                        }
                    }
                }
            }
        }
    }
    if (!req1.empty()) {
        _rightSearch(req1, sdb);
    }
}

void ERMiner::fit(const std::vector<std::vector<int>>& sdb) {
    if(debug) std::cout << "Fitting started..." <<std::endl;
    auto [leq, req] = _firstScan(sdb);
    if(debug) std::cout << "First scan finished with LEQ of size (" <<leq.size() << ") and REQ of size (" << req.size() << ")..." <<std::endl;
    if (!single_consequent) {
        if(debug) std::cout << "Left searches to be performed: " << leq.size() <<std::endl;
        for (const auto& [_, H] : leq) {
            _leftSearch(H, sdb);
        }
    }
    std::cout << "Right searches to be performed: " << req.size() <<std::endl;
    for (const auto& [_, J] : req) {
        _rightSearch(J, sdb);
    }
    if(!single_consequent) {
        if(debug) std::cout << "Left searches to be performed: " << _left_store.size() <<std::endl;
        for(const auto& [_, K] : _left_store) {
            _leftSearch(K, sdb);
        }
    }
}

void ERMiner::rulesToCsv(const std::string& csvFile) const {
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

std::set<Rule> ERMiner::getValidRules() const {return valid_rules;}

bool ERMiner::getSingleConsequent() const {return single_consequent;}

