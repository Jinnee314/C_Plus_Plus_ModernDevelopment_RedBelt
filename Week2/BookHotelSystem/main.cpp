#include "MyUnitTestFreimwork.hpp"
#include "LogDuration.hpp"

#include <string>
#include <cstdint>
#include <deque>
#include <set>
#include <map>
#include <iostream>

#include <fstream>
#include <random>
#include <stdexcept>

using namespace std;

class HotelManager
{
private:
	struct Book
	{
		int64_t time = 0;
		uint32_t clientId = 0;
		uint32_t roomCount = 0;
	};

	class Hotel
	{
	private:
		static const int32_t SEC_IN_DAY = 86400;

		std::string name;
		size_t numRooms = 0;
		int32_t numDiffClients = 0;
		std::multiset<uint32_t> clients;
		std::deque<Book> lastDay;

		void correctingData(int64_t curTime)
		{
			while (!lastDay.empty() && lastDay.front().time <= curTime - SEC_IN_DAY)
			{
				numRooms -= lastDay.front().roomCount;
				auto iter = clients.lower_bound(lastDay.front().clientId);
				auto nextIter = next(iter);

				if (nextIter != clients.end() && *nextIter != lastDay.front().clientId)
				{
					--numDiffClients;
				}

				clients.erase(iter);

				lastDay.pop_front();
			}
		}

	public:
		/*Hotel(
			const std::string& nameHotel,
			const Book& book
		) : name(nameHotel),
			curTime(book.time),
			numRooms(book.roomCount),
			numDiffClients(1)
		{
			lastDay.push_back(book);
			clients.insert(book.clientId);
		}*/

		void book(const Book& book)
		{
			numRooms += book.roomCount;
			if (clients.find(book.clientId) == clients.end())
			{
				++numDiffClients;
			}
			clients.insert(book.clientId);
			lastDay.push_back(book);
		}

		size_t getNumRooms(int64_t curTime)
		{
			correctingData(curTime);
			return numRooms;
		}

		int32_t getNumDiffClients(int64_t curTime)
		{
			correctingData(curTime);
			return numDiffClients;
		}
	};

	int64_t curTime;
	map<string, Hotel> hotels;

public:
	void book(const int64_t time, const string& name,
		uint32_t id, uint32_t numRooms)
	{
		Book b{ time, id, numRooms };
		curTime = time;
		hotels[name].book(b);
	}

	size_t getNumRooms(const string& name)
	{
		return hotels[name].getNumRooms(curTime);
	}

	uint32_t getNumDiffClients(const string& name)
	{
		return hotels[name].getNumDiffClients(curTime);
	}
};

//void tests()
//{
//	{
//		Book b1{ 10, 1, 2 };
//		Hotel h("ramz", b1);
//		ASSERT_EQUAL(h.getNumDiffClients(), 1);
//		ASSERT_EQUAL(h.getNumRooms(), 2);
//
//		Book b2{ 20, 2, 4 };
//		h.book(b2);
//		ASSERT_EQUAL(h.getNumDiffClients(), 2);
//		ASSERT_EQUAL(h.getNumRooms(), 6);
//
//		Book b3{ 30, 1, 4 };
//		h.book(b3);
//		ASSERT_EQUAL(h.getNumDiffClients(), 2);
//		ASSERT_EQUAL(h.getNumRooms(), 10);
//
//		Book b4{ 40, 3, 1 };
//		h.book(b4);
//		ASSERT_EQUAL(h.getNumDiffClients(), 3);
//		ASSERT_EQUAL(h.getNumRooms(), 11);
//
//		Book b5{ 4000, 77, 3 };
//		h.book(b5);
//		ASSERT_EQUAL(h.getNumDiffClients(), 4);
//		ASSERT_EQUAL(h.getNumRooms(), 14);
//
//		Book b6{ 87400, 13, 2 };
//		h.book(b6);
//		ASSERT_EQUAL(h.getNumDiffClients(), 2);
//		ASSERT_EQUAL(h.getNumRooms(), 5);
//
//		Book b7{ 187400, 13, 2 };
//		h.book(b7);
//		ASSERT_EQUAL(h.getNumDiffClients(), 1);
//		ASSERT_EQUAL(h.getNumRooms(), 2);
//	}
//	{
//		Book b1{ 10, 1, 2 };
//		Hotel h("ramz", b1);
//
//		Book b2{ 86410, 2, 4 };
//		h.book(b2);
//		ASSERT_EQUAL(h.getNumDiffClients(), 1);
//		ASSERT_EQUAL(h.getNumRooms(), 4);
//	}
//	{
//		Book b1{ 10, 1, 2 };
//		Hotel h("ramz", b1);
//
//		Book b2{ 20, 3, 7 };
//		h.book(b2);
//
//		Book b3{ 86410, 2, 4 };
//		h.book(b3);
//		ASSERT_EQUAL(h.getNumDiffClients(), 2);
//		ASSERT_EQUAL(h.getNumRooms(), 11);
//	}
//}
//
//void testTime()
//{
//	const size_t NUM_REQUEST = 100'000;
//
//	{
//		Book b1{ 0, 1, 2 };
//		Hotel h("ramz", b1);
//		LOG_DURATION("100'000 req increase id_client");
//		for (size_t i = 1; i < NUM_REQUEST; i++)
//		{
//			h.book(Book{ static_cast<int64_t>(i), static_cast<uint32_t>(i + 1), static_cast<uint32_t>(i % 1000) });
//		}
//	}
//
//	{
//		Book b1{ 0, 1, 2 };
//		Hotel h("ramz", b1);
//		LOG_DURATION("100'000 req, id_client 10'000");
//		for (size_t i = 1; i < NUM_REQUEST; i++)
//		{
//			h.book(Book{ static_cast<int64_t>(i), static_cast<uint32_t>((i + 1) % 10'000), static_cast<uint32_t>(i % 1000) });
//		}
//	}
//}

//vector<string> createNames(int numNames)
//{
//	string n = "wyrdjfhg";
//	set<string> res;
//	res.insert(n);
//	mt19937 rnd(7);
//
//	while (static_cast<int>(res.size()) != numNames)
//	{
//		shuffle(n.begin(), n.end(), rnd);
//		res.insert(n);
//	}
//
//	return vector<string>(res.begin(), res.end());
//}
//
//void createRequest()
//{
//	std::vector<string> nameHotels(move(createNames(100)));
//	mt19937 rnd(7);
//	uniform_int_distribution<int> commCh(0, 5);
//	uniform_int_distribution<int> nameCh(0, static_cast<int>(nameHotels.size() - 1));
//	uniform_int_distribution<int> idCh(0, 1'000'000'000);
//	uniform_int_distribution<int> numRumsCh(0, 1'000);
//
//	ofstream out("comm.txt");
//
//	int coefTime = 10;
//	for (size_t i = 0; i < 100'000; ++i)
//	{
//		int comm = commCh(rnd);
//		string str = "";
//		switch (comm)
//		{
//		case 0:
//			str = "BOOK " + to_string(i * coefTime) + " "
//				+ nameHotels[nameCh(rnd)] + " " + to_string(idCh(rnd)) + " "
//				+ to_string(numRumsCh(rnd)) + "\n";
//
//			out << str;
//
//			break;
//		case 1:
//			str = "CLIENTS " + nameHotels[nameCh(rnd)] + "\n";
//
//			out << str;
//
//			break;
//		case 2:
//			str = "ROOMS " + nameHotels[nameCh(rnd)] + "\n";
//			out << str;
//
//			break;
//		default:
//			str = "BOOK " + to_string(i * coefTime) + " "
//				+ nameHotels[nameCh(rnd)] + " " + to_string(idCh(rnd)) + " "
//				+ to_string(numRumsCh(rnd)) + "\n";
//
//			out << str;
//			break;
//		}
//	}
//
//	out.close();
//}

//void processingRequest()
//{
//
//	map<string, Hotel> hotels;
//
//	int query_count;
//	std::cin >> query_count;
//
//	LOG_DURATION("requests");
//
//	for (int query_id = 0; query_id < query_count; ++query_id) {
//		string query_type;
//		cin >> query_type;
//
//		if (query_type == "BOOK") {
//			Book b;
//
//			cin >> b.time;
//			string nameHotel;
//			cin >> nameHotel;
//			cin >> b.clientId;
//			cin >> b.roomCount;
//
//			if (!hotels.contains(nameHotel))
//			{
//				hotels.emplace(make_pair(nameHotel, Hotel(nameHotel, b)));
//			}
//			else
//			{
//				hotels[nameHotel].book(b);
//			}
//		}
//		else if (query_type == "CLIENTS") {
//			string nameHotel;
//			cin >> nameHotel;
//			if (hotels.contains(nameHotel))
//			{
//				cout << hotels[nameHotel].getNumDiffClients() << "\n";
//			}
//			else
//			{
//				cout << 0 << "\n";
//			}
//		}
//		else if (query_type == "ROOMS")
//		{
//			string nameHotel;
//			cin >> nameHotel;
//			if (hotels.contains(nameHotel))
//			{
//				cout << hotels[nameHotel].getNumRooms() << "\n";
//			}
//			else
//			{
//				cout << 0 << "\n";
//			}
//		}
//	}
//}

//int main()
//{
//	//createRequest();
//
//	std::ios::sync_with_stdio(false);
//	std::cin.tie(nullptr);
//
//	/*MyUnitTest::TestRunner tr;
//	RUN_TEST(tr, tests);
//
//	testTime();*/
//	//scout << "Enter" << endl;
//	/*string str;
//	while (getline(cin, str))
//	{
//		cout << str;
//	}*/
//
//	processingRequest();
//
//	return 0;
//}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cerr << "MOI RESHENIE" << endl;
	HotelManager manager;

	int query_count;
	cin >> query_count;

	LOG_DURATION("timeWork");
	for (int query_id = 0; query_id < query_count; ++query_id) {
		string query_type;
		cin >> query_type;

		if (query_type == "BOOK") {
			int64_t time;
			cin >> time;
			string hotel_name;
			cin >> hotel_name;
			int client_id, room_count;
			cin >> client_id >> room_count;
			manager.book(time, hotel_name, client_id, room_count);
		}
		else {
			string hotel_name;
			cin >> hotel_name;
			if (query_type == "CLIENTS") {
				cout << manager.getNumDiffClients(hotel_name) << "\n";
			}
			else if (query_type == "ROOMS") {
				cout << manager.getNumRooms(hotel_name) << "\n";
			}
		}
	}

	return 0;
}

//#include "LogDuration.hpp"
//#include <cstdint>
//#include <iostream>
//#include <map>
//#include <queue>
//
//using namespace std;
//
//
//class HotelManager {
//public:
//    void Book(int64_t time, const string& hotel_name,
//        int client_id, int room_count) {
//        current_time_ = time;
//        hotels_[hotel_name].Book({ time, client_id, room_count });
//    }
//    int ComputeClientCount(const string& hotel_name) {
//        return hotels_[hotel_name].ComputeClientCount(current_time_);
//    }
//    int ComputeRoomCount(const string& hotel_name) {
//        return hotels_[hotel_name].ComputeRoomCount(current_time_);
//    }
//
//private:
//    struct Booking {
//        int64_t time;
//        int client_id;
//        int room_count;
//    };
//
//    class HotelInfo {
//    public:
//        void Book(const Booking& booking) {
//            last_bookings_.push(booking);
//            room_count_ += booking.room_count;
//            ++client_booking_counts_[booking.client_id];
//        }
//        int ComputeClientCount(int64_t current_time) {
//            RemoveOldBookings(current_time);
//            return static_cast<int>(client_booking_counts_.size());
//        }
//        int ComputeRoomCount(int64_t current_time) {
//            RemoveOldBookings(current_time);
//            return room_count_;
//        }
//    private:
//        static const int TIME_WINDOW_SIZE = 86400;
//        queue<Booking> last_bookings_;
//        int room_count_ = 0;
//        map<int, int> client_booking_counts_;
//
//        void PopBooking() {
//            const Booking& booking = last_bookings_.front();
//            room_count_ -= booking.room_count;
//            const auto client_stat_it =
//                client_booking_counts_.find(booking.client_id);
//            if (--client_stat_it->second == 0) {
//                client_booking_counts_.erase(client_stat_it);
//            }
//            last_bookings_.pop();
//        }
//        void RemoveOldBookings(int64_t current_time) {
//            while (
//                !last_bookings_.empty()
//                && last_bookings_.front().time <= current_time - TIME_WINDOW_SIZE
//                ) {
//                PopBooking();
//            }
//        }
//    };
//
//    int64_t current_time_ = 0;
//    map<string, HotelInfo> hotels_;
//};
//
//int main() {
//    ios::sync_with_stdio(false);
//    cin.tie(nullptr);
//
//    HotelManager manager;
//    cerr << "IH RESHENIE" << endl;
//    int query_count;
//    cin >> query_count;
//    LOG_DURATION("Time work");
//    for (int query_id = 0; query_id < query_count; ++query_id) {
//        string query_type;
//        cin >> query_type;
//
//        if (query_type == "BOOK") {
//            int64_t time;
//            cin >> time;
//            string hotel_name;
//            cin >> hotel_name;
//            int client_id, room_count;
//            cin >> client_id >> room_count;
//            manager.Book(time, hotel_name, client_id, room_count);
//        }
//        else {
//            string hotel_name;
//            cin >> hotel_name;
//            if (query_type == "CLIENTS") {
//                cout << manager.ComputeClientCount(hotel_name) << "\n";
//            }
//            else if (query_type == "ROOMS") {
//                cout << manager.ComputeRoomCount(hotel_name) << "\n";
//            }
//        }
//    }
//
//    return 0;
//}
