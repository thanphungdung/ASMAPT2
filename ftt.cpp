#include <iostream>
#include "LinkedList.h"

/**
 * manages the running of the program, initialises data structures, loads
 * data, display the main menu, and handles the processing of options. 
 * Make sure free memory and close all files before exiting the program.
 **/
int main(int argc, char **argv)
{
    LinkedList menu;
    menu.loadFromFile("foods.dat");  // Load the food menu from file

    int choice = 0;
    std::cout << "Press 1 to display the menu, 0 to exit: ";
    while (std::cin >> choice && choice != 0) {
        if (choice == 1) {
            menu.display();
        }
        std::cout << "Press 1 to display the menu, 0 to exit: ";
    }

    return 0;
}
