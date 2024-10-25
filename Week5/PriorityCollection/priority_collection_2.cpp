#include "MyUnitTestFreimwork.hpp"
#include "LogDuration.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <list>


using namespace std;

//template <typename T>
//class PriorityCollection {
//public:
//	using Id = size_t; /* тип, используемый для идентификаторов */;
//
//	// Добавить объект с нулевым приоритетом
//	// с помощью перемещения и вернуть его идентификатор
//	Id Add(T object)
//	{
//		Id id = data.size();
//		data.push_back({ move(object), 0 });
//		if (data[idMaxElem].second == 0)
//		{
//			idMaxElem = id;
//		}
//		return id;
//	}
//
//	// Добавить все элементы диапазона [range_begin, range_end)
//	// с помощью перемещения, записав выданные им идентификаторы
//	// в диапазон [ids_begin, ...)
//	template <typename ObjInputIt, typename IdOutputIt>
//	void Add(ObjInputIt range_begin, ObjInputIt range_end,
//		IdOutputIt ids_begin)
//	{
//		for (; range_begin != range_end; ++range_begin)
//		{
//			*ids_begin = data.size();
//			++ids_begin;
//			data.push_back({ move(*range_begin), 0 });
//		}
//		if (data[idMaxElem].second == 0)
//		{
//			idMaxElem = data.size() - 1;
//		}
//	}
//
//	// Определить, принадлежит ли идентификатор какому-либо
//	// хранящемуся в контейнере объекту
//	bool IsValid(Id id) const
//	{
//		return id < data.size();
//	}
//
//	// Получить объект по идентификатору
//	const T& Get(Id id) const
//	{
//		return data[id];
//	}
//
//	// Увеличить приоритет объекта на 1
//	void Promote(Id id)
//	{
//		++data[id].second;
//		if ((data[id].second == data[idMaxElem].second && id > idMaxElem) || data[id].second > data[idMaxElem].second)
//		{
//			idMaxElem = id;
//		}
//	}
//
//	// Получить объект с максимальным приоритетом и его приоритет
//	pair<const T&, int> GetMax() const
//	{
//		return data[idMaxElem];
//	}
//
//	// Аналогично GetMax, но удаляет элемент из контейнера
//	pair<T, int> PopMax()
//	{
//		pair<T, int> max = eraseMax();		
//
//		for (int32_t i = static_cast<int32_t>(idMaxElem); i > -1; --i)
//		{
//			if (data[i].second > data[idMaxElem].second)
//			{
//				idMaxElem = i;
//				if (data[idMaxElem].second == static_cast<uint32_t>(max.second))
//				{
//					break;
//				}
//			}			
//		}
//
//		return max;
//	}
//
//private:
//	// Приватные поля и методы
//	vector<pair<T, uint32_t>> data;
//	Id idMaxElem;
//
//	pair<T, int> eraseMax()
//	{
//		pair<T, int> max = move(data[idMaxElem]);
//		
//		for (size_t i = idMaxElem + 1; i < data.size(); ++i)
//		{
//			data[i - 1] = move(data[i]);
//		}
//
//		data.pop_back();
//
//		if (idMaxElem == data.size())
//		{
//			--idMaxElem;
//		}
//		return max;
//	}
//};

//template <typename T>
//class PriorityCollection {
//public:
//	using Id = list<pair<T, uint32_t>>::iterator; /* тип, используемый для идентификаторов */;
//
//	// Добавить объект с нулевым приоритетом
//	// с помощью перемещения и вернуть его идентификатор
//	Id Add(T object)
//	{
//		data.push_back({ move(object), 0 });
//		if (idMaxElem == data.end() || idMaxElem->second == 0)
//		{
//			idMaxElem = prev(data.end());
//		}
//		return prev(data.end());
//	}
//
//	// Добавить все элементы диапазона [range_begin, range_end)
//	// с помощью перемещения, записав выданные им идентификаторы
//	// в диапазон [ids_begin, ...)
//	template <typename ObjInputIt, typename IdOutputIt>
//	void Add(ObjInputIt range_begin, ObjInputIt range_end,
//		IdOutputIt ids_begin)
//	{
//		for (; range_begin != range_end; ++range_begin, ++ids_begin)
//		{
//			data.push_back({ move(*range_begin), 0 });
//			*ids_begin = prev(data.end());
//		}
//		if (idMaxElem == data.end() || idMaxElem->second == 0)
//		{
//			idMaxElem = prev(data.end());
//		}
//	}
//
//	// Определить, принадлежит ли идентификатор какому-либо
//	// хранящемуся в контейнере объекту
//	bool IsValid(Id id) const
//	{
//		for (auto it = data.begin(); it != data.end(); ++it)
//		{
//			if (it == id) {
//				return true;
//			}
//		}
//		return false;
//	}
//
//	// Получить объект по идентификатору
//	const T& Get(Id id) const
//	{
//		return id->first;
//	}
//
//	// Увеличить приоритет объекта на 1
//	void Promote(Id id)
//	{
//		++(id->second);
//		if (id->second > idMaxElem->second || (id->second == idMaxElem->second && static_cast<size_t>(distance(idMaxElem, id)) < data.size()))
//		{
//			idMaxElem = id;
//		}
//	}
//
//	// Получить объект с максимальным приоритетом и его приоритет
//	pair<const T&, int> GetMax() const
//	{
//		return *idMaxElem;
//	}
//
//	// Аналогично GetMax, но удаляет элемент из контейнера
//	pair<T, int> PopMax()
//	{
//		pair<T, int> max = eraseMax();
//
//		if (data.size() == 0)
//		{
//			return max;
//		}
//		
//		if (data.size() == 1)
//		{
//			idMaxElem = data.begin();
//			return max;
//		}
//
//		idMaxElem = prev(idMaxElem);
//		if (idMaxElem->second == max.second)
//		{
//			return max;
//		}
//
//		for (auto it = data.begin(); it != idMaxElem; ++it)
//		{
//			if (it->second >= idMaxElem->second)
//			{
//				idMaxElem = it;
//			}
//		}
//
//		return max;
//	}
//
//private:
//	// Приватные поля и методы
//	list<pair<T, uint32_t>> data;
//	Id idMaxElem = data.end();
//
//	pair<T, int> eraseMax()
//	{
//		pair<T, int> max = move(*idMaxElem);
//		idMaxElem = next(idMaxElem);
//		data.erase(prev(idMaxElem));
//
//		return max;
//	}
//};

template <typename T>
class PriorityCollection {
public:
	using Id = size_t;/* тип, используемый для идентификаторов */;

	// Добавить объект с нулевым приоритетом
	// с помощью перемещения и вернуть его идентификатор
	Id Add(T object)
	{
		Id id = data.size();
		data.push_back({ move(object) });
		elemsSortedByPriority.insert({ 0, id });
		return id;
	}

	// Добавить все элементы диапазона [range_begin, range_end)
	// с помощью перемещения, записав выданные им идентификаторы
	// в диапазон [ids_begin, ...)
	template <typename ObjInputIt, typename IdOutputIt>
	void Add(ObjInputIt range_begin, ObjInputIt range_end,
		IdOutputIt ids_begin)
	{
		for (; range_begin != range_end; ++range_begin, ++ids_begin)
		{
			*ids_begin = Add(move(*range_begin));
			
		}
	}

	// Определить, принадлежит ли идентификатор какому-либо
	// хранящемуся в контейнере объекту
	bool IsValid(Id id) const
	{
		return id >= 0 && id < data.size() && data[id].priority != nonPriority;
	}

	// Получить объект по идентификатору
	const T& Get(Id id) const 
	{
		return data[id];
	}

	// Увеличить приоритет объекта на 1
	void Promote(Id id)
	{
		auto& prior = data[id].priority;
		elemsSortedByPriority.erase({ prior, id });
		elemsSortedByPriority.insert({ ++prior, id });
	}

	// Получить объект с максимальным приоритетом и его приоритет
	pair<const T&, size_t> GetMax() const
	{
		return make_pair(data[elemsSortedByPriority.rbegin()->second].data, elemsSortedByPriority.rbegin()->first);
	}

	// Аналогично GetMax, но удаляет элемент из контейнера
	pair<T, size_t> PopMax()
	{
		auto& max = data[elemsSortedByPriority.rbegin()->second];
		auto priority = max.priority;
		max.priority = nonPriority;
		elemsSortedByPriority.erase(--elemsSortedByPriority.end());
		return make_pair(move(max.data), move(priority));
	}

private:
	// Приватные поля и методы
	struct Elem
	{
		T data;
		int priority = 0;
	};

	vector<Elem> data;
	set<pair<int, Id>> elemsSortedByPriority;
	int nonPriority = -1;
};

class StringNonCopyable : public string {
public:
	using string::string;  // Позволяет использовать конструкторы строки
	StringNonCopyable(const StringNonCopyable&) = delete;
	StringNonCopyable(StringNonCopyable&&) = default;
	StringNonCopyable& operator=(const StringNonCopyable&) = delete;
	StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
	PriorityCollection<StringNonCopyable> strings;
	const auto white_id = strings.Add("white");
	const auto yellow_id = strings.Add("yellow");
	const auto red_id = strings.Add("red");

	strings.Promote(yellow_id);
	for (int i = 0; i < 2; ++i) {
		strings.Promote(red_id);
	}
	strings.Promote(yellow_id);
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "red");
		ASSERT_EQUAL(item.second, 2);
	}
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "yellow");
		ASSERT_EQUAL(item.second, 2);
	}
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "white");
		ASSERT_EQUAL(item.second, 0);
	}
}

void testAddRange()
{
	vector<StringNonCopyable> strings;
	strings.push_back("11111");
	strings.push_back("22222");
	strings.push_back("33333");
	vector<PriorityCollection<StringNonCopyable>::Id> ids(3);
	PriorityCollection<StringNonCopyable> pc;

	pc.Add(strings.begin(), strings.end(), ids.begin());
	pc.Promote(ids[0]);
	for (int i = 0; i < 2; ++i) 
	{
		pc.Promote(ids[1]);
	}
	pc.Promote(ids[0]);
	{
		const auto item = pc.PopMax();
		ASSERT_EQUAL(item.first, "22222");
		ASSERT_EQUAL(item.second, 2);
	}
	{
		const auto item = pc.PopMax();
		ASSERT_EQUAL(item.first, "11111");
		ASSERT_EQUAL(item.second, 2);
	}
	{
		const auto item = pc.PopMax();
		ASSERT_EQUAL(item.first, "33333");
		ASSERT_EQUAL(item.second, 0);
	}
}

void testTime()
{

}

int main() {
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, TestNoCopy);
	RUN_TEST(tr, testAddRange);
	return 0;
}
