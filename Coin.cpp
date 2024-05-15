#include "Coin.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>  
#include <iomanip>
#include <limits>

std::map<Denomination, int> VendingMachine::denominationValue;
Transaction* VendingMachine::head = nullptr;
std::map<int, int> data;
std::map<Denomination, int> VendingMachine::coinInventory;


std::map<int, int> loadCurrentData() {
    std::ifstream inFile("coins.dat");
    int denomination, count;

    if (!inFile.is_open()) {
        std::cerr << "Failed to open coins.dat for reading." << std::endl;
        return data;
    }

    while (inFile >> denomination && inFile.ignore() && inFile >> count) {
        data[denomination] = count;
    }

    inFile.close();
    return data;
}

void VendingMachine::initializeDenominationValues() {
    denominationValue[FIVE_CENTS] = 5;
    denominationValue[TEN_CENTS] = 10;
    denominationValue[TWENTY_CENTS] = 20;
    denominationValue[FIFTY_CENTS] = 50;
    denominationValue[ONE_DOLLAR] = 100;
    denominationValue[TWO_DOLLARS] = 200;
    denominationValue[FIVE_DOLLARS] = 500;
    denominationValue[TEN_DOLLARS] = 1000;
    denominationValue[TWENTY_DOLLARS] = 2000;
    denominationValue[FIFTY_DOLLARS] = 5000;
}

void VendingMachine::handlePurchase(float foodCost) {
    if (!denominationValue.size()) {
        initializeDenominationValues();  
    }

    Transaction* transaction = new Transaction();
    transaction->next = head;
    head = transaction;

    int totalInput = 0;
    int costInCents = static_cast<int>(foodCost * 100);
    std::cout << "Your total needs to be at least $" << foodCost << std::endl;

    int enterCount = 0;  // Count consecutive Enter key presses
    while (totalInput < costInCents) {
        std::string inputLine;
        std::cout << "Please insert money (in cents): ";
        std::getline(std::cin, inputLine);

        if (inputLine.empty()) {
            enterCount++;
            if (enterCount == 1 ) {
                std::cout << "Transaction cancelled. Returning to main menu.\n";
                VendingMachine::clearTransactions();
                return;  // Exit the function to cancel the transaction
            }
            continue;
        } else {
            enterCount = 0;  // Reset enter count when input is not empty
        }

        // Convert string input to integer
        int input;
        try {
            input = std::stoi(inputLine);
        } catch (const std::invalid_argument& ia) {
            std::cout << "Invalid input. Please enter a number: ";
            continue;
        }

        bool valid = false;
        for (const auto& pair : denominationValue) {
            if (pair.second == input) {
                addMonetaryInput(transaction, pair.first, 1);
                valid = true;
                break;
            }
        }

        if (!valid) {
            std::cout << "Invalid denomination. Please try again.\n";
            continue;
        }

        totalInput = calculateTotalInput(transaction);
        if (totalInput < costInCents) {
            std::cout << "You still need to give us $" << static_cast<double>(costInCents - totalInput) / 100.0 << std::endl;
        }
        std::cout << "Total inserted: $" << static_cast<double>(totalInput) / 100.0 << std::endl;
    }

    int change = totalInput - costInCents;
    if (change > 0) {
        calculateChange(transaction, change);
    } else if (change == 0) {
        std::cout << "Exact amount received. No change required.\n";
    }

    std::cout << "Transaction complete. Thank you for your purchase!\n";
}


void VendingMachine::addMonetaryInput(Transaction* transaction, Denomination denom, unsigned count) {
    auto& inputs = transaction->monetaryInputs;
    auto it = std::find_if(inputs.begin(), inputs.end(), [denom](const Coin& c) {
        return c.denom == denom;
    });
    if (it != inputs.end()) {
        it->count += count;
    } else {
        inputs.emplace_back(denom, count);
    }
}

int VendingMachine::calculateTotalInput(Transaction* transaction) {
    int total = 0;
    for (const Coin& coin : transaction->monetaryInputs) {
        total += denominationValue[coin.denom] * coin.count;
    }
    return total;
}

void VendingMachine::calculateChange(Transaction* transaction, int change) {
    // Prepare a sorted list of denominations from largest to smallest
    std::vector<std::pair<int, Denomination>> sortedDenoms;
    for (const auto& pair : denominationValue) {
        sortedDenoms.push_back({pair.second, pair.first});
    }
    // Sorting in descending order for optimal change calculation
    std::sort(sortedDenoms.begin(), sortedDenoms.end(), std::greater<>());

    // This vector will store each denomination used for change
    std::vector<Coin> changeCoins;

    // Calculate change using the largest denominations first
    for (const auto& denom : sortedDenoms) {
        while (change >= denom.first) {
            changeCoins.push_back(Coin(denom.second, 1)); // Add a coin of this denomination
            change -= denom.first;
        }
    }

    // Append the calculated coins to the transaction's changeGiven
    for (const auto& coin : changeCoins) {
        transaction->changeGiven.push_back(coin);
    }

    // Display the change in ascending order based on denomination value
    std::sort(changeCoins.begin(), changeCoins.end(), [](const Coin& a, const Coin& b) {
        return a.denom < b.denom;  // Sort by denomination enum values
    });

    std::cout << "Your change is: ";
    for (const Coin& coin : changeCoins) {
        int denomValue = denominationValue[coin.denom];
        if (denomValue >= 100) {  // Output in dollars if denomination is 100 cents or more
            std::cout << "$" << denomValue / 100 << " ";
        } else {  // Output in cents otherwise
            std::cout << denomValue << "c ";
        }
    }
    std::cout << std::endl;
}



void VendingMachine::clearTransactions() {
    while (head) {
        Transaction* temp = head;
        head = head->next;
        delete temp;
    }
}



void VendingMachine::updateCoinInventory() {
    std::map<int, int> data = loadCurrentData();

    // Update counts based on transactions
    Transaction* current = head;
    while (current != nullptr) {
        for (const Coin& coin : current->monetaryInputs) {
            data[denominationValue[coin.denom]] += coin.count;
        }
        for (const Coin& coin : current->changeGiven) {
            data[denominationValue[coin.denom]] -= coin.count;
        }
        current = current->next;
    }

    // Update the coinInventory with the latest data
    coinInventory.clear();
    for (const auto& pair : data) {
        coinInventory[static_cast<Denomination>(pair.first)] = pair.second;
    }
}


void VendingMachine::saveInventoryToFile() {
    std::ofstream outFile("coins.dat", std::ios::out | std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open coins.dat for writing." << std::endl;
        return;
    }

    for (const auto& pair : coinInventory) {
        outFile << pair.first << "," << pair.second << std::endl;
    }
    outFile.close();
}


void VendingMachine::displayBalance() {


    VendingMachine::updateCoinInventory();
    int totalValueCents = 0;

    std::cout << "\nBalance Summary\n"
              << "---------------\n"
              << "Denom | Quantity | Value\n"
              << "--------------------------\n";

    for (const auto& pair : coinInventory) {
        int value = pair.first * pair.second;
        totalValueCents += value;

        std::cout << std::left << std::setw(5) << pair.first  
                  << " | " << std::left << std::setw(8) << pair.second
                  << " |$" << std::right << std::setw(7) << std::fixed << std::setprecision(2) << value / 100.0 << "\n";
    }
    std::cout << "--------------------------\n"
              << std::setw(19) << "$" << std::right << std::setw(7) << std::fixed << std::setprecision(2) << totalValueCents / 100.0 << "\n";
}