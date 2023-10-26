#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <climits>

using namespace std;


struct flight{
  string from;
  string to;
  int hour;
  int min;
  int price;
  int ID;
  
  flight *next;
  flight *prev;
  
  // Constructor
  flight(string from, string to, int hour, int min, int price) {
    this->from = from;
    this->to = to;
    this->hour = hour;
    this->min = min;
    this->price = price;
    this->ID = 0; // Initialize ID to 0
    this->next = nullptr;
    this->prev = nullptr;
  }
};

struct airline{
  string name;
  int ID;
  airline *next;
  flight *flights;

  // Constructor
  airline(string name, int ID) {
    this->name = name;
    this->ID = ID;
    this->next = nullptr;
    this->flights = nullptr;
  }
};

// [50|NARA->BERLIN|17:45|1150]->[249|BERLIN->TOKYO|2:20|1150]->[268|TOKYO->EDIRNE|2:25|700] $TOTAL PRICE: 3000*/
// store the flight route in a struct like above
struct flightRoute{
  flight* f;
  flightRoute* next;
  flightRoute(flight* f) {
    this->f = f;
    this->next = nullptr;
  }
};

// Flights must be kept ascending in time in the list.
void sort_list(airline* head){
  // Traverse the linked list and sort the flights of each airline
  airline* curr = head;
  while (curr != nullptr) {
    // Sort the flights of the current airline
    flight* f = curr->flights;
    while (f != nullptr) {
      flight* min = f;
      flight* curr2 = f->next;
      while (curr2 != nullptr) {
        if (curr2->hour < min->hour) {
          min = curr2;
        }
        else if (curr2->hour == min->hour && curr2->min < min->min) {
          min = curr2;
        }
        curr2 = curr2->next;
      }
      // Swap the flights if the current flight is not the minimum
      if (min != f) {
        // Swap the IDs
        int temp = f->ID;
        f->ID = min->ID;
        min->ID = temp;
        // Swap the froms
        string temp2 = f->from;
        f->from = min->from;
        min->from = temp2;
        // Swap the tos
        temp2 = f->to;
        f->to = min->to;
        min->to = temp2;
        // Swap the hours
        temp = f->hour;
        f->hour = min->hour;
        min->hour = temp;
        // Swap the mins
        temp = f->min;
        f->min = min->min;
        min->min = temp;
        // Swap the prices
        temp = f->price;
        f->price = min->price;
        min->price = temp;
      }
      f = f->next;
    }
    curr = curr->next;
  }
}


pair<vector<string>, vector<vector<flight>>> read_files(bool input_done){
  
  ///READ FLIGHTS///
  vector<string> airlines;
  vector<vector<flight>> flights(100); //start with a previously sized vector or you get seg fault (for sure)
  
  while(input_done!=true){
    string airline_name;
    string filename;
    cout << "Enter the name of airline: ";
    cin >> airline_name;
    cout << "Enter the name of airline file: ";
    cin >> filename;
    ifstream input(filename);

    if(filename == "done"){
      input_done = true;
      return make_pair(airlines, flights);
    }
    
    while( input.fail() ){
      cout << "Could not open file: " << filename << endl;
      cout << "Enter the name of airline: ";
      cin >> airline_name;
      cout << "Enter the name of airline file: ";
      cin >> filename;
      input.open(filename);

      if(filename == "done"){
      input_done = true;
      return make_pair(airlines, flights);
      }
    }
    
    airlines.push_back(airline_name);
    
   
    cout << "Processing input file: " << filename <<endl;
    
    int line_no = 0;
    vector<string> lines;
    string line;
    int vector_location = airlines.size() - 1;
    while(getline(input, line)){
      lines.push_back(line);
      line_no += 1;
      if(line_no % 5 == 0){
	flight a_flight(lines[line_no-5], lines[line_no-4], stoi(lines[line_no-3]), stoi(lines[line_no-2]), stoi(lines[line_no-1]));
	flights[vector_location].push_back(a_flight);
      }
    }
  }
  if(input_done){
    cout << "Input files are already read, however you can do manual changes.." << endl;
  }

  return make_pair(airlines, flights);
}



airline* make_linked_list_structure(vector<string> airlines, vector<vector<flight>> flights){
  airline *head = nullptr, *tail = nullptr;
  for(int i = 0; i < airlines.size(); i++){
    airline *new_airline = new airline(airlines[i], i);
    if(head == nullptr){
      head = new_airline;
      tail = new_airline;
    }
    else{
      tail->next = new_airline;
      tail = new_airline;
    }
    for(int j = 0; j < flights[i].size(); j++){
      flight *new_flight = new flight(flights[i][j].from, flights[i][j].to, flights[i][j].hour, flights[i][j].min, flights[i][j].price);
      new_flight->ID = j;
      if(new_airline->flights == nullptr){
        new_airline->flights = new_flight;
      }
      else{
        flight *curr = new_airline->flights;
        while(curr->next != nullptr){
          curr = curr->next;
        }
        curr->next = new_flight;
        new_flight->prev = curr;
      }
    }
  }
  sort_list(head);
  return head;
}


int findCheapestFlight(airline* head, const string& from, const string& to, int transfers_left, flightRoute* &route, int &price) {
  int minPrice = INT_MAX; // initialize minPrice to a very large number
  airline* curr = head;
  
  flightRoute* bestRoute = nullptr; // Initialize bestRoute to nullptr
  int bestPrice = INT_MAX; // Initialize bestPrice to a very large number
  
  while (curr){
    flight* f = curr->flights;
    while (f){
      if (f->from == from){
        if (f->to == to) {
          if (f->price < minPrice) {
            minPrice = f->price;
            // Update bestRoute and bestPrice with the current flight information
            if (bestRoute) {
              delete bestRoute; // Release previous bestRoute if it exists
            }
            bestRoute = new flightRoute(f);
            bestPrice = f->price;
          }
        } else if (transfers_left > 0) {
          flightRoute* route2 = nullptr; // Initialize route2 to nullptr
          int price2 = INT_MAX; // Initialize price2 to a very large number
          
          int result = findCheapestFlight(head, f->to, to, transfers_left - 1, route2, price2);
          
          if (result == 1) {
            int total_price = f->price + price2;
            if (total_price < minPrice) {
              minPrice = total_price;
              
              // Update bestRoute with the current flight information
              if (bestRoute) {
                delete bestRoute; // Release previous bestRoute if it exists
              }
              bestRoute = new flightRoute(f);
              bestRoute->next = route2;
              bestPrice = total_price;
            } else {
              delete route2; // Release route2 if it's not part of the best route
            }
          } else {
            delete route2; // Release route2 if no route is found
          }
        }
      }
      f = f->next;
    }
    curr = curr->next;
  }
  
  // Update the route and price with the bestRoute and bestPrice
  if (bestRoute) {
    route = bestRoute;
    price = bestPrice;
  } else {
    route = nullptr; // No route is found
    price = 0;
  }

  return (minPrice == INT_MAX) ? 0 : 1;
}


void printFlightRoute(flightRoute* route) {
  flightRoute* curr = route;
  while (curr != nullptr) {
    cout << "[" << curr->f->ID << "|" << curr->f->from << "->" << curr->f->to << "|" << curr->f->hour << ":" << curr->f->min << "|" << curr->f->price << "TRY]->";
    curr = curr->next;
  }
  cout << endl;

  //Delete the flight route
  curr = route;
  while (curr != nullptr) {
    flightRoute* temp = curr;
    curr = curr->next;
    delete temp;
  }

}


int pathfinder(airline* head) {
    int transfers_left;
    string from, destination;
    
    // Input the current location
    cout << "Where are you now? ";
    cin >> from;
    
    // Input the destination
    cout << "Where do you want to go? ";
    cin >> destination;

    // Input the number of transfers left
    cout << "Maximum number of transfers: ";
    cin >> transfers_left;
    
    // Find the cheapest flight
    flightRoute* route;
    int price;
    int result = findCheapestFlight(head, from, destination, transfers_left, route, price);

    // Print the result
    if (result == 1) {
        cout << "##Best price path##" << endl;
        printFlightRoute(route);
        cout << "$TOTAL PRICE: " << price << endl;
    }
    else {
        cout << "No route found" << endl;
    }

    // Base case: if there are no transfers left, return 0
    if (transfers_left == 0) {
        return 0;
    }
    return 1;
}


void delete_linked_list(airline* &head){
  // Traverse the linked list and delete each node
  while (head != nullptr) {
    // Delete all flights of the current airline
    flight* f = head->flights;
    while (f != nullptr) {
      flight* temp = f;
      f = f->next;
      delete temp;
    }
    // Delete the current airline node
    airline* temp = head;
    head = head->next;
    delete temp;
  }
}


void print_all(airline* head){
  airline* curr = head;
  while (curr != nullptr) {
    cout << "### AIRLINE ID: " << curr->ID << " ###" << endl;
    cout << "NAME: " << curr->name << endl;
    cout << "FLIGHTS: ";
    flight* f = curr->flights;
    while (f != nullptr) {
      cout << "#[" << f->ID << "|" << f->from << "->" << f->to << "|" << f->hour << ":" << f->min << "|" << f->price << "TRY]#";
      f = f->next;
    }
    cout << endl;
    curr = curr->next;
  }
}


void add_flight_with_input(airline* head){
  string airline_name;
  string from;
  string to;
  int hour;
  int min;
  int price;
  cout << "Adding manual entry:" << endl;
  cout << "AIRLINE: ";
  cin >> airline_name;
  cout << "FROM: ";
  cin >> from;
  cout << "TO: ";
  cin >> to;
  cout << "HOUR: ";
  cin >> hour;
  cout << "MIN: ";
  cin >> min;
  cout << "PRICE: ";
  cin >> price;
  

  // Check if the airline exists if it doesn't exist create a new airline
  bool airline_exists = false;
  airline* curr1 = head;
  while (curr1 != nullptr) {
    if (curr1->name == airline_name) {
      airline_exists = true;
      break;
    }
    curr1 = curr1->next;
  }
  if (!airline_exists){
    // Find the biggest airline ID
    int max_airline_id = -1;
    airline* curr_airline = head;
    while (curr_airline != nullptr) {
      if (curr_airline->ID > max_airline_id) {
        max_airline_id = curr_airline->ID;
      }
      curr_airline = curr_airline->next;
    }

    // Add the new airline with an ID that is one greater than the biggest airline ID
    airline* curr = head;
    while (curr != nullptr) {
      if (curr->next == nullptr) {
        airline* new_airline = new airline(airline_name, max_airline_id + 1);
        curr->next = new_airline;
        break;
      }
      curr = curr->next;
    }
  }

  
  // Find the biggest flight ID
  int max_flight_id = -1;
  airline* curr_airline = head;
  while (curr_airline != nullptr) {
    flight* curr_flight = curr_airline->flights;
    while (curr_flight != nullptr) {
      if (curr_flight->ID > max_flight_id) {
        max_flight_id = curr_flight->ID;
      }
      curr_flight = curr_flight->next;
    }
    curr_airline = curr_airline->next;
  }

  // Add the new flight with an ID that is one greater than the biggest flight ID
  airline* curr = head;
  while (curr != nullptr) {
    if (curr->name == airline_name) {
      flight* f = curr->flights;
      if (f == nullptr) {
        // If there are no flights yet, create a new one
        curr->flights = new flight(from, to, hour, min, price);
        curr->flights->ID = max_flight_id + 1;
      } else {
        // Otherwise, find the last flight and add a new one after it
        while (f->next != nullptr) {
          f = f->next;
        }
        flight* new_flight = new flight(from, to, hour, min, price);
        new_flight->ID = max_flight_id + 1;
        f->next = new_flight;
        new_flight->prev = f;
      }
      break;
    }
    curr = curr->next;
  }
  sort_list(head);
}



void remove_flight_with_input(airline* head){
  int flight_id;
  cout << "Flight id to remove: ";
  cin >> flight_id;
  airline* curr = head;
  bool found = false; // add a flag to check if the flight is found
  while (curr != nullptr) {
    flight* f = curr->flights;
    while (f != nullptr) {
      if (f->ID == flight_id) {
        cout << "Removing flight id: " << flight_id << endl;
        if (f->prev == nullptr) {
          curr->flights = f->next;
          if (f->next != nullptr) {
            f->next->prev = nullptr;
          }
        }
        else {
          f->prev->next = f->next;
          if (f->next != nullptr) {
            f->next->prev = f->prev;
          }
        }
        delete f;
        found = true; // set the flag to true if the flight is found
        break;
      }
      f = f->next;
    }
    if (found) { // break out of the outer loop if the flight is found
      break;
    }
    curr = curr->next;
  }
  if (!found) { // print the message if the flight is not found
    cout << "There is no flight named no " << flight_id << endl;
  }
}

void printMainMenu() {
  cout << endl;
  cout <<"I***********************************************I"<<endl
       <<"I               0 - DELETE DATA                 I"<<endl
       <<"I               1 - READ FILES                  I"<<endl
       <<"I               2 - PRINT ALL FLIGHTS           I"<<endl
       <<"I               3 - ADD FLIGHT                  I"<<endl
       <<"I               4 - REMOVE FLIGHT               I"<<endl
       <<"I               5 - PATH FINDER                 I"<<endl
       <<"I               6 - EXIT                        I"<<endl
       <<"I***********************************************I"<<endl
       <<">>";
  cout << endl;
}  

void processMainMenu() {

  pair<vector<string>, vector<vector<flight>>> lines_flights;
  airline* head;
  bool input_done = false;
  char input;
  do{
    printMainMenu();
    cout << "Please enter your option " << endl;
    cin >> input;
    switch (input) {
    case '0':
      // cout << "Commented out functionalities are going to be implemented" << endl;
      delete_linked_list(head);
      cout << "Data is deleted!" << endl;
      input_done = false;
      break;
    case '1':
      if(input_done!=true){
	lines_flights = read_files(input_done);
	head = make_linked_list_structure(lines_flights.first, lines_flights.second);
	cout << "Files are read.." << endl;
      }
      else{
	cout << "Files are already read.." << endl;
      }
      input_done = true;
      break;
    case '2':
      // cout << "Commented out functionalities are going to be implemented" << endl;
      print_all(head);
      break;
    case '3':
      // cout << "Commented out functionalities are going to be implemented" << endl;
      add_flight_with_input(head);
      break;
    case '4':
      // cout << "Commented out functionalities are going to be implemented" << endl;
      remove_flight_with_input(head);
      break;
    case '5':
      // cout << "Commented out functionalities are going to be implemented" << endl;
      pathfinder(head);
      break;
    case '6':
      cout << "Exiting.." << endl;
      exit(0);
    default:
      cout << "Invalid option: please enter again" << endl;
    }
  } while(true);
}



int main(){
  
  processMainMenu();
  //Comply with the provided structs and specified linked list structure for a seamless grading
  
  return 0;
  
}



  
