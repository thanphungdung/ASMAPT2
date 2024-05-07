#include <iostream>
#include <fstream>
#include <sstream>
#include "LinkedList.h"
#include <iomanip>  // Include for output formatting


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
        foodPrice.cents = static_cast<unsigned>((price - foodPrice.dollars) * 100);

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
    }
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
