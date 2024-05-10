#ifndef COIN_H
#define COIN_H

#include <vector>
#include <map>

// Enumeration for coin denominations
enum Denomination {
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS,
    ONE_DOLLAR, TWO_DOLLARS, FIVE_DOLLARS,
    TEN_DOLLARS, TWENTY_DOLLARS, FIFTY_DOLLARS
};

// Coin class for managing currency in the system
class Coin {
public:
    Denomination denom;
    unsigned int count;

    Coin(Denomination d, unsigned int c = 0);

    // Transaction-related functions
    static void addMonetaryInput(Denomination denom, unsigned count = 1);
    static int calculateTotalInput();
    static void handlePurchase(float foodCost);
    static void calculateChange(int change);
    // Helper to initialize denomination values
    static void initializeDenominationValues();
    static std::map<Denomination, int> denominationValue;
};

#endif // COIN_H
