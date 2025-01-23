---
marp: true
---

# ERMiner: A C++ Implementation for Rule Mining
ERMiner is a C++ implementation of a frequent sequential rule mining algorithm. It is designed to efficiently discover frequent rules in sequential databases. The project supports operations such as equivalence class generation, rule validation, and exporting results to a CSV file.

## Features
* Frequent Rule Mining: Identify rules that meet user-defined minimum support (minsup) and confidence (minconf) thresholds.

* Equivalence Classes:
- Left Equivalence Class (_findLeftEquivalenceClasses): Groups rules by their antecedents.
- Right Equivalence Class (_findRightEquivalenceClasses): Groups rules by their consequents.
* Recursive Search:
- _leftSearch: Expands rules by appending items to the consequent.
- _rightSearch: Expands rules by appending items to the antecedent.
* CSV Export: Save the discovered rules to a CSV file with their respective support and confidence values.

## Project Structure
```plaintext
.
├── include/
│   ├── erminer.hpp       # Core ERMiner class
│   ├── itemset.hpp       # Itemset class for handling sets of items
│   ├── rules.hpp         # Rule class for storing and validating rules
├── lib/
│   ├── CMakeLists.txt    # CMake build configuration
│   ├── liberminer.a      # Compiled static library for the project
├── source/
│   ├── erminer.cpp       # Core ERMiner class implementation
│   ├── itemset.cpp       # Itemset class implementation
│   ├── rules.cpp         # Rule class implementation
├── tests/
│   ├── test_erminer.cpp  # GTest-based unit tests
│   ├── test_itemset.cpp  # GTest-based unit tests
│   ├── test_rules.cpp    # GTest-based unit tests
├── doc/                  # Directory for documentation
│   ├── ...               # GTest-based unit tests
├── .gitignore            # Definition of files excluded from repository
├── README.md             # Project description and usage instructions
├── main.cpp              # Example program using the `erminer` library
├── LICENSE               # Project license
└── CMakeLists.txt        # CMake build configuration
```

## Dependencies
* C++17: Required for modern C++ features like structured bindings and `std::optional`.
* Google Test (GTest): Used for unit testing the implementation.

## Installation
### Prerequisites
1. Ensure you have a C++17-compatible compiler (e.g., GCC 7+ or Clang 5+).
2. Install Google Test if not already available.

### Build Instructions
1. Clone the repository:

```bash
git clone https://github.com/yourusername/erminer-cpp.git
cd erminer-cpp
```
2. Create a build directory and run CMake:

```bash
mkdir build && cd build
cmake ..
make
```

3. Run the unit tests:

```bash
./runItemsetTests
./runRulesTests
./runERMinerTests
```

## Usage
### Example Input
The input is a sequence database represented as a vector of vectors, where each inner vector is a sequence of integers.

```cpp
std::vector<std::vector<int>> sdb = {
    {1, 2, 3, 4, 1},
    {2, 3, 4, 1, 2},
    {3, 1, 4, 2}
};
```

### Discovering Rules
```cpp
#include "erminer.hpp"

int main() {
    // Set minimum support and confidence thresholds
    double minsup = 0.5;
    double minconf = 0.5;

    // Initialize the ERMiner instance
    ERMiner miner(minsup, minconf);

    // Fit the model on the sequence database
    miner.fit(sdb);

    // Retrieve the discovered rules
    auto rules = miner.getValidRules();
    for (const auto& rule : rules) {
        rule.printRule();
    }

    // Export rules to a CSV file
    miner.rulesToCsv("rules.csv");

    return 0;
}
```

## CSV Output
The rules are saved in a CSV file named rules.csv (or any user-defined name). The file contains columns for antecedents, consequents, support, and confidence:

```csv
antecedent,consequent,support,confidence
"Itemset{1}","Itemset{2}",1.000000,1.000000
"Itemset{1}","Itemset{2 4}",0.666667,0.666667
"Itemset{1}","Itemset{4}",0.666667,0.666667
```

## Fork the repository.
1. Create a new branch for your feature or bug fix.
2. Submit a pull request with a clear description of your changes.

## License
This project is licensed under the [GNU GENERAL PUBLIC LICENSE](./LICENSE)

## References
The algorithm is based on a paper:
```bibtex
@inproceedings{fournier2014erminer,
  title={ERMiner: sequential rule mining using equivalence classes},
  author={Fournier-Viger, Philippe and Gueniche, Ted and Zida, Souleymane and Tseng, Vincent S},
  booktitle={Advances in Intelligent Data Analysis XIII: 13th International Symposium, IDA 2014, Leuven, Belgium, October 30--November 1, 2014. Proceedings 13},
  pages={108--119},
  year={2014},
  organization={Springer}
}
```

If the current implementation is to be cited, please refer as:
```bibtex 
@misc{ERMinerRS2025,
  author = {Richard Staszkiewicz},
  title = {ERMiner: Cpp},
  year = {2025  },
  publisher = {GitHub},
  journal = {GitHub repository},
  howpublished = {\url{https://github.com/RichardStaszkiewicz/ERMiner-CPP}},
  release = {1}
}
```

## Acknowledgements
This project is inspired by the Python implementation of the ERMiner algorithm by [alessandroaere](https://github.com/alessandroaere) and aims to provide a more efficient C++ solution for rule mining.

<!-- 
the program might be working much slower because the Rules do not cache the antadescant and consequent as singletons. To be improved upon.
-->