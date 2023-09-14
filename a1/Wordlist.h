// Wordlist.h

#pragma once

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : Haoran Miao
// St.# : 301542189
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
// Do not use any other #includes
//
#include "Wordlist_base.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Wordlist : public Wordlist_base
{
    //
    // Use this Node to implement the singly-linked list for the word list. You
    // can *add* extra variables/methods/features to this struct if you like,
    // but you must keep its name the same, and also keep the word, count, and
    // next variables as defined.
    //
    struct Node
    {
        string word;
        int count;
        Node *next;
    };

       Node *head; 

public:
    // Default constructor
    Wordlist() : head(nullptr) {}

    // second constructor
    Wordlist(const string &filename) : head(nullptr)
    {
        ifstream file(filename);
        if (file.is_open())
        {
            string word;
            while (file >> word)
            {
                add_word(word);
            }
            file.close();
        }
        else
        {
            cout << "can not open: " << filename << endl;
        }
    }

    // Destructor
    ~Wordlist()
{
    // clear all nodes in the linked list
    Node *current = head;
    while (current != nullptr)
    {
        Node *next = current->next;
        delete current;
        current = next;
    }
    head = nullptr; // Set head to null
}

    int get_count(const string &w) const
    {
        Node *p = head;
        int count = 0;
        while (p != NULL)
        {
            if (p->word == w){
                count = p->count;
                break;
            }else{
                p = p->next;
            }
            
        }
        //used a loop to iterate through all the nodes in the list and check the input word w
        // if matched, return the count. else return 0.
        return count;
    }



    // basic iteration to see how many nodes there is in the list
    int num_different_words() const
    {
        int count = 0;
        Node *ptr = head;
        while (ptr != NULL)
        {
            count++;
            ptr = ptr->next;
        }
        return count;
    }


    // going through all the nodes and then adding the count for each node to count
    int total_words() const
    {
        int count = 0;
        Node *ptr = head;
        while (ptr != NULL)
        {
            count += ptr->count;
            ptr = ptr->next;
        }
        return count;
    }

    //check if each word is sorted by comparing using ASCII table
    bool is_sorted() const
    {
        Node *p = head;
        while (p != NULL && p->next != NULL)
        {
            if (p->word > p->next->word)
                return false;
            p = p->next;
        }
        return true;
    }


    //go through each node, if count is larger than before, change the frequentcount to be the same as the count for that node.
    string most_frequent() const
    {
        if (head == nullptr)
            return ""; 

        string frequentword = head->word;
        int frequentcount = head->count;
        Node *ptr = head->next;

        while (ptr != nullptr)
        {
            if (ptr->count > frequentcount)
            {
                frequentword = ptr->word;
                frequentcount = ptr->count;
            }
            ptr = ptr->next;
        }

        return frequentword + " " + to_string(frequentcount);
    }

    //going through the list, if count = 1, the int count gets 1 added.
    int num_singletons() const
    {
        int count = 0;
        Node *p = head;
        while (p != NULL)
        {
            if (p->count == 1)
                count++;
            p = p->next;
        }
        return count;
    }


    //first check where to place the new word, then add the node.
    void add_word(const string &w) 
    {
        if (head == nullptr || w < head->word)
        {
            Node *newnode = new Node{w, 1, head};
            head = newnode;
        }
        else if (w == head->word)
        {
            head->count++;
        }
        else
        {
            Node *ptr = head;
            while (ptr->next != NULL && w > ptr->next->word)
            {
                ptr = ptr->next;
            }

            if (ptr->next != NULL && w == ptr->next->word)
            {
                ptr->next->count++;
            }
            else
            {
                Node *newnode = new Node{w, 1, ptr->next};
                ptr->next = newnode;
            }
        }
    }


    //iterating all the nodes and print the values in format.
    void print_words() const
    {
        Node *ptr = head;
        int number = 1;

        while (ptr != nullptr)
        {
            cout << number << ". {\"" << ptr->word << "\", " << ptr->count << "}" << endl;
            number++;
            ptr = ptr->next;
        }
    }


    //
    // ... you can write helper methods if you need them ...
    //

    //
    // ... write a default constructor that creates an empty Wordlist ...
    //

}; // class Wordlist

//
// ... you can write helper functions here (or before Wordlist) if you need them
// ...
//

//
// ... write functions to test every method of Wordlist ...
//
