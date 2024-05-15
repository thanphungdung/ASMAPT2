#include <iostream>
#include <cstdlib>
#include <limits>
#include <fstream>
#include "LinkedList.h"
#include "Coin.h"

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
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore wrong input
            std::cout << "Invalid menu choice. Please select a number between 1 and 7: ";
        }
    }
}



int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <foodsfile> <coinsfile>" << std::endl;
    exit(EXIT_FAILURE);
  }

  LinkedList menu;
  try {
    menu.loadFromFile(argv[1]);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }

  while (true) {
    displayMainMenu();
    int choice = getMenuChoice();

    if (choice == 1) {
      menu.display();
    } else if (choice == 2) {
      std::string foodId;
      std::cout << "Enter the ID of the food you wish to purchase: ";
      std::cin >> foodId;
      menu.selectFoodToPurchase(foodId); 
    } else if (choice == 3) {
      menu.saveToFile("foods.dat");
      VendingMachine vm;
      vm.updateCoinInventory();  
      vm.saveInventoryToFile();

      return EXIT_SUCCESS;
    } else if (choice == 4) {
      menu.addFoodItem();
    } else if (choice == 5) {
      menu.removeFoodItem();
    } else if (choice == 6) {
      VendingMachine::displayBalance();
    } else if (choice == 7) {
      menu.abortProgram(); 
    } else {
      std::cout << "Invalid choice, please choose again.\n";
    }
  }

  return EXIT_SUCCESS;
}
