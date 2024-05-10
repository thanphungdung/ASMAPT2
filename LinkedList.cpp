#include <iostream>
#include <fstream>
#include <sstream>
#include "LinkedList.h"
#include <iomanip>  // Include for output formatting
#include <limits>  // Required for std::numeric_limits




LinkedList::~LinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
}

void LinkedList::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {  // Check if file opening failed
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    Node* last = nullptr; // Pointer to keep track of the last node
    int currentMaxId = 0;  // To keep track of the highest ID number

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string id, name, description, priceStr;
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, description, '|');
        getline(ss, priceStr);
        double price = std::stod(priceStr);  // Convert string to double

        Price foodPrice;
        foodPrice.dollars = static_cast<unsigned>(price);
        foodPrice.cents = static_cast<unsigned>((price - foodPrice.dollars) * 100 + 0.5);  // Add 0.5 for rounding

        FoodItem* item = new FoodItem(id, name, description, foodPrice, DEFAULT_FOOD_STOCK_LEVEL);
        Node* newNode = new Node();
        newNode->data = item;
        newNode->next = nullptr;  // New node will be the last node, so next is nullptr

        if (last == nullptr) {  // This means the list is empty
            head = newNode;  // The new node is now the head of the list
            last = newNode;  // And also the last node
        } else {
            last->next = newNode;  // Append new node at the end of the list
            last = newNode;  // Update the last node to be the new node
        }

        // Extract numeric part of the ID and update currentMaxId if necessary
        int idNum = std::stoi(id.substr(1));  // Convert ID to integer, stripping the 'F'
        if (idNum > currentMaxId) {
            currentMaxId = idNum;
        }
    }

    lastId = currentMaxId; // Update lastId with the highest ID found
    file.close();
}

void LinkedList::display() const {
    if (head == nullptr) {
        std::cout << "The menu is currently empty." << std::endl;
        return;
    }

    // Print the header
    std::cout << "Food Menu" << std::endl;
    std::cout << "--------" << std::endl;
    std::cout << std::left << std::setw(6) << "ID" << "|"
              << std::setw(15) << "Name" << "|"
              << "Price" << std::endl;
    std::cout << "-----------------------------------------------------------------" << std::endl;

    Node* current = head;
    while (current != nullptr) {
        std::cout << std::left << std::setw(6) << current->data->id << "|"
                  << std::setw(15) << current->data->name << "|$"
                  << std::right << std::setw(6) << current->data->price.dollars << "."
                  << std::setw(2) << std::setfill('0') << current->data->price.cents
                  << std::setfill(' ') << std::endl;
        current = current->next;
    }
}

void LinkedList::removeFoodItem(const std::string& id) {
    if (head != nullptr) {
        Node* current = head;
        Node* previous = nullptr;

        while (current != nullptr && current->data->id != id) {
            previous = current;
            current = current->next;
        }

        if (current == nullptr) { // Food item not found
            std::cerr << "Food item with ID " << id << " not found." << std::endl;
            return;
        }

        if (previous == nullptr) {
            head = current->next;
        } else {
            previous->next = current->next;
        }

        std::cout << id << " – " << current->data->name << " - " << current->data->description 
                  << " has been removed from the system." << std::endl;

        delete current->data;  // Delete the FoodItem object first
        // delete current;  // Then delete the node itself

        saveToFile("foods.dat");
    } else {
        std::cerr << "The menu is currently empty." << std::endl;
    }
}

void LinkedList::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Unable to open file for writing: " << filename << std::endl;
        return;
    }

    Node* current = head;
    while (current != nullptr) {
        file << current->data->id << '|' 
             << current->data->name << '|' 
             << current->data->description << '|' 
             << std::to_string(current->data->price.dollars) << "." 
             << std::setw(2) << std::setfill('0') << current->data->price.cents
             << std::setfill(' ') << std::endl;
        current = current->next;
    }
    file.close();
}
void LinkedList::addFoodItem() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

    std::string name, description, priceStr;
    std::cout << "Enter the item name: ";
    std::getline(std::cin, name);  
    std::cout << "Enter the item description: ";
    std::getline(std::cin, description);  
    std::cout << "Enter the price for this item (in dollars and cents, e.g., 8.50): ";
    std::getline(std::cin, priceStr); 
    double price = std::stod(priceStr);  

    lastId++;  
    std::stringstream ss;
    ss << 'F' << std::setw(4) << std::setfill('0') << lastId;  
    std::string id = ss.str();  

    Price foodPrice;  
    foodPrice.dollars = static_cast<unsigned>(price);
    foodPrice.cents = static_cast<unsigned>((price - static_cast<double>(foodPrice.dollars)) * 100 + 0.5);  

    FoodItem* newItem = new FoodItem{id, name, description, foodPrice, DEFAULT_FOOD_STOCK_LEVEL};
    Node* newNode = new Node();  
    newNode->data = newItem;  
    newNode->next = nullptr;  

    if (head == nullptr) {
        head = newNode;  
    } else {
        Node* last = head;
        while (last->next != nullptr) {  
            last = last->next;
        }
        last->next = newNode;  
    }
    count++;  

    std::cout << "This item \"" << name << " – " << description << "\" has now been added to the food menu.\n";
    saveToFile("foods.dat");
}


void LinkedList::searchFoodItem(const std::string& id) {
    Node* current = head;
    while (current != nullptr && current->data->id != id) {
        current = current->next;
    }

    if (current == nullptr) { // Food item not found
        std::cerr << "Food item with ID " << id << " not found." << std::endl;
        return;
    }

    // Displaying the selected food item
    std::cout << "You have selected \"" << current->data->name << " - " << current->data->description << "\". This will cost you $"
              << current->data->price.dollars << "." << std::setw(2) << std::setfill('0') << current->data->price.cents << std::setfill(' ') << std::endl;
    
    // Calculate the total cost in dollars
    float price = current->data->price.dollars + current->data->price.cents / 100.0f;
    
    std::cout << "Please hand over the money - type in the value of each note/coin in cents." << std::endl;

    // Call Coin's handlePurchase to process the payment
    Coin::handlePurchase(price);
}