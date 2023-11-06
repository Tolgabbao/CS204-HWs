//CS204 Fall 2023 - HW3: Stacks & Queues and SU services

#include <iostream>
#include <fstream>       // for ifstream
#include <cctype>        
#include <string>
#include <vector>
#include <sstream>      // std::stringstream

#include "dataStructures.h"

using namespace std;
//TODO: Modify this part to make sure to impelement the logic for 10 requests for instructors and 1 request for students

void processWorkload(InstructorQueue& instructorsQueue, StudentQueue& studentsQueue, ServiceList& serviceList, PaymentList& paymentList) {
	string id = "";
	string name = "";
    if (!instructorsQueue.isEmpty()) {
        cout << "Processing instructors queue..." << endl;
        string request = instructorsQueue.dequeue(id, name);
        cout << "Processing prof." << name << "'s request (with ID " << id << ") of service (function):" << endl << request << endl;
		cout << "-------------------------------------------------------" << endl;
		CommandStack programStack;
        serviceList.executeService(request,paymentList, programStack, id, name, "instructor");
        cout << "GOING BACK TO MAIN MENU" << endl;
    } else if (!studentsQueue.isEmpty()) {
        cout << "Instructors queue is empty. Proceeding with students queue..." << endl;
        string request = studentsQueue.dequeue(id, name);
        cout << "Processing" << name << "'s request (with ID " << id << ") of service (function):" << endl << request << endl;
		cout << "-------------------------------------------------------" << endl;
		CommandStack programStack;
        serviceList.executeService(request, paymentList, programStack, id, name, "student");
        cout << "GOING BACK TO MAIN MENU" << endl;
    } else {
        cout << "Both instructor's and student's queue is empty." << endl << "No request is processed." << endl << "GOING BACK TO MAIN MENU" << endl;
    }
}

void addInstructorWorkload(InstructorQueue& instructorsQueue) {
	string request;
	string name;
	string id;
	cout << "Add a service (function) that the instructor wants to use: ";
	cin >> request;
	cout << "Give instructor's name: ";
	cin >> name;
	cout << "Give instructor's ID: ";
	cin >> id;
	instructorsQueue.enqueue(request, name, id);
	cout << "Prof. " << name << "'s service request of " << request << "has been put in the instructor's queue." << endl; 
	cout << "Waiting to be served..." << endl;
}

void addStudentWorkload(StudentQueue& studentsQueue) {
	string request;
	string name;
	string id;
	cout << "Add a service (function) that the student wants to use: ";
	cin >> request;
	cout << "Give student's name: ";
	cin >> name;
	cout << "Give student's ID: ";
	cin >> id;
	studentsQueue.enqueue(request, name, id);
	cout << name << "'s service request of " << request << "has been put in the student's queue." << endl;
	cout << "Waiting to be served..." << endl;
}

void displayUsersPayments(PaymentList paymentList) {
	paymentList.printPayments();
}

//Initialize the service list (read from the file)
void init(ServiceList& serviceList){
	//Initialize the queues
	//Initialize the service list
	//Read from the file
	//Add services to the service list
	
	//Read from the file
	ifstream file;
	char choice = 'y';
	while (choice == 'Y' || choice == 'y') {
		cout << "If you want to open a service (function) defining file," << endl;
		cout << "then press (Y/y) for 'yes', otherwise press any single key" << endl;
		cin >> choice;
		if (choice == 'Y' || choice == 'y') {
			string fileName;
			cout << "Enter the input file name: ";
			cin >> fileName;
			file.open(fileName);
			if (!file) {
				cout << "Unable to open file" << endl;
				exit(1);
			}
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
	"------------------------------------------------------------------" << endl;
	serviceList.printServices();
}

int main()
{
	InstructorQueue instructorsQueue(10);
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
				addInstructorWorkload(instructorsQueue);
				break;
			case 2:
				addStudentWorkload(studentsQueue);
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