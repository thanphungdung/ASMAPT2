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
std::string VendingMachine::filename;  // Define the static filename member




void loadCurrentData(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open " << filename << " for reading." << std::endl;
        return;
    }

    data.clear();  // Clear existing data
    int denomination, count;
    while (inFile >> denomination && inFile.ignore() && inFile >> count) {
        data[denomination] = count;
    }
    inFile.close();
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

// Handles the purchase transaction by collecting money and providing change.
void VendingMachine::handlePurchase(float foodCost) {
    // Initialize the denominations if they haven't been initialized yet.
    if (!denominationValue.size()) {
        initializeDenominationValues();  
    }

    // Create a new transaction and link it to the list of transactions.
    Transaction* transaction = new Transaction();
    transaction->next = head;
    head = transaction;

    int totalInput = 0;  // Total monetary input by the user in cents.
    int costInCents = static_cast<int>(foodCost * 100);  // Convert food cost from dollars to cents.
    std::cout << "Your total needs to be at least $" << foodCost << std::endl;

    int enterCount = 0;  // Tracks consecutive empty input to handle cancellation.
    while (totalInput < costInCents) {
        std::string inputLine;
        std::cout << "Please insert money (in cents): ";
        std::getline(std::cin, inputLine);

        // Handle empty input by increasing count or cancelling the transaction.
        if (inputLine.empty()) {
            enterCount++;
            if (enterCount == 1) {
                std::cout << "Transaction cancelled. Returning to main menu.\n";
                VendingMachine::clearTransactions();
                return;  // Exit the function to cancel the transaction.
            }
            continue;
        } else {
            enterCount = 0;  // Reset count on valid input.
        }

        // Attempt to convert input to an integer.
        int input;
        try {
            input = std::stoi(inputLine);
        } catch (const std::invalid_argument& ia) {
            std::cout << "Invalid input. Please enter a number: ";
            continue;
        }

        // Check if the denomination is valid.
        bool valid = false;
        for (const auto& pair : denominationValue) {
            if (pair.second == input) {
                addMonetaryInput(transaction, pair.first, 1);
                valid = true;
                break;
            }
        }

        // Prompt user to try again if the input denomination is not valid.
        if (!valid) {
            std::cout << "Invalid denomination. Please try again.\n";
            continue;
        }

        // Update total input and provide feedback.
        totalInput = calculateTotalInput(transaction);
        if (totalInput < costInCents) {
            std::cout << "You still need to give us $" << static_cast<double>(costInCents - totalInput) / 100.0 << std::endl;
        }
        std::cout << "Total inserted: $" << static_cast<double>(totalInput) / 100.0 << std::endl;
    }

    // Handle change if necessary.
    int change = totalInput - costInCents;
    if (change > 0) {
        calculateChange(transaction, change);
    } else if (change == 0) {
        std::cout << "Exact amount received. No change required.\n";
    }

    // Final message to confirm the transaction completion.
    std::cout << "Transaction complete. Thank you for your purchase!\n";
}



// Adds a specified amount of monetary input to a transaction.
void VendingMachine::addMonetaryInput(Transaction* transaction, Denomination denom, unsigned count) {
    // Access the monetaryInputs vector of the transaction.
    auto& inputs = transaction->monetaryInputs;

    // Search for an existing coin of the same denomination in the transaction's monetary inputs.
    auto it = std::find_if(inputs.begin(), inputs.end(), [denom](const Coin& c) {
        return c.denom == denom; // Predicate to find the coin with the same denomination.
    });

    // If the coin of the specified denomination is found, increase its count.
    if (it != inputs.end()) {
        it->count += count;
    } else {
        // If the coin of the specified denomination is not found, add a new coin with the count.
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
    VendingMachine::updateCoinInventory(); // Ensure the latest inventory is loaded

    // Sorted denominations from highest to lowest for optimal change calculation
    std::vector<std::pair<int, Denomination>> sortedDenoms;
    for (const auto& pair : denominationValue) {
        sortedDenoms.push_back({pair.second, pair.first});
    }
    std::sort(sortedDenoms.begin(), sortedDenoms.end(), [](const std::pair<int, Denomination>& a, const std::pair<int, Denomination>& b) {
        return a.first > b.first; // Descending order
    });

    std::vector<Coin> changeCoins;

    // Calculate the change efficiently using high to low denominations
    for (const auto& denom : sortedDenoms) {
        while (change >= denom.first && coinInventory[denom.second] > 0) {
            changeCoins.emplace_back(denom.second, 1);
            change -= denom.first;
            coinInventory[denom.second]--;
        }
    }

    if (change > 0) { // Restore coin inventory if complete change cannot be made
        for (const auto& coin : changeCoins) {
            coinInventory[coin.denom]++;
        }
        std::cout << "Unable to give the correct change due to insufficient coin inventory.\n";
        std::cout << "Transaction cancelled. Please collect any coins entered.\n";
        transaction->monetaryInputs.clear(); // Clear any recorded inputs
        return;
    }

    // If change is possible, officially record the change given
    transaction->changeGiven.insert(transaction->changeGiven.end(), changeCoins.begin(), changeCoins.end());

    // Display the change from smallest to largest denomination
    std::cout << "Your change is: ";
    // Sort the change coins for display from smallest to largest denomination
    std::sort(changeCoins.begin(), changeCoins.end(), [](const Coin& a, const Coin& b) {
        return VendingMachine::denominationValue[a.denom] < VendingMachine::denominationValue[b.denom];
    });

    // Display each coin separately
    for (const Coin& coin : changeCoins) {
        int denomValue = VendingMachine::denominationValue[coin.denom];
        std::cout << (denomValue >= 100 ? "$" : "") << (denomValue >= 100 ? denomValue / 100 : denomValue) << (denomValue < 100 ? "c " : " ");
    }
    std::cout << std::endl;
    VendingMachine::updateCoinInventory();
}




void VendingMachine::clearTransactions() {
    while (head) {
        Transaction* temp = head;
        head = head->next;
        delete temp;
    }
}




void VendingMachine::updateCoinInventory() {
    loadCurrentData(filename);  // Load or refresh data from the specified file

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

void VendingMachine::saveInventoryToFile(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::out | std::ios::trunc);  // Use the provided filename
    if (!outFile.is_open()) {
        std::cerr << "Failed to open " << filename << " for writing." << std::endl;  // Update the error message to use the filename variable
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