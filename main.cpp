#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include "erminer.hpp"
#include "itemset.hpp"
#include "rules.hpp"

const std::string DATA_PATH = "./data/SIGN.txt";
const std::string OUTPUT_PATH = "./data/rules.csv";
const std::string SEQUENCE_SEPARATOR = " -1 -2\n";
const std::string ITEM_SEPARATOR = " -1 ";
const double MINSUP = 0.5; 
const double MINCONF = 0.75;

std::vector<std::vector<int>> loadData(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open data file: " + filePath);
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                        (std::istreambuf_iterator<char>()));
    file.close();

    std::vector<std::string> rawSequences;
    size_t pos = 0;
    while ((pos = content.find(SEQUENCE_SEPARATOR)) != std::string::npos) {
        rawSequences.push_back(content.substr(0, pos));
        content.erase(0, pos + SEQUENCE_SEPARATOR.length());
    }
    rawSequences.push_back(content);

    std::vector<std::vector<int>> data;
    for (const auto& rawSeq : rawSequences) {
        std::vector<int> sequence;
        std::stringstream ss(rawSeq);
        std::string item;
        while (std::getline(ss, item, ITEM_SEPARATOR[0])) {
            if(std::stoi(item) >= 0)
                sequence.push_back(std::stoi(item));
        }
        data.push_back(sequence);
    }

    return data;
}

int main() {
    try {
        // Load data
        std::cout << "Loading data from: " << DATA_PATH << std::endl;
        auto data = loadData(DATA_PATH);

        std::cout << "Loaded (" << data.size() << ") lines of data" <<std::endl;
         // Initialize the ERMiner model
        ERMiner miner(MINSUP, MINCONF, false, false);

        // for(int i = 0; i < data.size(); i++){
        //     std::cout << "[";
        //     for(int j = 0; j < data[i].size(); j++) std::cout << data[i][j] << ", ";
        //     std::cout << "]" <<std::endl;
        // }

        // Fit the model
        std::cout << "Fitting the model..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        miner.fit(data); 
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = stop - start;

        // Output results
        std::cout << "Time elapsed: " << elapsed.count() << " seconds" << std::endl;
        std::cout << miner.getValidRules().size() << " valid rules found." << std::endl;

        // Save rules to a CSV file
        std::cout << "Saving rules to: " << OUTPUT_PATH << std::endl;
        miner.rulesToCsv(OUTPUT_PATH);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
