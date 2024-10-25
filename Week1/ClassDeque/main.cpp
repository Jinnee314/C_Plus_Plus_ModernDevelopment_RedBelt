#include "MyUnitTestFreimwork.hpp"
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <deque>

using namespace std;

template<class T>
class Deque
{
private:
	vector<T> front, back;

public:
	explicit Deque() {};
	explicit Deque(T elem) : back(vector<T>{elem}) {}
	explicit Deque(std::initializer_list<T> data) : back(data) {}

	bool Empty() const
	{
		return front.empty() && back.empty();
	}

	size_t Size() const
	{
		return front.size() + back.size();
	}

	T& operator[](size_t index)
	{
		if (index <= front.size() - 1 && !front.empty())
		{
			return front[front.size() - 1 - index];
		}
		else
		{
			return back[index - front.size()];
		}
	}
	const T& operator[](size_t index) const
	{
		if (index <= front.size() - 1 && !front.empty())
		{
			return front[front.size() - 1 - index];
		}
		else
		{
			return back[index - front.size()];
		}
	}

	T& At(size_t index)
	{
		if (index > this->Size())
		{
			std::string file{ __FILE__ };
			std::string line = to_string(__LINE__);
			throw std::out_of_range("index > deque size, " + file + ":" + line);
		}
		if (index <= front.size() - 1 && !front.empty())
		{
			return front.at(front.size() - 1 - index);
		}
		else
		{
			return back.at(index - front.size());
		}
	}
	const T& At(size_t index) const
	{
		if (index > this->Size())
		{
			std::string file{ __FILE__ };
			std::string line = to_string(__LINE__);
			throw std::out_of_range("index > deque size, " + file + ":" + line);
		}
		if (index <= front.size() - 1 && !front.empty())
		{
			return front.at(front.size() - 1 - index);
		}
		else
		{
			return back.at(index - front.size());
		}
	}

	T& Front()
	{
		return front.empty() ? return back.front() : return front.back();
	}
	const T& Front() const
	{
		return front.empty() ? return back.front() : return front.back();
	}

	T& Back()
	{
		return back.empty() ? front.front() : return back.back();
	}
	const T& Back() const
	{
		return back.empty() ? front.front() : return back.back();
	}

	void PushFront(const T& elem)
	{
		front.push_back(elem);
	}

	void PushBack(const T& elem)
	{
		back.push_back(elem);
	}
};


void testDeque()
{
	{
		Deque d('c');
		ASSERT_EQUAL(d[0], 'c');
	}
	{
		Deque d{ 5,8,9,10 };
		ASSERT_EQUAL(d[0], 5);
		ASSERT_EQUAL(d[3], 10);
		d[1] = 1;
		ASSERT_EQUAL(d[1], 1);
	}
	{
		Deque<int> d;
		ASSERT_EQUAL(d.Size(), 0);
	}
	{
		Deque d{ 4,5,6,7,8 };
		ASSERT_EQUAL(d.Size(), 5);
	}
	{
		Deque<int> d;
		ASSERT_EQUAL(d.Size(), 0);

		d.PushBack(2);
		ASSERT_EQUAL(d.Size(), 1);
		ASSERT_EQUAL(d[0], 2);

		d.PushFront(10);
		ASSERT_EQUAL(d.Size(), 2);
		ASSERT_EQUAL(d[0], 10);
		ASSERT_EQUAL(d[1], 2);

		d.PushBack(22);
		ASSERT_EQUAL(d[2], 22);
		ASSERT_EQUAL(d.Size(), 3);

		ASSERT_EQUAL(d.Front(), 10);
		ASSERT_EQUAL(d.Back(), 22);

		d.PushFront(9);
		d.PushBack(11);
		ASSERT_EQUAL(d.Front(), 9);
		ASSERT_EQUAL(d.Back(), 11);
		
		d.Front() = 111;
		d.Back() = 222;
		ASSERT_EQUAL(d.Front(), 111);
		ASSERT_EQUAL(d.Back(), 222);
	}
	{
		Deque d{ 0,1,2,3,4 };
		ASSERT_EQUAL(d.At(0), 0);
		ASSERT_EQUAL(d.At(2), 2);

		d.At(2) = 20;
		const auto num = d.At(2);
		ASSERT_EQUAL(d.At(2), num);
	}
	{
		Deque<int> d;
		ASSERT(d.Empty());
		d.PushFront(5);
		ASSERT(!d.Empty());
	}
}

void testAtException()
{
	Deque d{ 1,2,3,4,5 };
	ASSERT(d.At(0));
	ASSERT(d.At(10))
}

void testDequeOther()
{
	{
		Deque<int> const my_deque;
		deque<int> const exp_d;
		ASSERT_EQUAL(my_deque.Size(), exp_d.size());
		ASSERT_EQUAL(my_deque.Empty(), exp_d.empty());
	}
	{
		Deque<int> my_deque;
		deque<int> expected_d;
		my_deque.PushFront(1);
		expected_d.push_front(1);
		ASSERT_EQUAL(my_deque.Size(), expected_d.size());
		ASSERT_EQUAL(my_deque.Front(), expected_d.front());
		ASSERT_EQUAL(my_deque.Back(), expected_d.back());
		ASSERT_EQUAL(my_deque.At(0), expected_d.at(0));
		ASSERT_EQUAL(my_deque[0], expected_d[0]);
	}
	{
		Deque<int> my_deque;
		deque<int> exp_d;
		my_deque.PushBack(1);
		exp_d.push_back(1);
		ASSERT_EQUAL(my_deque.Size(), exp_d.size());
		ASSERT_EQUAL(my_deque.Front(), exp_d.front());
		ASSERT_EQUAL(my_deque.Back(), exp_d.back());
		ASSERT_EQUAL(my_deque.At(0), exp_d.at(0));
		ASSERT_EQUAL(my_deque[0], exp_d[0]);
	}
	{
		Deque<string> my_deque;
		deque<string> exp_d;
		my_deque.PushFront("One");
		my_deque.PushBack("Two");
		exp_d.emplace_front("One");
		exp_d.emplace_back("Two");
		ASSERT_EQUAL(my_deque.Size(), exp_d.size());
		ASSERT_EQUAL(my_deque.Front(), exp_d.front());
		ASSERT_EQUAL(my_deque.Back(), exp_d.back());
		ASSERT_EQUAL(my_deque.At(0), exp_d.at(0));
		ASSERT_EQUAL(my_deque.At(1), exp_d.at(1));
		ASSERT_EQUAL(my_deque[0], exp_d[0]);
		ASSERT_EQUAL(my_deque[1], exp_d[1]);
	}
	{
		Deque<int> my_deque;
		my_deque.PushFront(1);
		ASSERT_EQUAL(my_deque.Size(), 1U);
		ASSERT_EQUAL(my_deque.Front(), 1);
		ASSERT_EQUAL(my_deque.Back(), 1);
		string error;
		try {
			my_deque.At(3);
		}
		catch (out_of_range& er) {
			error = er.what();
			error = "catch error";
		}
		ASSERT_EQUAL(error, "catch error");
	}
	{
		Deque<int> my_deque;
		deque<double> exp_d;
		my_deque.PushFront(1);
		my_deque.PushBack(2);
		exp_d.push_front(1);
		exp_d.push_back(2);
		my_deque[0] = 3;
		my_deque[1] = 4;
		exp_d[0] = 3;
		exp_d[1] = 4;
		ASSERT_EQUAL(my_deque.Size(), exp_d.size());
		ASSERT_EQUAL(my_deque.Front(), exp_d.front());
		ASSERT_EQUAL(my_deque.Back(), exp_d.back());
		ASSERT_EQUAL(my_deque.At(0), exp_d.at(0));
		ASSERT_EQUAL(my_deque.At(1), exp_d.at(1));
		ASSERT_EQUAL(my_deque[0], exp_d[0]);
		ASSERT_EQUAL(my_deque[1], exp_d[1]);
	}
	{
		Deque<int> my_deque;
		deque<int> exp_d;
		my_deque.PushFront(1);
		my_deque.PushFront(2);
		my_deque.PushBack(3);
		my_deque.PushBack(4);
		exp_d.push_front(1);
		exp_d.push_front(2);
		exp_d.push_back(3);
		exp_d.push_back(4);
		ASSERT_EQUAL(my_deque.Size(), exp_d.size());
		ASSERT_EQUAL(my_deque.Front(), exp_d.front());
		ASSERT_EQUAL(my_deque.Back(), exp_d.back());
		ASSERT_EQUAL(my_deque.At(0), exp_d.at(0));
		ASSERT_EQUAL(my_deque.At(3), exp_d.at(3));
		ASSERT_EQUAL(my_deque[1], exp_d[1]);
		ASSERT_EQUAL(my_deque[3], exp_d[3]);
	}
}

int main()
{
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, testDeque);
	//RUN_TEST(tr, testAtException);
	RUN_TEST(tr, testDequeOther);

	return 0;
}