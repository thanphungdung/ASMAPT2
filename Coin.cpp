#include "Coin.h"
#include <iostream>
#include <vector>
#include <algorithm>

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
    std::vector<std::pair<int, Denomination>> sortedDenoms;

    for (const auto& pair : denominationValue) {
        sortedDenoms.push_back({pair.second, pair.first});
    }
    std::sort(sortedDenoms.begin(), sortedDenoms.end(), std::greater<>());

    std::cout << "Your change is: ";
    for (const auto& denom : sortedDenoms) {
        int count = change / denom.first;
        if (count > 0) {
            if (denom.first >= 100) {
                std::cout << count << " x $" << denom.first / 100 << " ";
            } else {
                std::cout << count << " x " << denom.first << "c ";
            }
            change -= count * denom.first;
        }
        if (change == 0) break;
    }
    std::cout << std::endl;
}
