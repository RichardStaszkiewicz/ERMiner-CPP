/**
 * @file itemset.hpp
 * @brief Declaration of the Itemset class for handling itemsets and their occurrences.
 * @details The Itemset class extends the `std::set<int>` to provide additional functionality,
 *          such as computing occurrences within sequences, generating string representations,
 *          and checking subset relationships.
 * @version 1.0.0
 * @date 2024-12-14
 * @author Richard Staszkiewicz
 */

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

/**
 * @class Itemset
 * @brief Represents a set of items with additional functionality for sequence analysis.
 * @details The Itemset class is an extension of `std::set<int>` with additional methods 
 *          for managing occurrences, generating string representations, and computing 
 *          relationships with sequences.
 */
class Itemset : public std::set<int> {
private:
    /**
     * @brief Map storing item occurrences within sequences.
     * @details The key represents the sequence ID, while the value is a pair of integers
     *          representing the first and last positions of the itemset in the sequence.
     */
    std::map<int, std::pair<int, int>> occurrences;

public:
    /**
     * @brief Default constructor for an empty Itemset.
     * @details Initializes an empty set and an empty map of occurrences.
     */
    Itemset() : std::set<int>(), occurrences() {}

    /**
     * @brief Constructs an Itemset from a vector of integers.
     * @param elements A vector of integers to initialize the itemset.
     * @param initOccurrences Optional map of initial occurrences.
     */
    Itemset(const std::vector<int>& elements, const std::map<int, std::pair<int, int>>& initOccurrences = {})
    : std::set<int>(elements.begin(), elements.end()), occurrences(initOccurrences) {}


    /**
     * @brief Computes the hash value for the Itemset.
     * @return The computed hash value.
     * @details Combines the hash values of all elements in the itemset.
     */
    std::size_t hash() const;

    /**
     * @brief Computes occurrences of the itemset in a sequence database.
     * @param sdb A sequence database represented as a vector of integer vectors.
     * @throw std::logic_error if the Itemset is empty.
     */
    void computeOccurrences(const std::vector<std::vector<int>>& sdb);

    /**
     * @brief Updates the occurrences by adding a new item.
     * @param item The item to add.
     * @param sdb The sequence database.
     * @return A map of updated occurrences.
     */
    std::map<int, std::pair<int, int>> updateOccurrences(int item, const std::vector<std::vector<int>>& sdb);

    /**
     * @brief Prints the occurrences for debugging purposes.
     * @details Outputs the sequence ID and the range of occurrences for each sequence.
     */
    void printOccurrences() const;

    /**
     * @brief Converts the Itemset to a string representation.
     * @return A string representation of the Itemset.
     */
    std::string toString() const;

    /**
     * @brief Retrieves the occurrences map.
     * @return A copy of the map storing the occurrences.
     */
    std::map<int, std::pair<int, int>> getOccurrences() const;

    /**
     * @brief Checks if the Itemset is a subset of a given sequence.
     * @param sequence A sequence of integers.
     * @return `true` if the Itemset is a subset of the sequence; otherwise, `false`.
     */
    bool isSubsetOf(const std::vector<int>& sequence) const;

    /**
     * @brief Converts the Itemset to a sorted vector.
     * @return A vector containing the sorted elements of the Itemset.
     */
    std::vector<int> toSortedVector() const;
};

/**
 * @namespace std
 * @brief Provides a specialization of the `std::hash` template for the Itemset class.
 */
namespace std {
    template <>
    struct hash<Itemset> {
        /**
         * @brief Hashes an Itemset instance.
         * @param itemset The Itemset to hash.
         * @return The hash value of the Itemset.
         */
        size_t operator()(const Itemset& itemset) const {
            return itemset.hash();
        }
    };
}

#endif
