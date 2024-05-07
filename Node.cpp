#include "Node.h" // Include the header file for the Node class.

// Constructor: Initializes the data and next pointers to null.
Node::Node() : data(nullptr), next(nullptr) {}

// Destructor: Safely deletes the FoodItem object to prevent memory leaks.
Node::~Node() {
    delete data; // This ensures that the FoodItem resources are freed when a Node is destroyed.
}
