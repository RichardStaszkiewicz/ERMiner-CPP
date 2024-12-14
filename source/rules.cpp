#include "rules.hpp"

std::string Rule::toString() const {
    return antecedent.toString() + " -> " + consequent.toString();
}

std::size_t Rule::hash() const {
    return std::hash<std::string>()(toString());
}

void Rule::findSequencesWithRule(const std::vector<std::vector<int>>& sdb) const {
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

void Rule::computeSupport(const std::vector<std::vector<int>>& sdb) const {
    if (sequences_with_rule.empty()) {
        findSequencesWithRule(sdb);
    }
    support = static_cast<double>(sequences_with_rule.size()) / sdb.size();
}

void Rule::computeConfidence(const std::vector<std::vector<int>>& sdb) const {
    if (support < 0) {
        computeSupport(sdb);
    }
    confidence = support * sdb.size() / antecedent.getOccurrences().size();
}

bool Rule::isFrequent(const std::vector<std::vector<int>>& sdb, double minsup) const {
    if (support < 0) {
        computeSupport(sdb);
    }
    return support >= minsup;
}

bool Rule::isValid(const std::vector<std::vector<int>>& sdb, double minsup, double minconf) const {
    if (!isFrequent(sdb, minsup)) {
        return false;
    }
    if (confidence < 0) {
        computeConfidence(sdb);
    }
    return confidence >= minconf;
}

double Rule::getSupport() const { return support; }
double Rule::getConfidence() const { return confidence; }
std::vector<int> Rule::getSequencesWithRule() const {return sequences_with_rule;}
const Itemset& Rule::getAntecedent() const {return antecedent;}
const Itemset& Rule::getConsequent() const {return consequent;}

bool Rule::operator<(const Rule& other) const {
    // Compare the antecedents first, and if they are equal, compare the consequents
    if (antecedent < other.antecedent) return true;
    if (other.antecedent < antecedent) return false;
    return consequent < other.consequent;
}

void Rule::printRule() const {
    std::cout << toString() << " [support: " << support
                << ", confidence: " << confidence << "]" << std::endl;
}

