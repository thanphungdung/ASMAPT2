#ifndef COIN_H
#define COIN_H

#include <map>

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

// Coin class for managing currency in the system
class Coin {
public:
    Denomination denom;
    unsigned int count;

    // Constructor
    Coin(Denomination d, unsigned int c = 0);

    // Static methods for handling transactions and inventory
    static void initializeDenominationValues();
    static void addMonetaryInput(Denomination denom, unsigned count = 1);
    static int calculateTotalInput();
    static void handlePurchase(float foodCost);
    static void calculateChange(int change);

    // Inventory management
    static void saveCoinInventory();
    static void loadCoinInventory();

    // Maps to hold denomination values and current inventory
    static std::map<Denomination, int> denominationValue;
    static std::map<Denomination, int> denominationInventory;
};

#endif // COIN_H
