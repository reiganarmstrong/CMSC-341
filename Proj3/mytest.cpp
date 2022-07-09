/*****************************************
** File:    mytest.cpp
** Project: CMSC 341 Project 3, Fall 2021
** Author:  Reagan Armstrong
** Date:    11/17/2021
** Section: 03
** E-mail:  reagana1@gl.umbc.edu
**
** Given sample tests for mqueue class as well as the Tester class,
** also used to test mqueue class
**
**
**
**
***********************************************/
// CMSC 341 - Fall 2021 - Project 3

#include "mqueue.h"
using namespace std;
// Priority functions compute an integer priority for an order.  Internal
// computations may be floating point, but must return an integer.
// tester class
class Tester {
   public:
    //    contructor runs all tests
    Tester(prifn_t priorityFun1, prifn_t priorityFun2) {
        m_priorityFn1 = priorityFun1;
        m_priorityFn2 = priorityFun2;
        srand(time(nullptr));
        cout << "Testing Copy Constructor..." << endl;
        // checks copy constructor
        if (testCopyConstructor()) {
            cout << "Copy Constructor Passed!\n" << endl;
        } else {
            cout << "Copy Constructor Failed!\n" << endl;
        }
        cout << "Testing Assignment Operator..." << endl;
        // checks aassignment operator
        if (testAssignmentOperator()) {
            cout << "Assignment Operator Passed!\n" << endl;
        } else {
            cout << "Assignment Operator Failed!\n" << endl;
        }
        // tests runtime
        testRuntime();
        // tests nomal cases (given in driver to us)
        testNormalCases();
        cout << "Testing merge queues with different priority functions..."
             << endl;
        // tests if we can't merge queues with different priority functions
        if (testMergeWithDifferentFunc()) {
            cout << "Test Passed!\n" << endl;
        } else {
            cout << "Test Failed!\n" << endl;
        }
        cout << "Testing Dequeue Empty queue..." << endl;
        // tests to see if we can't dequeue an empty queue
        if (testDequeueEmptyQueue()) {
            cout << "Test Passed!\n" << endl;
        } else {
            cout << "Test Failed!\n" << endl;
        }
    }
    // tests if we can't merge queues with different priority functions
    bool testMergeWithDifferentFunc() {
        MQueue first(m_priorityFn1);
        MQueue second(m_priorityFn2);
        // tries to merge if it does then return false if it doesn't then
        // return true
        try {
            first.mergeWithQueue(second);
            return false;
        } catch (domain_error e) {
            return true;
        }
        return true;
    }

    // tries to dequeue and empty queue if it does then return false if it
    // doesn't return true
    bool testDequeueEmptyQueue() {
        MQueue queue(m_priorityFn1);
        try {
            queue.getNextOrder();
            return false;
        } catch (domain_error e) {
            return true;
        }
    }

    // tests the runtime
    void testRuntime() {
        // randomizes all of the parameters for Order
        MQueue tempQueue(m_priorityFn1);
        int material = rand() % 5;
        int quantity = rand() % 5;
        int priority = rand() % 57 + 1;
        string customer = "phil";
        int arrival = rand() % 49 + 1;
        double T = 0.0;
        clock_t startTimer, stopTimer;
        // the removal text to be outputted later
        string removalString = "";
        cout << "Testing Insertions (clock ticks should double as number of "
                "insertions double for log time)"
             << endl;
        // tests 4 iterations
        for (int j = 2; j <= 32; j *= 2) {
            // starts clock before nsertions
            startTimer = clock();
            // inserts 200000*j orders into tempQueue
            for (int i = 0; i < 200000 * j; i++) {
                Order order =
                    Order(customer, priority, material, quantity, arrival);
                tempQueue.insertOrder(order);
                // randomizes Order parameters for next time
                material = rand() % 5;
                quantity = rand() % 5;
                priority = rand() % 57 + 1;
                customer = "phil";
                arrival = rand() % 49 + 1;
            }
            // stops timer
            stopTimer = clock();
            // finds difference
            T = stopTimer - startTimer;
            cout << "Insertion of " << (200000 * j) << " Orders took " << T
                 << " clock ticks (" << T / CLOCKS_PER_SEC << " seconds)!"
                 << endl;
            // same process but for removal
            startTimer = clock();
            tempQueue.clear();
            stopTimer = clock();
            T = stopTimer - startTimer;
            // adds to string instead of console
            removalString += "Removal of " + to_string(200000 * j) +
                             " Orders took " + to_string(T) +
                             " clock ticks (" + to_string(T / CLOCKS_PER_SEC) +
                             " seconds)!\n";
        }
        cout << "\nTesting Removals (clock ticks should double as number of "
                "insertions double for log time)"
             << endl;
        //  outputs generated string
        cout << removalString << endl;
    }

    // tests copy constructor for deep copies
    bool testCopyConstructor() {
        // creates empty queues
        MQueue emptyQueue(m_priorityFn1);
        MQueue normalQueueFunc1(m_priorityFn1);
        MQueue normalQueueDuplicateFunc1(m_priorityFn1);
        MQueue normalQueueFunc2(m_priorityFn2);
        MQueue normalQueueDuplicateFunc2(m_priorityFn2);
        MQueue otherNormalQueueFunc1(m_priorityFn1);
        MQueue otherNormalQueueDuplicateFunc1(m_priorityFn1);
        // randomizes Order parameters
        int material = rand() % 5;
        int quantity = rand() % 5;
        int priority = rand() % 57 + 1;
        string customer = "phil";
        int arrival = rand() % 49 + 1;
        // adds a random number of orders to normalQueueFunc1 and
        // normalQueueDuplicateFunc1
        for (int i = 0; i < rand() % 50 + 15; i++) {
            Order order =
                Order(customer, priority, material, quantity, arrival);
            normalQueueFunc1.insertOrder(order);
            normalQueueDuplicateFunc1.insertOrder(order);
            // randomizes Order parameters
            material = rand() % 5;
            quantity = rand() % 5;
            priority = rand() % 57 + 1;
            customer = "phil";
            arrival = rand() % 49 + 1;
        }
        // adds a random number of orders to normalQueueFunc2 and
        // normalQueueDuplicateFunc2
        for (int i = 0; i < rand() % 50 + 15; i++) {
            Order order =
                Order(customer, priority, material, quantity, arrival);
            normalQueueFunc2.insertOrder(order);
            normalQueueDuplicateFunc2.insertOrder(order);
            // randomizes Order parameters
            material = rand() % 5;
            quantity = rand() % 5;
            priority = rand() % 57 + 1;
            customer = "phil";
            arrival = rand() % 49 + 1;
        }
        // adds a random number of orders to otherNormalQueueFunc1 and
        // otherNormalQueueDuplicateFunc1
        for (int i = 0; i < rand() % 50 + 15; i++) {
            Order order =
                Order(customer, priority, material, quantity, arrival);
            otherNormalQueueFunc1.insertOrder(order);
            otherNormalQueueDuplicateFunc1.insertOrder(order);
            // randomizes Order parameters
            material = rand() % 5;
            quantity = rand() % 5;
            priority = rand() % 57 + 1;
            customer = "phil";
            arrival = rand() % 49 + 1;
        }
        // copys a normal Queue
        MQueue copyNormalQueueDuplicateFunc1 = MQueue(normalQueueFunc1);
        // clears
        normalQueueFunc1.clear();
        // checks if the copy is the same as the duplicate of the MQueue it
        // copied from after that MQueue got deleted
        if (!equalMQueues(copyNormalQueueDuplicateFunc1,
                          normalQueueDuplicateFunc1)) {
            return false;
        }
        normalQueueFunc1 = normalQueueDuplicateFunc1;
        // copies empty
        MQueue copyEmptyQueue = MQueue(emptyQueue);
        // checks if the copy is empy
        if (!equalMQueues(copyEmptyQueue, emptyQueue)) {
            return false;
        }
        // copys another normal queue
        MQueue copyOtherNormalQueueFunc1 = MQueue(otherNormalQueueFunc1);
        otherNormalQueueFunc1.clear();
        // checks if the copy is the same as the duplicate of the MQueue it
        // copied from after that MQueue got deleted
        if (!equalMQueues(copyOtherNormalQueueFunc1,
                          otherNormalQueueDuplicateFunc1)) {
            return false;
        }
        otherNormalQueueFunc1 = otherNormalQueueDuplicateFunc1;
        // copys another normal queue
        MQueue copyNormalQueueFunc2 = MQueue(normalQueueFunc2);
        normalQueueFunc2.clear();
        // checks if the copy is the same as the duplicate of the MQueue it
        // copied from after that MQueue got deleted
        if (!equalMQueues(copyNormalQueueFunc2, normalQueueDuplicateFunc2)) {
            return false;
        }
        normalQueueFunc1 = normalQueueDuplicateFunc1;
        normalQueueFunc2 = normalQueueDuplicateFunc2;
        return true;
    }
    bool testAssignmentOperator() {
        // creates empty queues
        MQueue emptyQueue(m_priorityFn1);
        MQueue normalQueueFunc1(m_priorityFn1);
        MQueue normalQueueDuplicateFunc1(m_priorityFn1);
        MQueue normalQueueFunc2(m_priorityFn2);
        MQueue normalQueueDuplicateFunc2(m_priorityFn2);
        MQueue otherNormalQueueFunc1(m_priorityFn1);
        MQueue otherNormalQueueDuplicateFunc1(m_priorityFn1);
        // randomizes Order parameters
        int material = rand() % 5;
        int quantity = rand() % 5;
        int priority = rand() % 57 + 1;
        string customer = "phil";
        int arrival = rand() % 49 + 1;
        // adds a random number of orders to normalQueueFunc1 and
        // normalQueueDuplicateFunc1
        for (int i = 0; i < rand() % 50 + 15; i++) {
            Order order =
                Order(customer, priority, material, quantity, arrival);
            normalQueueFunc1.insertOrder(order);
            normalQueueDuplicateFunc1.insertOrder(order);
            // randomizes Order parameters
            material = rand() % 5;
            quantity = rand() % 5;
            priority = rand() % 57 + 1;
            customer = "phil";
            arrival = rand() % 49 + 1;
        }
        // adds a random number of orders to normalQueueFunc2 and
        // normalQueueDuplicateFunc2
        for (int i = 0; i < rand() % 50 + 15; i++) {
            Order order =
                Order(customer, priority, material, quantity, arrival);
            normalQueueFunc2.insertOrder(order);
            normalQueueDuplicateFunc2.insertOrder(order);
            // randomizes Order parameters
            material = rand() % 5;
            quantity = rand() % 5;
            priority = rand() % 57 + 1;
            customer = "phil";
            arrival = rand() % 49 + 1;
        }
        // adds a random number of orders to otherNormalQueueFunc1 and
        // otherNormalQueueDuplicateFunc1
        for (int i = 0; i < rand() % 50 + 15; i++) {
            Order order =
                Order(customer, priority, material, quantity, arrival);
            otherNormalQueueFunc1.insertOrder(order);
            otherNormalQueueDuplicateFunc1.insertOrder(order);
            // randomizes Order parameters
            material = rand() % 5;
            quantity = rand() % 5;
            priority = rand() % 57 + 1;
            customer = "phil";
            arrival = rand() % 49 + 1;
        }
        // self assignment check
        normalQueueFunc1 = normalQueueFunc1;
        // checks that the normalQueueFunc1 did not change
        if (!equalMQueues(normalQueueFunc1, normalQueueDuplicateFunc1)) {
            return false;
        }
        normalQueueFunc1 = normalQueueDuplicateFunc1;
        // empty queue assigned to non empty queue
        emptyQueue = normalQueueFunc1;
        normalQueueFunc1.clear();
        // checks if the MQueue is still equivalent to the MQueue it was
        // assigned to by checking its dupulicate, while the original MQueue it
        // was assigned to is cleared (shows deep copy)
        if (!equalMQueues(normalQueueDuplicateFunc1, emptyQueue)) {
            return false;
        }
        normalQueueFunc1 = normalQueueDuplicateFunc1;
        emptyQueue.clear();
        // normal assigned to empty
        normalQueueFunc2 = emptyQueue;
        // checks if normalQueueFunc2 is empty
        if (!equalMQueues(normalQueueFunc2, emptyQueue)) {
            return false;
        }
        normalQueueFunc2 = normalQueueDuplicateFunc2;
        // normal assigned to other normal with same priority function
        normalQueueFunc1 = otherNormalQueueFunc1;
        otherNormalQueueFunc1.clear();
        // checks if the MQueue is still equivalent to the MQueue it was
        // assigned to by checking its dupulicate, while the original MQueue it
        // was assigned to is cleared (shows deep copy)
        if (!equalMQueues(normalQueueFunc1, otherNormalQueueDuplicateFunc1)) {
            return false;
        }
        normalQueueFunc1 = normalQueueDuplicateFunc1;
        otherNormalQueueFunc1 = otherNormalQueueDuplicateFunc1;
        // normal assigned to normal with different priority function
        normalQueueFunc1 = normalQueueFunc2;
        normalQueueFunc2.clear();
        // checks if the MQueue is still equivalent to the MQueue it was
        // assigned to by checking its dupulicate, while the original MQueue it
        // was assigned to is cleared (shows deep copy)
        if (!equalMQueues(normalQueueFunc1, normalQueueDuplicateFunc2)) {
            return false;
        }
        normalQueueFunc1 = normalQueueDuplicateFunc1;
        normalQueueFunc2 = normalQueueDuplicateFunc2;
        return true;
    }
    // checks if a heap satisfies a Priority function property
    bool satisfiesPriorQueueProp(const Node *start, const prifn_t priorFunc) {
        // base case start is false
        if (!start) {
            return true;
        }
        int priority = priorFunc(start->m_order);
        bool leftSatisfies = true;
        bool rightSatisfies = true;
        if (start->m_left) {
            // if this priority is smaller than it's left make check the rest
            // of the left sub heap
            if (priority <= priorFunc(start->m_left->m_order)) {
                leftSatisfies =
                    satisfiesPriorQueueProp(start->m_left, priorFunc);
            } else {
                return false;
            }
        }
        if (start->m_right) {
            // if this priority is smaller than it's right make check the rest
            // of the left sub heap
            if (priority <= priorFunc(start->m_right->m_order)) {
                rightSatisfies =
                    satisfiesPriorQueueProp(start->m_right, priorFunc);
            } else {
                return false;
            }
        }
        // checks both sub heap
        return (rightSatisfies && leftSatisfies);
    }

    // recursively checks if two MQueues are equal
    bool equalMQueues(const MQueue &first, const MQueue &second) {
        // checks m_size and m_priorFunc first
        if (first.m_size != second.m_size) {
            return false;
        }
        if (first.m_priorFunc != second.m_priorFunc) {
            return false;
        }
        // checks heaps
        return equalHeaps(first.m_heap, second.m_heap);
    }
    // checks if two heaps are equivalend
    bool equalHeaps(const Node *first, const Node *second) {
        // returns false if they are both not nullptr, true if they are
        if (!first) {
            if (!second) {
                return true;
            }
            return false;
        } else if (!second) {
            return false;
        }
        // returns false if they are not deep copies
        if (first == second) {
            return false;
        }
        // if the two orders Nodes are equal
        if (equalOrders(first->getOrder(), second->getOrder())) {
            // call equalHeaps on the left nodes and right nodes and return
            return (equalHeaps(first->m_left, second->m_left) &&
                    equalHeaps(first->m_right, second->m_right));
        }
        return false;
    }
    // checks if all the data in Orders are equal
    bool equalOrders(const Order &first, const Order &second) {
        if (first.m_arrivalTime == second.m_arrivalTime &&
            first.m_customer == second.m_customer &&
            first.m_material == second.m_material &&
            first.m_overWritten == second.m_overWritten &&
            first.m_quantity == second.m_quantity) {
            return true;
        }
        return false;
    }

    // These are the Normal Cases, given to us in the driver online but I added
    // and expected vs actual on all of the outputs
    void testNormalCases() {
        try {
            // freopen("output.txt", "w", stdout);
            cout << "Test creating a queue" << endl;
            MQueue queue1(m_priorityFn1);
            cout << "Test Passed!" << endl;
            MQueue queue2(m_priorityFn1);
            // Create some test orders and insert them into the queue
            // Order(string customer, int priority, int material, int quantity,
            // int arrival)
            Order order1("Ford", 1, 1, 1, 20);
            Order order2("Honda", 5, 1, 2, 21);
            Order order3("Toyota", 2, 0, 0, 22);
            Order order4("GM", 4, 3, 2, 23);

            queue1.insertOrder(order3);
            queue1.insertOrder(order1);
            queue1.insertOrder(order2);
            queue1.insertOrder(order4);

            std::cout
                << "\nqueue1 dump after inserting 4 orders (Toyota, Honda, "
                   "Ford, GM):"
                << std::endl;
            cout << "\nExpected output:\n"
                 << "(((28:GM)22:Toyota)22:Ford(24:Honda))" << endl;
            cout << "\nActual Output:" << endl;
            queue1.dump();

            Order order5("Ferrari", 6, 3, 2, 1);
            Order order6("Hyundai", 3, 0, 0, 10);

            queue2.insertOrder(order5);
            queue2.insertOrder(order6);

            std::cout << "\nqueue2 dump after inserting 2 students (Ferrari, "
                         "Hyundai):"
                      << std::endl;
            queue2.dump();
            cout << "\nExpected output:\n"
                 << "((10:Hyundai)6:Ferrari)" << endl;
            cout << "\nActual Output:" << endl;
            queue1.mergeWithQueue(queue2);

            // Print the queue, change priority function, print again, change
            // again, print again.  Changing the priority function should cause
            // the heap to be rebuilt.

            std::cout << std::endl
                      << "Contents of queue1 after merging with queue2:\n";
            string output =
                "[6] Order: Ferrari, PM priority: 6, quantity: 50000, "
                "material "
                "availability: 25%, arrival time: 1\n[22] Order: Ford, PM "
                "priority: 1, quantity: 75000, material availability: 75%, "
                "arrival time: 20\n[22] Order: Toyota, PM priority: 2, "
                "quantity: "
                "100000, material availability: 100%, arrival time: 22\n[28] "
                "Order: GM, PM priority: 4, quantity: 50000, material "
                "availability: 25%, arrival time: 23\n[24] Order: Honda, PM "
                "priority: 5, quantity: 50000, material availability: 75%, "
                "arrival time: 21\n[10] Order: Hyundai, PM priority: 3, "
                "quantity: "
                "100000, material availability: 100%, arrival time: 10";
            cout << "\nExpected output:\n" << output << endl;
            cout << "\nActual Output:" << endl;
            queue1.printOrderQueue();
            std::cout << "\nDump queue1 after merging with queue2:\n";
            cout << "\nExpected output:\n"
                 << "((((28:GM)22:Toyota)22:Ford(24:Honda))6:Ferrari(10:"
                    "Hyundai))"
                 << endl;
            cout << "\nActual Output:" << endl;
            queue1.dump();
            queue1.setPriorityFn(m_priorityFn2);

            std::cout
                << std::endl
                << "Contents of queue1 after changing the priority function "
                   "to m_priorityFn2:\n";
            output =
                "[1] Order: Ford, PM priority: 1, quantity: 75000, material "
                "availability: 75%, arrival time: 20\n[2] Order: Toyota, PM "
                "priority: 2, quantity: 100000, material availability: 100%, "
                "arrival time: 22\n[4] Order: GM, PM priority: 4, quantity: "
                "50000, material availability: 25%, arrival time: 23\n[3] "
                "Order: "
                "Hyundai, PM priority: 3, quantity: 100000, material "
                "availability: 100%, arrival time: 10\n[5] Order: Honda, PM "
                "priority: 5, quantity: 50000, material availability: 75%, "
                "arrival time: 21\n[6] Order: Ferrari, PM priority: 6, "
                "quantity: "
                "50000, material availability: 25%, arrival time: 1\n";
            cout << "\nExpected output:\n" << output << endl;
            cout << "\nActual Output:" << endl;
            queue1.printOrderQueue();
            std::cout
                << "\nDump queue1 after changing the priority function to "
                   "m_priorityFn2:\n";
            cout << "\nExpected output:\n"
                 << "(((4:GM)2:Toyota)1:Ford((5:Honda)3:Hyundai(6:Ferrari)))"
                 << endl;
            cout << "\nActual Output:" << endl;
            queue1.dump();

            queue1.setPriorityFn(m_priorityFn1);

            std::cout
                << std::endl
                << "Contents of queue1 after changing the priority function "
                   "back to m_priorityFn1:\n";
            output =
                "[6] Order: Ferrari, PM priority: 6, quantity: 50000, "
                "material "
                "availability: 25%, arrival time: 1\n[10] Order: Hyundai, PM "
                "priority: 3, quantity: 100000, material availability: 100%, "
                "arrival time: 10\n[22] Order: Toyota, PM priority: 2, "
                "quantity: "
                "100000, material availability: 100%, arrival time: 22\n[24] "
                "Order: Honda, PM priority: 5, quantity: 50000, material "
                "availability: 75%, arrival time: 21\n[22] Order: Ford, PM "
                "priority: 1, quantity: 75000, material availability: 75%, "
                "arrival time: 20\n[28] Order: GM, PM priority: 4, quantity: "
                "50000, material availability: 25%, arrival time: 23\n";
            cout << "\nExpected output:\n" << output << endl;
            cout << "\nActual Output:" << endl;
            queue1.printOrderQueue();
            std::cout
                << "\nDump queue1 after changing the priority function back "
                   "to m_priorityFn1:\n";
            cout << "\nExpected output:\n"
                 << "((((24:Honda)22:Toyota(22:Ford))10:Hyundai(28:GM))6:"
                    "Ferrari)"
                 << endl;
            cout << "\nActual Output:" << endl;
            queue1.dump();

            cout << "Testing the  priority queue property of queue1..."
                 << endl;
            // checks priority queue property
            if (satisfiesPriorQueueProp(queue1.m_heap, queue1.m_priorFunc)) {
                cout << "Test Passed!\n" << endl;
            } else {
                cout << "Test Failed!\n" << endl;
            }
            // Remove and print orders in priority order

            std::cout << "\nFinal queue1 in priority order:\n";
            output =
                "[6] Order: Ferrari, PM priority: 6, quantity: 50000, "
                "material "
                "availability: 25%, arrival time: 1\n[10] Order: Hyundai, PM "
                "priority: 3, quantity: 100000, material availability: 100%, "
                "arrival time: 10\n[22] Order: Toyota, PM priority: 2, "
                "quantity: "
                "100000, material availability: 100%, arrival time: 22\n[22] "
                "Order: Ford, PM priority: 1, quantity: 75000, material "
                "availability: 75%, arrival time: 20\n[24] Order: Honda, PM "
                "priority: 5, quantity: 50000, material availability: 75%, "
                "arrival time: 21\n[28] Order: GM, PM priority: 4, quantity: "
                "50000, material availability: 25%, arrival time: 23\n";
            cout << "\nExpected output:\n" << output << endl;
            cout << "\nActual Output:" << endl;
            while (queue1.numOrders() > 0) {
                Order order = queue1.getNextOrder();
                // this will be printed in the ascending order (sorted)
                std::cout << "[" << m_priorityFn1(order) << "] " << order
                          << std::endl;
            }
        } catch (std::out_of_range &e) {
            cout << e.what() << endl;
        }
        // fclose(stdout);
    }

   private:
    prifn_t m_priorityFn1;
    prifn_t m_priorityFn2;
};
int priorityFn1(const Order &order);
int priorityFn2(const Order &order);

int main() { Tester test(priorityFn1, priorityFn2); }

int priorityFn1(const Order &order) {
    // priority value is determined based on some criteria
    // priority value falls in the range [1-58]
    // the smaller value means the higher priority
    int priority = order.getMaterialAvailability() + order.getQuantity() +
                   order.getArrivalTime();
    return priority;
}

int priorityFn2(const Order &order) {
    // priority is determined by a production manager
    // priority value falls in the range [1-58]
    // the smaller value means the higher priority
    return order.getPMPriority();
}