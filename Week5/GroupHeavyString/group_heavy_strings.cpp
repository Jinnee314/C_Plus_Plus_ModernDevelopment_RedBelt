#include "MyUnitTestFreimwork.hpp"
#include "LogDuration.hpp"

#include <algorithm>
#include <string>
#include <vector>

using namespace std;

// Объявляем Group<String> для произвольного типа String
// синонимом vector<String>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор групп — vector<Group<String>>.
template <typename String>
using Group = vector<String>;

// Ещё один шаблонный синоним типа
// позволяет вместо громоздкого typename String::value_type
// использовать Char<String>
template <typename String>
using Char = typename String::value_type;

template <typename String>
String createKey(const String& str)
{
	String copy = str;
	sort(copy.begin(), copy.end());
	copy.erase(unique(copy.begin(), copy.end()), copy.end());
	return copy;
}

template <typename String>
vector<Group<String>> mGroupHeavyStrings(vector<String> strings) 
{
	// Напишите реализацию функции,
	// использовав не более 1 копирования каждого символа
	map<String, Group<String>> groups;
	for (auto& el : strings)
	{
		groups[createKey(el)].push_back(move(el));
	}

	vector<Group<String>> res;
	//res.reserve(groups.size());
	for (auto& [key, group] : groups)
	{
		res.push_back(move(group));
	}

	return res;
}

template <typename String>
using Key = String;

template <typename String>
Key<String> ComputeStringKey(const String& string) {
	String chars = string;
	sort(begin(chars), end(chars));
	chars.erase(unique(begin(chars), end(chars)), end(chars));
	return chars;
}


template <typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
	map<Key<String>, Group<String>> groups_map;
	for (String& string : strings) {
		groups_map[ComputeStringKey(string)].push_back(move(string));
	}
	vector<Group<String>> groups;
	for (auto& [key, group] : groups_map) {
		groups.push_back(move(group));
	}
	return groups;
}

void TestGroupingABC() {
	vector<string> strings = { "caab", "abc", "cccc", "bacc", "c" };
	auto groups = GroupHeavyStrings(strings);
	ASSERT_EQUAL(groups.size(), 2);
	sort(begin(groups), end(groups));  // Порядок групп не имеет значения
	ASSERT_EQUAL(groups[0], vector<string>({ "caab", "abc", "bacc" }));
	ASSERT_EQUAL(groups[1], vector<string>({ "cccc", "c" }));
}

void TestGroupingReal() {
	vector<string> strings = { "law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl" };
	auto groups = GroupHeavyStrings(strings);
	ASSERT_EQUAL(groups.size(), 4);
	sort(begin(groups), end(groups));  // Порядок групп не имеет значения
	ASSERT_EQUAL(groups[0], vector<string>({ "laptop", "paloalto" }));
	ASSERT_EQUAL(groups[1], vector<string>({ "law", "wall", "awl" }));
	ASSERT_EQUAL(groups[2], vector<string>({ "port" }));
	ASSERT_EQUAL(groups[3], vector<string>({ "top", "pot" }));
}

void testTime()
{
	const size_t SIZE_VEC = 100'000;
	const size_t SIZE_STR = 100;
	vector<string> vec(SIZE_VEC);

	for (size_t i = 0; i < SIZE_VEC; ++i)
	{
		vec[i] = string(SIZE_STR, 'a' + i % 25);
	}


	{
		LOG_DURATION("group heavy strings");
		auto groups = GroupHeavyStrings(vec);
	}
	{
		LOG_DURATION("group heavy strings");
		auto groups = GroupHeavyStrings(vec);
	}
	{
		LOG_DURATION("group heavy strings");
		auto groups = GroupHeavyStrings(vec);
	}
	{
		LOG_DURATION("group heavy strings");
		auto groups = GroupHeavyStrings(vec);
	}

	{
		LOG_DURATION("my group heavy strings");
		auto groups = mGroupHeavyStrings(vec);
	}
	{
		LOG_DURATION("my group heavy strings");
		auto groups = mGroupHeavyStrings(vec);
	}
	{
		LOG_DURATION("my group heavy strings");
		auto groups = mGroupHeavyStrings(vec);
	}
	{
		LOG_DURATION("my group heavy strings");
		auto groups = mGroupHeavyStrings(vec);
	}


}

int main() {
	//MyUnitTest::TestRunner tr;
	//RUN_TEST(tr, TestGroupingABC);
	//RUN_TEST(tr, TestGroupingReal);
	testTime();
	return 0;
}
