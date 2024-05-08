#include <iostream>
#include "LinkedList.h"
#include <cstdlib>
#include <fstream>

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
  std::cin >> choice;
  return choice;
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
      // Handle purchase meal option
      std::cout << "Purchasing meal...\n";
    } else if (choice == 3) {
      // Handle save and exit option
      std::cout << "Saving changes and exiting...\n";
      return EXIT_SUCCESS;
    } else if (choice == 4) {
      // Handle add food option
      std::cout << "Adding food...\n";
    } else if (choice == 5) {
      std::cout << "Enter the food id of the food to remove: ";
      std::string idToRemove;
      std::cin >> idToRemove;
      menu.removeFoodItem(idToRemove);
    } else if (choice == 6) {
      // Handle display balance option
      std::cout << "Displaying balance...\n";
    } else if (choice == 7) {
      std::cout << "Program aborted by user.\n";
      return EXIT_SUCCESS;
    } else {
      std::cerr << "Invalid menu choice. Please select a valid option (1-7).\n";
    }
  }

  return EXIT_SUCCESS;
}
