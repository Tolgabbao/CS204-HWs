//CS204 Fall 2023 - HW3: Stacks & Queues and SU services

#include <iostream>
#include <fstream>       // for ifstream
#include <cctype>        
#include <string>
#include <vector>
#include <sstream>      // std::stringstream
using namespace std;
//TODO: Modify this part to make sure to impelement the logic for 10 requests for instructors and 1 request for students
void processWorkload()
{
	if (!instructorsQueue.isEmpty())
	{
		cout<<"Processing instructors queue..."<<endl;
		
		cout<<"Processing "<<"JOB NAME"<<"'s request (with ID "<<"JOB ID"<<") of service (function):\n"<<"FUNCTION NAME"<endl;
		//You need to implement the processWorkload --> you can modify inputs
        //processWorkload(...);
		
        cout<<"GOING BACK TO MAIN MENU"<<endl;
	}
	else if (!studentsQueue.isEmpty())
	{
        //TODO: This should print when you implemented 10 requests for instructors and 1 request for students logic
		//cout<<"10 instructors are served. Taking 1 student from the queue..."<<endl;

		cout<<"Instructors queue is empty. Proceeding with students queue..."<<endl;
		cout<<"Processing "<<"JOB NAME"<<"'s request (with ID "<<"JOB ID"<<") of service (function):\n"<<"FUNCTION NAME"<endl;
		cout<<"-------------------------------------------------------"<<endl;
        
        //You need to implement the processWorkload --> you can modify inputs
		//processWorkload(...);
		
        cout<<"GOING BACK TO MAIN MENU"<<endl;
	}
	else
	{
		cout<<"Both instructor's and student's queue is empty.\nNo request is processed."<<endl<<"GOING BACK TO MAIN MENU"<<endl;
	}
}

int main()
{
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
				displayUsers();
				break;
			default:
				cout<<"INVALID OPTION!!! Try again"<<endl;
		}
	}
	return 0;
}