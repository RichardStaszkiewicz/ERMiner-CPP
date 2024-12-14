#include "itemset.hpp"

std::size_t Itemset::hash() const {
    std::vector<int> sorted_elements(this->begin(), this->end());
    std::sort(sorted_elements.begin(), sorted_elements.end());
    std::size_t h = 0;
    for (int elem : sorted_elements) {
        h ^= std::hash<int>()(elem) + 0x9e3779b9 + (h << 6) + (h >> 2); // Standard hash combine
    }
    return h;
}

void Itemset::printOccurrences() const {
    for (const auto& [sid, range] : occurrences) {
        std::cout << "Sequence " << sid << ": [" << range.first << ", " << range.second << "]\n";
    }
}

std::string Itemset::toString() const {
    std::ostringstream oss;

    for (std::set<int>::const_iterator it = this->cbegin(); it != this->cend(); ++it) {
        oss << *it;
        if (std::next(it) != this->cend()) { // Add a separator if it's not the last element
            oss << " ";
        }
    }

    // Convert the stream to a string
    std::string result = oss.str();
    return "Itemset{" + result + "}";
}

bool Itemset::isSubsetOf(const std::vector<int>& sequence) const {
    auto seq_it = sequence.begin();
    for (int item : *this) {
        seq_it = std::find(seq_it, sequence.end(), item);
        if (seq_it == sequence.end()) {
            return false; // If any item is not found, it's not a subset
        }
    }
    return true; // All items were found in the sequence in order
}

void ITEMSET::Itemset::computeOccurrences(const std::vector<std::vector<int>>& sdb) {
    if (this->empty()) {
        throw std::logic_error("Itemset is empty!");
    }

    auto iter = this->begin();
    int first_item = *iter;
    occurrences.clear();

    // Initial computation for the first item
    for (size_t i = 0; i < sdb.size(); ++i) {
        const auto& seq = sdb[i];
        auto first_pos = std::find(seq.begin(), seq.end(), first_item);
        if (first_pos != seq.end()) {
            auto last_pos = std::find(seq.rbegin(), seq.rend(), first_item).base() - 1;
            occurrences[i] = {std::distance(seq.begin(), first_pos), std::distance(seq.begin(), last_pos)};
        }
    }

    // Update occurrences for the remaining items
    while (++iter != this->end()) {
        occurrences = updateOccurrences(*iter, sdb);
    }
}


std::map<int, std::pair<int, int>> Itemset::updateOccurrences(int item, const std::vector<std::vector<int>>& sdb) {
    std::map<int, std::pair<int, int>> updatedOccurrences;
    
    for (const auto& [sid, range] : occurrences) {
        const auto& seq = sdb[sid];
        auto first_pos = std::find(seq.begin(), seq.end(), item);
        if (first_pos != seq.end()) {
            auto last_pos = std::find(seq.rbegin(), seq.rend(), item).base() - 1;

            int new_first = std::max(range.first, static_cast<int>(std::distance(seq.begin(), first_pos)));
            int new_last = std::min(range.second, static_cast<int>(std::distance(seq.begin(), last_pos)));

            updatedOccurrences[sid] = {new_first, new_last};
        }
    }
    
    return updatedOccurrences;
}

std::map<int, std::pair<int, int>> Itemset::getOccurrences() const{return occurrences;}

std::vector<int> Itemset::toSortedVector() const {return std::vector<int>(this->begin(), this->end());}