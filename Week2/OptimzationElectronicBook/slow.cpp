#include "LogDuration.hpp"
#include "MyUnitTestFreimwork.hpp"

#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <numeric>

#include <random>

using namespace std;

//class ReadingManager {
//public:
//	ReadingManager()
//		: user_page_counts_(MAX_USER_COUNT_ + 1, 0),
//		sorted_users_(),
//		user_positions_(MAX_USER_COUNT_ + 1, -1) {}
//
//	void Read(int user_id, int page_count) {
//		if (user_page_counts_[user_id] == 0) {
//			AddUser(user_id);
//		}
//		user_page_counts_[user_id] = page_count;
//		int& position = user_positions_[user_id];
//		while (position > 0 && page_count > user_page_counts_[sorted_users_[position - 1]]) {
//			SwapUsers(position, position - 1);
//		}
//	}
//
//	double Cheer(int user_id) const {
//		if (user_page_counts_[user_id] == 0) {
//			return 0;
//		}
//		const int user_count = GetUserCount();
//		if (user_count == 1) {
//			return 1;
//		}
//		const int page_count = user_page_counts_[user_id];
//		int position = user_positions_[user_id];
//		while (position < user_count &&
//			user_page_counts_[sorted_users_[position]] == page_count) {
//			++position;
//		}
//		if (position == user_count) {
//			return 0;
//		}
//		// По умолчанию деление целочисленное, поэтому
//		// нужно привести числитель к типу double.
//		// Простой способ сделать это — умножить его на 1.0.
//		return (user_count - position) * 1.0 / (user_count - 1);
//	}
//
//	void getSortedNumPages()
//	{
//		for (size_t i = 0; i < sorted_users_.size(); ++i)
//		{
//			std::cout << user_page_counts_[sorted_users_[i]] << "\n";
//		}
//	}
//
//private:
//	// Статическое поле не принадлежит какому-то конкретному
//	// объекту класса. По сути это глобальная переменная,
//	// в данном случае константная.
//	// Будь она публичной, к ней можно было бы обратиться снаружи
//	// следующим образом: ReadingManager::MAX_USER_COUNT.
//	static const int MAX_USER_COUNT_ = 100'000;
//
//	vector<int> user_page_counts_;
//	vector<int> sorted_users_;   // отсортированы по убыванию количества страниц
//	vector<int> user_positions_; // позиции в векторе sorted_users_
//
//	int GetUserCount() const {
//		return static_cast<int>(sorted_users_.size());
//	}
//	void AddUser(int user_id) {
//		sorted_users_.push_back(user_id);
//		user_positions_[user_id] = static_cast<int>(sorted_users_.size() - 1);
//	}
//	void SwapUsers(int lhs_position, int rhs_position) {
//		const int lhs_id = sorted_users_[lhs_position];
//		const int rhs_id = sorted_users_[rhs_position];
//		swap(sorted_users_[lhs_position], sorted_users_[rhs_position]);
//		swap(user_positions_[lhs_id], user_positions_[rhs_id]);
//	}
//};

class ReadingManager {
public:
	ReadingManager()
		: userPagesCount(MAX_USER_COUNT + 1, 0),
		statReadedPages(MAX_PAGES_COUNT + 1, 0) {}

	void Read(int user_id, int page_count) {
		if (userPagesCount[user_id] == 0)
		{
			++numReaders;
		}

		for (int i = ++userPagesCount[user_id]; i <= page_count; ++i)
		{
			++statReadedPages[i];
		}

		userPagesCount[user_id] = page_count;
	}

	double Cheer(int user_id) const {
		if (userPagesCount[user_id] == 0) {
			return 0;
		}

		if (numReaders == 1) {
			return 1;
		}

		const int page_count = userPagesCount[user_id];
		int countReaderLess = numReaders - statReadedPages[page_count];
		// По умолчанию деление целочисленное, поэтому
		// нужно привести числитель к типу double.
		// Простой способ сделать это — умножить его на 1.0.
		return countReaderLess * 1.0 / (numReaders - 1);
	}

	/*void getSortedNumPages()
	{
		for (size_t i = 0; i < sorted_users_.size(); ++i)
		{
			std::cout << user_page_counts_[sorted_users_[i]] << "\n";
		}
	}*/

private:
	// Статическое поле не принадлежит какому-то конкретному
	// объекту класса. По сути это глобальная переменная,
	// в данном случае константная.
	// Будь она публичной, к ней можно было бы обратиться снаружи
	// следующим образом: ReadingManager::MAX_USER_COUNT.
	static const size_t MAX_USER_COUNT = 100'000;
	static const size_t MAX_PAGES_COUNT = 1'000;
	
	int numReaders = 0;
	vector<int> userPagesCount;
	vector<int> statReadedPages;
};

const size_t numRequest = 1'000'000;
const size_t numReaders = 100'000;
const size_t maxPage = 1000;

void getTime()
{
	{
		ReadingManager rm;
		{
			const size_t numPages = 10;
			LOG_DURATION("time read increase pages");
			for (size_t i = 1; i <= numPages; ++i)
			{
				for (size_t j = numReaders; j > 0; --j)
				{
					rm.Read(static_cast<int>(j), static_cast<int>(i));
				}
			}
		}

		{
			const size_t numIters2 = 10;
			LOG_DURATION("time cheer");
			for (size_t i = 0; i < numIters2; ++i)
			{
				for (size_t j = 0; j < numReaders; ++j)
				{
					rm.Cheer(static_cast<int>(j));
				}
			}
		}
	}

	{
		ReadingManager rm;
		{
			const size_t numPages = 10;
			LOG_DURATION("time read decrease pages and user_id");
			for (size_t i = numPages; i > 0; --i)
			{
				for (size_t j = numReaders; j > 0; --j)
				{
					rm.Read(static_cast<int>(j), static_cast<int>(i));
				}
			}
		}

		{
			const size_t numIters2 = 10;
			LOG_DURATION("time cheer");
			for (size_t i = 0; i < numIters2; ++i)
			{
				for (size_t j = 0; j < numReaders; ++j)
				{
					rm.Cheer(static_cast<int>(j));
				}
			}
		}
	}

	{
		ReadingManager rm;
		const size_t numIters = 100;
		const size_t numReadPages = 1000;

		{
			LOG_DURATION("ot 0 do 1000 10 raz")
			for (size_t i = 0; i < numIters; ++i)
			{
				for (size_t j = 0; j < numReadPages; ++j)
				{
					rm.Read(static_cast<int>(i * numReadPages + j), static_cast<int>(j));
				}
			}
		}

		LOG_DURATION("time cheer");
		const size_t numIters2 = 10;
		for (size_t i = 0; i < numIters2; ++i)
		{
			for (size_t j = 0; j < numReaders; ++j)
			{
				rm.Cheer(static_cast<int>(j));
			}
		}
	}
}

void getSortedNumPages()
{
	ReadingManager rm;
	const size_t numIters = 10;
	const size_t numReadPages = 10;
	for (size_t i = 0; i < numIters; ++i)
	{
		for (size_t j = 0; j < numReadPages; ++j)
		{
			rm.Read(static_cast<int>(i * numReadPages + j), static_cast<int>(j));
		}
	}
	//rm.getSortedNumPages();
}

void TestReadPage()
{
    {
        ReadingManager manager;
        ASSERT_EQUAL(manager.Cheer(1), 0.0);
        manager.Read(1, 1);
        ASSERT_EQUAL(manager.Cheer(1), 1.0);
    }
    {
        ReadingManager manager;
        ASSERT_EQUAL(manager.Cheer(1), 0.0);
        manager.Read(1, 1);
        ASSERT_EQUAL(manager.Cheer(2), 0.0);
    }
    {
        ReadingManager manager;
        manager.Read(1, 1);
        ASSERT_EQUAL(manager.Cheer(1), 1.0);
        manager.Read(1, 2);
        ASSERT_EQUAL(manager.Cheer(1), 1.0);
    }
    {
        ReadingManager manager;
        manager.Read(1, 1);
        manager.Read(2, 2);
        manager.Read(3, 3);
        ASSERT_EQUAL(manager.Cheer(2), 0.5);
        manager.Read(4, 4);
        ASSERT_EQUAL(manager.Cheer(2), 1.0 / 3.0);
    }
    {
        ReadingManager manager;
        manager.Read(1, 1);
        manager.Read(2, 2);
        manager.Read(3, 3);
        ASSERT_EQUAL(manager.Cheer(2), 0.5);
        manager.Read(4, 4);
        ASSERT_EQUAL(manager.Cheer(3), 2.0 / 3.0);
    }
    {
        ReadingManager manager;
        manager.Read(1, 0);
        manager.Read(2, 0);
        manager.Read(3, 1);
        manager.Read(4, 2);
        ASSERT_EQUAL(manager.Cheer(3), 2.0 / 3.0);
    }
    {
        ReadingManager manager;
        manager.Read(1, 5);
        manager.Read(2, 5);
        manager.Read(3, 5);
        manager.Read(4, 5);
        ASSERT_EQUAL(manager.Cheer(3), 0.0);
    }
    {
        ReadingManager manager;
        manager.Read(4, 5);
        manager.Read(3, 4);
        manager.Read(2, 3);
        manager.Read(1, 2);
        ASSERT_EQUAL(manager.Cheer(3), 2.0 / 3.0);
    }
    {
        ReadingManager manager;
        manager.Read(4, 5);
        manager.Read(3, 4);
        manager.Read(4, 6);
        manager.Read(3, 5);
        ASSERT_EQUAL(manager.Cheer(4), 1.0);
        ASSERT_EQUAL(manager.Cheer(3), 0.0);
        manager.Read(10, 2);
        ASSERT_EQUAL(manager.Cheer(3), 0.5);
        ASSERT_EQUAL(manager.Cheer(4), 1.0);
    }
    {
        ReadingManager manager;
        manager.Read(4, 5);
        ASSERT_EQUAL(manager.Cheer(4), 1.0);
        manager.Read(4, 6);
        ASSERT_EQUAL(manager.Cheer(4), 1.0);
        manager.Read(4, 100);
        ASSERT_EQUAL(manager.Cheer(4), 1.0);
        ASSERT_EQUAL(manager.Cheer(3), 0.0);
        ASSERT_EQUAL(manager.Cheer(5), 0.0);
    }
    {// ten users
        ReadingManager manager;
        manager.Read(1, 1);
        manager.Read(2, 2);
        manager.Read(3, 3);
        manager.Read(4, 4);
        manager.Read(5, 5);
        manager.Read(6, 6);
        manager.Read(7, 7);
        manager.Read(8, 8);
        manager.Read(9, 9);
        manager.Read(100, 10);
        manager.Read(111, 11);

        ASSERT_EQUAL(manager.Cheer(1), 0.0);
        ASSERT_EQUAL(manager.Cheer(2), 0.1);
        ASSERT_EQUAL(manager.Cheer(3), 0.2);
        ASSERT_EQUAL(manager.Cheer(4), 0.3);
        ASSERT_EQUAL(manager.Cheer(5), 0.4);
        ASSERT_EQUAL(manager.Cheer(6), 0.5);
        ASSERT_EQUAL(manager.Cheer(7), 0.6);
        ASSERT_EQUAL(manager.Cheer(8), 0.7);
        ASSERT_EQUAL(manager.Cheer(9), 0.8);
        ASSERT_EQUAL(manager.Cheer(100), 0.9);
        ASSERT_EQUAL(manager.Cheer(111), 1.0);
        ASSERT_EQUAL(manager.Cheer(12), 0.0);
    }
    {
        ReadingManager manager;

        ASSERT_EQUAL(manager.Cheer(1), 0.0);
        ASSERT_EQUAL(manager.Cheer(1), 0.0);
        manager.Read(1, 1);
        ASSERT_EQUAL(manager.Cheer(1), 1.0);
        ASSERT_EQUAL(manager.Cheer(1), 1.0);

        ASSERT_EQUAL(manager.Cheer(2), 0);
    }
    {
        const int MAX_USER_COUNT = 100'000;
        ReadingManager manager;
        ASSERT_EQUAL(manager.Cheer(MAX_USER_COUNT), 0.0);
        manager.Read(MAX_USER_COUNT, 1);
        ASSERT_EQUAL(manager.Cheer(MAX_USER_COUNT), 1.0);
    }
    {
        ReadingManager manager;
        ASSERT_EQUAL(manager.Cheer(1), 0.0);
        manager.Read(1, 1000);
        ASSERT_EQUAL(manager.Cheer(1), 1.0);
    }
    {
        const int MAX_USER_COUNT = 100'000;
        ReadingManager manager;
        ASSERT_EQUAL(manager.Cheer(MAX_USER_COUNT), 0.0);
        manager.Read(MAX_USER_COUNT, 1000);
        ASSERT_EQUAL(manager.Cheer(MAX_USER_COUNT), 1.0);
    }
    {
        ReadingManager manager;
        ASSERT_EQUAL(manager.Cheer(0), 0.0);
        manager.Read(0, 1000);
        ASSERT_EQUAL(manager.Cheer(0), 1.0);
    }
    {// from coursera (https://www.coursera.org/learn/c-plus-plus-red/programming/e72ST/eliektronnaia-knigha/discussions/threads/hLK3cT0AEemHtRKqiMW_lA)
        ReadingManager manager;
        manager.Read(1, 1);
        manager.Read(1, 3);
        manager.Read(2, 2);
        ASSERT_EQUAL(manager.Cheer(1), 1.0);
        ASSERT_EQUAL(manager.Cheer(2), 0.0);
    }
}

int main() {
	//getSortedNumPages();
	getTime();
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, TestReadPage);

	// Для ускорения чтения данных отключается синхронизация
	// cin и cout с stdio,
	// а также выполняется отвязка cin от cout
	/*ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ReadingManager manager;

	int query_count;
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) {
		string query_type;
		cin >> query_type;
		int user_id;
		cin >> user_id;

		if (query_type == "READ") {
			int page_count;
			cin >> page_count;
			manager.Read(user_id, page_count);
		}
		else if (query_type == "CHEER") {
			cout << setprecision(6) << manager.Cheer(user_id) << "\n";
		}
	}*/

	return 0;
}