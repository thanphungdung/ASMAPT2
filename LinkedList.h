#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Node.h"
#include <string>


class LinkedList
{
public:
    LinkedList() : head(nullptr),lastId(0) {}
    ~LinkedList();
    void loadFromFile(const std::string& filename);
    void display() const;
    void removeFoodItem(const std::string& id);
    void saveToFile(const std::string& filename);
    void addFoodItem();

    // more functions to be added perhaps...

private:
    // the beginning of the list
    Node* head;
    int lastId;
  
    // how many nodes are there in the list?
    unsigned count;
};

#endif  // LINKEDLIST_H


