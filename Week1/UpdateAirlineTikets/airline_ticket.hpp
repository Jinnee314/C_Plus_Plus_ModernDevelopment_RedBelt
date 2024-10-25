#pragma once

#include <string>
using namespace std;

struct Date {
	int year, month, day;
	/*Date() {}
	Date(const string& str);
	Date(int y, int m, int d);*/

	void operator=(const Date& d);
};

struct Time {
	int hours, minutes;
	/*Time() {}
	Time(const string& str);
	Time(int h, int m);*/

	void operator=(const Time& t);
};

struct AirlineTicket {

	string from;
	string to;
	string airline;
	Date departure_date;
	Time departure_time;
	Date arrival_date;
	Time arrival_time;
	int price;

	//AirlineTicket() {}
};