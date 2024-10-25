#include "MyUnitTestFreimwork.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
	T* Allocate()
	{
		T* res;
		if (free.empty())
		{
			res = free.front();
			free.pop();
		}
		else
		{
			res = new T;
		}

		busy.insert(res);
		return res;
	}
	T* TryAllocate()
	{
		if (free.empty())
		{
			return nullptr;
		}

		return Allocate();
	}

	void Deallocate(T* object)
	{
		auto it = busy.find(object);

		if (it == busy.end())
		{
			throw invalid_argument("No object.");
		}

		free.push(*it);
		busy.erase(it);
	}

	~ObjectPool()
	{
		for (const auto& a : busy)
		{
			delete a;
		}

		while (!free.empty())
		{
			delete free.front();
			free.pop();
		}
	}

private:
	// Добавьте сюда поля
	set<T*> busy;
	queue<T*> free;
};

void TestObjectPool() {
	ObjectPool<string> pool;

	auto p1 = pool.Allocate();
	auto p2 = pool.Allocate();
	auto p3 = pool.Allocate();

	*p1 = "first";
	*p2 = "second";
	*p3 = "third";

	pool.Deallocate(p2);
	ASSERT_EQUAL(*pool.Allocate(), "second");

	pool.Deallocate(p3);
	pool.Deallocate(p1);
	ASSERT_EQUAL(*pool.Allocate(), "third");
	ASSERT_EQUAL(*pool.Allocate(), "first");

	pool.Deallocate(p1);
}

int main() {
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, TestObjectPool);

	return 0;
}
