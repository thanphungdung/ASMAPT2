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
      // Handle purchase meal option
      std::cout << "Purchasing meal...\n";
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
      // Handle display balance option
      std::cout << "Displaying balance...\n";
    } else if (choice == 7) {
      std::cout << "Program aborted by user.\n";
      return EXIT_SUCCESS;
    }
  }

  return EXIT_SUCCESS;
}
