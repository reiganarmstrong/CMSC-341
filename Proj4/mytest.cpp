/*****************************************
** File:    tester.cpp
** Project: CMSC 341 Project 2, Fall 2021
** Author:  Reagan Armstrong
** Date:    12/07/2021
** Section: 03
** E-mail:  reagana1@gl.umbc.edu
**
** tester class definitions
**
**
**
**
******************************************/
// CMSC 341 - Fall 2021 - Project 4
#include <iostream>
#include <random>
#include <vector>

#include "hash.h"
using namespace std;
enum RANDOM { UNIFORM, NORMAL };
class Random {
   public:
    Random(int min, int max, RANDOM type = UNIFORM)
        : m_min(min), m_max(max), m_type(type) {
        if (type == NORMAL) {
            m_generator = std::mt19937(m_device());
            // the data set will have the mean of 50 and standard deviation of
            // 20
            m_normdist = std::normal_distribution<>(50, 20);
        } else {
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);  // 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min, max);
        }
    }

    int getRandNum() {
        int result = 0;
        if (m_type == NORMAL) {
            // returns a random number in a set with normal distribution
            // we limit random numbers by the min and max values
            result = m_min - 1;
            while (result < m_min || result > m_max)
                result = m_normdist(m_generator);
        } else {
            // this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

   private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;      // normal distribution
    std::uniform_int_distribution<> m_unidist;  // uniform distribution
};

// The hash function used by HashTable class
unsigned int hashCode(const string str);

class Tester {  // Tester class to implement test functions
   public:
    Tester() {
        // test the non colliding cases
        nonCollidingTests();
        // tests the colliding cases
        collidingTests();
        cout << "THERE WILL BE A LOT OF OUTPUT AND SOME MAY GET CUT OFF "
                "DEPENDING ON THE TERMINAL SIZING, PLEASE REFER TO THE CODE "
                "WHEN "
                "GRADING IF SOME OF THE OUTPUT GETS CUT OFF RATHER THAN THE "
                "CONSOLE"
             << endl;
    }
    ~Tester() {}
    // gets the correct index the file should be places in into the table
    unsigned getCorrectIndex(HashTable &table, File file) {
        string name = file.key();
        // looks at which table to look through
        if (table.m_newTable == TABLE1) {
            // index is the quadratic probing algorithm with i that increments
            for (unsigned
                     i = 0,
                     index = ((table.m_hash(file.key()) % table.m_capacity1) +
                              (i * i)) %
                             table.m_capacity1;
                 i < table.m_capacity1; i++,
                     index = ((table.m_hash(file.key()) % table.m_capacity1) +
                              (i * i)) % table.m_capacity1) {
                if (table.m_table1[index] == EMPTY) {
                    // returns the index if the probing resulted in an empty
                    // space
                    return index;
                }
            }
            // same concept but with table2
        } else if (table.m_newTable == TABLE2) {
            for (unsigned
                     i = 0,
                     index = ((table.m_hash(file.key()) % table.m_capacity2) +
                              (i * i)) %
                             table.m_capacity2;
                 i < table.m_capacity1; i++,
                     index = ((table.m_hash(file.key()) % table.m_capacity2) +
                              (i * i)) % table.m_capacity2) {
                if (table.m_table2[index] == EMPTY) {
                    return index;
                }
            }
        }
        // return in case if nothing else returns
        return -1;
    }
    // the non colliding cases
    void nonCollidingTests() {
        cout << "---------INSERTING NON-COLLIDING----------" << endl;
        Random diskBlockGen(DISKMIN, DISKMAX);
        File files[50] = {EMPTY};
        int fileIndex = 0;
        string name = "";
        HashTable aTable(MINPRIME, hashCode);
        bool noCollision = false;
        unsigned int temp;
        cout << "m_size1 before insertions: " << aTable.m_size1 << endl;
        // makes a random table of capacity 50 and variable size, all keys are
        // strings of length>=1 comprising of 'a'
        for (int i = 0; i < 50; i++) {
            temp = diskBlockGen.getRandNum();
            name += "a";
            files[fileIndex] = File(name, temp);
            // checks if any collision occured (turnary operator is the case
            // not accounting for collision , getCOrrectIndex returns
            // the index accounting for collsion)
            noCollision = (getCorrectIndex(aTable, files[fileIndex]) ==
                           ((aTable.m_newTable == TABLE1)
                                ? (((aTable.m_hash(files[fileIndex].key()) %
                                     aTable.m_capacity1)) %
                                   aTable.m_capacity1)
                                : (((aTable.m_hash(files[fileIndex].key()) %
                                     aTable.m_capacity2)) %
                                   aTable.m_capacity2)));
            // if there was no collition add the file to the table and record
            // it in files[]
            if (noCollision) {
                cout << "Insertion # " << fileIndex << " => " << temp
                     << " -- before collsions index "
                     << ((aTable.m_newTable == TABLE1)
                             ? (((aTable.m_hash(files[fileIndex].key()) %
                                  aTable.m_capacity1)) %
                                aTable.m_capacity1)
                             : (((aTable.m_hash(files[fileIndex].key()) %
                                  aTable.m_capacity2)) %
                                aTable.m_capacity2))
                     << " -- after collisions index (will be same if no "
                        "collisions): "
                     << getCorrectIndex(aTable, files[fileIndex]) << endl;
                aTable.insert(files[fileIndex]);
                // add to the index
                fileIndex++;
            } else {
                files[fileIndex] = EMPTY;
            }
        }
        cout << "m_size1 after insertions: " << aTable.m_size1 << endl;
        cout << " Dumping non colliding hastable:" << endl;
        aTable.dump();
        cout << "--------TESTING FIND OPERATION (GETFILE() FUNCTION) WITH A "
                "FEW NON-COLLIDING KEYS...------------"
             << endl;
        // runs getFile on every file
        bool findWorks = true;
        for (int i = 0; i < fileIndex && findWorks; i++) {
            findWorks = !(
                aTable.getFile(files[i].key(), files[i].diskBlock()) == EMPTY);
        }
        if (findWorks) {
            cout << "GETFILE() WORKS!" << endl;
        } else {
            cout << "GETFILE() DOES NOT WORK!" << endl;
        }
        int toRemove = floor(fileIndex / 4);
        cout << "------TESTING REMOVE ON A FEW NON COLLIDING(" << toRemove
             << ")------------" << endl;
        for (int i = 0; i < toRemove; i++) {
            aTable.remove(files[i]);
        }
        cout << "Dump after removals" << endl;
        aTable.dump();
    }
    void collidingTests() {
        // This program presents a sample use of the class HashTable
        // It does not represent any rehashing
        cout << "-------------COLLIDING TESTS--------------" << endl;
        Random diskBlockGen(DISKMIN, DISKMAX);
        int tempDiskBlocks[50] = {0};
        // holds the text.txt diskblocks
        int testDiskBlocks[51] = {0};
        HashTable aTable(MINPRIME, hashCode);
        int temp = 0;
        int secondIndex = 0, thirdIndex = 0;
        for (int i = 0; i < 50; i++) {
            temp = diskBlockGen.getRandNum();
            if (i % 3 ==
                0) {  // this saves 17 numbers from the index range [0 - 49]
                tempDiskBlocks[secondIndex] = temp;
                // cout << temp << " was saved for later use." << endl;
                secondIndex++;
            } else {
                testDiskBlocks[thirdIndex] = temp;
                // cout << temp << " was saved for later use." << endl;
                thirdIndex++;
            }
            cout << "Insertion # " << i
                 << ((i % 3 != 0) ? (" (key:\"test.txt\") ")
                                  : (" (key:\"driver.cpp\") "))
                 << " => " << temp;
            if (i % 3 != 0) {
                // prints out index accounting for and not accounting for
                // collision
                cout << " -- before collsions index "
                     << ((aTable.m_newTable == TABLE1)
                             ? (((aTable.m_hash("test.txt") %
                                  aTable.m_capacity1)) %
                                aTable.m_capacity1)
                             : (((aTable.m_hash("test.txt") %
                                  aTable.m_capacity2)) %
                                aTable.m_capacity2))
                     << " -- after collisions index:"
                     << getCorrectIndex(aTable, File("test.txt", temp))
                     << endl;
                aTable.insert(File("test.txt", temp));
            } else {
                // prints out index accounting for and not accounting for
                // collision
                cout << " -- before collsions index: "
                     << ((aTable.m_newTable == TABLE1)
                             ? (((aTable.m_hash("driver.cpp") %
                                  aTable.m_capacity1)) %
                                aTable.m_capacity1)
                             : (((aTable.m_hash("driver.cpp") %
                                  aTable.m_capacity2)) %
                                aTable.m_capacity2))
                     << " --Expected after collisions index:"
                     << getCorrectIndex(aTable, File("driver.cpp", temp))
                     << endl;
                // these will be deleted
                aTable.insert(File("driver.cpp", temp));
            }
        }

        cout << "Message: dump after 50 insertions in a table with MINPRIME "
                "(101)buckets : "
             << endl;
        aTable.dump();
        cout << "---------------------TEST GETFILE ON MANY COLLIDING WITHOUT "
                "REHASHING---------------------"
             << endl;
        // runs getFIle on every test.txt file in aTable
        bool findWorks = true;
        for (int i = 0; i < thirdIndex && findWorks; i++) {
            findWorks =
                !(aTable.getFile("test.txt", testDiskBlocks[i]) == EMPTY);
        }
        if (findWorks) {
            cout << "GETFILE() WORKS!" << endl;
        } else {
            cout << "GETFILE() DOES NOT WORK!" << endl;
        }
        cout << "Dump after testing many getfiles" << endl;
        aTable.dump();
        temp = diskBlockGen.getRandNum();
        cout << "Insertion # " << 51 << " => " << temp << endl;
        // adds 51st file which satisfies hash condition
        aTable.insert(File("test.txt", temp));
        cout << "-------------------------TEST "
                "REHASHING AFTER NUMEROUS COLLIDING "
                "INSERTIONS--------------------------------------------"
             << endl;
        cout << "Message: dump after 51 insertions in a table with MINPRIME "
                "(101)buckets : "
             << endl;
        aTable.dump();
        for (int i = 0; i < 14 && secondIndex >= 0; i++, secondIndex--)
            aTable.remove(File("driver.cpp", tempDiskBlocks[i]));
        cout << "-------REMOVE OPERATION WITH A NUMBER OF COLLIDING KEYS "
                "WITHOUT TRIGGERING A REHASH-------------"
             << endl;
        cout << "Message: dump after removing 14 buckets," << endl;
        aTable.dump();
        // removes enough for rehashing
        for (int i = 0; i < 29 && thirdIndex >= 0; i++, thirdIndex--)
            aTable.remove(File("test.txt", testDiskBlocks[i]));
        cout << "--------------------REHASHING IS TRIGGERED AFTER A NUMBER OF "
                "COLLIDING KEYS REMOVAL---------------------------------"
             << endl;
        aTable.dump();
    }

   private:
};

int main() {
    Tester test;
    return 0;
}

unsigned int hashCode(const string str) {
    unsigned int val = 0;
    const unsigned int thirtyThree = 33;  // magic number from textbook
    for (int i = 0; i < int(str.length()); i++)
        val = val * thirtyThree + str[i];
    return val;
}