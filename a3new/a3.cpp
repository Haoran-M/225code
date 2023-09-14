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
// Do not #include any other files!
//
#include "Announcement.h"
#include "JingleNet_announcer.h"
#include "Queue_base.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

//Define a Node struct, for doubly linked list

struct Node
    {
        Announcement data;
        Node *prev;
        Node *next;

        Node(const Announcement &item) : data(item), prev(nullptr), next(nullptr) {}
    };


// A doubly linked list queue 
template <typename T>
class Queue : public Queue_base<T>
{
public:
    
    Node *frontNode; 
    Node *end;
    int queueSize;   // how many elements are stored


    Queue() : frontNode(nullptr), end(nullptr), queueSize(0) {}   // constructor

    ~Queue()         // destructor
    {
        while (frontNode != nullptr)
        {
            Node *temp = frontNode;
            frontNode = frontNode->next;
            delete temp;
        }
    }

    int size() const
    {
        return queueSize;
    }

    void enqueue(const T &item)
    {
        Node *newNode = new Node(item);

        if (frontNode == nullptr)
        {
            frontNode = newNode;
            end = newNode;
        }
        else
        {
            end->next = newNode;
            newNode->prev = end;
            end = newNode;
        }

        queueSize++;
    }

    const T &front() const
    {
        if (frontNode == nullptr)
        {
            cout << "queue is empty" << endl;
        }

        return frontNode->data;
    }

    void dequeue()
    {
        if (frontNode == nullptr)
        {
            cout << "queue is empty" << endl;
        }

        Node *temp = frontNode;
        frontNode = frontNode->next;

        if (frontNode != nullptr)
        {
            frontNode->prev = nullptr;
        }
        else
        {
            end = nullptr;
        }

        delete temp;
        queueSize--;
    }
};

class JingleNet
{
private:
    Queue<Announcement> santaQ;
    Queue<Announcement> reindeerQ;
    Queue<Announcement> elf2Q;
    Queue<Announcement> elf1Q;
    Queue<Announcement> snowmanQ;
    // created 5 queues

    // remove function for a single queue, takes in queue name and sender name
    void removeMessagesBySender(Queue<Announcement> &queue, const string& sender)
    {
    Node* current = queue.frontNode;
    while (current != nullptr)
    {
        if (current->data.get_sender_name() == sender)
        {
            Node* nextNode = current->next;
            Node* prevNode = current->prev;

            if (prevNode != nullptr)
            {
                prevNode->next = nextNode;
            }
            else
            {
                // If the matching node is the front node, update
                queue.frontNode = nextNode;
            }

            if (nextNode != nullptr)
            {
                nextNode->prev = prevNode;
            }
            else
            {
                // If the matching node is the end node, update
                queue.end = prevNode;
            }

            delete current;
            queue.queueSize--;
            current = nextNode;
        }
        else
        {
            current = current->next;
        }
    }
    }

public:
    // send message to queue function using a switch loop.
    void send(const string &sender, const string &rank, const string &message)
    {
        Rank currentRank = to_rank(rank);
        Announcement announcement(sender, currentRank, message);
        // compare the currentRank with the other Ranks if true, enqueue
        switch (currentRank)
        {
        case Rank::SANTA:
            santaQ.enqueue(announcement);
            break;
        case Rank::REINDEER:
            reindeerQ.enqueue(announcement);
            break;
        case Rank::ELF2:
            elf2Q.enqueue(announcement);
            break;
        case Rank::ELF1:
            elf1Q.enqueue(announcement);
            break;
        case Rank::SNOWMAN:
            snowmanQ.enqueue(announcement);
            break;
        default:
            break;
        }
    }

    // Remove_all function, calls removeMessagesBySender for all five queue
    void remove_all(const string &sender)
    {
        removeMessagesBySender(santaQ, sender);
        removeMessagesBySender(reindeerQ, sender);
        removeMessagesBySender(elf1Q, sender);
        removeMessagesBySender(elf2Q, sender);
        removeMessagesBySender(snowmanQ, sender);

    }


   void promote(const string& sender)
{
    // Promote messages from reindeerQ to santaQ
    promoteMessages(reindeerQ, sender);

    // Promote messages from elf2Q to reindeerQ
    promoteMessages(elf2Q, sender);

    // Promote messages from elf1Q to elf2Q
    promoteMessages(elf1Q, sender);

    // Promote messages from snowmanQ to elf1Q
    promoteMessages(snowmanQ, sender);
    // Santa queue does not need to be promoted
}

void promoteMessages(Queue<Announcement>& queue, const string& sender)
{
    Queue<Announcement> tempQueue;

    // Move sender messages from the queue to the temp queue
    Node* current = queue.frontNode;
    while (current != nullptr)
    {
        if (current->data.get_sender_name() == sender)
        {
            tempQueue.enqueue(current->data);

            Node* nextNode = current->next;
            Node* prevNode = current->prev;

            if (prevNode != nullptr)
            {
                prevNode->next = nextNode;
            }
            else
            {
                // If the matching node is the front node, update front
                queue.frontNode = nextNode;
            }

            if (nextNode != nullptr)
            {
                nextNode->prev = prevNode;
            }
            else
            {
                // If the matching node is the end node, update
                queue.end = prevNode;
            }

            delete current;
            queue.queueSize--;
            current = nextNode;
        }
        else
        {
            current = current->next;
        }
    }

    // Enqueue the messages from the temp queue to the correct higher rank queue
    current = tempQueue.frontNode;
    while (current != nullptr)
    {
        const Rank currentRank = current->data.get_rank();
        const string text = current->data.get_text();
        // Used if else in while loop to check the rank and then enqueue to according queue
        if (currentRank == Rank::SNOWMAN)
        {
            Announcement temp_announce = Announcement(sender, Rank::ELF1, text);
            elf1Q.enqueue(temp_announce);
        }
        else if (currentRank == Rank::ELF1)
        {
            Announcement temp_announce = Announcement(sender, Rank::ELF2, text);
            elf2Q.enqueue(temp_announce);
        }
        else if (currentRank == Rank::ELF2)
        {
            Announcement temp_announce = Announcement(sender, Rank::REINDEER, text);
            reindeerQ.enqueue(temp_announce);
        }
        else if (currentRank == Rank::REINDEER)
        {
            Announcement temp_announce = Announcement(sender, Rank::SANTA, text);
            santaQ.enqueue(temp_announce);
        }
       

        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }

    // Clear the temp queue
    tempQueue.frontNode = nullptr;
    tempQueue.end = nullptr;
    tempQueue.queueSize = 0;
}

    void announce(int n)
{
    int count = 0;

    // Dequeue and announce announcements from the santa queue
    while (santaQ.frontNode != nullptr && count < n)
    {
        Announcement announcement = santaQ.front();
        santaQ.dequeue();
        jnet.announce(announcement);
        count++;
    }

    // Dequeue and announce announcements from the reindeer queue
    while (reindeerQ.frontNode != nullptr && count < n)
    {
        Announcement announcement = reindeerQ.front();
        reindeerQ.dequeue();
        jnet.announce(announcement);
        count++;
    }

    // Dequeue and announce announcements from the elf2 queue
    while (elf2Q.frontNode != nullptr && count < n)
    {
        Announcement announcement = elf2Q.front();
        elf2Q.dequeue();
        jnet.announce(announcement);
        count++;
    }

    // Dequeue and announce announcements from the elf1 queue
    while (elf1Q.frontNode != nullptr && count < n)
    {
        Announcement announcement = elf1Q.front();
        elf1Q.dequeue();
        jnet.announce(announcement);
        count++;
    }

    // Dequeue and announce announcements from the snowman queue
    while (snowmanQ.frontNode != nullptr && count < n)
    {
        Announcement announcement = snowmanQ.front();
        snowmanQ.dequeue();
        jnet.announce(announcement);
        count++;
    }
}

};

int main(int argc, char* argv[])
{
    JingleNet jingleNet;
    string command;
    // used argc so we can run ./a3 test.txt, else we have to reenter test.txt
   if (argc < 2)
    {
        cerr << "Please provide the input file name as a command-line argument." << endl;
        return 1;
    }
    // used a ifstream for the txt file 
    string inputFileName = argv[1];
    ifstream file(inputFileName);

    if (!file)
    {
        cerr << "Error opening file: " << inputFileName << endl;
        return 1;
    }

    while (file >> command)
    {
        if (command == "SEND")
        {
            string sender, rank, message;
            file >> sender >> rank;

            getline(file, message);
            // Remove leading whitespace from the message, used get line fucntion 
            message = message.substr(message.find_first_not_of(" \t"));

            jingleNet.send(sender, rank, message);
        }
        else if (command == "REMOVE_ALL")
        {
            string sender;
            file >> sender;
            jingleNet.remove_all(sender);
        }
        else if (command == "PROMOTE_ANNOUNCEMENTS")
        {
            string sender;
            file >> sender;
            jingleNet.promote(sender);
        }
        else if (command == "ANNOUNCE")
        {
            int n;
            file >> n;
            jingleNet.announce(n);
        }
    }// call the according functions for getline

    file.close();

    return 0;
}      