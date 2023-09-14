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
#include <string>

using namespace std;

template <typename T>
class Queue : public Queue_base<T>
{
private:
    struct Node
    {
        T data;
        Node *prev;
        Node *next;

        Node(const T &item) : data(item), prev(nullptr), next(nullptr) {}
    };

    Node *front;
    Node *end;
    int queueSize;

public:
    Queue() : front(nullptr), end(nullptr), queueSize(0) {}

    ~Queue()
    {
        while (front != nullptr)
        {
            Node *temp = front;
            front = front->next;
            delete temp;
        }
    }

    int size() const override
    {
        return queueSize;
    }

    void enqueue(const T &item) override
    {
        Node *newNode = new Node(item);

        if (front == nullptr)
        {
            front = newNode;
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

    const T &front() const override
    {
        if (front == nullptr)
        {
            throw runtime_error("front: queue is empty");
        }

        return front->data;
    }

    void dequeue() override
    {
        if (front == nullptr)
        {
            cout << "dequeue: queue is empty"<<endl;
        }

        Node *temp = front;
        front = front->next;

        if (front != nullptr)
        {
            front->prev = nullptr;
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

public:
    void send(const string &sender, const string &rank, const string &message)
    {
        Rank priority = to_rank(rank);
        Announcement announcement(sender, priority, message);

        switch (priority)
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

    void remove_all(const string &sender)
    {
        remove_announcements(santaQ, sender);
        remove_announcements(reindeerQ, sender);
        remove_announcements(elf2Q, sender);
        remove_announcements(elf1Q, sender);
        remove_announcements(snowmanQ, sender);
    }

    void promote_announcements(const string &sender)
    {
        promote_announcements(santaQ, sender);
        promote_announcements(reindeerQ, sender);
        promote_announcements(elf2Q, sender);
        promote_announcements(elf1Q, sender);
    }

    void announce(int n)
    {
        ofstream file("announcements.txt");
        if (!file)
        {
            cerr << "Error opening file: announcements.txt" << endl;
            return;
        }

        print_announcements(file, santaQ, n);
        print_announcements(file, reindeerQ, n);
        print_announcements(file, elf2Q, n);
        print_announcements(file, elf1Q, n);
        print_announcements(file, snowmanQ, n);

        file.close();
        cout << n << " announcements written to announcements.txt" << endl;
    }

private:
    void remove_announcements(Queue<Announcement> &queue, const string &sender)
    {
        Queue<Announcement> tempQueue;
        while (queue.size() > 0)
        {
            const Announcement &announcement = queue.front();
            if (announcement.get_sender_name() != sender)
            {
                tempQueue.enqueue(announcement);
            }
            queue.dequeue();
        }
        while (tempQueue.size() > 0)
        {
            queue.enqueue(tempQueue.front());
            tempQueue.dequeue();
        }
    }

    void promote_announcements(Queue<Announcement> &queue, const string &sender)
    {
        Queue<Announcement> tempQueue;
        while (queue.size() > 0)
        {
            const Announcement &announcement = queue.front();
            if (announcement.get_sender_name() == sender)
            {
                Rank currentRank = announcement.get_rank();
                if (currentRank != Rank::SANTA)
                {
                    Rank newRank = static_cast<Rank>(static_cast<int>(currentRank) + 1);
                    Announcement promotedAnnouncement(announcement.get_sender_name(), newRank, announcement.get_text());
                    tempQueue.enqueue(promotedAnnouncement);
                }
            }
            else
            {
                tempQueue.enqueue(announcement);
            }
            queue.dequeue();
        }
        while (tempQueue.size() > 0)
        {
            queue.enqueue(tempQueue.front());
            tempQueue.dequeue();
        }
    }

  void print_announcements(ofstream& file, Queue<Announcement>& queue, int n) {
    int count = 1;
    while (count <= n && queue.size() > 0) {
        const Announcement& announcement = queue.front();
        file << count << ": " << announcement.to_string() << endl;
        count++;
        // Dequeue the announcement only if it has been successfully written to the file
        if (count <= n) {
            queue.dequeue();
        }
    }
}
};

int main()
{
    JingleNet jingleNet;
    string command;

    string inputFileName;
    cin >> inputFileName;
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
            // Remove leading whitespace from the message
            message = message.substr(message.find_first_not_of(" \t"));

            jingleNet.send(sender, rank, message);
        }
        else if (command == "REMOVE")
        {
            string sender;
            file >> sender;
            jingleNet.remove_all(sender);
        }
        else if (command == "PROMOTE")
        {
            string sender;
            file >> sender;
            jingleNet.promote_announcements(sender);
        }
        else if (command == "ANNOUNCE")
        {
            int n;
            file >> n;
            jingleNet.announce(n);
        }
    }

    file.close();

    return 0;
}
