/*****************************************
** File:    mytest.cpp
** Project: CMSC 341 Project 2, Fall 2021
** Author:  Reagan Armstrong
** Date:    10/26/2021
** Section: 03
** E-mail:  reagana1@gl.umbc.edu
**
** Tester class and definitions
**
**
**
**
***********************************************/
// UMBC - CSEE - CMSC 341 - Fall 2021 - Proj2
#include <random>

#include "swarm.h"

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

class Tester {
   public:
    Tester() {
        // tests insertion
        testInsertion(Robot());
        // tests inserting many bots
        insertManyBots();
        // tests removals
        testRemovals();
        // tests duplicate insertions
        insertDuplicates();
        // tests runtimes
        runtimes();
        // tests edge cases
        edgeCases();
    }
    void testInsertion(Robot aBot) {
        m_swarm.insert((aBot));
        if (m_swarm.findBot(aBot.getID())) {
            cout << "Insertion at head passed" << endl;
        }
        m_swarm.clear();
    }
    void insertManyBots() {
        for (int i = 0; i < 100; i++) {
            Robot robot(i);
            m_swarm.insert(robot);
        }
        // checks BST and AVL properties of insert
        if (checkBST(m_swarm.m_root)) {
            cout << "Insertion preserves BST property after many insertions"
                 << endl;
        }
        if (checkBalanced()) {
            cout << "Insertion preserves AVL property (balance) after many "
                    "insertions"
                 << endl;
        }
        m_swarm.clear();
    }
    void testRemovals() {
        for (int i = 0; i < 100; i++) {
            if (i % 2 == 0) {
                Robot robot(i, DEFAULT_TYPE, ALIVE);
                m_swarm.insert(robot);
            } else {
                Robot robot(i, DEFAULT_TYPE, DEAD);
                m_swarm.insert(robot);
            }
        }
        // tests removal and removeDead()
        cout << "\n\nTree with Alive and Dead before any removal:" << endl
             << endl;
        m_swarm.dumpTree();
        cout << endl;
        m_swarm.listRobots();
        cout << endl;
        cout << "\n\nTesting removal by removing id:1" << endl;
        m_swarm.remove(1);
        if (!m_swarm.findBot(1)) {
            cout << "Removal Passed!" << endl;
        }
        cout << "\n\nTree with Alive and Dead after removeDead():" << endl
             << endl;
        m_swarm.removeDead();
        m_swarm.dumpTree();
        cout << endl;
        m_swarm.listRobots();
        cout << endl;
        // checks BST and AVL properties of remove
        if (checkBST(m_swarm.m_root)) {
            cout << "\n\nRemoval preserves BST property!" << endl;
        } else {
            cout << "Removal failed" << endl;
        }
        if (checkBalanced()) {
            cout << "Removal preserves AVL property(balance)" << endl;
        } else {
            cout << "removal does not preserve AVL property" << endl;
        }
        m_swarm.clear();
    }
    // checks if the subtree under aBot is a BST recursively
    bool checkBST(Robot* aBot) {
        if (aBot) {
            if (aBot->getRight() &&
                aBot->getRight()->getID() < aBot->getID()) {
                return false;
            }
            if (aBot->getLeft() && aBot->getLeft()->getID() > aBot->getID()) {
                return false;
            }
            return (checkBST(aBot->getRight()) && checkBST(aBot->getLeft()));
        }
        return true;
    }
    // checks if the subtree under aBot is a balanced recursively
    bool checkBalanced() {
        if (m_swarm.lowestSubImbalance(m_swarm.m_root)) {
            return false;
        }
        return true;
    }
    // tests if duplicated can be inserted
    void insertDuplicates() {
        cout << "Testing inserting duplicate Robots with ID:0" << endl;
        Robot robot;
        m_swarm.insert(robot);
        m_swarm.insert(robot);
        cout << "resulting Tree" << endl << endl;
        m_swarm.dumpTree();
        m_swarm.clear();
    }
    // shows the runtimes of remove and insert
    void runtimes() {
        for (int j = 1; j <= 3; j++) {
            clock_t start, stop;
            double T;
            start = clock();
            // changes j to 4 when j=3 to have 2000,4000,and 8000 inserts and
            // removals
            if (j == 3) {
                j = 4;
            }
            for (int i = 0; i < 2000 * j; i++) {
                Robot robot(i);
                m_swarm.insert(robot);
            }
            stop = clock();
            T = stop - start;  // number of clock ticks the algorithm took
            cout << "Inserting " << 2000 * j << " items took " << T
                 << " clock ticks (" << T / CLOCKS_PER_SEC << " seconds)!"
                 << endl
                 << endl;
            start = clock();
            m_swarm.clear();
            stop = clock();
            T = stop - start;  // number of clock ticks the algorithm took
            cout << "Removing " << 2000 * j << " items took " << T
                 << " clock ticks (" << T / CLOCKS_PER_SEC << " seconds)!"
                 << endl
                 << endl;
        }
    }
    // driver given to us plus more
    void edgeCases() {
        // given code
        Random idGen(MINID, MAXID);
        Random typeGen(0, 4);
        {
            Swarm team;
            int teamSize = 10;
            int tempID = 0;
            int ID = 0;
            for (int i = 0; i < teamSize; i++) {
                ID = idGen.getRandNum();
                if (i == teamSize / 2)
                    tempID = ID;  // we store this ID for later use
                Robot robot(ID, static_cast<ROBOTTYPE>(typeGen.getRandNum()));
                team.insert(robot);
            }
            cout << "\nDump after inserting " << teamSize << " nodes:\n\n";
            team.dumpTree();
            cout << "\n\nList of Robots after inserting " << teamSize
                 << " nodes:\n";
            team.listRobots();
            cout << endl;
            // made by me
            cout << "\n\nRemoval Edge Cases:" << endl;
            team.remove(tempID);
            cout << "\nDump after removig the node with ID: " << tempID
                 << "\n\n";
            team.dumpTree();
            cout << "\n\nList of Robots after removing the node with ID: "
                 << tempID << "\n";
            team.listRobots();
            // test removing the root
            cout << "\nDump after removig the root with id:"
                 << team.m_root->getID() << "\n\n";
            team.remove(team.m_root->getID());
            team.dumpTree();
            // testing non exististnt removal
            cout << "\n\nDump after removing the node with nonexistant "
                    "ID: "
                 << -21232123 << "\n";
            team.remove(-21232123);
            team.dumpTree();
            team.clear();
            cout << "\n\nInsert Edge Case, unordered set of numbers:" << endl;
            // using an unordered array to make a tree that must be rebalanced
            // when inserting
            int unorderedArr[10] = {2, 4, 5, 6, 11, 13, 14, 17, 18, 19};
            for (int i = 0; i < 10; i++) {
                team.insert(Robot(unorderedArr[i]));
            }
            team.dumpTree();
            cout << endl;
        }
    }

   private:
    Swarm m_swarm;
};

int main() {
    Tester tester;
    return 0;
}
