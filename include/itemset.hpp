#ifndef ITEMSET
#define ITEMSET

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <sstream>

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
    std::size_t hash() const;

    // Compute occurrences for the entire itemset
    void computeOccurrences(const std::vector<std::vector<int>>& sdb);

    // Update occurrences with an additional item
    std::map<int, std::pair<int, int>> updateOccurrences(int item, const std::vector<std::vector<int>>& sdb);

    // Print occurrences (for debugging)
    void printOccurrences() const;

    std::string toString() const;

    std::map<int, std::pair<int, int>> getOccurrences() const;

    bool isSubsetOf(const std::vector<int>& sequence) const;

    // Convert the Itemset to a sorted vector
    std::vector<int> toSortedVector() const;


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
