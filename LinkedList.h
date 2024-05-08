#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Node.h"


class LinkedList
{
public:
    LinkedList() : head(nullptr) {}
    ~LinkedList();
    void loadFromFile(const std::string& filename);
    void display() const;
    void removeFoodItem(const std::string& id);
    void saveToFile(const std::string& filename);
    // more functions to be added perhaps...

private:
    // the beginning of the list
    Node* head;
  
    // how many nodes are there in the list?
    unsigned count;
};

#endif  // LINKEDLIST_H


