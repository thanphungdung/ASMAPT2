#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include "LinkedList.h"

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
    if (!file) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    Node* last = nullptr;
    lastId = 0;  // Make sure to reset lastId when loading new file

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string id, name, description, priceStr;
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, description, '|');
        getline(ss, priceStr);

        double price = 0.0;
        try {
            price = std::stod(priceStr);
        } catch (const std::invalid_argument& e) {
            continue;  // Skip this item and continue with the next if price conversion fails
        }

        Price foodPrice;
        foodPrice.dollars = static_cast<unsigned>(price);
        foodPrice.cents = static_cast<unsigned>((price - foodPrice.dollars) * 100 + 0.5);

        FoodItem* item = new FoodItem(id, name, description, foodPrice, DEFAULT_FOOD_STOCK_LEVEL);
        Node* newNode = new Node();
        newNode->data = item;
        newNode->next = nullptr;

        if (last == nullptr) {
            head = newNode;
            last = newNode;
        } else {
            last->next = newNode;
            last = newNode;
        }

        int idNum = std::stoi(id.substr(1));
        if (idNum > lastId) {
            lastId = idNum;
        }
    }    file.close();
}

void LinkedList::display() const {
    if (head == nullptr) {
        std::cout << "The menu is currently empty." << std::endl;
        return;
    }

    std::cout << "\nFood Menu" << std::endl;
    std::cout << "--------" << std::endl;
    std::cout << std::left << std::setw(5) << "ID" << "|"
              << std::setw(48) << "Name" << "|"
              << "Length" << std::endl;
    std::cout << "-----------------------------------------------------------------" << std::endl;

    Node* current = head;
    while (current != nullptr) {
        std::cout << std::left << std::setw(5) << current->data->id << "|"
                  << std::setw(48) << current->data->name << "|$"
                  << std::right << std::setw(2) << current->data->price.dollars << "."
                  << std::setw(2) << std::setfill('0') << current->data->price.cents
                  << std::setfill(' ') << std::endl;
        current = current->next;
    }
}

void LinkedList::removeFoodItem() {
    if (head == nullptr) {
        std::cerr << "The menu is currently empty." << std::endl;
    }

    std::string id;
    std::cout << "Enter the food id of the food to remove: ";
    std::cin >> id;
    Node* nodeToRemove = searchFoodItem(id);
    Node* current = head;
    Node* previous = nullptr;

    while (current != nodeToRemove) {
        previous = current;
        current = current->next;
    }

    if (previous == nullptr) {
        head = nodeToRemove->next;
    } else {
        previous->next = nodeToRemove->next;
    }

    std::cout << id << " – " << nodeToRemove->data->name << " - " << nodeToRemove->data->description 
              << " has been removed from the system." << std::endl;

    delete nodeToRemove->data;
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
    double price;  
    try {
        price = std::stod(priceStr);
    } catch (const std::invalid_argument& e) {
        return;
    }

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
}

Node* LinkedList::searchFoodItem(const std::string& id) {
    Node* current = head;
    while (current != nullptr) {
        if (current->data->id == id) {
            return current;
        }
        current = current->next;
    }
    std::cerr << "Food item with ID " << id << " not found." << std::endl;
    return nullptr;
}

void LinkedList::selectFoodToPurchase(const std::string& id) {
    Node* selectedFood = searchFoodItem(id);
    std::cout << "You have selected \"" << selectedFood->data->name << " - " << selectedFood->data->description << "\". This will cost you $"
              << selectedFood->data->price.dollars << "." << std::setw(2) << std::setfill('0') << selectedFood->data->price.cents << std::setfill(' ') << std::endl;
    
    float price = selectedFood->data->price.dollars + selectedFood->data->price.cents / 100.0f;
    
    std::cout << "Please hand over the money - type in the value of each note/coin in cents." << std::endl;

    Coin::handlePurchase(price);
}

void LinkedList::abortProgram() {
    std::cout << "Program is being aborted. All unsaved changes will be lost.\n";
    this->~LinkedList();
    exit(EXIT_FAILURE);
}
