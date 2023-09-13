#ifndef FLIGHT_TICKET_H
#define FLIGHT_TICKET_H

#include<iostream>
using namespace std;

class Flight_Ticket
{
	private:
		string companyName;
		int flightNumber;
		string country_of_origin;
		string country_of_destination;
		string stopOver;
		string price;
		string time_of_departure;
		string time_of_arrival;
		string date;
        friend class FlightHASHTABLE;
	public:
		Flight_Ticket(string companyName, int flightNumber, string country_of_origin, string country_of_destination, string stopOver, string price, string time_of_departure, string time_of_arrival, string date): companyName(companyName), flightNumber(flightNumber), country_of_origin(country_of_origin), country_of_destination(country_of_destination), stopOver(stopOver), price(price), time_of_departure(time_of_departure), time_of_arrival(time_of_arrival), date(date)
        {}
		// Please add additional methods, contructor if needed
};

#endif
