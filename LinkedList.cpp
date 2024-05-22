#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include "LinkedList.h"
#include <math.h>

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
    bool fileIsEmpty = true;  // Flag to check if file is empty
    lastId = 0;  // Make sure to reset lastId when loading new file

    while (getline(file, line)) {
        fileIsEmpty = false;

        std::stringstream ss(line);
        std::string id, name, description, priceStr;
        if (getline(ss, id, '|') && getline(ss, name, '|') && getline(ss, description, '|') && getline(ss, priceStr)) {
            // Validate ID
            if (id.length() != IDLEN || id[0] != 'F' || !std::all_of(id.begin() + 1, id.end(), ::isdigit)) {
                throw std::runtime_error("Error: Invalid ID format - " + id);
            }

            // Validate name
            if (!std::all_of(name.begin(), name.end(), [](char c) { return std::isalpha(c) || std::isspace(c); })) {
                throw std::runtime_error("Error: Invalid name format - " + name);
            }
            if (name.length() > NAMELEN) {
                throw std::runtime_error("Error: Name exceeds maximum length - " + name);
            }

            // Validate description
            if (description.length() > DESCLEN) {
                throw std::runtime_error("Error: Description exceeds maximum length - " + description);
            }

            // Validate and parse price
            double price = 0.0;
            try {
                size_t dotPos = priceStr.find('.');
                if (dotPos == std::string::npos) {
                    throw std::invalid_argument("Invalid price format");
                }
                price = std::stod(priceStr);
            } catch (const std::exception& e) {
                throw std::runtime_error("Error: Invalid food data - " + std::string(e.what()));
            }

            Price foodPrice;
            foodPrice.dollars = static_cast<unsigned>(price);
            foodPrice.cents = static_cast<unsigned>((price - foodPrice.dollars) * 100 + 0.5);

            FoodItem* item = new FoodItem(id, name, description, foodPrice, DEFAULT_FOOD_STOCK_LEVEL);
            Node* newNode = new Node();
            newNode->data = item;
            newNode->next = nullptr;

            // Insert node in sorted order
            if (head == nullptr || head->data->name > item->name) {
                newNode->next = head;
                head = newNode;
            } else {
                Node* current = head;
                while (current->next != nullptr && current->next->data->name < item->name) {
                    current = current->next;
                }
                newNode->next = current->next;
                current->next = newNode;
            }

            int idNum = std::stoi(id.substr(1));
            if (idNum > lastId) {
                lastId = idNum;
            }
        } else {
            throw std::runtime_error("Error: Invalid food file format.");
        }
    }    

    if (fileIsEmpty) {
        throw std::runtime_error("Error: The file is empty.");
    }

    file.close();
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
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string id;
    while (true) {
        std::cout << "Enter the food id of the food to remove: ";
        
        std::getline(std::cin, id); // Use getline to capture the entire line input

        // Trim leading and trailing whitespaces
        id.erase(0, id.find_first_not_of(" \t\n\r\f\v")); // Remove leading spaces (including tabs and new lines)
        id.erase(id.find_last_not_of(" \t\n\r\f\v") + 1); // Remove trailing spaces

        if (id.empty()) {
            std::cerr << "Invalid empty input. Please enter a valid food id." << std::endl;
            continue; // Prompt again if the input is empty
        }

        Node* nodeToRemove = searchFoodItem(id);
        if (nodeToRemove == nullptr) {
            std::cerr << "Food item with ID '" << id << "' not found. Please try again." << std::endl;
            continue; // Prompt again if no valid ID is found
        }

        // Node removal logic
        Node* current = head;
        Node* previous = nullptr;
        while (current != nodeToRemove) {
            previous = current;
            current = current->next;
        }

        if (previous == nullptr) { // The node to remove is the head
            head = current->next;
        } else { // The node to remove is not the head
            previous->next = current->next;
        }

        std::cout << "Food item " << id << " – " << nodeToRemove->data->name << " - " << nodeToRemove->data->description 
                  << " has been removed from the system." << std::endl;

        delete nodeToRemove->data;
        break; // Exit the loop once the item is removed
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
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer once at the start

    std::string name, description, priceStr;

    while (true) {
        std::cout << "Enter the item name: ";
        if (!std::getline(std::cin, name) || name.empty()) {
            std::cerr << "Invalid empty input. Please enter a valid name." << std::endl;
            continue;
        }
        if (!std::all_of(name.begin(), name.end(), [](char c) { return std::isalpha(c) || std::isspace(c); })) {
            std::cerr << "Invalid input: Name must contain only letters and spaces." << std::endl;
            continue;
        }
        if (name.length() > NAMELEN) {
            std::cerr << "Error: Name cannot exceed 40 characters." << std::endl;
            continue;
        }
        break;
    }

    while (true) {
        std::cout << "Enter the item description: ";
        if (!std::getline(std::cin, description) || description.empty()) {
            std::cerr << "Invalid empty input. Please enter a valid description." << std::endl;
            continue;
        }
        if (description.length() > DESCLEN) {
            std::cerr << "Error: Description cannot exceed 255 characters." << std::endl;
            continue;
        }
        break;
    }

    double price;  
    while (true) {
            std::cout << "Enter the price for this item (in dollars and cents, e.g., 8.50): ";
            if (!std::getline(std::cin, priceStr) || priceStr.empty()) {
                std::cerr << "Invalid empty input. Please enter a valid price." << std::endl;
                continue;
            }
            try {
                price = std::stod(priceStr);
                if (price < 0) {
                    std::cerr << "Invalid minus number. Price must be non-negative." << std::endl;
                    continue;
        }
                int cents = static_cast<int>(std::round(price * 100));
                if (cents % 5 != 0) {
                    std::cerr << "Price must be a multiple of 0.05." << std::endl;
                    continue;
                }
                break;
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid price entered. Please enter a valid price." << std::endl;
                continue;
            }
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

void LinkedList::selectFoodToPurchase(std::string& id) {
    Node* selectedFood = nullptr;
    
    while (true) {
        selectedFood = searchFoodItem(id);  // Search for the food item using the provided ID
        if (selectedFood) {
            // If a valid food item is found, proceed with displaying the information
            std::cout << "You have selected \"" << selectedFood->data->name << " - " << selectedFood->data->description << "\". This will cost you $"
                      << selectedFood->data->price.dollars << "." << std::setw(2) << std::setfill('0') << selectedFood->data->price.cents << std::setfill(' ') << std::endl;
            break;  // Exit the loop after successful handling
        } else {
            // If not found, inform the user and continue to prompt for new ID
            std::cerr << "Invalid food ID. Please try again." << std::endl;
        }

        std::cout << "Enter the ID of the food you wish to purchase (or type 'exit' to return): ";
        std::getline(std::cin, id);  // Read new input from the user
        if (id == "exit") {
            return;  // Allow user to exit the purchase process
        }
    }
    
    float price = selectedFood->data->price.dollars + selectedFood->data->price.cents / 100.0f;
    std::cout << "Please hand over the money - type in the value of each note/coin in cents." << std::endl;
    VendingMachine::handlePurchase(price);
}
void LinkedList::abortProgram() {
    std::cout << "Program is being aborted. All unsaved changes will be lost.\n";
    this->~LinkedList();
    exit(EXIT_FAILURE);
}
