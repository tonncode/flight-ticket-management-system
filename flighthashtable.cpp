#include<iostream>
#include<string>
#include<fstream>
#include<exception>
#include<sstream>
#include<cstdlib>
#include<ctime>
#include<math.h>
#include "flighthashtable.h"

using namespace std;

FlightHASHTABLE::FlightHASHTABLE() //constructor
{
    buckets = new MyList<HashNode>[1009]; // list
    this->capacity = 1009; //capacity
    this->size = 0; //size - tickets
    this->collisions = 0; // collisions
}

FlightHASHTABLE::FlightHASHTABLE(int capacity) //constructor
{
    buckets = new MyList<HashNode>[capacity];
    this->capacity = capacity;
    this->size = 0;
    this->collisions = 0;
}

FlightHASHTABLE::~FlightHASHTABLE() //destructor
{
    delete [] buckets;
}

unsigned long FlightHASHTABLE::hashCode(string key) //implement and test different hash functions
{
    //polynomial hash code
    unsigned long string_hash = 0;
    for (int i=0; i < key.length(); ++i)
    {
        string_hash = string_hash + int(key[i]) * i;
    }
    unsigned long hash_code = string_hash % capacity; //within the capacity
    return hash_code;
    
    //summation hash code
    /*unsigned long string_hash = 0;
    for (int i=0; i<key.length(); i++)
    {
    string_hash = string_hash + int(key[i]);
    }
    unsigned long hash_code = string_hash % capacity; //within the capacity
    return  hash_code;*/
    
    //cycle shift hash code
    /*unsigned long string_hash = 0;
    for (int i=0; i < key.length(); ++i)
    {
        string_hash = string_hash + key[i];
        string_hash = (string_hash << 5) | (string_hash >> 27);
    }
    unsigned long hash_code = string_hash % capacity; //within the capacity
    return hash_code;*/
}

int FlightHASHTABLE::importCSV(string path) //Load all the flight-tickets from the CSV file to the HashTable
{
    ifstream fin(path);
    int n = 0;
    string v, d; // d is the input data(company name, country name, price, stopover, time, date)
    getline(fin, v); // skipping the first line
    bool dot = false;
    while (!fin.eof())
    {
        dot = false;
        string company_name, country_origin, country_destination, stopover, price, time_departure, time_arrival, date;
        int flight_number = 0; //default value
        int number = 1;
        getline(fin, v); //reading the line
        if(v == "") //the last line is empty, so reading is finished
        {
            break;
        }
        v = v + ",,"; //adding commas to label the end
        for (int i = 0; i < v.length(); ++i)
        {
            if (v[i] != ',' && v[i] != '"') //until ',' or '"' is identified, we continue to collect data
            {
                d = d + v[i];
            }
            else if(dot && v[i] != '"') //we are inside "..."
            {
                d = d + v[i];
            }
            else if (v[i] == '"')
            {
                if (dot)
                {
                    dot = false; //we are outside "..."
                }
                else
                {
                    dot = true; //we are inside "..."
                }
            }
            else
            {
                if (number == 1) //company name
                {
                    company_name = d;
                    number = number + 1;
                    d = "";
                }
                else if (number == 2) //flight number
                {
                    flight_number = stoi(d);
                    number = number + 1;
                    d = "";
                }
                else if (number == 3) // country origin
                {
                    country_origin = d;
                    number = number + 1;
                    d = "";
                }
                else if (number == 4) // country destination
                {
                    country_destination = d;
                    number = number + 1;
                    d = "";
                }
                else if (number == 5) // stopover
                {
                    stopover = d;
                    number = number + 1;
                    d = "";
                }
                else if (number == 6) // price
                {
                    price = d;
                    number = number + 1;
                    d = "";
                }
                else if (number == 7) // time departure
                {
                    time_departure = d;
                    number = number + 1;
                    d = "";
                }
                else if (number == 8) // time arrival
                {
                    time_arrival = d;
                    number = number + 1;
                    d = "";
                }
                else if (number == 9) // date
                {
                    
                    if(d.length() == 5) // 1Jun
                    {
                        date = date + d[0] + '-' + d[1] + d[2] + d[3];
                    }
                    else if(d.length() == 6) //12Jun
                    {
                        date = date + d[0] + d[1] + '-' + d[2] + d[3] + d[4];
                    }
                    number = number + 1;
                    d = "";
                }
                else if (number == 10) // the last step - adding the flight ticket
                {
                    Flight_Ticket* data = new Flight_Ticket(company_name,flight_number, country_origin, country_destination, stopover, price, time_departure, time_arrival, date); // flight ticket*
                    if(add(data) == 1)
                    {
                        n = n + 1; // counting entered flight tickets
                    }
                    number = 1; // to restart and insert/read another flight ticket/line from the file
                    d = "";
                    break;
                }
            }
        }
    }
    fin.close();
    return n; //imported tickets
}

int FlightHASHTABLE::exportCSV(string path) //Export all the flight-tickets from the HashTable to a CSV file in an ascending order
{
    ofstream fout(path);
    int n = 0; // tickets
    if (fout.is_open())
    {
        for (int i=0; i<capacity; ++i)
        {
            if (buckets[i].size() != 0) // checking the number of tickets inside the busket
            {
                MyList<HashNode>::Iterator it; // iterator
                for (it.ptr = buckets[i].header->next; it.ptr != buckets[i].trailer; ++it)
                {
                    fout<<it.ptr->hashnode.flight_data->companyName + "," + to_string(it.ptr->hashnode.flight_data->flightNumber) + "," + it.ptr->hashnode.flight_data->country_of_origin + "," + it.ptr->hashnode.flight_data->country_of_destination + "," + it.ptr->hashnode.flight_data->stopOver + "," + it.ptr->hashnode.flight_data->price + "," + it.ptr->hashnode.flight_data->time_of_departure + "," + it.ptr->hashnode.flight_data->time_of_arrival + "," + it.ptr->hashnode.flight_data->date<<endl;
                    n = n + 1;
                }
            }
        }
    }
    fout.close();
    return n; // exported tickets
}

int FlightHASHTABLE::count_collisions() //return the number of collisions on the HashTable
{
    cout<<size<<endl;
    cout<<"Total Number of Collisions is: ";
    return collisions;
}

void FlightHASHTABLE::add_helper() // collecting required data about the ticket
{
    bool input_error = false;
    string company_name, sflight_number, country_origin, country_destination, stopover, price, time_departure, time_arrival, date;
    cout<<"Please the details if the flight-ticket:"<<endl;
    do
    {
        cout<<"Company Name: ";
        getline(cin,company_name); //input
        input_error = false;
        if (company_name.length() <= 1) //checking for the length
        {
            cout<<"Company Name cannot be only 1 character"<<endl;
            input_error = true; //error
        }
        for(int i=0; i<company_name.length(); ++i)
        {
            if(isalpha(company_name[i]) == false) //checking for letters
            {
                cout<<"Company Name consists only letters"<<endl;
                input_error = true; //error
                break;
            }
        }
    }
    while(input_error);
    
    do
    {
        cout<<"Flight Number: ";
        getline(cin,sflight_number); //input
        input_error = false;
        for (int i=0; i<sflight_number.length(); ++i)
        {
            if (!isdigit(sflight_number[i])) //checking for the letters
            {
                input_error = true; //error
                cout<<"Flight Number cantains only digits"<<endl;
                break;
            }
        }
        if (sflight_number.length() == 0) //checking for the length
        {
            input_error = true; //error
            cout<<"Flight Number cannot be empty"<<endl;
        }
    }
    while(input_error);
    
    do
    {
        cout<<"Country of Origin: ";
        getline(cin,country_origin); //input
        input_error = false;
        if (country_origin.length() <= 1) //checking for the length
        {
            cout<<"Country Name cannot be only 1 character"<<endl;
            input_error = true; //error
        }
        for(int i=0; i<country_origin.length(); ++i)
        {
            if(isalpha(country_origin[i]) == false) //checking for the letters
            {
                cout<<"Country Name consists only letters"<<endl;
                input_error = true; //error
                break;
            }
        }
    }
    while(input_error);
    
    do
    {
        cout<<"Country of Destination: ";
        getline(cin,country_destination); //input
        input_error = false;
        if (country_destination.length() <= 1) //checking for the length
        {
            cout<<"Country Name cannot be only 1 character"<<endl;
            input_error = true; //error
        }
        if(!input_error)
        {
            for(int i=0; i<country_destination.length(); ++i)
            {
                if(isalpha(country_destination[i]) == false) //checking for the letters
                {
                    cout<<"Country Name consists only letters"<<endl;
                    input_error = true; //error
                    break;
                }
            }
        }
    }
    while(input_error);
    
    do
    {
        cout<<"Stopover: ";
        getline(cin,stopover); //input
        input_error = false;
        for (int i=0; i<stopover.length(); ++i)
        {
            if (!isdigit(stopover[i])) //checking for the digits
            {
                input_error = true; //error
                cout<<"Stopover cantains only digits"<<endl;
                break;
            }
        }
        if (stopover.length() == 0) //checking for the length
        {
            input_error = true; //error
            cout<<"Flight Number cannot be empty, if there is no stopover, enter 0"<<endl;
        }
    }
    while(input_error);
    
    do
    {
        cout<<"Price: ";
        getline(cin,price); //input
        input_error = false;
        for (int i=0; i<price.length(); ++i)
        {
            if (!isdigit(price[i])) //checking for the digits
            {
                input_error = true; //error
                cout<<"Price cantains only digits"<<endl;
                break;
            }
        }
        if (stopover.length() == 0) //checking for the length
        {
            price = true; //error
            cout<<"Price cannot be empty"<<endl;
        }
    }
    while(input_error);
    
    do
    {
        cout<<"Time of Departure: ";
        getline(cin,time_departure); //input
        input_error = false;
        int u = 0;
        if (time_departure.length() != 4 && time_departure.length() != 5) //checking for the length (12:12, 1:12)
        {
            cout<<"Invalid time entered(Ex. '8:25' or '11:25')!!!"<<endl;
            input_error = true; //error
        }
        if(!input_error) //if the length is correct
        {
            for (int i=0; i<time_departure.length(); ++i)
            {
                if (!isdigit(time_departure[i])) //checking digits and determining the index "-"
                {
                    if (time_departure.length() == 4 && i == 1 && time_departure[i] == ':') // input type 1:12
                    {
                        u = 1; //true
                    }
                    else if(time_departure.length() == 5 && i == 2 && time_departure[i] == ':') // input type 12:12
                    {
                        u = 1; //true
                    }
                    else
                    {
                        input_error = true; //error
                        cout<<"Invalid time entered(Ex. '8:25' or '11:25')!!!"<<endl;
                        break;
                    }
                }
            }
            if(u == 0)
            {
                input_error = true; //error
                cout<<"Invalid time entered(Ex. '8:25' or '11:25')!!!"<<endl;
            }
        }
    }
    while(input_error);
    
    do
    {
        cout<<"Time of Arrival: ";
        getline(cin,time_arrival); //input
        input_error = false;
        int u = 0;
        if (time_arrival.length() != 4 && time_arrival.length() != 5) //checking for the length (12:12, 1:12)
        {
            cout<<"Invalid time entered(Ex. '8:25' or '11:25')!!!"<<endl;
            input_error = true; //error
        }
        if(!input_error)
        {
            for (int i=0; i<time_arrival.length(); ++i)
            {
                if (!isdigit(time_arrival[i])) //checking digits and determining the index "-"
                {
                    if (time_arrival.length() == 4 && i == 1 && time_arrival[i] == ':') //input type 1:12
                    {
                        u = 1; //true
                    }
                    else if(time_arrival.length() == 5 && i == 2 && time_arrival[i] == ':') //input type 12:12
                    {
                        u = 1;//true
                    }
                    else
                    {
                        input_error = true; //error
                        cout<<"Invalid time entered(Ex. '8:25' or '11:25')!!!"<<endl;
                        break;
                    }
                }
            }
            if(u == 0)
            {
                input_error = true; //error
                cout<<"Invalid time entered(Ex. '8:25' or '11:25')!!!"<<endl;
            }
        }
    }
    while(input_error);
    
    do
    {
        cout<<"Date: ";
        getline(cin,date);
        input_error = false;
        if (date.length() != 5 && date.length() != 6) //checking for the length (1-Jun or 12-Jun)
        {
            cout<<"Invalid date entered(Ex. '1-Jun' or '29-Jun')!!!"<<endl;
            input_error = true; //error
        }
        if (!input_error && !isdigit(date[0])) //check first character
        {
            cout<<"Invalid date entered(Ex. '1-Jun' or '29-Jun')!!!"<<endl;
            input_error = true; //error
        }
        else if(!input_error && !isdigit(date[1])) // possible input 1-Jun
        {
            if(date[1] != '-') //if second character is not '-'
            {
                cout<<"Invalid date entered(Ex. '1-Jun' or '29-Jun')!!!"<<endl;
                input_error = true; //error
            }
            else if(date[1] == '-') //if second character is '-'
            {
                if (date.length() == 5) //checking the length
                {
                    if(!isalpha(date[2]) || !isalpha(date[3]) || !isalpha(date[4])) //checking the name of the month
                    {
                        cout<<"Invalid date entered(Ex. '1-Jun' or '29-Jun')!!!"<<endl;
                        input_error = true; //error
                    }
                }
                else
                {
                    cout<<"Invalid date entered(Ex. '1-Jun' or '29-Jun')!!!"<<endl;
                    input_error = true; //error
                }
            }
        }
        else if(!input_error && date[2] == '-') //possible input 12-Jun
        {
            if (date.length() == 6) //checking the length
            {
                if(!isalpha(date[3]) || !isalpha(date[4]) || !isalpha(date[5])) //checking the name of the month
                {
                    cout<<"Invalid date entered(Ex. '1-Jun' or '29-Jun')!!!"<<endl;
                    input_error = true; //error
                }
            }
            else
            {
                cout<<"Invalid date entered(Ex. '1-Jun' or '29-Jun')!!!"<<endl;
                input_error = true; //error
            }
            
        }
        else if(!input_error && date[2] != '-') //wrong input, wrong position of '-'
        {
            cout<<"Invalid date entered(Ex. '1-Jun' or '29-Jun')!!!"<<endl;
            input_error = true; //error
        }
    }
    while(input_error);
    
    Flight_Ticket* data = new Flight_Ticket(company_name, stoi(sflight_number), country_origin, country_destination, stopover, price, time_departure, time_arrival, date); //flight ticket*
    
    int add_int = add(data); //inserting
    
    if (add_int == -1) //-1 means : the ticket is already entered
    {
        cout<<"The entered record already exists"<<endl;
    }
    else
    {
        cout<<company_name<<" "<<stoi(sflight_number)<<" is inserted"<<endl; //inserted*
    }
}

int FlightHASHTABLE::add(Flight_Ticket* data) //add new flight to the HashTable
{
    string key = data->companyName + " " + to_string(data->flightNumber); //key
    for (int ii=0; ii<key.length(); ++ii)
    {
        if (!isdigit(key[ii]))
        {
            key[ii] = tolower(key[ii]); //company name to lower case
        }
    }
    HashNode new_node = HashNode(key, data); //hashnode
    unsigned long hash_code = hashCode(key); //hashcode of the key
    bool found = false;
    MyList<HashNode>::Iterator i; //iterator
    for (i.ptr = buckets[hash_code].header->next; i.ptr != buckets[hash_code].trailer; ++i)
    {
        if (i.ptr->hashnode.key == key && i.ptr->hashnode.flight_data->date == data->date && i.ptr->hashnode.flight_data->country_of_origin == data->country_of_origin && i.ptr->hashnode.flight_data->country_of_destination == data->country_of_destination && i.ptr->hashnode.flight_data->price == data->price && i.ptr->hashnode.flight_data->stopOver == data->stopOver && i.ptr->hashnode.flight_data->time_of_departure == data->time_of_departure && i.ptr->hashnode.flight_data->time_of_arrival == data->time_of_arrival) // ticket availability check
        {
            found = true; // ticket is found
            return -1; //ticket is already entered
        }
    }
    MyList<HashNode>::Iterator p; //iterator
    for (p.ptr = buckets[hash_code].header->next; p.ptr != buckets[hash_code].trailer; ++p)
    {
        if (p.ptr->hashnode.key == key) // key availability check
        {
            found = true; //ticket is found
            size = size + 1; //one emore ticket in the hashtable
            buckets[hash_code].size_n = buckets[hash_code].size_n + 1;
            buckets[hash_code].addBack(new_node); //adding
            return 1; //inserted
        }
    }
    if (!found && buckets[hash_code].size() == 0) // key is not found + bucket is empty -> no collision
    {
        buckets[hash_code].addBack(new_node); // adding
        size = size + 1; // size
        buckets[hash_code].size_n = buckets[hash_code].size_n + 1;
        buckets[hash_code].nn = buckets[hash_code].nn + 1;
        return 1; // inserted
    }
    else if (!found && buckets[hash_code].size() > 0) // key is not found + bucket is not empty -> collision
    {
        buckets[hash_code].addBack(new_node); //adding
        size = size + 1; //size
        buckets[hash_code].size_n = buckets[hash_code].size_n + 1;
        buckets[hash_code].nn = buckets[hash_code].nn + 1;
        collisions = collisions + 1; //collision
        return 1; //inserted
    }
    return -1; // not inserted
}

void FlightHASHTABLE::removeRecord (string companyName, int flightNumber) //Delete a record with key from the hashtable
{
    string key = companyName + " " + to_string(flightNumber); //key
    for (int ii=0; ii<key.length(); ++ii)
    {
        if (!isdigit(key[ii]))
        {
            key[ii] = tolower(key[ii]); //company to lower case
        }
    }
    unsigned long hash_code = hashCode(key); //key hash code within capacity
    MyList<HashNode>::Iterator i; //iterator
    int n = 0;
    for (i.ptr = buckets[hash_code].header->next; i.ptr != buckets[hash_code].trailer; ++i)
    {
        if (i.ptr->hashnode.key == key) // finding all tickets with the key*
        {
            n = n + 1;
            cout<<n<<". Company Name: "<<i.ptr->hashnode.flight_data->companyName<<endl;
            cout<<"    Flight Number: "<<i.ptr->hashnode.flight_data->flightNumber<<endl;
            cout<<"    Country of Origin: "<<i.ptr->hashnode.flight_data->country_of_origin<<endl;
            cout<<"    Country of Destination: "<<i.ptr->hashnode.flight_data->country_of_destination<<endl;
            cout<<"    Stopover: "<<i.ptr->hashnode.flight_data->stopOver<<endl;
            cout<<"    Price: "<<i.ptr->hashnode.flight_data->price<<endl;
            cout<<"    Time of Departure: "<<i.ptr->hashnode.flight_data->time_of_departure<<endl;
            cout<<"    Time of Arrival: "<<i.ptr->hashnode.flight_data->time_of_arrival<<endl;
            cout<<"    Date: "<<i.ptr->hashnode.flight_data->date<<endl;
        }
    }
    if (n == 1) //if only one ticket is found -> the ticket is automatically deleted
    {
        MyList<HashNode>::Iterator i; //iterator
        for (i.ptr = buckets[hash_code].header->next; i.ptr != buckets[hash_code].trailer; ++i)
        {
            if (i.ptr->hashnode.key == key) // finding the ticket with the key entered
            {
                buckets[hash_code].remove(i); //deleting
                cout<<"The flight-ticket record has been successfully deleted!"<<endl;
            }
        }
    }
    else if(n > 1) //if more tickets found
    {
        bool input_error = false;
        string nn; // number of the ticket to be deleted
        do
        {
            nn = "";
            cout<<"Please select the record you want to delete: "<<endl;
            getline(cin, nn); // input
            input_error = false;
            if (nn.length() == 0) //checking the length
            {
                input_error = true; //error
                cout<<"Invalid input!!!"<<endl;
            }
            for (int a=0; a<nn.length(); ++a)
            {
                if(!isdigit(nn[a])) //checking the digits
                {
                    input_error = true; //error
                    cout<<"Invalid input!!!"<<endl;
                    break;
                }
            }
        }
        while(input_error);
        int nn_ = stoi(nn); //string -> int
        
        MyList<HashNode>::Iterator i; //iterator
        n = 0;
        bool deleted = false;
        for (i.ptr = buckets[hash_code].header->next; i.ptr != buckets[hash_code].trailer; ++i)
        {
            if (i.ptr->hashnode.key == key) //finding all tickets with the key entered
            {
                n = n + 1; // counting to find the exact key
                if (nn_ == n)
                {
                    buckets[hash_code].remove(i); //deleting
                    deleted = true;
                    cout<<"The flight-ticket record has been successfully deleted!"<<endl;
                    break;
                }
            }
        }
        if(!deleted) //not found
        {
            cout<<"Not Found!!!"<<endl;
        }
    }
    else
    {
        cout<<"Not Found!!!"<<endl;
    }
}

void FlightHASHTABLE::find(string companyName, int flightNumber) //Find and Display records with same key entered
{
    string key = companyName + " " + to_string(flightNumber); //key
    for (int ii=0; ii<key.length(); ++ii)
    {
        if (!isdigit(key[ii]))
        {
            key[ii] = tolower(key[ii]); //company name to lower case
        }
    }
    unsigned long hash_code = hashCode(key); // key hash code within capacity
    MyList<HashNode>::Iterator i; //iterator
    int n = 0;
    for (i.ptr = buckets[hash_code].header->next; i.ptr != buckets[hash_code].trailer; ++i)
    {
        if (i.ptr->hashnode.key == key) //tickets with the key entered
        {
            n = n + 1; //number of tickets found
            //displaying
            cout<<n<<". Company Name: "<<i.ptr->hashnode.flight_data->companyName<<endl;
            cout<<"    Flight Number: "<<i.ptr->hashnode.flight_data->flightNumber<<endl;
            cout<<"    Country of Origin: "<<i.ptr->hashnode.flight_data->country_of_origin<<endl;
            cout<<"    Country of Destination: "<<i.ptr->hashnode.flight_data->country_of_destination<<endl;
            cout<<"    Stopover: "<<i.ptr->hashnode.flight_data->stopOver<<endl;
            cout<<"    Price: "<<i.ptr->hashnode.flight_data->price<<endl;
            cout<<"    Time of Departure: "<<i.ptr->hashnode.flight_data->time_of_departure<<endl;
            cout<<"    Time of Arrival: "<<i.ptr->hashnode.flight_data->time_of_arrival<<endl;
            cout<<"    Date: "<<i.ptr->hashnode.flight_data->date<<endl;
        }
    }
    if (n == 0) //not found
    {
        cout<<"Not Found!"<<endl;
    }
}

void FlightHASHTABLE::allinday(string date) //Find and Display records with same day entered
{
    int n = 0;
    MyList<HashNode>::Iterator it; //iterator
    for (int i=0; i<capacity; ++i) //scanning all buckets
    {
        if (buckets[i].size() != 0) //checking the size of the bucket
        {
            for (it.ptr = buckets[i].header->next; it.ptr != buckets[i].trailer; ++it)
            {
                if (it.ptr->hashnode.flight_data->date == date) //ticket with the date entered
                {
                    cout<<it.ptr->hashnode.flight_data->companyName + "," + to_string(it.ptr->hashnode.flight_data->flightNumber) + "," + it.ptr->hashnode.flight_data->country_of_origin + "," + it.ptr->hashnode.flight_data->country_of_destination + "," + it.ptr->hashnode.flight_data->stopOver + "," + it.ptr->hashnode.flight_data->price + "," + it.ptr->hashnode.flight_data->time_of_departure + "," + it.ptr->hashnode.flight_data->time_of_arrival + "," + it.ptr->hashnode.flight_data->date<<endl; // displaying
                    n = n + 1; //tickets found
                }
            }
        }
    }
    if (n == 0) //not found
    {
        cout<<"Not Found!"<<endl;
    }
}

void FlightHASHTABLE::printASC(string companyName, int flightNumber) //display the collisions for the entered key in an ascending order
{
    string key = companyName + " " + to_string(flightNumber); //key
    for (int ii=0; ii<key.length(); ++ii)
    {
        if (!isdigit(key[ii]))
        {
            key[ii] = tolower(key[ii]); //company name to lower case
        }
    }
    unsigned long hash_code = hashCode(key); //key hash code within capacity
    int c = 0;
    MyList<HashNode> mylist; //list to store tickets
    MyList<HashNode>::Iterator i; //iterator
    int n = 0;
    bool inserted = false;
    for (i.ptr = buckets[hash_code].header->next; i.ptr != buckets[hash_code].trailer; ++i)
    {
        inserted = false;
        if (i.ptr->hashnode.key == key) //ticket with key entered
        {
            n = n + 1; //tickets found
            if (mylist.empty()) // if the list is empty
            {
                mylist.addBack(i.ptr->hashnode); // adding the ticket
            }
            else // checking the country names
            {
                Node<HashNode>* ptr = mylist.header->next; // first ticket
                while(ptr != mylist.trailer)
                {
                    c = i.ptr->hashnode.flight_data->country_of_destination.compare(ptr->hashnode.flight_data->country_of_destination);
                    if (c <= 0) // new ticket's country name < ptr(ticket)'s country name
                    {
                        mylist.addBefore(ptr, i.ptr->hashnode); // adding before ptr
                        inserted = true; //inserted
                        break;
                    }
                    ptr = ptr->next; //checking all tickets
                }
                if (!inserted) //if not inserted
                {
                    mylist.addBack(i.ptr->hashnode); // adding to the back
                }
            }
        }
    }
    if (n == 0) // no ticket found
    {
        cout<<"Not Found!"<<endl;
    }
    else
    {
        Node<HashNode>* ptr = mylist.header->next; //first ticket with the lowest country name
        cout<<n<<" records found:"<<endl;
        for (int i=0; i<n; ++i)
        {
            //displaying all tickets in ASC
            cout<<i+1<<". Company Name: "<<ptr->hashnode.flight_data->companyName<<endl;
            cout<<"    Flight Number: "<<ptr->hashnode.flight_data->flightNumber<<endl;
            cout<<"    Country of Origin: "<<ptr->hashnode.flight_data->country_of_origin<<endl;
            cout<<"    Country of Destination: "<<ptr->hashnode.flight_data->country_of_destination<<endl;
            cout<<"    Stopover: "<<ptr->hashnode.flight_data->stopOver<<endl;
            cout<<"    Price: "<<ptr->hashnode.flight_data->price<<endl;
            cout<<"    Time of Departure: "<<ptr->hashnode.flight_data->time_of_departure<<endl;
            cout<<"    Time of Arrival: "<<ptr->hashnode.flight_data->time_of_arrival<<endl;
            cout<<"    Date: "<<ptr->hashnode.flight_data->date<<endl;
            ptr = ptr->next; // next ticket*
        }
    }
}
