#ifndef SERVICE_LIST_H
#define SERVICE_LIST_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;



// Structure for a command node
struct CommandNode {
    std::string command;
    CommandNode* next;
};

// Structure for a service node
struct ServiceNode {
    std::string serviceName;
    ServiceNode* nextService;
    CommandNode* commandList;
};


// Structure for a request node (for the student queue)
struct RequestNode {
    string request;
    string name;
    string id;
    RequestNode* next;
};

class PaymentList {
public:
    PaymentList(); // Constructor
    ~PaymentList(); // Destructor

    void addPayment(string name, string id, int payment, string job); // Method to add a payment to the list
    void printPayments(); // Method to print all payments
    void printPayments(string job); // Method to print all payments for students or instructors
private:
    struct PaymentNode {
        string name;
        string id;
        string job;
        int payment;
        PaymentNode* next;
    };
    PaymentNode* head; // Head of the linked list
};

class CommandStack{
public:
    CommandStack(); // Constructor
    ~CommandStack(); // Destructor

    void push(string variable_name, string service_name, string command); // Method to add a command to the stack
    string pop(); // Method to remove a command from the stack
    bool isEmpty(); // Method to check if the stack is empty
    void printStack(); // Method to print all commands in the stack
    void printReverseStack(); // Method to print all commands in the stack in reverse order
private:
    struct CommandNode {
        string variable_name;
        string service_name;
        string command;
        CommandNode* next;
    };
    CommandNode* top; // Top of the stack
};

// Class for managing the linked list of services
class ServiceList {
public:
    ServiceList(); // Constructor
    ~ServiceList(); // Destructor

    void addService(std::string serviceName, std::vector<std::string> commands); // Method to add a service to the list
    void printServices(); // Method to print all services and their commands
    void executeService(string serviceName, PaymentList& paymentList, CommandStack& programStack, string name, string id, string job); // Method to execute a specific service

private:
    ServiceNode* head; // Head of the linked list
};

// Class for managing the queue of student requests
class StudentQueue {
public:
    StudentQueue(); // Constructor
    ~StudentQueue(); // Destructor

    void enqueue(std::string request, string name, string id); // Method to add a request to the queue
    std::string dequeue(string &id, string &name); // Method to remove a request from the queue
    bool isEmpty(); // Method to check if the queue is empty

private:
    RequestNode* front; // Front of the queue
    RequestNode* rear; // Rear of the queue
};

// Class for managing the queue of instructor requests
class InstructorQueue {
public:
    InstructorQueue(int size); // Constructor
    ~InstructorQueue(); // Destructor

    void enqueue(std::string request, string name, string id); // Method to add a request to the queue
    string dequeue(string &id, string &name); // Method to remove a request from the queue
    bool isEmpty(); // Method to check if the queue is empty
    bool isFull(); // Method to check if the queue is full

private:
    int front; // Front of the queue
    int rear; // Rear of the queue
    int size; // Size of the queue
    RequestNode* array; // Array to hold the requests
};

#endif // SERVICE_LIST_H