//CS204 Fall 2023 - HW3: Stacks & Queues and SU services
//Tolga Toker 32639


#include <iostream>
#include <fstream>       // for ifstream
#include <cctype>        
#include <string>
#include <vector>
#include <sstream>      // std::stringstream

// #include "dataStructures.h" -> since we're not allowed to use header files, we'll just copy the contents of the header file here

using namespace std;


// dataStructures.h

// Structure for a command node
struct CommandNode {
    string command;
    CommandNode* next;
};

// Structure for a service node
struct ServiceNode {
    string serviceName;
    ServiceNode* nextService;
    CommandNode* commandList;
};


// Structure for a request node (for the student queue)
struct RequestNode {
    string request;
    string name;
    int id;
    RequestNode* next;
};

class PaymentList {
public:
    PaymentList(); // Constructor
    ~PaymentList(); // Destructor

    void addPayment(string name, int id, int payment, string job); // Method to add a payment to the list
    void printPayments(); // Method to print all payments
    void printPayments(string job); // Method to print all payments for students or instructors
private:
    struct PaymentNode {
        string name;
        int id;
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
    string getServiceName(); // Method to get the service name of the top command
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

    void addService(string serviceName, std::vector<string> commands); // Method to add a service to the list
    void printServices(); // Method to print all services and their commands
    void processWorkload(string serviceName, PaymentList& paymentList, CommandStack& programStack, string name, int id, string job); // Method to execute a specific service
    bool isServiceAvailable(string serviceName); // Method to check if a service is available
private:
    ServiceNode* head; // Head of the linked list
};

// Class for managing the queue of student requests
class StudentQueue {
public:
    StudentQueue(); // Constructor
    ~StudentQueue(); // Destructor

    void enqueue(string request, string name, int id, PaymentList& paymentList); // Method to add a request to the queue
    string dequeue(int &id, string &name); // Method to remove a request from the queue
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

    void enqueue(string request, string name, int id, PaymentList& PaymentList); // Method to add a request to the queue
    string dequeue(int &id, string &name); // Method to remove a request from the queue
    bool isEmpty(); // Method to check if the queue is empty
    bool isFull(); // Method to check if the queue is full

private:
    int front; // Front of the queue
    int rear; // Rear of the queue
    int size; // Size of the queue
    RequestNode* array; // Array to hold the requests
};

// dataStructures.cpp

// Constructor
ServiceList::ServiceList() {
    head = nullptr;
}

// Destructor
ServiceList::~ServiceList() {
    ServiceNode* tempService = head;
    while (tempService != nullptr) {
        ServiceNode* nextService = tempService->nextService;

        // Deallocate command nodes for the current service
        CommandNode* tempCommand = tempService->commandList;
        while (tempCommand != nullptr) {
            CommandNode* nextCommand = tempCommand->next;
            delete tempCommand;
            tempCommand = nextCommand;
        }

        // Deallocate the current service node
        delete tempService;

        tempService = nextService;
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
void ServiceList::processWorkload(string serviceName, PaymentList& paymentList, CommandStack& programStack, string name, int id, string job) {
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
                    
                    processWorkload(calledServiceName, paymentList, programStack, name, id, job);
                    
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


//Global variables
InstructorQueue instructorsQueue(500);
StudentQueue studentsQueue;	
ServiceList serviceList;
PaymentList paymentList;


// Method to process the workload
void processWorkload() {
    static int instructorCount = 0;
    int id = 0;
    string name = "";

    if (!instructorsQueue.isEmpty() && instructorCount < 10) {
        cout << "Processing instructors queue..." << endl;
        string request = instructorsQueue.dequeue(id, name);
        cout << "Processing prof." << name << "'s request (with ID " << id << ") of service (function):" << endl << request << endl;
        cout << "-------------------------------------------------------" << endl;
        CommandStack programStack;
        serviceList.processWorkload(request, paymentList, programStack, name, id, "instructor");
        cout << "GOING BACK TO MAIN MENU" << endl;
        instructorCount++;
    } else if (!studentsQueue.isEmpty()) {
        if (instructorsQueue.isEmpty()){
            cout << "Instructors queue is empty. Proceeding with students queue..." << endl;
        }
        else{
            cout << "10 instructors are served. Taking 1 student from the queue..." << endl;
        }
        string request = studentsQueue.dequeue(id, name);
        cout << "Processing " << name << "'s request (with ID " << id << ") of service (function):" << endl << request << endl;
        cout << "-------------------------------------------------------" << endl;
        CommandStack programStack;
        serviceList.processWorkload(request, paymentList, programStack, name, id, "student");
        cout << "GOING BACK TO MAIN MENU" << endl;
        instructorCount = 0;
    } else {
        cout << "Both instructor's and student's queue is empty." << endl << "No request is processed." << endl << "GOING BACK TO MAIN MENU" << endl;
    }
}

void addInstructorWorkload() {
	string request = "";
	string name = "";
	int id = 0;
	cout << "Add a service (function) that the instructor wants to use: " << endl;
	cin >> request;
	if (!serviceList.isServiceAvailable(request)) {
		cout << "The requested service (function) does not exist." << endl;
		cout << "GOING BACK TO MAIN MENU" << endl;
	}
	else {
		cout << "Give instructor's name: ";
		cin >> name;
		cout << "Give instructor's ID (an int): ";
		cin >> id;
		instructorsQueue.enqueue(request, name, id, paymentList);
		cout << "Prof. " << name << "'s service request of " << request << endl <<"has been put in the instructor's queue." << endl; 
		cout << "Waiting to be served..." << endl;
	}
}

void addStudentWorkload() {
	string request = "";
	string name = "";
	int id = 0;
	cout << "Add a service (function) that the student wants to use: " << endl;
	cin >> request;
	if (!serviceList.isServiceAvailable(request)) {
		cout << "The requested service (function) does not exist." << endl;
		cout << "GOING BACK TO MAIN MENU" << endl;
	}
	else {
		cout << "Give student's name: ";
		cin >> name;
		cout << "Give student's ID (an int): ";
		cin >> id;
		studentsQueue.enqueue(request, name, id, paymentList);
		cout << name << "'s service request of " << request << " has been put in the student's queue." << endl;
		cout << "Waiting to be served..." << endl;
	}
}

void displayUsersPayments() {
	paymentList.printPayments();
}

//Initialize the service list (read from the file)
void init(){
	//Initialize the service list
	//Read from the file
	//Add services to the service list
	ifstream file;
	char choice = 'y';
	cout << "If you want to open a service (function) defining file," << endl;
	cout << "then press (Y/y) for 'yes', otherwise press any single key" << endl;
	while (choice == 'Y' || choice == 'y') {
		cin >> choice;
		if (choice == 'Y' || choice == 'y') {
			string fileName;
			cout << "Enter the input file name: ";
			cin >> fileName;
			file.open(fileName);
			if (!file) {
				cout << "Could not open the file " << fileName << endl;
				exit(1);
			}
			cout << "Do you want to open another service defining file?" << endl;
			cout << "Press (Y/y) for 'yes', otherwise press anykey" << endl;
		}
		
		string line;
		vector<string> commands;
		string serviceName;
		while (getline(file, line)) {
			if (line.find(":") != string::npos) {
				serviceName = line.substr(0, line.find(":"));
			} else if (line.find(";") != string::npos) {
				commands.push_back(line.substr(0, line.find(";")));
			}
		}
		file.close();
		serviceList.addService(serviceName, commands);
	}
	//Add services to the service list
	cout << "------------------------------------------------------------------" << endl <<
	"PRINTING AVAILABLE SERVICES (FUNCTIONS) TO BE CHOSEN FROM THE USERS" << endl <<
	"------------------------------------------------------------------" << endl << endl << endl;
	serviceList.printServices();
}

int main()
{
	init();
	while (true){		
       cout << endl;
 		cout<<"**********************************************************************"<<endl
	    <<"**************** 0 - EXIT PROGRAM                        *************"<<endl
	    <<"**************** 1 - ADD AN INSTRUCTOR SERVICE REQUEST   *************"<<endl
	    <<"**************** 2 - ADD A STUDENT SERVICE REQUEST       *************"<<endl	
	    <<"**************** 3 - SERVE (PROCESS) A REQUEST           *************"<<endl
		<<"**************** 4 - DISPLAY USER PAYMENTS               *************"<<endl
	    <<"**********************************************************************"<<endl;
		cout << endl;
		int option;
		cout << "Pick an option from above (int number from 0 to 3): ";
		cin>>option;
		switch (option)
		{
			case 0:
				cout<<"PROGRAM EXITING ... "<<endl;
				exit(0);
			case 1:
				addInstructorWorkload();
				break;
			case 2:
				addStudentWorkload();
				break;
			case 3:
				processWorkload();
				break;
			case 4:
				displayUsersPayments();
				break;
			default:
				cout<<"INVALID OPTION!!! Try again"<<endl;
		}
	}
	return 0;
}