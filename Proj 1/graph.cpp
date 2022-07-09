/*****************************************
** File:    graph.cpp
** Project: CMSC 341 Project 1, Fall 2021
** Author:  Reagan Armstrong
** Date:    10/3/2021
** Section: 03
** E-mail:  reagana1@gl.umbc.edu
**
** Function definitions for the Graph class functions
**
**
**
**
***********************************************/
// UMBC - CSEE - CMSC 341 - Fall 2021 - Proj1
#include "graph.h"

// initializes member variables
Graph::Graph() {
    m_head = nullptr;
    m_dataFile = "";
    m_numNodes = 0;
}

// initializes member variables with given dataFile
Graph::Graph(string dataFile) : m_dataFile(dataFile) {
    m_numNodes = 0;
    m_head = nullptr;
    loadData();
}

// clears all member variables
Graph::~Graph() { clearGraph(); }

// loads all of the data from the file (given function)
void Graph::loadData() {
    int numNodes;
    int node, n, e, s, w;
    ifstream dataFile;
    dataFile.open(m_dataFile);
    if (dataFile.is_open()) {
        dataFile >> numNodes;
        m_numNodes = numNodes;
        for (int i = 0; i < numNodes; i++) {
            dataFile >> node >> n >> e >> s >> w;
            insert(node, n, e, s, w);
        }
        // closes the file
        dataFile.close();
    } else
        // the following statement provides the reason if a file doesn't open
        // please note: not all platforms are providing the same message
        cerr << "Error: " << strerror(errno) << endl;
}

// inserts node
// precondition: node is not -1
// postcondition: inserts node if it does not exist as well as its north, west,
// east, and south nodes unless if any of these nodes exist already, then it
// updates their values
void Graph::insert(int node, int n, int e, int s, int w) {
    Node *current, *north, *east, *south, *west;
    // pointer to this current node that we are working on
    current = findNode(node);
    // creates and inserts the node if it doesn't exist unless if node = -1
    if (!current && node != -1) {
        current = new Node(node);
        insertAtHead(current);
    }
    north = findNode(n);
    // if the north node does not exist and n is not -1 then insert the node
    // with no north, east, south, and west pointers
    if (!north && n != -1) {
        insert(n, -1, -1, -1, -1);
        // updates north pointer after insertion
        north = findNode(n);
    }
    // sets the updated node to north of current
    current->setNorth(north);
    east = findNode(e);
    // if the east node does not exist and e is not -1 then insert the node
    // with no north, east, south, and west pointers
    if (!east && e != -1) {
        insert(e, -1, -1, -1, -1);
        // updates east pointer after insertion
        east = findNode(e);
    }
    // sets the updated node to east of current
    current->setEast(east);
    south = findNode(s);
    // if the south node does not exist and s is not -1 then insert the node
    // with no north, east, south, and west pointers
    if (!south && s != -1) {
        insert(s, -1, -1, -1, -1);
        // updates south pointer after insertion
        south = findNode(s);
    }
    // sets the updated node to south of current
    current->setSouth(south);
    west = findNode(w);
    // if the west node does not exist and w is not -1 then insert the node
    // with no north, east, south, and west pointers
    if (!west && w != -1) {
        insert(w, -1, -1, -1, -1);
        // updates west pointer after insertion
        west = findNode(w);
    }
    // sets the updated node to west of current
    current->setWest(west);
}

// inserts the given node aNode at the fron of the linked list and sets it as
// the head
void Graph::insertAtHead(Node* aNode) {
    aNode->setNext(m_head);
    m_head = aNode;
}

// finds and returns a node given it's value if it exists, otherwise nullptr
Node* Graph::findNode(int nodeValue) {
    Node* foo = m_head;
    // iterates through the node linked list
    while (foo != nullptr) {
        // returns the node if it has the given value
        if (foo->getValue() == nodeValue) {
            return foo;
        }
        foo = foo->getNext();
    }
    // if the value was not found return nullptr
    return nullptr;
}

// finds a path from start to end and updates m_path and returns true if it
// does, otherwise returns false
bool Graph::findPath(int start, int end) {
    // clears m_path
    clearResult();
    // sets m_visited in each node to false
    clearVisited();
    Node* startNode = findNode(start);
    // if either the start or end node don't exist return false
    if (!startNode || !findNode(end)) {
        return false;
    }
    // return the result of the other findPath
    return findPath(startNode, end);
}

// finds a path from aNode to the node with value end and updates m_path and
// returns true if it does, otherwise returns false
bool Graph::findPath(Node* aNode, int end) {
    // if it was already visited return false to prevent looping
    if (aNode->getVisited()) {
        return false;
    }
    // sets it to visited and adds it to path assuming the node is in the
    // correct path
    aNode->setVisited(true);
    m_path.push(aNode->getValue());
    // if aNodes value is end then path is found so return true
    if (aNode->getValue() == end) {
        return true;
    }
    Node* foo = aNode->getEast();
    // if east exists and a path exists from it to end return true
    if (foo) {
        if (findPath(foo, end)) {
            return true;
        }
    }
    foo = aNode->getWest();
    // if west exists and a path exists from it to end return true
    if (foo) {
        if (findPath(foo, end)) {
            return true;
        }
    }
    foo = aNode->getNorth();
    // if north exists and a path exists from it to end return true
    if (foo) {
        if (findPath(foo, end)) {
            return true;
        }
    }
    foo = aNode->getSouth();
    // if south exists and a path exists from it to end return true
    if (foo) {
        if (findPath(foo, end)) {
            return true;
        }
    }
    // sets visited to false, pop from m_path, and return false because aNode
    // was not found to start a path to end
    aNode->setVisited(false);
    m_path.pop();
    return false;
}

// prints the path in m_path
void Graph::dump() {
    stack<int> tempStack;
    // adds all of the values in m_path to tempStack in reverse order
    while (!m_path.empty()) {
        tempStack.push(m_path.top());
        m_path.pop();
    }
    // prints out all of the values is formatting
    while (!tempStack.empty()) {
        m_path.push(tempStack.top());
        cout << m_path.top() << " => ";
        tempStack.pop();
    }
    cout << "END" << endl;
}

// clears m_path
void Graph::clearResult() {
    while (!m_path.empty()) {
        m_path.pop();
    }
}

// sets all nodes to unvisited
void Graph::clearVisited() {
    Node* current = m_head;
    // iterates through linked list and sets Visited to false
    while (current) {
        current->setVisited(false);
        current = current->getNext();
    }
}

// makes nodes based on the file with the given name
void Graph::buildGraph(string file) {
    // first clears the graph
    clearGraph();
    int numNodes;
    int node, n, e, s, w;
    ifstream dataFile;
    dataFile.open(file);
    // if file is a valid file name
    if (dataFile.is_open()) {
        // first number equals numNodes which m_numNodes equals
        dataFile >> numNodes;
        m_numNodes = numNodes;
        // iterates through all of the lines in data file inserts all of the
        // nodes from the information there
        for (int i = 0; i < numNodes; i++) {
            dataFile >> node >> n >> e >> s >> w;
            insert(node, n, e, s, w);
        }
        // closes the file
        dataFile.close();
    } else
        // the following statement provides the reason if a file doesn't open
        // please note: not all platforms are providing the same message
        cerr << "Error: " << strerror(errno) << endl;
}

// clears the graph, bascically the destructor but you can call it
void Graph::clearGraph() {
    // deletes all of the nodes if the linked list is not empty
    Node* next = m_head;
    // iterates through the linked list
    while (m_head != nullptr) {
        // deletes head and sets m_head to the next node
        next = m_head->getNext();
        delete m_head;
        m_head = next;
    }
    // clears the other member variables
    m_dataFile = "";
    m_numNodes = 0;
    // pops all of the values from m_path
    while (!m_path.empty()) {
        m_path.pop();
    }
}

// overloaded assignment operator
const Graph& Graph::operator=(const Graph& rhs) {
    // checks for self assignment
    if (this == &rhs) {
        return *this;
    }
    // clears this graph
    clearGraph();
    // assigns member variables
    m_numNodes = rhs.m_numNodes;
    m_dataFile = rhs.m_dataFile;
    // if the linked list for rhs is empty
    if (!rhs.m_head) {
        m_head = nullptr;
        return *this;
    }
    // if the linked list is not empty
    // iterate m_numNodes times
    for (int i = m_numNodes - 1; i >= 0; i--) {
        Node* rhsNode = rhs.m_head;
        Node* foo = nullptr;
        // finds the last node in rhs linked list that has not been copied over
        for (int j = i; j > 0; j--) {
            rhsNode = rhsNode->getNext();
        }
        int n = -1, s = -1, e = -1, w = -1;
        // gets all of the values for north, west, south, and east nodes if
        // they exist, otherwise they stay -1
        foo = rhsNode->getNorth();
        if (foo) {
            n = foo->getValue();
        }
        foo = rhsNode->getSouth();
        if (foo) {
            s = foo->getValue();
        }
        foo = rhsNode->getWest();
        if (foo) {
            w = foo->getValue();
        }
        foo = rhsNode->getEast();
        if (foo) {
            e = foo->getValue();
        }
        // insert the node with all of the obtained info
        insert(rhsNode->getValue(), n, e, s, w);
        // set the node to visited if it was visited in rhs
        m_head->setVisited(rhsNode->getVisited());
        // reset the used variables
        rhsNode = rhs.m_head;
        n = -1;
        s = -1;
        e = -1;
        w = -1;
        foo = nullptr;
    }
    // since the stack is a int stack you can do this and it is still a deep
    // copy
    m_path = rhs.m_path;
    return *this;
}

// given function
bool Graph::empty() const  // is the list empty?
{
    return m_head == nullptr;
}