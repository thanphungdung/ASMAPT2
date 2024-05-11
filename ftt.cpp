#include <iostream>
#include "LinkedList.h"
#include "Coin.h"
#include <cstdlib>
#include <fstream>
#include <vector>



// Function to display the main menu
void displayMainMenu() {
  std::cout << "\nMain Menu:\n";
  std::cout << "1. Display Meal Options\n";
  std::cout << "2. Purchase Meal\n";
  std::cout << "3. Save and Exit\n";
  std::cout << "Administrator-Only Menu:\n";
  std::cout << "4. Add Food\n";
  std::cout << "5. Remove Food\n";
  std::cout << "6. Display Balance\n";
  std::cout << "7. Abort Program\n";
  std::cout << "Select your option (1-7): ";
}

// Function to get user input for menu selection
int getMenuChoice() {
    int choice;
    while (true) { // Infinite loop to keep asking until a valid input is given
        if (std::cin >> choice) {
            if (choice >= 1 && choice <= 7) {
                return choice; // Valid choice, break the loop
            } else {
                std::cout << "Invalid menu choice. Please select a number between 1 and 7: ";
            }
        } else {
            // Handle non-numeric input
            std::cin.clear(); // Clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore wrong input until a newline is encountered
            std::cout << "Invalid input. Please enter a number: ";
        }
    }
}

/**
 * Manages the running of the program, initializes data structures, loads
 * data, displays the main menu, and handles the processing of options. 
 * Make sure to free memory and close all files before exiting the program.
 **/
int main(int argc, char **argv) {
  LinkedList menu;
  menu.loadFromFile("foods.dat");  // Load the food menu from file

  while (true) {
    displayMainMenu();
    int choice = getMenuChoice();

    if (choice == 1) {
      menu.display();
    } else if (choice == 2) {
      // std::cout << "Enter the Food Item ID to purchase: ";     
      // std::string id;
      // std::cin>> id;
      // menu.searchFoodItem(id);
      Coin::initializeDenominationValues(); // Initialize coin values
      std::string foodId;
      std::cout << "Enter the ID of the food you wish to purchase: ";
      std::cin >> foodId;

      menu.selectFoodToPurchase(foodId); // Search and initiate purchase process

    } else if (choice == 3) {
      // Handle save and exit option
      std::cout << "Saving changes and exiting...\n";
      return EXIT_SUCCESS;
    } else if (choice == 4) {
      // Handle add food option
      menu.addFoodItem();
    } else if (choice == 5) {
      menu.removeFoodItem();
    } else if (choice == 6) {
      Coin coin; // Create an instance of the Coin class
      coin.displayBalance(); // Call the displayBalance() function on the instance
    } else if (choice == 7) {
      menu.abortProgram();
    }
  }

  return EXIT_SUCCESS;
}
