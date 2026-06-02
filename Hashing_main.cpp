#include<iostream>
#include<string>
#include"ChainingHashMap.h"
#include"OpenAddressing.h"

int main() {

    // Test 1: Verification Test of ChainingHashMap

    std::cout << " TESTING SEPARATE CHAINING MAP \n";
    ChainingHashMap<int, std::string> chainMap;
    chainMap[1] = "Apple";
    chainMap[2] = "Banana";
    chainMap[10] = "Cherry"; 

    ChainingHashMap<int, std::string> secureCopy = chainMap; 
    secureCopy[1] = "Altered Apple"; 

    std::cout << "Original Map Content via Iterator Range Loop:\n";
    for (auto item : chainMap) {
        std::cout << "  Key: " << item.first << " -> Value: " << item.second << "\n";
    }
    std::cout << "Copied Map Content:\n";
    for (auto item : secureCopy) {
        std::cout << "  Key: " << item.first << " -> Value: " << item.second << "\n";
    }

    // Test 2: Verification Test of OpenAddressingHashMap

    std::cout << "\n TESTING OPEN ADDRESSING MAP (DOUBLE HASHING) \n";
    OpenAddressingHashMap<std::string, int> openMap(7, DOUBLE_HASH);
    openMap["Physics"] = 98;
    openMap["Chemistry"] = 87;
    openMap["Biology"] = 93;

    // Test item deletion (Tombstone layout)
    openMap.erase("Chemistry");

    std::cout << "Active Map Content after deletion via Iterator Range Loop:\n";
    for (auto item : openMap) {
        std::cout << "  Subject: " << item.first << " -> Score: " << item.second << "\n";
    }

    // Test Error Handling 
    try {
        std::cout << "\nAttempting access to dropped subject 'Chemistry' via .at():\n";
        std::cout << openMap.at("Chemistry") << std::endl;
    } 
    catch (const std::out_of_range& e) {
        std::cout << "Exception Caught ->" << e.what() << "\n";
    }

    return 0;
}