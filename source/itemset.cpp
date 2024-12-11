#include "itemset.hpp"

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

std::map<int, std::pair<int, int>> Itemset::getOccurrences() const{
    return occurrences;
}
