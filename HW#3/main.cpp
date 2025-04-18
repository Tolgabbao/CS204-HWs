//CS204 Fall 2023 - HW3: Stacks & Queues and SU services
//Tolga Toker 32639


#include <iostream>
#include <fstream>       // for ifstream
#include <cctype>        
#include <string>
#include <vector>
#include <sstream>      // std::stringstream

#include "dataStructures.h"

using namespace std;

// Method to process the workload
void processWorkload(InstructorQueue& instructorsQueue, StudentQueue& studentsQueue, ServiceList& serviceList, PaymentList& paymentList) {
    static int instructorCount = 0;
    int id = 0;
    string name = "";

    if (!instructorsQueue.isEmpty() && instructorCount < 10) {
        cout << "Processing instructors queue..." << endl;
        string request = instructorsQueue.dequeue(id, name);
        cout << "Processing prof." << name << "'s request (with ID " << id << ") of service (function):" << endl << request << endl;
        cout << "-------------------------------------------------------" << endl;
        CommandStack programStack;
        serviceList.executeService(request, paymentList, programStack, name, id, "instructor");
        cout << "GOING BACK TO MAIN MENU" << endl;
        instructorCount++;
    } else if (!studentsQueue.isEmpty()) {
		cout << "Instructors queue is empty. Proceeding with students queue..." << endl;
        string request = studentsQueue.dequeue(id, name);
        cout << "Processing " << name << "'s request (with ID " << id << ") of service (function):" << endl << request << endl;
        cout << "-------------------------------------------------------" << endl;
        CommandStack programStack;
        serviceList.executeService(request, paymentList, programStack, name, id, "student");
        cout << "GOING BACK TO MAIN MENU" << endl;
        instructorCount = 0;
    } else {
        cout << "Both instructor's and student's queue is empty." << endl << "No request is processed." << endl << "GOING BACK TO MAIN MENU" << endl;
    }
}

void addInstructorWorkload(InstructorQueue& instructorsQueue, PaymentList& paymentList, ServiceList& serviceList) {
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

void addStudentWorkload(StudentQueue& studentsQueue, PaymentList& paymentList, ServiceList& serviceList) {
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

void displayUsersPayments(PaymentList& paymentList) {
	paymentList.printPayments();
}

//Initialize the service list (read from the file)
void init(ServiceList& serviceList){
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
	InstructorQueue instructorsQueue(500);
	StudentQueue studentsQueue;	
	ServiceList serviceList;
	PaymentList paymentList;
	init(serviceList);
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
				addInstructorWorkload(instructorsQueue, paymentList, serviceList);
				break;
			case 2:
				addStudentWorkload(studentsQueue, paymentList, serviceList);
				break;
			case 3:
				processWorkload(instructorsQueue, studentsQueue, serviceList, paymentList);
				break;
			case 4:
				displayUsersPayments(paymentList);
				break;
			default:
				cout<<"INVALID OPTION!!! Try again"<<endl;
		}
	}
	return 0;
}