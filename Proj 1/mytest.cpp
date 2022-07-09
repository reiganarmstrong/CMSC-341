/*****************************************
** File:   m_graph.cpp
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

class Tester {  // Tester class to implement test functions
   public:
    // initialize variables and calls all of the functions
    Tester();
    // tests normal case of an alternative constructor
    void testNormalGraph();
    // tests filling an empty graph
    void fillEmptyGraph();
    // tests a non existant path
    void testNonExistantPath();
    // tests path to self
    void testPathToSelf();
    // tests a path with a start not in graph
    void testNonExistantStart();
    // tests path with a start not in graph
    void testNonExistantEnd();
    // tests a existant path
    void testExistantPath();
    // tests assignment operator
    void testAssignmentOperator();

   private:
    // checks if nodes are equal
    bool equalNodes(Node*, Node*);
    // checks if graphs are equal
    bool compareGraphs(Graph&, Graph&);
    // name of the file
    const string m_fileName;
    Graph m_graph;
};
// initializes m_filName
Tester::Tester() : m_fileName("testdata.txt") {
    // runs tests
    testNormalGraph();
    fillEmptyGraph();
    testNonExistantPath();
    testPathToSelf();
    testNonExistantStart();
    testNonExistantEnd();
    testExistantPath();
    testAssignmentOperator();
}

// tests non existant path
void Tester::testNonExistantPath() {
    m_graph.findPath(13, 1);
    cout << "\nTesting nonexistant path with existing nodes (13 to 1): \n";
    m_graph.dump();
}

// tests path to self
void Tester::testPathToSelf() {
    m_graph.findPath(0, 0);
    cout << "\nTesting path to self (0 to 0): \n";
    m_graph.dump();
}

// tests non existant start
void Tester::testNonExistantStart() {
    m_graph.findPath(20, 5);
    cout << "\nTesting nonexistant start (20 to 5): \n";
    m_graph.dump();
}

// tests non existant end
void Tester::testNonExistantEnd() {
    m_graph.findPath(5, 20);
    cout << "\nTesting nonexistant end (5 to 20): \n";
    m_graph.dump();
}

// tests notmal case
void Tester::testNormalGraph() {
    Graph aGraph(m_fileName);
    aGraph.findPath(1, 4);
    cout << "\nTesting normal case for alternative constructor (finding "
            "path from 1 to 4): \n";
    aGraph.dump();
}

// fills empty graph
void Tester::fillEmptyGraph() {
    try {
        cout << "\nTrying to fill an empty graph...\n";
        m_graph.buildGraph(m_fileName);
        cout << "Filled an empty graph!\n";
    } catch (int i) {
        cout << "Failed... :(\n";
    }
}

// tests existant path
void Tester::testExistantPath() {
    m_graph.findPath(1, 5);
    cout << "\nTesting normal case (finding path from 1 to 5\n";
    m_graph.dump();
}

// tests assignment operator
void Tester::testAssignmentOperator() {
    cout << "\nTesting Assignment Operator...\n";
    // create temperary graph to clear to test assignment operator, this graph
    // is identical to m_graph but the assignment operator was not used to make
    // them identical
    Graph temp;
    temp.buildGraph(m_fileName);
    // finds the same path on bothe m_graph and temp
    temp.findPath(1, 13);
    m_graph.findPath(1, 13);
    // creates a array of three graphs, the first has 3 nodes, second has 1,
    // the third has no nodes
    Graph cases[3];
    cases[0].insert(1, 2, -1, -1, -1);
    cases[0].insert(2, 3, -1, -1, -1);
    cases[0].insert(3, -1, -1, -1, -1);
    cases[1].insert(1, -1, -1, -1, -1);
    // assign the cases all to temp
    cases[0] = cases[1] = cases[2] = temp;
    // clear temp
    temp.clearGraph();
    bool allTrue = true;
    // iterate through the cases
    for (int i = 0; i < 3 && allTrue; i++) {
        // check if each graph is a deep copy of m_graph
        if (!compareGraphs(cases[i], m_graph)) {
            allTrue = false;
        }
    }
    // prints out if the assignment operator works
    cout << ((allTrue) ? ("Assignment Operator Passed!\n")
                       : ("Assignment Operator Failed. :( \n"));
}

// checks if two nodes are equal
bool Tester::equalNodes(Node* first, Node* second) {
    // if the first and second esist and their values don't equal, return false
    if (first) {
        if (second) {
            if (first->getValue() != second->getValue()) {
                return false;
            }
        } else {
            // return false if first exists bit second does not
            return false;
        }
    } else if (second) {
        // return false if first exists but second does not
        return false;
    }
    return true;
}

// compares two graphs are returns if they are equal or not
bool Tester::compareGraphs(Graph& graph1, Graph& graph2) {
    // checks if the member variables m_numNodes are equal
    if (graph1.m_numNodes != graph2.m_numNodes) {
        return false;
    }
    // checks if the member variables m_dataFile are equal
    if (graph1.m_dataFile != graph2.m_dataFile) {
        return false;
    }
    // checks that both m_heads exist or both do not exist
    if ((!graph2.m_head && graph1.m_head) ||
        (!graph1.m_head && graph2.m_head)) {
        return false;
    }
    // iterates through the nodes of each graph
    for (int j = graph1.m_numNodes - 1; j >= 0; j--) {
        Node* graph1Node = graph1.m_head;
        Node* graph2Node = nullptr;
        Node* foo = nullptr;
        Node* caseFoo = nullptr;
        // sets graph1Node to last unchecked node using j
        for (int k = j; k > 0; k--) {
            graph1Node = graph1Node->getNext();
        }
        // sets graph2Node to the node with the same value as graph1Node in
        // graph2
        graph2Node = graph1.findNode(graph1Node->getValue());
        // checks that graph2Node exists if graph1Node exists
        if (!graph2Node) {
            return false;
        }
        // checks that the north nodes are equal
        foo = graph1Node->getNorth();
        caseFoo = graph2Node->getNorth();
        if (!equalNodes(foo, caseFoo)) {
            return false;
        }
        // checks that the south nodes are equal
        foo = graph1Node->getSouth();
        caseFoo = graph2Node->getSouth();
        if (!equalNodes(foo, caseFoo)) {
            return false;
        }
        // checks that the west nodes are equal
        foo = graph1Node->getWest();
        caseFoo = graph2Node->getWest();
        if (!equalNodes(foo, caseFoo)) {
            return false;
        }
        // checks that the east nodes are equal
        foo = graph1Node->getEast();
        caseFoo = graph2Node->getEast();
        if (!equalNodes(foo, caseFoo)) {
            return false;
        }
    }
    // checks that the m_paths in each graph are the same size
    if (graph1.m_path.size() != graph2.m_path.size()) {
        return false;
    } else {
        // creates place holder stacks to read and remake original stacks in
        // graph1 and graph2
        stack<int> graphPath1;
        stack<int> graphPath2;
        bool equalStacks = true;
        // iterates through graph1.m_path
        while (!graph1.m_path.empty()) {
            // push to graphPath1 and graphPath2
            graphPath1.push(graph1.m_path.top());
            graphPath2.push(graph2.m_path.top());
            // checks if corresponding values are equal
            if (graphPath1.top() != graphPath2.top()) {
                equalStacks = false;
            }
            // pop out the values pushed to graphPath1 and graphPath2
            graph1.m_path.pop();
            graph2.m_path.pop();
        }
        // transfers the data back to graph1.m_path and graph2.m_path from
        // graphPath1 and graphPath2 respectively
        while (!graphPath1.empty()) {
            graph1.m_path.push(graphPath1.top());
            graph2.m_path.push(graphPath2.top());
            graphPath1.pop();
            graphPath2.pop();
        }
        // returns false if the two stacks did not have equal values
        if (!equalStacks) {
            return false;
        }
    }
    return true;
}

int main() {
    Graph aGraph("testdata.txt");
    // testing a path from 0 to 5
    aGraph.findPath(0, 5);
    cout << "Finding the path from node 0 to node 5: " << endl;
    aGraph.dump();

    aGraph.findPath(2, 2);
    // testing a path from 2 to 2
    cout << "Finding the path from node 2 to itself: " << endl;
    aGraph.dump();
    aGraph.findPath(4, 0);
    // testing a path from 4 to 0
    cout << "Finding the path from node 4 to node 0: " << endl;
    aGraph.dump();

    Graph bGraph;
    bGraph.findPath(1, 14);
    // testing a path from 1 to 14
    cout << "Finding the path from node 1 to node 14 in an empty Graph object "
            ": "
         << endl;
    bGraph.dump();

    // creates the Tester object which runs all of it's tests in the
    // constructor
    Tester test;
    return 0;
}
