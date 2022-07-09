/*****************************************
** File:    mqueue.cpp
** Project: CMSC 341 Project 3, Fall 2021
** Author:  Reagan Armstrong
** Date:    11/17/2021
** Section: 03
** E-mail:  reagana1@gl.umbc.edu
**
** Function definitions for the mqueue class functions and overloaded insertion
** operators for Nodes and Orders
**
**
**
**
***********************************************/
// CMSC 341 - Fall 2021 - Project 3 - MQueue Class

#include "mqueue.h"
using namespace std;
using std::domain_error;

// constructor for MQueue
MQueue::MQueue(prifn_t priFn) {
    // initializes m_head to nullptr
    m_heap = nullptr;
    // m_priorFunc is initialized to priFn
    m_priorFunc = priFn;
    // initializes m_size to 0
    m_size = 0;
}

// destructor for MQUeue calls clear() which deletes all of the Nodes
MQueue::~MQueue() { clear(); }

// Copy Constructor makes a deep copy of rhs to this object
MQueue::MQueue(const MQueue& rhs) {
    // recursively copies the heap rooted at rhs.m_heap into m_heap
    m_heap = recursiveCopyNodes(rhs.m_heap);
    // copies over the size
    m_size = rhs.m_size;
    // copies over the function pointer
    m_priorFunc = rhs.m_priorFunc;
}
// recursive function that returns a copy of the heap starting from Node*
// rhsStart (preorder traversal)
Node* MQueue::recursiveCopyNodes(Node* rhsStart) {
    // if rhsStart is nullptr then return nullptr
    if (!rhsStart) {
        return rhsStart;
    }
    // create a copy of rhsStart
    Node* top = new Node(rhsStart->getOrder());
    // call recursiveCopyNodes on rhsStart's left and right nodes and set them
    // to top's left and right nodes respectively
    top->m_left = recursiveCopyNodes(rhsStart->m_left);
    top->m_right = recursiveCopyNodes(rhsStart->m_right);
    return top;
}

// overloaded assignment operator for MQueue creates a deep copy of rhs
MQueue& MQueue::operator=(const MQueue& rhs) {
    // protects against self assignment
    if (this != &rhs) {
        // recursively copies the heap rooted at rhs.m_heap into m_heap
        m_heap = recursiveCopyNodes(rhs.m_heap);
        // copies over the size
        m_size = rhs.m_size;
        // copies over the function pointer
        m_priorFunc = rhs.m_priorFunc;
    }
    return *this;
}

// inserts a node with a given Order input into m_heap
void MQueue::insertOrder(const Order& input) {
    // makes a new MQueue with the same m_priorFunc
    MQueue tempQueue = MQueue(m_priorFunc);
    // inserts a new Node with Order input to it's heap and increments the size
    tempQueue.m_heap = new Node(input);
    tempQueue.m_size++;
    // merges with this queue
    mergeWithQueue(tempQueue);
}

// gets the highest priority order and deletes's its Node form the heap
Order MQueue::getNextOrder() {
    // accounts for error case of empty heap
    if (!m_heap) {
        throw domain_error("empty heap on getNextOrder");
    }
    // stores left and right sub heaps of m_heap and the order of m_heap
    Node* left = m_heap->m_left;
    Node* right = m_heap->m_right;
    Order highest = m_heap->getOrder();
    // deletes the top Node (highest priority)
    delete m_heap;
    // decrements the m_size
    m_size--;
    // creates a new Mqueue with the right subheap as it's m_heap
    // temp.m_size will be 0 and will stay 0 and the size of m_size will be 1
    // minus the original so when the two sizes get added later on the number
    // of total nodes in the heap will stay accurate
    MQueue temp = MQueue(m_priorFunc);
    temp.m_heap = right;
    // sets left subheap as m_heap
    m_heap = left;
    // merges the two heaps
    mergeWithQueue(temp);
    return highest;
}

// merges this MQueue with rhs, rhs.m_heap will be empty at the end
void MQueue::mergeWithQueue(MQueue& rhs) {
    // accounts for error case where the two MQueues have different m_priorFunc
    if (m_priorFunc != rhs.m_priorFunc) {
        throw domain_error("non equal functions for mergeWithQUeue");
    }
    // protects against self merging
    if (this != &rhs) {
        // adds the sizes together
        m_size += rhs.m_size;
        // merges the heaps
        m_heap = recursiveMerge(rhs);
        // sets rhs.m_heap to nullptr so its no longer pointing at a Node in
        // m_heap
        rhs.m_heap = nullptr;
        // updates rhs.m_size
        rhs.m_size = 0;
    }
}

// recursively merges this MQueue with rhs
Node* MQueue::recursiveMerge(MQueue& rhs) {
    // if the heap of one MQueue is empty return the other
    if (!rhs.m_heap) {
        return m_heap;
    } else if (!m_heap) {
        return rhs.m_heap;
    }
    Node* the_rest;
    // if the root Node of this heap is smaller than that of rhs
    if (m_priorFunc(m_heap->getOrder()) <
        m_priorFunc(rhs.m_heap->getOrder())) {
        // set the_rest to m_ heap and set m_heap to it's right sub heap
        the_rest = m_heap;
        m_heap = m_heap->m_right;
        // if the root Node of this heap is larger than that of rhs
    } else {
        // set the_rest to rhs.m_ heap and set rhs.m_heap to it's right sub
        // heap
        the_rest = rhs.m_heap;
        rhs.m_heap = rhs.m_heap->m_right;
    }
    // call recursiveMerge with rhs and set it to the_rest->m_right
    the_rest->m_right = recursiveMerge(rhs);
    // swap the_rest->m_right and the_rest->m_left
    Node* temp = the_rest->m_right;
    the_rest->m_right = the_rest->m_left;
    the_rest->m_left = temp;
    return the_rest;
}

// clears this MQueue
void MQueue::clear() {
    // sets m_size to 0
    m_size = 0;
    // deletes all the nodes in m_heap
    recursiveClear(m_heap);
    // sets m_heap to nullptr so we know its empty in the future
    m_heap = nullptr;
}

// recursively clears the heap rooted at Node* start
void MQueue::recursiveClear(Node* start) {
    // makes sure start is not nulltpr
    if (start) {
        // calls recursiveClear on start's left sub heap, then the right sub
        // heap then deletes start
        recursiveClear(start->m_left);
        recursiveClear(start->m_right);
        delete start;
    }
}

// gets m_size
int MQueue::numOrders() const { return m_size; }

// prints out the order of the Queue using pre order traversal
void MQueue::printOrderQueue() const { preOrderPrint(m_heap); }

// traverses m_heap using preorder traversal and prints the Nodes in the heap
void MQueue::preOrderPrint(Node* start) const {
    // makes sure start exists
    if (start) {
        // prints the start Node prepended by it's priority
        cout << "[" << m_priorFunc(start->getOrder()) << "] " << *start
             << endl;
        // calls preOrderPrint on the left and right sub Nodes
        preOrderPrint(start->m_left);
        preOrderPrint(start->m_right);
    }
}

// returns m_priorFunc
prifn_t MQueue::getPriorityFn() const { return m_priorFunc; }

// sets m_priorFunc and rebuilds the heap
void MQueue::setPriorityFn(prifn_t priFn) {
    // makes a copy of MQueue, will use this to store m_heap
    MQueue copy = MQueue(*this);
    // sets the new priority function
    m_priorFunc = priFn;
    // clears this MQueue (deletes the heap (m_heap ends up ass nullptr) and
    // sets the m_size=0)
    clear();
    // while copy has nodes
    while (copy.numOrders()) {
        // insert the node with order returned by copy.getNextOrder() (this
        // Node with this order is deleted in copy.m_heap)
        insertOrder(copy.getNextOrder());
    }
}

// overloaded insertion operator for Order class (given code)
ostream& operator<<(ostream& sout, const Order& order) {
    sout << "Order: " << order.getCustomer()
         << ", PM priority: " << order.getPMPriority()
         << ", quantity: " << order.getQuantityValue()
         << ", material availability: " << order.getAvailabilityProbablity()
         << "%"
         << ", arrival time: " << order.getArrivalTime();
    return sout;
}

// overloaded insertion operator for Node class (given code)
ostream& operator<<(ostream& sout, const Node& node) {
    sout << node.getOrder();
    return sout;
}

// for debugging
void MQueue::dump() const {
    if (m_size == 0) {
        cout << "Empty skew heap.\n";
    } else {
        dump(m_heap);
        cout << endl;
    }
}

// for debugging
void MQueue::dump(Node* pos) const {
    if (pos != nullptr) {
        cout << "(";
        dump(pos->m_left);
        cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.getCustomer();
        dump(pos->m_right);
        cout << ")";
    }
}
