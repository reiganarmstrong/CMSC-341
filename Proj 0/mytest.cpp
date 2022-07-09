/*****************************************
** File:    mytest.cpp
** Project: CMSC 341 Project 0, Fall 2021
** Author:  Reagan Armstrong
** Date:    9/14/2021
** Section: 03
** E-mail:  reagana1@gl.umbc.edu
**
** This has the Tester class functions and prototypes that
** test the Hist class
**
**
**
***********************************************/
#include "hist.h"

class Tester {  // Tester class to implement test functions
   public:
    // Name: testCopyConstructor()
    // Description: Tests if the copy constructor works
    // Precondition: histogram exists
    // Postcondition: Returns whether or not copy constructor works
    bool testCopyConstructor(const Hist& histogram);
    // Name: measureSimulationTime()
    // Description: measures and prints simulate runtime
    // Precondition: A valid number of trials, number of item and number of
    // students Postconditions: prints the time it takes to simulate for
    // multiple trials (shows linear growth)
    void measureSimulationTime(int numTrials, int items, int students);
    // Name: measureCopyConstructorTime()
    // Description: neasures and prnts copy constructor runtime
    // Precodition: A valid number of trials and items
    // Postcondtion: Prints the time it takes for the copy constructor to
    // execute for multipke trials (shows linear growth)
    void measureCopyConstructorTime(int numTrials, int items);
    // Name: testEdgeCases()
    // Description: Tests edge cases
    // Preconditions: None
    // Postcoditions: Tests edge cases and prints the results
    void testEdgeCases();
    // Name: testErrorCase()
    // Description: Tests error cases
    // Preconditions: None
    // Postcondtion: Tests error case and prints the result
    void testErrorCase();
    // Name: testNormalCase()
    // Description: Tests a normal case
    // Precondition: None
    // Postcondition: Tests Normal case and prints the result
    void testNormalCase();
    // Name: testAssignmentOperator()
    // Description: Tests to see if the assignment operator makes a deep copy
    // Precondition: None
    // Postcondition: Tests and prints if the assignemnt operator makes a deep
    // copy
    bool testAssignmentOperator(const Hist& histogram);
    // Name: testSelfAssignment()
    // Description: Tests to see if there are guards for self assignment
    // Preconditon: None
    // Postcondition: Tests and checks if self assignment is accounted for in
    // overloaded assignment operator
    bool testSelfAssignment();

   private:
    // Name: compareTwoHists()
    // Description: compares two Hist objects to see if they are deep copies
    // Precondition: Two valid Hist objects
    // Postcondition: A bool indicating whether or not the Hist objects are
    // deep copies
    bool compareTwoHists(const Hist& histogram, const Hist& aCopy) const;
};
// main
// the main funtion that runns all of the tests
int main() {
    // creates a tester object
    Tester tester;
    // tests the normal case
    tester.testNormalCase();
    {
        // dumps a hist object with 2 items and 35 students
        cout << "\nDump of 2 grading items for 35 students:\n" << endl;
        Hist h1(2);
        h1.simulateData(35);
        h1.dump();
    }
    // tests edge cases
    tester.testEdgeCases();
    // test error case
    tester.testErrorCase();
    {
        try {
            // tests the error case when the number of items is less than 1
            cout << "Testing exception case for number of items less than "
                    "1:\n"
                 << endl;
            Hist h1(0);
            h1.simulateData(123);
        } catch (const std::out_of_range& e) {
            // gets and prints the error string
            cout << e.what() << endl;
        }
    }
    {
        // test deep copy, object with many members
        Hist h1(20);           // 20 items, e.g. exams, quizzes, homeworks
        h1.simulateData(400);  // process grades for 400 students
        cout << "\nTest case, Copy Constructor: testing for deep copy with 20 "
                "items and 400 students:\n"
             << endl;
        //  tests copy constructor
        if (tester.testCopyConstructor(h1))
            cout << "Copy constructor passed!" << endl;
        else
            cout << "Copy constructor failed!" << endl;
    }
    {
        // Measuring the efficiency of insertion functionality
        cout << "\nMeasuring the efficiency of insertion functionality:\n"
             << endl;
        int M = 4;      // number of trials
        int items = 2;  // number of items, e.g. exams
        int students =
            100000;  // original input size, number of students taking tests
        tester.measureSimulationTime(M, items, students);
    }
    {
        cout << "\nMeasuring the efficiency of Copy Constructor:\n" << endl;
        int M = 5;           // number of trials
        int items = 100000;  // number of items, e.g. exams
        tester.measureCopyConstructorTime(M, items);
    }
    cout << endl;
    {
        // edge case
        Hist h1(0);
        // edge case
        Hist h2(1);
        h2.simulateData(1);
        // normal case
        Hist h3(100);
        h3.simulateData(22);
        // tests assignment operator
        cout << "\nTesting Assignment Opertator...\n" << endl;
        // makes sure assinment operator both makes deep copies and checks
        // for guards against self assignment
        if (tester.testAssignmentOperator(h1) &&
            tester.testAssignmentOperator(h2) &&
            tester.testAssignmentOperator(h3) && tester.testSelfAssignment()) {
            cout << "Assignment Operator Passed!" << endl;
        } else {
            cout << "Assignment Operator Failed. :(" << endl;
        }
    }

    return 0;
}

// testCopyConstructor
// tests that the copy constructor makes a deep copy
bool Tester::testCopyConstructor(const Hist& histogram) {
    // uses copy constructor to make a hist object
    Hist aCopy(histogram);
    // finds if the two hist objects are deep copies
    return compareTwoHists(histogram, aCopy);
}

// meaureSimulationTime
// finds how long simulation takes and verifies linear growth
void Tester::measureSimulationTime(int numTrials, int items, int students) {
    // Measuring the efficiency of data simulation algorithm with system clock
    // ticks Clock ticks are units of time of a constant but system-specific
    // length,
    //  as those returned by function clock().
    // Dividing a count of clock ticks by CLOCKS_PER_SEC yields the number of
    // seconds.
    const int a = 2;      // scaling factor for input size, everytime it will
                          // double
    double T = 0.0;       // to store running times
    clock_t start, stop;  // stores the clock ticks while running the program
    for (int k = 0; k < numTrials - 1; k++) {
        Hist h1(items);
        start = clock();
        h1.simulateData(
            students);  // the algorithm to be analyzed for efficiency
        stop = clock();
        T = stop - start;  // number of clock ticks the algorithm took
        cout << "Counting " << items * students << " grades took " << T
             << " clock ticks (" << T / CLOCKS_PER_SEC << " seconds)!" << endl;
        students =
            students * a;  // increase number of students by increase factor
    }
}

// meaureCopyConstructorTime
// finds how long copy constructor takes and verifies linear growth
void Tester::measureCopyConstructorTime(int numTrials, int items) {
    // Measuring the efficiency of data simulation algorithm with system clock
    // ticks Clock ticks are units of time of a constant but system-specific
    // length,
    //  as those returned by function clock().
    // Dividing a count of clock ticks by CLOCKS_PER_SEC yields the number of
    // seconds.
    const int a = 2;      // scaling factor for input size, everytime it will
                          // double
    double T = 0.0;       // to store running times
    clock_t start, stop;  // stores the clock ticks while running the program
    for (int k = 0; k < numTrials - 1; k++) {
        Hist h1(items);
        start = clock();
        Hist h2(h1);  // the algorithm to be analyzed for efficiency
        stop = clock();
        T = stop - start;  // number of clock ticks the algorithm took
        cout << "Copying " << items << " items took " << T << " clock ticks ("
             << T / CLOCKS_PER_SEC << " seconds)!" << endl;
        items = items * a;  // increase number of students by increase factor
    }
}

// testErrorCase
// tests the error case -1 (<0 items)
void Tester::testErrorCase() {
    // trys to create a hist object with less than 0 items
    cout << "Trying to create a Hist object with -1 items...\n" << endl;
    try {
        Hist h1(-1);
        cout << "Sucess!" << endl;
    } catch (const std::out_of_range& e) {
        // catches the error thrown which should happen because -1 is out of
        // range
        cout << e.what() << endl;
    }
}

// testEdgeCases
// tests edge cases 0 items and 1 items
void Tester::testEdgeCases() {
    // tests an edge case with  0 items
    cout << "Trying to create a Hist object with 0 items...\n" << endl;
    try {
        // creates the hist object with 0 items
        Hist h1(0);
        cout << "Sucess!" << endl;
        cout << "Dump of 0 grading items for 35 students:\n" << endl;
        // dumps h1 if it is created
        h1.simulateData(35);
        h1.dump();
        cout << endl;

    } catch (const std::out_of_range& e) {
        // catches and prints out of range error
        cout << e.what() << endl;
    }
    cout << "Trying to create a Hist object with 1 items...\n" << endl;
    try {
        // creates the hist object with 1 item
        Hist h1(1);
        cout << "Sucess!" << endl;
        cout << "Dump of 1 grading items for 35 students:\n" << endl;
        h1.simulateData(35);
        // dumps h1 if it is created
        h1.dump();
    } catch (const std::out_of_range& e) {
        // catches and prints out of range error
        cout << e.what() << endl;
    }
}

// testNormalcase
//  tests that the normal case works
void Tester::testNormalCase() {
    cout << "Trying to create a Hist object with 100 items...\n" << endl;
    try {
        // trys to create a Hist object with 100 items
        Hist h1(100);
        // if created prints success
        cout << "Sucess!" << endl;
    } catch (const std::out_of_range& e) {
        // catches and prints out of range error
        cout << e.what() << endl;
    }
}

// testAssignmentOperator
// tests that Assignment operator makes a deep copy
bool Tester::testAssignmentOperator(const Hist& histogram) {
    // creates a completely different Hist objects
    Hist aCopy(histogram.m_items + 1);
    aCopy.simulateData(26);
    // uses assignment operator
    aCopy = histogram;
    // returns whether or not they are deep copies
    return compareTwoHists(histogram, aCopy);
}

// testsSelfAssignment
// Tests to see if the overloaded assignment operator guards against self
// assignment
bool Tester::testSelfAssignment() {
    Hist histogram(2);
    histogram.simulateData(10);
    // creates a deep copy of histogram for later comparison
    Hist aCopy(histogram);
    // self assignment
    histogram = histogram;
    // if there was self assignment, histogram would not stay the same as
    // before, so this would return false if self assignment was not guarded
    // against and true if it was
    return compareTwoHists(histogram, aCopy);
}

// compareTwoHists
// compares two Hist objects passed by reference and sees if they are deep
// copies
bool Tester::compareTwoHists(const Hist& histogram, const Hist& aCopy) const {
    // the case of empty objects
    if (histogram.m_items == 0 && aCopy.m_items == 0) return true;
    // the case that number of items is the same and the table pointers are not
    // the same
    else if (histogram.m_items == aCopy.m_items &&
             histogram.m_table != aCopy.m_table) {
        for (int i = 0; i < histogram.m_items; i++) {
            // check whether every Data object is a deep copy or not
            if (histogram.m_table[i] == aCopy.m_table[i])
                // if they are the same objects, then is not a deep copy
                return false;
            if (histogram.m_table[i].m_arraySize !=
                aCopy.m_table[i].m_arraySize)
                // if they have different sized arrays this is not a copy
                return false;
            for (int j = 0; j < histogram.m_table[i].m_arraySize; j++) {
                // checks whether the integer values in m_data in both hist
                // objects are equal or not
                if (histogram.m_table[i].m_data[j] !=
                    aCopy.m_table[i].m_data[j]) {
                    // if they are not the same value then
                    // they are not copies
                    return false;
                }
            }
        }
        return true;
    }
    // everthing else
    else
        return false;
}