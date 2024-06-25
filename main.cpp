#include "ArbolB.h"

#include <iostream>
#include <fstream>
#include <string>
#include "libs/json.hpp"


using json = nlohmann::json;

void storeJsonInfo(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filePath << std::endl;
        return;
    }

    json jsonData;
    try {
        file >> jsonData;
    } catch (const std::exception& e) {
        std::cout << "Failed to parse JSON data: " << e.what() << std::endl;
        return;
    }

    // TODO: Process the JSON data as needed

    file.close();
}

int main() {
    BTree btree;
    btree.insert(10);
    btree.insert(20);
    btree.insert(5);
    btree.insert(15);
    btree.insert(25);
    btree.insert(30);

    btree.print();

    std::string filePath = "/path/to/your/file.json";
    storeJsonInfo(filePath);

    return 0;
}