/*****************************************
** File:    hash.cpp
** Project: CMSC 341 Project 2, Fall 2021
** Author:  Reagan Armstrong
** Date:    12/07/2021
** Section: 03
** E-mail:  reagana1@gl.umbc.edu
**
** hash class definitions
**
**
**
**
******************************************/
// CMSC 341 - Fall 2021 - Project 4
#include "hash.h"

#include "math.h"

// constructor for hastable, initializes all values
HashTable::HashTable(unsigned size, hash_fn hash) {
    m_hash = hash;
    m_newTable = TABLE1;
    m_capacity1 = size;
    // makes sure capacity is between the constraints
    if (m_capacity1 < MINPRIME) {
        m_capacity1 = MINPRIME;
    } else if (m_capacity1 > MAXPRIME) {
        m_capacity1 = MAXPRIME;
    } else if (!isPrime(m_capacity1)) {
        m_capacity1 = findNextPrime(m_capacity1);
    }
    m_size1 = 0;
    m_size2 = 0;
    m_capacity2 = 0;
    m_numDeleted1 = 0;
    m_numDeleted2 = 0;
    m_table2 = nullptr;
    // allocates memory for the array
    m_table1 = new File[size];
}

// destructor deletes tables if they exist
HashTable::~HashTable() {
    if (m_table1) {
        delete[] m_table1;
    }
    if (m_table2) {
        delete[] m_table2;
    }
}

// gets the file if it exists otherwise returns EMPTY
File HashTable::getFile(string name, unsigned int diskBlock) {
    // checks capacities to see if they exist
    if (m_capacity1 > 0) {
        // uses given equation to traverse (uses i) abd returns when it finds
        // the matching index
        for (unsigned i = 0, index = ((m_hash(name) % m_capacity1) + (i * i)) %
                                     m_capacity1;
             i < m_capacity1;
             i++, index =
                      ((m_hash(name) % m_capacity1) + (i * i)) % m_capacity1) {
            if (m_table1[index].key() == name &&
                m_table1[index].diskBlock() == diskBlock) {
                return m_table1[index];
            }
        }
    }
    if (m_capacity2 > 0) {
        // uses given equation to traverse (uses i) abd returns when it finds
        // the matching index
        for (unsigned i = 0, index = ((m_hash(name) % m_capacity2) + (i * i)) %
                                     m_capacity2;
             i < m_capacity2;
             i++, index =
                      ((m_hash(name) % m_capacity2) + (i * i)) % m_capacity2) {
            if (m_table2[index].key() == name &&
                m_table2[index].diskBlock() == diskBlock) {
                return m_table2[index];
            }
        }
    }
    // handleTransfers();
    return EMPTY;
}

// inserts files and returns if it was successful or not
bool HashTable::insert(File file) {
    string name = file.key();
    unsigned int diskBlockNumber = file.diskBlock();
    // check if it is a valid file
    if (!(getFile(name, diskBlockNumber) == EMPTY &&
          diskBlockNumber >= DISKMIN && diskBlockNumber <= DISKMAX)) {
        return false;
    }
    bool inserted = false;
    // check which is the working table
    if (m_newTable == TABLE1) {
        // tries to insert, stops when it does
        for (unsigned i = 0,
                      index = ((m_hash(file.key()) % m_capacity1) + (i * i)) %
                              m_capacity1;
             i < m_capacity1 && !inserted;
             i++, index = ((m_hash(file.key()) % m_capacity1) + (i * i)) %
                          m_capacity1) {
            if (m_table1[index] == EMPTY) {
                m_table1[index] = file;
                m_size1++;
                // stops the for loop in the next run
                inserted = true;
            }
        }
    } else if (m_newTable == TABLE2) {
        // tries to insert, stops when it does
        for (unsigned i = 0,
                      index = ((m_hash(file.key()) % m_capacity2) + (i * i)) %
                              m_capacity2;
             i < m_capacity1 && !inserted;
             i++, index = ((m_hash(file.key()) % m_capacity2) + (i * i)) %
                          m_capacity2) {
            if (m_table2[index] == EMPTY) {
                m_table2[index] = file;
                m_size2++;
                // stops the loop in the next run
                inserted = true;
            }
        }
    }
    handleTransfers();
    return inserted;
}

// starts the rehash process
void HashTable::startRehash() {
    // checks if one of the two tables are empty
    if (m_capacity1 == 0 || m_capacity2 == 0) {
        // checks of the rehash conditions are met for the one that is not
        // empty
        if (m_newTable == TABLE1 &&
            (lambda(TABLE1) > .5 || deletedRatio(TABLE1) > .8)) {
            // sets up other table
            m_newTable = TABLE2;
            m_capacity2 = ((m_size1 - m_numDeleted1) * 4);
            m_capacity2 = (isPrime(m_capacity2))
                              ? (m_capacity2)
                              : (findNextPrime(m_capacity2));
            m_table2 = new File[m_capacity2];
            m_size2 = 0;
            m_numDeleted2 = 0;
            // hands off transfer process
            handleTransfers();
            // checks of the rehash conditions are met for the one that is not
            // empty
        } else if (m_newTable == TABLE2 &&
                   (lambda(TABLE2) > .5 || deletedRatio(TABLE2) > .8)) {
            // sets up other table
            m_newTable = TABLE1;
            m_capacity1 = ((m_size2 - m_numDeleted2) * 4);
            m_capacity1 = (isPrime(m_capacity1))
                              ? (m_capacity1)
                              : (findNextPrime(m_capacity1));
            m_table1 = new File[m_capacity1];
            m_size1 = 0;
            m_numDeleted1 = 0;
            // hands off transfer process
            handleTransfers();
        }
    }
}

// handles all of the transfer processes
void HashTable::handleTransfers() {
    // checks to see if a transfer is in progress
    if (m_capacity1 > 0 && m_capacity2 > 0) {
        // runs 25% transfer
        quarterTransfer();
        // checks to see if the transfer is complete, if it is clear the empty
        // table
        if (m_newTable == TABLE1 && m_size2 == m_numDeleted2) {
            m_size2 = 0;
            m_capacity2 = 0;
            m_numDeleted2 = 0;
            delete[] m_table2;
            m_table2 = nullptr;
        } else if (m_size1 == m_numDeleted1) {
            m_size1 = 0;
            m_capacity1 = 0;
            m_numDeleted1 = 0;
            delete[] m_table1;
            m_table1 = nullptr;
        }
        // if not in progress, tries to start it
    } else {
        startRehash();
    }
}

// transfers 25% of m_size files over to the new table
void HashTable::quarterTransfer() {
    int quarterSize;
    bool inserted = false;
    // checks to see which table to transfer to
    if (m_newTable == TABLE1) {
        // sets the number of files to transfer over
        quarterSize = floor(m_size2 / 4);
        // iterates until it copies over quarterSize amount of files
        for (unsigned x = 0; x < m_capacity2 && quarterSize > 0; x++) {
            if (!(m_table2[x] == EMPTY) && !(m_table2[x] == DELETED)) {
                // finds available index in new table
                for (unsigned
                         i = 0,
                         index = ((m_hash(m_table2[x].key()) % m_capacity1) +
                                  (i * i)) %
                                 m_capacity1;
                     i < m_capacity1 && !inserted; i++,
                         index = ((m_hash(m_table2[x].key()) % m_capacity1) +
                                  (i * i)) % m_capacity1) {
                    if (m_table1[index] == EMPTY) {
                        // adds the file
                        m_table1[index] = m_table2[x];
                        m_size1++;
                        // stops the for loop
                        inserted = true;
                    }
                }
                // resets the variable for next time
                inserted = false;
                // deletes old bucket in old table
                m_table2[x] = DELETED;
                m_numDeleted2++;
                // decrements the number of nodes needed to add
                quarterSize--;
            }
        }
        // same as the if but with the two tables switched
    } else {
        quarterSize = floor(m_size1 / 4);
        for (unsigned x = 0; x < m_capacity1 && quarterSize > 0; x++) {
            if (!(m_table1[x] == EMPTY) && !(m_table1[x] == DELETED)) {
                // finds the next available empty bucket based on given
                // equation
                for (unsigned
                         i = 0,
                         index = ((m_hash(m_table1[x].key()) % m_capacity2) +
                                  (i * i)) %
                                 m_capacity2;
                     i < m_capacity2 && !inserted; i++,
                         index = ((m_hash(m_table1[x].key()) % m_capacity2) +
                                  (i * i)) % m_capacity2) {
                    if (m_table2[index] == EMPTY) {
                        m_table2[index] = m_table1[x];
                        m_size2++;
                        inserted = true;
                    }
                }
                inserted = false;
                m_table1[x] = DELETED;
                m_numDeleted1++;
                quarterSize--;
            }
        }
    }
}

// removes the a file from the  hashtable
bool HashTable::remove(File file) {
    // checks to see if it is a removable file
    if (getFile(file.key(), file.diskBlock()) == EMPTY ||
        getFile(file.key(), file.diskBlock()) == DELETED) {
        return false;
    }
    bool deleted = false;
    string name = file.key();
    unsigned int diskBlock = file.diskBlock();
    // checks table1
    if (m_capacity1 > 0) {
        // iterated through until it deletes it or reaches the end
        for (unsigned i = 0, index = ((m_hash(name) % m_capacity1) + (i * i)) %
                                     m_capacity1;
             i < m_capacity1 && !deleted;
             i++, index =
                      ((m_hash(name) % m_capacity1) + (i * i)) % m_capacity1) {
            if (m_table1[index].key() == name &&
                m_table1[index].diskBlock() == diskBlock) {
                m_table1[index] = DELETED;
                m_numDeleted1++;
                // stops loop
                deleted = true;
            }
        }
    }
    // checks table 2
    if (m_capacity2 > 0) {
        // iterates through until it deletes it or reaches the end
        for (unsigned i = 0, index = ((m_hash(name) % m_capacity2) + (i * i)) %
                                     m_capacity2;
             i < m_capacity2 && !deleted;
             i++, index =
                      ((m_hash(name) % m_capacity2) + (i * i)) % m_capacity2) {
            if (m_table2[index].key() == name &&
                m_table2[index].diskBlock() == diskBlock) {
                m_table2[index] = DELETED;
                m_numDeleted2++;
                // stops loop
                deleted = true;
            }
        }
    }
    // handles transfers
    handleTransfers();
    return deleted;
}

// returns the load factor
float HashTable::lambda(TABLENAME tablename) const {
    if (tablename == TABLE1) {
        return float(m_size1) / float(m_capacity1);
    } else {
        return float(m_size2) / float(m_capacity2);
    }
}

// returns the ratio of deleted buckets and occupied buckets
float HashTable::deletedRatio(TABLENAME tableName) const {
    if (tableName == TABLE1) {
        return float(m_numDeleted1) / float(m_size1);
    } else {
        return float(m_numDeleted2) / float(m_size2);
    }
}

void HashTable::dump() const {
    cout << "Dump for table 1: " << endl;
    if (m_table1 != nullptr)
        for (unsigned i = 0; i < m_capacity1; i++) {
            cout << "[" << i << "] : " << m_table1[i] << endl;
        }
    cout << "Dump for table 2: " << endl;
    if (m_table2 != nullptr)
        for (unsigned i = 0; i < m_capacity2; i++) {
            cout << "[" << i << "] : " << m_table2[i] << endl;
        }
}

bool HashTable::isPrime(int number) {
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

int HashTable::findNextPrime(int current) {
    // we always stay within the range [MINPRIME-MAXPRIME]
    // the smallest prime starts at MINPRIME
    if (current < MINPRIME) current = MINPRIME - 1;
    for (int i = current; i < MAXPRIME; i++) {
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0)
                break;
            else if (j + 1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    // if a user tries to go over MAXPRIME
    return MAXPRIME;
}