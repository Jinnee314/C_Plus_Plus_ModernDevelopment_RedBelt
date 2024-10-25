#include "airline_ticket.hpp"
#include "MyUnitTestFreimwork.hpp"

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <tuple>
using namespace std;

#define UPDATE_FIELD(ticket, field, values)\
{\
    auto it = values.find(#field);\
	if (it != values.end())\
	{\
		istringstream is(it->second);\
		is >> ticket.field;\
	}\
}

bool operator < (const Date& lhs, const Date& rhs) 
{
	return tie(lhs.year, lhs.month, lhs.day) < tie(rhs.year, rhs.month, rhs.day);
}

bool operator == (const Date& lhs, const Date& rhs) 
{
	return tie(lhs.year, lhs.month, lhs.day) == tie(rhs.year, rhs.month, rhs.day);
}

bool operator != (const Date& lhs, const Date& rhs)
{
	return tie(lhs.year, lhs.month, lhs.day) != tie(rhs.year, rhs.month, rhs.day);
}

bool operator < (const Time& lhs, const Time& rhs) 
{
	return tie(lhs.hours, lhs.minutes) < tie(rhs.hours, rhs.minutes);
}

bool operator == (const Time& lhs, const Time& rhs) 
{
	return tie(lhs.hours, lhs.minutes) == tie(rhs.hours, rhs.minutes);
}

bool operator != (const Time& lhs, const Time& rhs)
{
	return tie(lhs.hours, lhs.minutes) != tie(rhs.hours, rhs.minutes);
}

ostream& operator<<(ostream& os, const Date& d) 
{
	return os << d.year << '-' << d.month << '-' << d.day;
}

ostream& operator<<(ostream& os, const Time& t) 
{
	return os << t.hours << ':' << t.minutes;
}

#define ASSIGNING_FIELDS(name, field) this->field = name.field

//Date::Date(int y, int m, int d) : year(y), month(m), day(d) {}
//
//Date::Date(const string& str)
//{
//	auto pos1 = str.find('-');
//	auto pos2 = str.find('-', pos1 + 1);
//
//	this->year = stoi(str.substr(0, pos1));
//	this->month = stoi(str.substr(pos1 + 1, pos2 - pos1 - 1));
//	this->day = stoi(str.substr(pos2));
//}

void Date::operator=(const Date& d)
{
	ASSIGNING_FIELDS(d, year);
	ASSIGNING_FIELDS(d, month);
	ASSIGNING_FIELDS(d, day);
}

//Time::Time(int h, int m) : hours(h), minutes(m) {}
//
//Time::Time(const string& str)
//{
//	auto pos = str.find(':');
//
//	this->hours = stoi(str.substr(0, pos));
//	this->minutes = stoi(str.substr(pos + 1));
//}

void Time::operator=(const Time& t)
{
	ASSIGNING_FIELDS(t, hours);
	ASSIGNING_FIELDS(t, minutes);
}

istream& operator>>(istream& is, Date& d)
{
	is >> d.year;
	is.ignore(1);
	is >> d.month;
	is.ignore(1);
	is >> d.day;

	return is;
}

istream& operator>>(istream& is, Time& t)
{
	is >> t.hours;
	is.ignore(1);
	is >> t.minutes;

	return is;
}

void TestUpdate() {
	AirlineTicket t;
	t.price = 0;
	const map<string, string> updates1 = {
	  {"departure_date", "2018-2-28"},
	  {"departure_time", "17:40"},
	};
	UPDATE_FIELD(t, departure_date, updates1);
	UPDATE_FIELD(t, departure_time, updates1);
	UPDATE_FIELD(t, price, updates1);

	ASSERT_EQUAL(t.departure_date, (Date{ 2018, 2, 28 }));
	ASSERT_EQUAL(t.departure_time, (Time{ 17, 40 }));
	ASSERT_EQUAL(t.price, 0);

	const map<string, string> updates2 = {
	  {"price", "12550"},
	  {"arrival_time", "20:33"},
	};
	UPDATE_FIELD(t, departure_date, updates2);
	UPDATE_FIELD(t, departure_time, updates2);
	UPDATE_FIELD(t, arrival_time, updates2);
	UPDATE_FIELD(t, price, updates2);

	// updates2 не содержит ключей "departure_date" и "departure_time", поэтому
	// значения этих полей не должны измениться
	ASSERT_EQUAL(t.departure_date, (Date{ 2018, 2, 28 }));
	ASSERT_EQUAL(t.departure_time, (Time{ 17, 40 }));
	ASSERT_EQUAL(t.price, 12550);
	ASSERT_EQUAL(t.arrival_time, (Time{ 20, 33 }));
}

int main() {
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, TestUpdate);
}