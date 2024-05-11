#include "Coin.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

std::vector<Coin> monetaryInputs; // To store inputs
std::map<Denomination, int> Coin::denominationValue;

Coin::Coin(Denomination d, unsigned int c) : denom(d), count(c) {}

void Coin::initializeDenominationValues() {
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

void Coin::addMonetaryInput(Denomination denom, unsigned count) {
    for (auto& coin : monetaryInputs) {
        if (coin.denom == denom) {
            coin.count += count;
            return;
        }
    }
    monetaryInputs.push_back(Coin{denom, count});
}

int Coin::calculateTotalInput() {
    int total = 0;
    for (const Coin& coin : monetaryInputs) {
        total += denominationValue[coin.denom] * coin.count;
    }
    return total;
}

void Coin::handlePurchase(float foodCost) {
    int totalInput = 0;
    int costInCents = static_cast<int>(foodCost * 100); // Convert dollars to cents
    std::cout << "Your total needs to be at least $" << foodCost << std::endl;

    while (totalInput < costInCents) {
        int input;
        std::cout << "Please insert money (in cents): ";
        std::cin >> input;
        bool valid = false;
        for (auto& pair : denominationValue) {
            if (pair.second == input) {
                addMonetaryInput(pair.first);
                valid = true;
                break;
            }
        }

        if (!valid) {
            std::cout << "Invalid denomination. Please try again." << std::endl;
            continue;
        }

        totalInput = calculateTotalInput();
        if (totalInput < costInCents) {
            std::cout << "You still need to give us $" << static_cast<double>(costInCents - totalInput) / 100.0 << std::endl;
        }
        std::cout << "Total inserted: $" << static_cast<double>(totalInput) / 100.0 << std::endl;
    }

    int change = totalInput - costInCents;
    if (change > 0) {
        std::cout << "Calculating your change...\n";
        calculateChange(change);
    } else {
        std::cout << "No change required.\n";
    }

    std::cout << "Transaction complete.\n";
}

void Coin::calculateChange(int change) {
    // Prepare a sorted list of denominations from largest to smallest
    std::vector<std::pair<int, Denomination>> sortedDenoms;
    for (const auto& pair : denominationValue) {
        sortedDenoms.push_back({pair.second, pair.first});
    }
    // Sorting in descending order for optimal change calculation
    std::sort(sortedDenoms.begin(), sortedDenoms.end(), std::greater<>());

    std::vector<int> changeDenoms; // To store each denomination for change

    // Calculate change using the largest denominations first
    for (const auto& denom : sortedDenoms) {
        while (change >= denom.first) {
            changeDenoms.push_back(denom.first); // Add each denomination used to the vector
            change -= denom.first;
        }
    }

    // Display the change in ascending order
    std::cout << "Your change is: ";
    std::sort(changeDenoms.begin(), changeDenoms.end()); // Sort the change denominations vector in ascending order
    for (const auto& denom : changeDenoms) {
        if (denom >= 100) {  // Output in dollars if denomination is 100 cents or more
            std::cout << "$" << denom / 100 << " ";
        } else {  // Output in cents otherwise
            std::cout << denom << "c ";
        }
    }
    std::cout << std::endl;
}

void Coin::displayBalance() {
    // Calculate the total value and prepare data for display
    std::vector<std::pair<int, int>> balanceSummary;
    int totalValueCents = 0;
    
    for (const auto& coin : monetaryInputs) {
        int value = denominationValue[coin.denom] * coin.count;
        balanceSummary.emplace_back(denominationValue[coin.denom], value);
        totalValueCents += value;
    }
    
    // Sort by denomination value
    std::sort(balanceSummary.begin(), balanceSummary.end());

    // Display formatted output
    std::cout << "\nBalance Summary\n"
              << "---------------\n"
              << "Denom | Quantity | Value  \n"
              << "--------------------------\n";
    for (const auto& entry : balanceSummary) {
        std::cout << std::left << std::setw(5) << entry.first  // Denomination left-aligned
                  << " | " << std::left << std::setw(8) << entry.second / entry.first  // Quantity left-aligned
                  << " |$" << std::right << std::setw(7) << std::fixed << std::setprecision(2) << entry.second / 100.0 << "\n";  // Value formatted and right-aligned
    }
    std::cout << "--------------------------\n"
              << std::setw(19) << "$" << std::right << std::setw(7) << std::fixed << std::setprecision(2) << totalValueCents / 100.0 << "\n";  // Total value right-aligned
}
