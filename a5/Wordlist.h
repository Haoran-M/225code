// Wordlist.h

#pragma once

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : <Haoran Miao>
// St.# : <301542189>
// Email: <hma98@sfu.ca>
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

//
// IMPORTANT: no global variables are permitted in this file!
//
// It is okay to define helper functions defined outside the class.
//

class Wordlist : public Wordlist_base
{
    //
    // Use this Node to implement an AVL tree for the word list. You can *add*
    // extra variables/methods/features to this struct if you like, but you must
    // keep its name the same, and also keep the word, count, left, and right
    // variables as defined.
    //
    struct Node
    {
        string word;
        int count;
        Node *left;
        Node *right;

        Node(const string &w, int c = 1) : word(w), count(c), left(nullptr), right(nullptr) {}  // a node format
    };

    Node *root;

public:
    Wordlist() : root(nullptr) {}

    Wordlist(const string &filename) : root(nullptr)
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

    ~Wordlist()
    {
        clear_tree(root);
        root = nullptr;
    }

    int get_count(const string &w) const
    {
        return get_count_helper(root, w);
    }

    int get_count_helper(Node *node, const string &w) const
    {
        if (!node)
            return 0;
        
        if (node->word == w)
            return node->count;
        
        if (w < node->word)
            return get_count_helper(node->left, w);
        
        return get_count_helper(node->right, w);
    }

    int num_different_words() const
    {
        return count_nodes(root);
    }

    int total_words() const
    {
        return total_count(root);
    }

    bool is_sorted() const
    {
        return is_tree_sorted(root);
    }

    string most_frequent() const
    {
        if (!root)
            return "";

        string frequentWord = "";
        int frequentCount = 0;

        find_most_frequent(root, frequentWord, frequentCount);

        return frequentWord + " " + to_string(frequentCount);
    }

    int num_singletons() const
    {
        return count_singletons(root);
    }

    void add_word(const string &w)
    {
        root = insert_word(root, w);
    }

    void print_words() const
{
    if (!root)
        return;

    int number = 1;
    print_in_order(root, number);
}

private:
    int height(Node *node)
    {
        if (!node)
            return 0;
        
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);
        
        return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }

    int balance_factor(Node *node)
    {
        if (!node)
            return 0;
        
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);
        
        return leftHeight - rightHeight;
    }
    
    Node *rotate_left(Node *node)
    {
        Node *newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        return newRoot;
    }

    Node *rotate_right(Node *node)
    {
        Node *newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        return newRoot;
    }

    Node *balance(Node *node)
    {
        if (!node)
            return nullptr;

        int bf = balance_factor(node);

        if (bf > 1) // Left-bigger
        {
            if (balance_factor(node->left) < 0) // Left-right case
            {
                node->left = rotate_left(node->left);
            }
            return rotate_right(node);
        }
        else if (bf < -1) // Right-bigger
        {
            if (balance_factor(node->right) > 0) // Right-left case
            {
                node->right = rotate_right(node->right);
            }
            return rotate_left(node);
        }

        return node; // Tree already balanced
    }

    Node *insert_word(Node *node, const string &w)
    {
        if (!node)
        {
            return new Node(w);
        }

        if (w < node->word)
        {
            node->left = insert_word(node->left, w);
        }
        else if (w > node->word)
        {
            node->right = insert_word(node->right, w);
        }
        else
        {
            node->count++;
            return node;
        }

        return balance(node);
    }

    Node *find_node(Node *node, const string &w) const
    {
        if (!node)
            return nullptr;

        if (w == node->word)
        {
            return node;
        }
        else if (w < node->word)
        {
            return find_node(node->left, w);
        }
        else
        {
            return find_node(node->right, w);
        }
    }

    void clear_tree(Node *node)
    {
        if (node)
        {
            clear_tree(node->left);
            clear_tree(node->right);
            delete node;
        }
    }

    int count_nodes(Node *node) const
    {
        if (!node)
            return 0;

        return 1 + count_nodes(node->left) + count_nodes(node->right);
    }

    int total_count(Node *node) const
    {
        if (!node)
            return 0;

        return node->count + total_count(node->left) + total_count(node->right);
    }

    bool is_tree_sorted(Node *node) const
    {
        if (!node)
            return true;

        bool leftSorted = is_tree_sorted(node->left);
        bool rightSorted = is_tree_sorted(node->right);

        if (leftSorted && rightSorted)
        {
            if (node->left && node->word < node->left->word)
                return false;
            if (node->right && node->word > node->right->word)
                return false;
            return true;
        }
        return false;
    }

    void find_most_frequent(Node *node, string &frequentWord, int &frequentCount) const
    {
        if (!node)
            return;

        find_most_frequent(node->left, frequentWord, frequentCount);

        if (node->count > frequentCount)
        {
            frequentWord = node->word;
            frequentCount = node->count;
        }

        find_most_frequent(node->right, frequentWord, frequentCount);
    }

    void print_in_order(Node *node, int &number) const
{
    if (node)
    {
        print_in_order(node->left, number);
        cout << number << ". {\"" << node->word << "\", " << node->count << "}" << endl;
        number++;
        print_in_order(node->right, number);
    }
}

    int count_singletons(Node *node) const
    {
        if (!node)
            return 0;

        int count = (node->count == 1) ? 1 : 0;
        count += count_singletons(node->left);
        count += count_singletons(node->right);
        return count;
    }
};// class Wordlist

//
// Make sure to thoroughly test your code as you go!
//
