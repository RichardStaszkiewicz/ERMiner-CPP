#include "itemset.hpp"

int main() {
    // Example usage
    std::vector<int> elements = {1, 2};
    Itemset itemset(elements);

    std::vector<std::vector<int>> sdb = {
        {1, 2, 3, 4, 1},
        {2, 3, 4, 1, 2},
        {3, 1, 4, 2}
    };

    itemset.computeOccurrences(sdb);
    itemset.printOccurrences();

    return 0;
}
