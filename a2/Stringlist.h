// Stringlist.h

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : Haoran Miao
// St.# : 301532189
// Email: hma98@sfu.ca
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//
/////////////////////////////////////////////////////////////////////////

//
// Do not use any other #includes or #pragmas in this file.
//

#pragma once

#include <cassert>
#include <iostream>
#include <string>

using namespace std;


class UndoStack   //undostack used to store the history of the stringlist
{
private:
    struct Node   // stores 1 history of the stringlist
    {
        string* data;  // stores a copy of the array of strings before making any changes
        int size;     //size of the copy of the string array
        Node* next;   // next in the stack
    };

    Node* top;
    int size; // size of the stack

public:
    UndoStack() : top(nullptr), size(0) {}  // constructor for the undostack

    ~UndoStack() {       // destructor
        while (top != nullptr) {
            Node* temp = top;       
            top = top->next;
            delete[] temp->data;
            delete temp;
        }// delete every node in the stack
    }

    void push(const string* arr, int size) {
        Node* newNode = new Node;         // push function for the undo stack
        newNode->data = new string[size];  // always put the stack at the top of the stack
        newNode->size = size;
        newNode->next = top;

        for (int i = 0; i < size; ++i) {    // a loop to copy all the information in the string arr to the data stored in the node
            newNode->data[i] = arr[i];
        }

        top = newNode;    //set the new node to top
        ++size; // increase the size of the stack
    }

    void pop() {
        assert(top != nullptr);   // check if the stack is empty

        Node* temp = top;    
        top = top->next;
        delete[] temp->data;   // remove the node from the stack
        delete temp;

        --size; // decrease the total size of the stack
    }

  string* getTop()  {   // returns a copy of the top element in the undo stack
        return top->data;
    }

    int getTopSize(){     // get how many items are stored in the node when returning
        if (top != nullptr) {    // check size of the 
            return top->size;
        } else {
            return 0;
        }
    }

};


class Stringlist
{
    int cap;     // capacity
    string *arr; // array of strings
    int sz;      // size
    UndoStack undoarr;  // create stack by calling the class

    //
    // Helper function for throwing out_of_range exceptions.
    //
    void bounds_error(const string &s) const
    {
        throw out_of_range("Stringlist::" + s + " index out of bounds");
    }

    //
    // Helper function for checking index bounds.
    //
    void check_bounds(const string &s, int i) const
    {
        if (i < 0 || i >= sz)
            bounds_error(s);
    }

    //
    // Helper function for copying another array of strings.
    //
    void copy(const string *other)
    {
        for (int i = 0; i < sz; i++)
        {
            arr[i] = other[i];
        }
    }

    //
    // Helper function for checking capacity; doubles size of the underlying
    // array if necessary.
    //
    void check_capacity()
    {
        if (sz == cap)
        {
            cap *= 2;
            string *temp = new string[cap];
            for (int i = 0; i < sz; i++)
            {
                temp[i] = arr[i];
            }
            delete[] arr;
            arr = temp;
        }
    }

public:
    //
    // Default constructor: makes an empty StringList.
    //
    Stringlist()
        : cap(10), arr(new string[cap]), sz(0)
    {
    }

    //
    // Copy constructor: makes a copy of the given StringList.
    //
    // Does *not* copy the undo stack, or any undo information from other.
    //
    Stringlist(const Stringlist &other)
        : cap(other.cap), arr(new string[cap]), sz(other.sz)
    {
        copy(other.arr);
    }

    //
    // destructor
    //
    ~Stringlist()
    {
        delete[] arr;

    }

    //
    // Assignment operator: makes a copy of the given StringList.
    //
    // undoable
    //
    // For undoing, when assignment different lists, the undo stack is not
    // copied:
    //
    //    lst1 = lst2; // lst1 undo stack is updated to be able to undo the //
    //    assignment; lst1 does not copy lst2's stack
    //    //
    //    // lst2 is not change in any way
    //
    // Self-assignment is when you assign a list to itself:
    //
    //    lst1 = lst1;
    //
    // In this case, nothing happens to lst1. Nothing is changed. Both its
    // stirng data and undo stack are left as-is.
    //
    Stringlist &operator=(const Stringlist &other)
    {
        if (this != &other)
        {
            // Push a copy of the current array to the undo stack
            undoarr.push(arr, sz);
            delete[] arr;
            cap = other.capacity();
            arr = new string[cap];
            sz = other.size();
            copy(other.arr);
        }
        return *this;
    }

    //
    // Returns the number of strings in the list.
    //
    int size() const { return sz; }

    //
    // Returns true if the list is empty, false otherwise.
    //
    bool empty() const { return size() == 0; }

    //
    // Returns the capacity of the list, i.e. the size of the underlying array.
    //
    int capacity() const { return cap; }

    //
    // Returns the string at the given index.
    //
    string get(int index) const
    {
        check_bounds("get", index);
        return arr[index];
    }

    //
    // Returns the index of the first occurrence of s in the list, or -1 if s is
    // not in the lst.
    //
    int index_of(const string &s) const
    {
        for (int i = 0; i < sz; i++)
        {
            if (arr[i] == s)
            {
                return i;
            }
        }
        return -1;
    }

    //
    // Returns true if s is in the list, false otherwise.
    //
    bool contains(const string &s) const
    {
        return index_of(s) != -1;
    }

    //
    // Returns a string representation of the list.
    //
    string to_string() const
    {
        string result = "{";
        for (int i = 0; i < size(); i++)
        {
            if (i > 0)
                result += ", ";
            result += "\"" + get(i) + "\"";
        }
        return result + "}";
    }

    //
    // Sets the string at the given index.
    //
    // undoable
    //
    void set(int index, string value)
    {
        // Push a copy of the current array to the undo stack
        undoarr.push(arr, sz);
        check_bounds("set", index);
        arr[index] = value;
    }

    //
    // Insert s before index; if necessary, the capacity of the underlying array
    // is doubled.
    //
    // undoable
    //
    void insert_before(int index, const string &s)
    {
        // Push a copy of the current array to the undo stack
        undoarr.push(arr, sz);
        if (index < 0 || index > sz) // allows insert at end, i == sz
            bounds_error("insert_before");
        check_capacity();

        for (int i = sz; i > index; i--)
        {
            arr[i] = arr[i - 1];
        }
        arr[index] = s;
        sz++;
    }

    //
    // Appends s to the end of the list; if necessary, the capacity of the
    // underlying array is doubled.
    //
    // undoable
    //
    void insert_back(const string &s)
    {
        // Push a copy of the current array to the undo stack
        undoarr.push(arr, sz);
        insert_before(size(), s);
    }

    //
    // Inserts s at the front of the list; if necessary, the capacity of the
    // underlying array is doubled.
    //
    // undoable
    //
    void insert_front(const string &s)
    {
        // Push a copy of the current array to the undo stack
        undoarr.push(arr, sz);
        insert_before(0, s);
    }

    //
    // Removes the string at the given index; doesn't change the capacity.
    //
    // undoable
    //
    void remove_at(int index)
    {
        // Push a copy of the current array to the undo stack
        undoarr.push(arr, sz);
        check_bounds("remove_at", index);
        for (int i = index; i < sz - 1; i++)
        {
            arr[i] = arr[i + 1];
        }
        sz--;
    }

    //
    // Removes all strings from the list; doesn't change the capacity.
    //
    // undoable
    //
    void remove_all()
    {
        // Push a copy of the current array to the undo stack
        undoarr.push(arr, sz);
        while (sz > 0)
        {
            remove_at(sz - 1);
        }
    }

    //
    // Removes the first occurrence of s in the list, and returns true. If s is
    // nowhere in the list, nothing is removed and false is returned.
    //
    // undoable
    //
    bool remove_first(const string &s)
    {
        // Push a copy of the current array to the undo stack
        undoarr.push(arr, sz);
        int index = index_of(s);
        if (index == -1)
            return false;
        remove_at(index);
        return true;
    }

    // //
    // // Removes all occurrences of s in the list, and returns the number of items
    // // removed.
    // //
    // // undoable
    // //
    // int remove_all(const string &s)
    // {
    //     int count = 0;
    //     for (int i = 0; i < sz; i++)
    //     {
    //         if (arr[i] == s)
    //         {
    //             remove_at(i);
    //             count++;
    //             i--;
    //         }
    //     }
    //     return count;
    // }

    //
    // Undoes the last operation that modified the list. Returns true if a
    // change was undone.
    //
    // If there is nothing to undo, does nothing and returns false.
    //
    bool undo()
{
    string* previousArr = undoarr.getTop();

    if (previousArr != nullptr)
    {
        delete[] arr;       //delete the current arr
        sz = 0;
        cap = undoarr.getTopSize();  //get the size of the previous array
        arr = new string[cap];      // memory allocation for the history array    

        // Copy the previous array to the current array
        for (int i = 0; i < cap; i++)
        {
            arr[i] = previousArr[i];     // copy each node
            if (!previousArr[i].empty())
                sz++;
        }

        undoarr.pop();  // delete the node from the stack

        return true;
    }

    return false;
}
}; // class Stringlist


//
// Prints list to in the format {"a", "b", "c"}.
//
ostream &operator<<(ostream &os, const Stringlist &lst)
{
    return os << lst.to_string();
}

//
// Returns true if the two lists are equal, false otherwise.
//
// Does *not* consider any undo information when comparing two Stringlists. All
// that matters is that they have the same strings in the same order.
//
bool operator==(const Stringlist &a, const Stringlist &b)
{
    if (a.size() != b.size())
    {
        return false;
    }
    for (int i = 0; i < a.size(); i++)
    {
        if (a.get(i) != b.get(i))
        {
            return false;
        }
    }
    return true;
}

//
// Returns true if the two lists are not equal, false otherwise.
//
// Does *not* consider any undo information when comparing two Stringlists.
//
bool operator!=(const Stringlist &a, const Stringlist &b)
{
    return !(a == b);
}
