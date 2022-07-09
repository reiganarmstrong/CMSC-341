/*****************************************
** File:    swarm.cpp
** Project: CMSC 341 Project 2, Fall 2021
** Author:  Reagan Armstrong
** Date:    10/26/2021
** Section: 03
** E-mail:  reagana1@gl.umbc.edu
**
** swarm class definitions
**
**
**
**
/*****************************************
** File:    swarm.cpp
** Project: CMSC 341 Project 2, Fall 2021
** Author:  Reagan Armstrong
** Date:    10/26/2021
** Section: 03
** E-mail:  reagana1@gl.umbc.edu
**
** Swarm function definitions
**
**
**
**
***********************************************/
// UMBC - CSEE - CMSC 341 - Fall 2021 - Proj2
#include "swarm.h"
// constructor initializes m_root
Swarm::Swarm() { m_root = nullptr; }

// destructor clears the tree
Swarm::~Swarm() { clear(); }

// inserts a robot into the tree and balances
void Swarm::insert(const Robot& robot) {
    const int ROBOT_ID = robot.getID();
    const ROBOTTYPE ROBOT_TYPE = robot.getType();
    const STATE ROBOT_STATE = robot.getState();
    // in case if the tree is empty
    if (m_root == nullptr) {
        m_root = new Robot(ROBOT_ID, ROBOT_TYPE, ROBOT_STATE);
        // in case if the bot is not already in the tree
    } else if (!findBot(ROBOT_ID)) {
        Robot* previous = nullptr;
        Robot* current = m_root;
        // iterates through tree on correct path
        while (current) {
            // traverses right if ROBOT_ID is larger, and left if not
            if (ROBOT_ID > current->getID()) {
                previous = current;
                current = current->getRight();
            } else {
                previous = current;
                current = current->getLeft();
            }
        }
        // current becomes null and previous becomes the bot we use
        // place a new bot with ROBOT_ID in the appropriate spot unde previous
        if (ROBOT_ID > previous->getID()) {
            previous->setRight(new Robot(ROBOT_ID, ROBOT_TYPE, ROBOT_STATE));
        } else {
            previous->setLeft(new Robot(ROBOT_ID, ROBOT_TYPE, ROBOT_STATE));
        }
        // updates the heights of all sub robots of m_root and m_root
        // recursively
        updateAllSubHeights(m_root);
        // recursively rebalances the tree
        rebalanceTree();
    }
}

// repreatedly deletes head until the tree is empty
void Swarm::clear() {
    while (m_root) {
        remove(m_root->getID());
    }
}

// finds the highest robot on the right of aBot or the lowest robot on the
// right
Robot* Swarm::lowestHighOrHighestLow(Robot* aBot) {
    Robot* directionalBot;
    if (aBot->getLeft() == nullptr) {
        // go to the right then keep on going as left as possible
        directionalBot = aBot->getRight();
        while (directionalBot->getLeft()) {
            directionalBot = directionalBot->getLeft();
        }
    } else {
        // go to the left and then keep on going as right as possible
        directionalBot = aBot->getLeft();
        while (directionalBot->getRight()) {
            directionalBot = directionalBot->getRight();
        }
    }
    // return the end bot
    return directionalBot;
}

// removes the bot with ID id
void Swarm::remove(int id) {
    Robot* toRemove = getBot(id);
    // if the bot toRemove exists
    if (toRemove) {
        Robot* left = toRemove->getLeft();
        Robot* right = toRemove->getRight();
        Robot* parent = getParent(toRemove->getID());
        Robot* directionalParent;
        // if either the left or right children of toRemove exist
        if (left || right) {
            // get the bot to replace toRemove
            Robot* toMoveUp = lowestHighOrHighestLow(toRemove);
            // delete the connection between the bot to move up and it's
            // current parent
            severBot(getParent(toMoveUp->getID()), toMoveUp);
            // if the parent of toRemove exists move up to underneath that,
            // otherwise move up to underneath the m_root pointer
            if (parent) {
                moveUp(toMoveUp, toRemove, parent);
            } else {
                // clear the link between m_root and toRemove
                m_root = nullptr;
                moveUp(toMoveUp, toRemove, parent);
            }
        } else {
            // if neither left or right exist and the parent does, sever the
            // bot fro the parent
            if (parent) {
                severBot(parent, toRemove);
            } else {
                // sever from m_root if not parent
                m_root = nullptr;
            }
        }
        // sever all toRemove connections
        severBot(toRemove, toRemove->getLeft());
        severBot(toRemove, toRemove->getRight());
        delete toRemove;
    }
    // update the heights and balance the tree
    updateAllSubHeights(m_root);
    rebalanceTree();
}

// id the parent and chilld both exist take away the parent's pointer to the
// child
void Swarm::severBot(Robot* parent, Robot* child) {
    if (parent && child) {
        if (parent->getRight() == child) {
            parent->setRight(nullptr);
        } else {
            parent->setLeft(nullptr);
        }
    }
}

// moves up toMoveUp to the position of toRemove
void Swarm::moveUp(Robot* toMoveUp, Robot* toRemove, Robot* parent) {
    if (parent == m_root) {
        // move up toMoveUp to underneath m_root if parent is equal to m_root
        m_root = toMoveUp;
        // replace toRemove's position with toMoveup
    } else if (parent->getRight() == toRemove) {
        parent->setRight(toMoveUp);
    } else {
        parent->setLeft(toMoveUp);
    }
    // connect the rest of the bots under toRemove to the ends of toMoveUp
    maxBot(toMoveUp)->setRight(toRemove->getRight());
    minBot(toMoveUp)->setLeft(toRemove->getLeft());
}

// finds the smallest bot under aBot
Robot* Swarm::minBot(Robot* aBot) {
    Robot* min = aBot;
    while (min->getLeft()) {
        min = min->getLeft();
    }
    return min;
}

// finds the largest bot under aBot
Robot* Swarm::maxBot(Robot* aBot) {
    Robot* max = aBot;
    while (max->getRight()) {
        max = max->getRight();
    }
    return max;
}

// recursively rebalances the tree
void Swarm::rebalanceTree() {
    // get the lowest imbalance in the tree
    Robot* imbalance = lowestSubImbalance(m_root);
    // rebalances while there are imbalances
    while (imbalance) {
        rebalance(imbalance);
        updateAllSubHeights(m_root);
        imbalance = lowestSubImbalance(m_root);
    }
}

// updates the height of a bot
void Swarm::updateHeight(Robot* aBot) {
    // if aBot exists
    if (aBot) {
        Robot* right = aBot->getRight();
        Robot* left = aBot->getLeft();
        int height = 0;
        if (left) {
            // if both left and right exist
            if (right) {
                // set the height to be the heigher height +1
                height = (left->getHeight() > right->getHeight())
                             ? (left->getHeight() + 1)
                             : (right->getHeight() + 1);
            } else {
                // if only left exists
                height = left->getHeight() + 1;
            }

        } else if (aBot->getRight()) {
            // if only right exists
            height = right->getHeight() + 1;
        }
        aBot->setHeight(height);
    }
}

// recursiveley updates all of the heights under Node
void Swarm::updateAllSubHeights(Robot* Node) {
    if (Node) {
        // postorder
        updateAllSubHeights(Node->getLeft());
        updateAllSubHeights(Node->getRight());
        updateHeight(Node);
    }
}

// removes all of the dead bots recursiveley
void Swarm::traverseAndRemoveDead(Robot* aBot) {
    if (aBot) {
        // postorder traversal
        traverseAndRemoveDead(aBot->getLeft());
        traverseAndRemoveDead(aBot->getRight());
        if (aBot->getState() == DEAD) {
            remove(aBot->getID());
        }
    }
}

// checks for an imbalance at aBot
int Swarm::checkImbalance(Robot* aBot) {
    if (!aBot) {
        return 0;
    }
    Robot* right = aBot->getRight();
    Robot* left = aBot->getLeft();
    // initialized the heights to -1 for empty nodes
    int rightHeight = -1;
    int leftHeight = -1;
    // gets existing heights
    if (left) {
        leftHeight = left->getHeight();
    }
    if (right) {
        rightHeight = right->getHeight();
    }
    // returns difference
    return rightHeight - leftHeight;
}

// recursively finds the lowest sub imbalance
Robot* Swarm::lowestSubImbalance(Robot* Node) {
    if (Node) {
        // post order
        Robot* left = lowestSubImbalance(Node->getLeft());
        Robot* right = lowestSubImbalance(Node->getRight());
        // return either if they exist
        if (left) {
            return left;
        }
        if (right) {
            return right;
        }
        // return if there is an imbalance else mnll
        int imbalance = checkImbalance(Node);
        if (imbalance > 1 || imbalance < -1) {
            return Node;
        } else {
            return nullptr;
        }
    } else {
        return nullptr;
    }
}

Robot* Swarm::rebalance(Robot* aBot) {
    const string POSSIBLE_ROTATIONS[2] = {"LEFT", "RIGHT"};
    const int NUM_ROTATIONS = 2;
    const int NUM_ROBOTS = 3;
    // stores directions to last node from first node in the trinode
    // restructure
    string rotations[NUM_ROTATIONS] = {"", ""};
    // stores all of the nodes
    Robot* robots[NUM_ROBOTS] = {aBot, nullptr, nullptr};
    int imbalance = checkImbalance(aBot);
    if (imbalance <= 1 && imbalance >= -1) {
        // return itself back if it is alredady balanced
        return aBot;
    }
    // adds the next robot to robots based on the last robot
    for (int i = 1; i < 3; i++) {
        if (robots[i - 1]->getLeft()) {
            if (robots[i - 1]->getRight()) {
                // based on heighest height
                if (robots[i - 1]->getLeft()->getHeight() >
                    robots[i - 1]->getRight()->getHeight()) {
                    // adds the direction it took
                    rotations[i - 1] = POSSIBLE_ROTATIONS[0];
                    robots[i] = robots[i - 1]->getLeft();
                } else {
                    rotations[i - 1] = POSSIBLE_ROTATIONS[1];
                    robots[i] = robots[i - 1]->getRight();
                }
            } else {
                rotations[i - 1] = POSSIBLE_ROTATIONS[0];
                robots[i] = robots[i - 1]->getLeft();
            }
        } else {
            rotations[i - 1] = POSSIBLE_ROTATIONS[1];
            robots[i] = robots[i - 1]->getRight();
        }
    }
    // if they are the same direction only LL or Rr rotation
    if (rotations[0] == rotations[1]) {
        if (rotations[0] == POSSIBLE_ROTATIONS[0]) {
            rightRotate(aBot);
        } else {
            leftRotate(aBot);
        }
        return aBot;
    }
    // different rotation cases
    if (rotations[0] == POSSIBLE_ROTATIONS[0]) {
        leftRotate(robots[1]);
    } else {
        rightRotate(robots[1]);
    }
    if (rotations[1] == POSSIBLE_ROTATIONS[0]) {
        leftRotate(robots[0]);
    } else {
        rightRotate(robots[0]);
    }
    return aBot;
}

// returns the parent of the robot with an id equal to the parameter
Robot* Swarm::getParent(int id) {
    Robot* current = m_root;
    Robot* previous = nullptr;
    while (current) {
        if (current->getID() == id) {
            return previous;
        }
        previous = current;
        if (current->getID() > id) {
            current = current->getLeft();
        } else {
            current = current->getRight();
        }
    }
    return nullptr;
}
// rotates right about aBot

void Swarm::rightRotate(Robot* aBot) {
    Robot* leftBot = aBot->getLeft();
    Robot* parent = getParent(aBot->getID());
    Robot* leftRightBot = leftBot->getRight();
    if (parent == nullptr) {
        m_root = leftBot;
    } else if (parent->getRight() == aBot) {
        parent->setRight(leftBot);
    } else {
        parent->setLeft(leftBot);
    }
    leftBot->setRight(aBot);
    aBot->setLeft(leftRightBot);
}
// rotates light about aBot

void Swarm::leftRotate(Robot* aBot) {
    Robot* rightBot = aBot->getRight();
    Robot* parent = getParent(aBot->getID());
    Robot* rightLeftBot = rightBot->getLeft();
    if (parent == nullptr) {
        m_root = rightBot;
    } else if (parent->getRight() == aBot) {
        parent->setRight(rightBot);
    } else {
        parent->setLeft(rightBot);
    }
    rightBot->setLeft(aBot);
    aBot->setRight(rightLeftBot);
}
// prints the robots in order
void Swarm::listRobots() const { printInOrder(m_root); }

// prints the robots in order in a formatted method
Robot* Swarm::printInOrder(Robot* aBot) const {
    const string ROBOTYPE_STRING[5] = {"BIRD", "DRONE", "REPTILE", "SUB",
                                       "QUADRUPED"};
    const string STATE_STRING[2] = {"ALIVE", "DEAD"};
    if (!aBot) {
        return nullptr;
    }
    printInOrder(aBot->getLeft());
    cout << aBot->getID() << ":" << STATE_STRING[aBot->getState()] << ":"
         << ROBOTYPE_STRING[aBot->getType()] << endl;
    printInOrder(aBot->getRight());
    return aBot;
}
// sets the state of a robot returns true if sucessful otherwise false
bool Swarm::setState(int id, STATE state) {
    Robot* bot = getBot(id);
    if (bot) {
        bot->setState(state);
        return true;
    }
    return false;
}

// remoes all of the read bots
void Swarm::removeDead() { traverseAndRemoveDead(m_root); }

// returns true is a botexists in the tree
bool Swarm::findBot(int id) const {
    Robot* current = m_root;
    while (current) {
        if (current->getID() == id) {
            return true;
        }
        if (current->getID() > id) {
            current = current->getLeft();
        } else {
            current = current->getRight();
        }
    }
    return false;
}

// returns a bot with ID id
Robot* Swarm::getBot(int id) {
    Robot* current = m_root;
    while (current) {
        if (current->getID() == id) {
            return current;
        }
        if (current->getID() > id) {
            current = current->getLeft();
        } else {
            current = current->getRight();
        }
    }
    return nullptr;
}

// dumps the tree using dump
void Swarm::dumpTree() const { dump(m_root); }

// dumps th eree
void Swarm::dump(Robot* aBot) const {
    if (aBot != nullptr) {
        cout << "(";
        dump(aBot->m_left);  // first visit the left child
        cout << aBot->m_id << ":"
             << aBot->m_height;  // second visit the node itself
        dump(aBot->m_right);     // third visit the right child
        cout << ")";
    }
}