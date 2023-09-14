// a2_test.cpp

#include "Stringlist.h"
#include <cassert>

using namespace std;

int main() {
    Stringlist lst;
    cout << lst << endl; // {}

    lst.insert_back("one");
    lst.insert_back("two");
    lst.insert_back("three");
    cout << lst << endl; // {"one", "two", "three"}

    lst.undo();
    cout << lst << endl; // {"one", "two"}

    lst.undo();
    cout << lst << endl; // {"one"}

    lst.undo();
    cout << lst << endl; // {}
}
