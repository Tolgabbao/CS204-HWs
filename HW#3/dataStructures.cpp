#include "dataStructures.h"
#include <iostream>
#include <string>
#include <vector>


using namespace std;

// Constructor
ServiceList::ServiceList() {
    head = nullptr;
}

// Destructor
ServiceList::~ServiceList() {
    ServiceNode* temp = head;
    while (temp != nullptr) {
        ServiceNode* next = temp->nextService;
        delete temp;
        temp = next;
    }
}

// Method to add a service to the list
void ServiceList::addService(string serviceName, vector<string> commands) {
    // Create a new service node
    ServiceNode* newService = new ServiceNode;
    newService->serviceName = serviceName;
    newService->nextService = nullptr;
    newService->commandList = nullptr;

    // Add the commands to the service
    CommandNode* temp = nullptr;
    for (int i = 0; i < commands.size(); i++) {
        CommandNode* newCommand = new CommandNode;
        newCommand->command = commands[i];
        newCommand->next = nullptr;

        if (temp == nullptr) {
            newService->commandList = newCommand;
        } else {
            temp->next = newCommand;
        }
        temp = newCommand;
    }

    // Add the service to the list
    if (head == nullptr) {
        head = newService;
    } else {
        ServiceNode* temp = head;
        while (temp->nextService != nullptr) {
            temp = temp->nextService;
        }
        temp->nextService = newService;
    }
}

// Method to print all services and their commands
void ServiceList::printServices() {
    ServiceNode* temp = head;
    while (temp->nextService != nullptr) {
        cout << temp->serviceName << ":" << endl;
        CommandNode* tempCommand = temp->commandList;
        string commands = "";
        while (tempCommand != nullptr) {
            commands += tempCommand->command + ";, ";
            tempCommand = tempCommand->next;
        }
        commands = commands.substr(0, commands.length() - 3) + ".";
        temp = temp->nextService;
        if (temp->nextService == nullptr) {
            cout << commands << endl;
        }
        else{
            cout << commands << endl << endl;
        }
        
    }
}
// cout << request << " is finished. Clearing the stack from it's data..." << endl;
void ServiceList::executeService(string serviceName, PaymentList& paymentList, CommandStack& programStack, string name, int id, string job) {
    ServiceNode* temp = head;
    while (temp != nullptr) {
        if (temp->serviceName == serviceName) {
            CommandNode* tempCommand = temp->commandList;
            int servicePrice = 0;
            while (tempCommand != nullptr) {
                string command = tempCommand->command;
                if (command.substr(0, 6) == "define") {
                    string variableName = command.substr(7);
                    programStack.push(variableName, temp->serviceName, tempCommand->command);
                    servicePrice += 1;
                } else if (command == "print stack") {
                    /*Executing print stack; command from function_1
                    PRINTING THE STACK TRACE:*/
                    if (tempCommand->next == nullptr){
                        cout << "Executing print stack command from "<< temp->serviceName << endl;
                    }
                    else{
                        cout << "Executing print stack; command from "<< temp->serviceName << endl;
                    }
                    cout << "PRINTING THE STACK TRACE:" << endl;
                    if (programStack.isEmpty()) {
                        cout << "The stack is empty" << endl;
                    } else {
                        programStack.printReverseStack();
                    }
                    servicePrice += 2;
                } else if (command.substr(0, 4) == "call") {
                    /*Calling function_2 from function_1*/
                    string calledServiceName = command.substr(5, command.length() - 5);
                    cout << "Calling " << calledServiceName << " from " << temp->serviceName << endl;
                    
                    executeService(calledServiceName, paymentList, programStack, name, id, job);
                    
                    servicePrice += 5;
                }
                tempCommand = tempCommand->next;
            }
            paymentList.addPayment(name, id, servicePrice, job);
            cout << serviceName << " is finished. Clearing the stack from it's data..." << endl;
            while (!programStack.isEmpty()) {
                if (programStack.getServiceName() == serviceName) {
                    programStack.pop();
                } else {
                    break;
                }
            }
            return;
        }
        temp = temp->nextService;
    }
    //cout << "Service not found" << endl;
}

bool ServiceList::isServiceAvailable(string serviceName) {
    ServiceNode* temp = head;
    while (temp != nullptr) {
        if (temp->serviceName == serviceName) {
            return true;
        }
        temp = temp->nextService;
    }
    return false;
}

string CommandStack::getServiceName() {
    if (top == nullptr) {
        return "";
    } else {
        return top->service_name;
    }
}

// Constructor
StudentQueue::StudentQueue() {
    front = nullptr;
    rear = nullptr;
}

// Destructor
StudentQueue::~StudentQueue() {
    RequestNode* temp = front;
    while (temp != nullptr) {
        RequestNode* next = temp->next;
        delete temp;
        temp = next;
    }
}

// Method to add a request to the queue
void StudentQueue::enqueue(string request, string name, int id, PaymentList& paymentList) {
    // Create a new request node
    RequestNode* newRequest = new RequestNode;
    newRequest->request = request;
    newRequest->name = name;
    newRequest->id = id;
    newRequest->next = nullptr;

    paymentList.addPayment(name, id, 0, "student");

    // Add the request to the queue
    if (front == nullptr) {
        front = newRequest;
        rear = newRequest;
    } else {
        rear->next = newRequest;
        rear = newRequest;
    }
}

// Method to remove a request from the queue
string StudentQueue::dequeue(int &id, string &name) {
    if (front == nullptr) {
        return "";
    } else {
        RequestNode* temp = front;
        front = front->next;
        id = temp->id;
        name = temp->name;
        string request = temp->request;
        delete temp;
        return request;
    }
}

// Method to check if the queue is empty
bool StudentQueue::isEmpty() {
    return front == nullptr;
}

// Constructor for InstructorQueue
InstructorQueue::InstructorQueue(int size) {
    front = -1;
    rear = -1;
    this->size = size;
    array = new RequestNode[size];
}

//Destructor for InstructorQueue
InstructorQueue::~InstructorQueue() {
    delete[] array;
}

// Method to add a request to the queue
void InstructorQueue::enqueue(string request, string name, int id, PaymentList& paymentList) {
    if (front == -1) {
        front = 0;
    }
    rear = (rear + 1) % size;
    array[rear].request = request;
    array[rear].name = name;
    array[rear].id = id;
    paymentList.addPayment(name, id, 0, "instructor");
}

// Method to remove a request from the queue
string InstructorQueue::dequeue(int &id, string &name) {
    if (front == -1) {
        return "";
    } else {
        id = array[front].id;
        name = array[front].name;
        string request = array[front].request;
        if (front == rear) {
            front = -1;
            rear = -1;
        } else {
            front = (front + 1) % size;
        }
        return request;
    }
}

// Method to check if the queue is empty
bool InstructorQueue::isEmpty() {
    return front == -1;
}

// Method to check if the queue is full
bool InstructorQueue::isFull() {
    return (rear + 1) % size == front;
}

// Constructor
PaymentList::PaymentList() {
    head = nullptr;
}

// Destructor
PaymentList::~PaymentList() {
    PaymentNode* temp = head;
    while (temp != nullptr) {
        PaymentNode* next = temp->next;
        delete temp;
        temp = next;
    }
}

// Method to add a payment to the list
void PaymentList::addPayment(string name, int id, int payment, string job) {
    // Traverse the list to find a node with the same ID
    PaymentNode* temp = head;
    while (temp != nullptr) {
        if (temp->id == id && temp->name == name) {
            // Add the payment to the existing node
            temp->payment += payment;
            return;
        }
        temp = temp->next;
    }

    // Create a new payment node
    PaymentNode* newPayment = new PaymentNode;
    newPayment->name = name;
    newPayment->id = id;
    newPayment->payment = payment;
    newPayment->job = job;
    newPayment->next = nullptr;

    // Add the payment to the end of the list
    if (head == nullptr) {
        head = newPayment;
    } else {
        temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newPayment;
    }
}

// Method to print all payments
void PaymentList::printPayments() {
    PaymentNode* temp = head;
    while (temp != nullptr) {
        cout << "Name: "<< temp->name << " ID: " << temp->id << " " << temp->payment << " TRY" << endl;
        temp = temp->next;
    }
    if (head == nullptr) {
        cout << "No payments defined." << endl;
    }
}

// Method to print all payments for students or instructors
void PaymentList::printPayments(string job) {
    PaymentNode* temp = head;
    while (temp != nullptr) {
        if (temp->job == job) {
            cout << temp->name << " " << temp->id << " " << temp->payment << endl;
        }
        temp = temp->next;
    }
}


// Constructor
CommandStack::CommandStack() {
    top = nullptr;
}

// Destructor
CommandStack::~CommandStack() {
    CommandNode* temp = top;
    while (temp != nullptr) {
        CommandNode* next = temp->next;
        delete temp;
        temp = next;
    }
}

// Method to add a command to the stack
void CommandStack::push(string variable_name, string service_name, string command) {
    // Create a new command node
    CommandNode* newCommand = new CommandNode;
    newCommand->variable_name = variable_name;
    newCommand->service_name = service_name;
    newCommand->command = command;
    newCommand->next = nullptr;

    // Add the command to the stack
    if (top == nullptr) {
        top = newCommand;
    } else {
        newCommand->next = top;
        top = newCommand;
    }
}


// Method to remove a command from the stack
string CommandStack::pop() {
    if (top == nullptr) {
        return "";
    } else {
        CommandNode* temp = top;
        top = top->next;
        string command = temp->command;
        delete temp;
        return command;
    }
}

// Method to check if the stack is empty
bool CommandStack::isEmpty() {
    return top == nullptr;
}

// Method to print all commands in the stack
void CommandStack::printStack() {
    CommandNode* temp = top;
    while (temp != nullptr) {
        cout << temp->variable_name << " " << temp->service_name << " " << temp->command << endl;
        temp = temp->next;
    }
}

// Method to get print all commands in the stack in reverse order
/*function_1: define x;
function_1: define y;*/
void CommandStack::printReverseStack() {
    CommandNode* temp = top;
    vector<string> commands;
    while (temp != nullptr) {
        commands.push_back(temp->service_name + ": " + temp->command + ";");
        temp = temp->next;
    }
    for (int i = commands.size() - 1; i >= 0; i--) {
        cout << commands[i] << endl;
    }
}


