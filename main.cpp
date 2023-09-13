
//=================================================================================
// Name         : DS Assignment#4
// Author       :
// Version      : 1.0
// Date Created : 00-00-2022
// Date Modified:
// Description  : Starter Code for Contact Travel Management System using HASH TABLE in C++
//=================================================================================
#include<iostream>
#include<cstdlib>
#include<sstream>
#include <chrono>
using namespace std::chrono;

#include "flighthashtable.h"
#include "flightticket.h"
#include "mylist.h"

//==========================================================
void listCommands()
{
    cout<<"\n----------------------------------"<<endl;
    cout<<"import <path>          :Import flight-tickets from a CSV file"<<endl
        <<"export <path>          :Export flight-tickets to a CSV file"<<endl
        <<"count_collisions       :Print Number of Collisions"<<endl
        <<"add                    :Add a new flight-ticket"<<endl
        <<"delete <key>           :Delete a flight-ticket"<<endl
        <<"find <key>             :Find a flight-ticket's details"<<endl
        <<"allinday <date>        :Display all flight-tickets in a day"<<endl
        <<"printASC <key>         :Print flight-tickets in ascending order"<<endl
        <<"exit                   :Exit the program"<<endl;
}
//==========================================================

// to be completed with all the necessary method definitions

//==========================================================
int main()
{
    FlightHASHTABLE myFlightHASHTABLE;
    string user_input; // user input
    string command; // command - import, add, find ...
    string command_argument; // key, date
    int start_index; // start index of the argument
    
    while(true)
    {
        user_input = "";
        command = "";
        command_argument = "";
        start_index = 0;
        
        listCommands();
        cout<<">";
        getline(cin,user_input); // input
        
        for (int i=0; i<user_input.length(); ++i)
        {
            if (user_input[i] != ' ') // checking the space
            {
                command = command + user_input[i]; // command
            }
            else
            {
                start_index = i + 1; //start index found
                break;
            }
        }
        if (start_index != 0 && user_input.length() > start_index) // checking the length and start index
        {
            for (int i=start_index; i<user_input.length(); ++i)
            {
                command_argument = command_argument + user_input[i]; // command argument - key/date
            }
        }
        
        if (command == "import")
        {
            if(command_argument == "")
            {
                cout<<"Invalid Command!!!"<<endl;
            }
            else
            {
                auto start = chrono::high_resolution_clock::now();// time start
                cout<<"Flight-Tickets imported: "<<myFlightHASHTABLE.importCSV(command_argument)<<endl;
                auto end = chrono::high_resolution_clock::now(); // time end
                auto duration = chrono::duration_cast<seconds>(end-start); // time taken
                cout<<"Time Taken: "<<duration.count()<<" (seconds)"<<endl;
            }
        }
        //---------------------------------------------------------------------
        else if (command == "export")
        {
            if(command_argument == "")
            {
                cout<<"Invalid Command!!!"<<endl;
            }
            else
            {
                auto start = chrono::high_resolution_clock::now(); //time start
                cout<<"Flight-Tickets exported: "<<myFlightHASHTABLE.exportCSV(command_argument)<<endl;
                auto end = chrono::high_resolution_clock::now(); //time end
                auto duration = chrono::duration_cast<milliseconds>(end-start); //time taken
                cout<<"Time Taken: "<<duration.count()<<" (milliseconds)"<<endl;
            }
        }
        //---------------------------------------------------------------------
        else if (command == "count_collisions")
        {
            if(command_argument != "") // if the argument is entered because the argument is not needed
            {
                cout<<"Invalid Command!!!"<<endl;
            }
            else
            {
                cout<<"Total Number of Flight Tickets is "<<myFlightHASHTABLE.count_collisions()<<endl;
            }
        }
        //---------------------------------------------------------------------
        else if (command == "add")
        {
            if(command_argument != "") //if the argument is entered because the argument is not needed
            {
                cout<<"Invalid Command!!!(key format: companyName,flightNumber)"<<endl;
            }
            else
            {
                myFlightHASHTABLE.add_helper(); //adding
            }
        }
        //---------------------------------------------------------------------
        else if (command == "delete")
        {
            string company_name = "";
            string sflight_number = "";
            bool int_error = false;
            if(command_argument == "") //if the argument is not entered because the argument is needed
            {
                cout<<"Invalid Command!!!(key format: companyName,flightNumber)"<<endl;
            }
            else
            {
                for (int i=0; i<command_argument.length(); ++i)
                {
                    if (command_argument[i] != ',') //checking the comma
                    {
                        company_name = company_name + command_argument[i]; // company name,flight number
                    }
                    else
                    {
                        sflight_number = command_argument.substr(i+1, command_argument.length());
                        break;
                    }
                }
                for (int i=0; i<sflight_number.length(); ++i)
                {
                    if (!isdigit(sflight_number[i])) //checking the digits
                    {
                        int_error = true; //error
                        break;
                    }
                }
                if (!int_error && sflight_number.length() != 0)
                {
                    int flight_number = stoi(sflight_number); // string -> int
                    myFlightHASHTABLE.removeRecord(company_name, flight_number);
                }
                else
                {
                    cout<<"Invalid Command!!!(key format: companyName,flightNumber)"<<endl;
                }
            }
        }
        //---------------------------------------------------------------------
        else if (command == "find")
        {
            string company_name = "";
            string sflight_number = "";
            bool int_error = false;
            if(command_argument == "")
            {
                cout<<"Invalid Command!!!(key format: companyName,flightNumber)"<<endl;
            }
            else
            {
                for (int i=0; i<command_argument.length(); ++i)
                {
                    if (command_argument[i] != ',')
                    {
                        company_name = company_name + command_argument[i]; //company name
                    }
                    else
                    {
                        for (int a=i+1; a<command_argument.length(); ++a)
                        {
                            sflight_number = sflight_number + command_argument[a]; //flight number
                        }
                        break;
                    }
                }
                for (int i=0; i<sflight_number.length(); ++i)
                {
                    if (!isdigit(sflight_number[i])) //checking the digits
                    {
                        int_error = true; //error
                        break;
                    }
                }
                if (!int_error && sflight_number.length() != 0)
                {
                    int flight_number = stoi(sflight_number); //string -> int
                    auto start = chrono::high_resolution_clock::now(); //time start
                    myFlightHASHTABLE.find(company_name, flight_number); // printing tickets with the key entered
                    auto end = chrono::high_resolution_clock::now(); //time end
                    auto duration = chrono::duration_cast<milliseconds>(end-start); //time taken
                    cout<<"Time Taken: "<<duration.count()<<" (milliseconds)"<<endl;
                }
                else
                {
                    cout<<"Invalid Command!!!(key format: companyName,flightNumber)"<<endl;
                }
            }
        }
        //---------------------------------------------------------------------
        else if (command == "allinday")
        {
            if(command_argument == "") //if the argument is not entered because the argument is needed
            {
                cout<<"Invalid Command!!!(date format: 27-Jun, 7-Jan)"<<endl;
            }
            else
            {
                myFlightHASHTABLE.allinday(command_argument); // tickets for the specified date
            }
        }
        //---------------------------------------------------------------------
        else if (command == "printASC")
        {
            string company_name = "";
            string sflight_number = "";
            bool int_error = false;
            if(command_argument == "") //if the argument is not entered because the argument is needed
            {
                cout<<"Invalid Command!!!(key format: companyName,flightNumber)"<<endl;
            }
            else
            {
                for (int i=0; i<command_argument.length(); ++i)
                {
                    if (command_argument[i] != ',') // checking the comma
                    {
                        company_name = company_name + command_argument[i]; // company name
                    }
                    else
                    {
                        sflight_number = command_argument.substr(i+1, command_argument.length()); //flight number
                        break;
                    }
                }
                for (int i=0; i<sflight_number.length(); ++i)
                {
                    if (!isdigit(sflight_number[i])) //checking the digits
                    {
                        int_error = true; //error
                        break;
                    }
                }
                if (!int_error && sflight_number.length() != 0)
                {
                    int flight_number = stoi(sflight_number); //string -> int
                    myFlightHASHTABLE.printASC(company_name, flight_number); //printing in ASC
                }
                else
                {
                    cout<<"Invalid Command!!!(key format: companyName,flightNumber)"<<endl;
                }
            }
        }
        //---------------------------------------------------------------------
        else if (command == "exit")
        {
            if(command_argument != "") //if the argument is entered because the argument is not needed
            {
                cout<<"Invalid Command!!!"<<endl;
            }
            else
            {
                break;
            }
        }
        else
        {
            cout<<"Invalid Command!!!"<<endl;
        }
        //to be completed
    }

    return 0;
}
