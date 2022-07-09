/*****************************************
** File:    mytest.cpp
** Project: CMSC 341 Project 0, Fall 2021
** Author:  Reagan Armstrong
** Date:    9/14/2021
** Section: 03
** E-mail:  reagana1@gl.umbc.edu
**
** Function definitions for the hist class functions
**
**
**
**
***********************************************/
#include "hist.h"

// Data
// Constructor for Data class that initializes m_data and m_arraySize
Data::Data() : m_arraySize(DATAARRAYSIZE) {
    // creates a dynamic array of int with size m_arraySize
    m_data = new int[m_arraySize];
    // zeros out all of the ints in m_data
    for (int i = 0; i < m_arraySize; i++) {
        m_data[i] = 0;
    }
}

// ~Data
// deletes m_data
Data::~Data() { delete[] m_data; }

// simulateData
// adds grades to their respective indexes, where there are students number of
// grades
void Data::simulateData(int students) {
    // creates a dynamic Random object
    Random *temp = new Random(MINGRADE, MAXGRADE);
    // creates and adds student number of random grades to m_data
    for (int i = 0; i < students; i++) {
        int grade = temp->getNormRand();
        // places at last index if its MAXGRADE
        if (grade == MAXGRADE) {
            m_data[m_arraySize - 1]++;
            // (grade - (grade % 10)) / 10 is a short way to find the index of
            // the grade rather than if statements , it chops of ones place and
            // divides by 10
        } else {
            int index = (grade - (grade % 10)) / 10;
            m_data[index]++;
        }
    }
    // deallocates temp to prevent leaks
    delete temp;
    temp = nullptr;
}

// dump
// prints a histogram for m_data
void Data::dump(int item) {
    // m_data is marked as item item(parameter)
    cout << "Histogram for item " << item << ":" << endl;
    // iterates through m_data
    for (int i = 0; i < m_arraySize; i++) {
        // converts indexes to 10-100
        cout << "  " << (i + 1) * 10 << " ";
        // prints asterisks equal to number at index
        for (int j = 0; j < m_data[i]; j++) {
            cout << "*";
        }
        cout << endl;
    }
}

// getData
// returns the int in m_data at a given index
int Data::getData(int index) { return m_data[index]; }

// getArraySize
// returns m_arraySIze
int Data::getArraySize() { return m_arraySize; }

// Overloaded equality operator. It returns true if two objects are the same.
// The "same objects" means they are the same pointers and carry the same value
// in every correpsonding cell.
// For testing inequality we can simply negate the return value of this
// operator.
bool operator==(const Data &lhs, const Data &rhs) {
    if (&lhs == &rhs) {
        for (int i = 0; i < lhs.m_arraySize; i++) {
            if (lhs.m_data[i] != rhs.m_data[i]) return false;
        }
        return true;
    } else
        return false;
}

// Hist
// initializes m_items and m_table
Hist::Hist(int items) : m_items(items) {
    // handles edge and normal cases
    if (m_items == 0) {
        m_table = nullptr;
    } else if (m_items > 0) {
        m_table = new Data[m_items];
        // handles error cases
    } else {
        throw(std::out_of_range(
            "Error at Constructor: number of items is out of range"));
    }
}

// ~Hist
// Destructor for Hist, deletes m_table
Hist::~Hist() { delete[] m_table; }

// dump
// calls dump for every Data object in m_data
void Hist::dump() {
    for (int i = 0; i < m_items; i++) {
        m_table[i].dump(i);
        // seperates with a newLine
        cout << endl;
    }
}

// simulateData
// calls simulateData for every data object in m_table with param students
void Hist::simulateData(int students) {
    if (m_items < 1) {
        // throws error in error case students<1
        throw(std::out_of_range(
            "Error at simulate: number of items is out of range!"));
    }
    // iterates through m_table
    for (int i = 0; i < m_items; i++) {
        m_table[i].simulateData(students);
    }
}

// Hist
// Copy constructor for Hist
Hist::Hist(const Hist &rhs) {
    // copies m_items
    m_items = rhs.m_items;
    // if there are items to copy
    if (m_items > 0) {
        // creates a table of the same size
        m_table = new Data[m_items];
        // iterates through m_table
        for (int i = 0; i < m_items; i++) {
            // iterates through m_data
            for (int j = 0; j < m_table[i].m_arraySize; j++) {
                // copies over all of the values in m_data
                m_table[i].m_data[j] = rhs.m_table[i].m_data[j];
            }
        }
    } else {
        // if there are no items to copy
        m_table = nullptr;
    }
}

// operator=
// overloaded assignment operator that deep copies rhs
const Hist &Hist::operator=(const Hist &rhs) {
    // protects against self assignment
    if (this != &rhs) {
        // if m_table isn't nullptr then delete m_table
        if (m_items != 0) {
            delete[] m_table;
        }
        // copies m_items
        m_items = rhs.m_items;
        // if there are items to copy
        if (m_items > 0) {
            // creates a table of the same size
            m_table = new Data[m_items];
            // iterates through m_table
            for (int i = 0; i < m_items; i++) {
                // iterates through m_data
                for (int j = 0; j < m_table[i].m_arraySize; j++) {
                    // copies over all of the values in m_data
                    m_table[i].m_data[j] = rhs.m_table[i].m_data[j];
                }
            }

        } else {
            // if there are no items to copy
            m_table = nullptr;
        }
        return *this;
    }
    return *this;
}