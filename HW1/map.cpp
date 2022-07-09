#include <iostream>
#include <string>
class LinkedList;
class ListNode {
    int key;
    std::string value;
    ListNode *prev, *next;

   public:
    ListNode(int key, std::string value, ListNode *const next) {
        this->key = key;
        this->value = value;
        this->next = next;
    };
    ListNode(){};
    friend class LinkedList;
};
class LinkedList {
    ListNode *head;

   public:
    LinkedList() {
        head = new ListNode();
        head->next = NULL;
    };
    void add(int key, std::string value) {
        ListNode *n = new ListNode(key, value, head->next);
        head->next = n;
    }
    std::string find(int key) {
        ListNode *temp = head;
        while (temp != nullptr) {
            if (temp->key == key) {
                return temp->value;
            }
            temp = temp->next;
        }
        return "";
    }
};