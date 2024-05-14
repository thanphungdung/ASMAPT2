#ifndef COIN_H
#define COIN_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

// Enumeration for coin denominations
enum Denomination {
    FIVE_CENTS = 5, 
    TEN_CENTS = 10, 
    TWENTY_CENTS = 20, 
    FIFTY_CENTS = 50,
    ONE_DOLLAR = 100, 
    TWO_DOLLARS = 200, 
    FIVE_DOLLARS = 500,
    TEN_DOLLARS = 1000, 
    TWENTY_DOLLARS = 2000, 
    FIFTY_DOLLARS = 5000
};

class Coin {
public:
    Denomination denom;
    unsigned int count;
    Coin() = default;
    Coin(Denomination d, unsigned int c = 1) : denom(d), count(c) {}
};

class Transaction {
public:
    std::vector<Coin> monetaryInputs;
    std::vector<Coin> changeGiven;
    Transaction* next;

    Transaction() : next(nullptr) {}
};

class VendingMachine {
public:
    static std::map<Denomination, int> denominationValue;
    static std::map<Denomination, int> coinInventory; // Maps denomination value to quantity

    static Transaction* head;
    // static void displayBalance();

    static void initializeDenominationValues();
    static void handlePurchase(float foodCost);
    static void addMonetaryInput(Transaction* transaction, Denomination denom, unsigned count);
    static int calculateTotalInput(Transaction* transaction);
    static void calculateChange(Transaction* transaction, int change);
    static void clearTransactions();
    static void updateCoinInventory(); // Save the current inventory to a file
    static void displayBalance();
    static void saveInventoryToFile();
    // static void updateInventory(); // Update inventory based on transactions

};

#endif // COIN_H
