#ifndef FlightHASHTABLE_H
#define FlightHASHTABLE_H

#include<iostream>
#include "flightticket.h"
#include "mylist.h"
// include additional header files if needed

using namespace std;

class HashNode
{
    private:
        string key; // key = companyName+flightNumber
        Flight_Ticket* flight_data; // ticket information
        HashNode* next; //next hashnode
        HashNode* prev; //prev hashnode
    public:
        HashNode()
        {
            
        }
        HashNode(string key, Flight_Ticket* data)
        {
            this->key = key;
            this->flight_data = data;
            this->next = NULL;
            this->prev = NULL;
        }
        friend class FlightHASHTABLE;
        friend class MyList<HashNode>;
};

//=============================================================================
class FlightHASHTABLE
{
    private:
        MyList<HashNode> *buckets;                //List of Buckets, Please create your own List Class called MyList
        int size;                        //Current Size of HashTable
        int capacity;                    // Total Capacity of HashTable
        int collisions;                  // Total Number of Collisions on HashTable
        // add more attributes needed
    public:
        FlightHASHTABLE();                        //constructor
        FlightHASHTABLE(int capacity);                        //constructor
        ~FlightHASHTABLE();                        //destructor
        unsigned long hashCode(string key);                         //implement and test different hash functions
        int importCSV(string path);             //Load all the flight-tickets from the CSV file to the HashTable
        int exportCSV(string path);             //Export all the flight-tickets from the HashTable to a CSV file in an ascending order
        int count_collisions();                    //return the number of collisions on the HashTable
        void add_helper();
        int add(Flight_Ticket* data);            //add new flight to the HashTable
        void removeRecord (string companyName, int flightNumber);    //Delete a record with key from the hashtable
        void find(string companyName, int flightNumber);        //Find and Display records with same key entered
        void allinday(string date);                      //Find and Display records with same day entered
        void printASC(string companyName, int flightNumber);          //display the collisions for the entered key in an ascending order

        //Add more methods and contructors, destructors if needed
};

#endif
