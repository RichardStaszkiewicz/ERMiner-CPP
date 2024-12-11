#ifndef ITEMSET
#define ITEMSET

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>
#include <stdexcept>

class Itemset : public std::set<int> {
private:
    // Map to store occurrences: key = sequence ID, value = pair(first, last)
    std::map<int, std::pair<int, int>> occurrences;

public:
    Itemset() : std::set<int>(), occurrences() {}

    // Constructor
    Itemset(const std::vector<int>& elements, const std::map<int, std::pair<int, int>>& initOccurrences = {})
        : std::set<int>(elements.begin(), elements.end()), occurrences(initOccurrences) {}

    // Custom hash function
    std::size_t hash() const {
        std::vector<int> sorted_elements(this->begin(), this->end());
        std::sort(sorted_elements.begin(), sorted_elements.end());
        std::size_t h = 0;
        for (int elem : sorted_elements) {
            h ^= std::hash<int>()(elem) + 0x9e3779b9 + (h << 6) + (h >> 2); // Standard hash combine
        }
        return h;
    }

    // Compute occurrences for the entire itemset
    void computeOccurrences(const std::vector<std::vector<int>>& sdb);

    // Update occurrences with an additional item
    std::map<int, std::pair<int, int>> updateOccurrences(int item, const std::vector<std::vector<int>>& sdb);

    // Print occurrences (for debugging)
    void printOccurrences() const {
        for (const auto& [sid, range] : occurrences) {
            std::cout << "Sequence " << sid << ": [" << range.first << ", " << range.second << "]\n";
        }
    }

    std::string toString() const {
        std::string s = "";
        for (const auto& [sid, range] : occurrences) {
            s += "Sequence " + std::to_string(sid) + ": [" + std::to_string(range.first) + ", " + std::to_string(range.second) + "]\n";
        }
        return s;
    }

    std::map<int, std::pair<int, int>> getOccurrences() const;

    bool isSubsetOf(const std::vector<int>& sequence) const {
        auto seq_it = sequence.begin();
        for (int item : *this) {
            seq_it = std::find(seq_it, sequence.end(), item);
            if (seq_it == sequence.end()) {
                return false; // If any item is not found, it's not a subset
            }
        }
        return true; // All items were found in the sequence in order
    }

    // Convert the Itemset to a sorted vector
    std::vector<int> toSortedVector() const {
        return std::vector<int>(this->begin(), this->end());
    }


};

namespace std {
    template <>
    struct hash<Itemset> {
        size_t operator()(const Itemset& itemset) const {
            return itemset.hash(); // Use the member hash function you defined
        }
    };
}

#endif
