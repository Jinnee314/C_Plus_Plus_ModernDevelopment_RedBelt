#include "MyUnitTestFreimwork.hpp"

#include <cstdint>
#include <iterator>
#include <numeric>
#include <vector>
#include <iostream>

#include <list>
#include <algorithm>
#include <utility>

using namespace std;

//template <typename RandomIt>
//void cycleShiftRight(RandomIt first, RandomIt last)
//{
//    typename RandomIt::value_type tmp = move(*last);
//    for (auto it = last; it != first; --it)
//    {
//        *it = move(*prev(it));
//    }
//    *first = move(tmp);
//}

template <typename Container, typename BiDirIter>
BiDirIter loopIterator(Container& c, BiDirIter it)
{
	return it == c.end() ? c.begin() : it;
}
 
//template <typename Container, typename ForwardIt>
//ForwardIt LoopIterator(Container& container, ForwardIt pos) {
//	return pos == container.end() ? container.begin() : pos;
//}

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) 
{
	list<typename RandomIt::value_type> pool(make_move_iterator(first), make_move_iterator(last));
	/*for (auto it = first; it != last; ++it)
	{
		pool.push_back(move(*it));
	}*/


	auto curr = pool.begin();
	while (!pool.empty())
	{
		*(first++) = move(*curr);
		if (pool.size() == 1) {
			break;
		}
		const auto nextPos = loopIterator(pool, next(curr));
		pool.erase(curr);
		curr = nextPos;

		for (uint32_t i = 1; i < step_size; ++i)
		{
			curr = loopIterator(pool, next(curr));
		}
	}
}

//template <typename RandomIt>
//void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
//	list<typename RandomIt::value_type> pool;
//	for (auto it = first; it != last; ++it) {
//		pool.push_back(move(*it));
//	}
//	auto cur_pos = pool.begin();
//	while (!pool.empty()) {
//		*(first++) = move(*cur_pos);
//		if (pool.size() == 1) {
//			break;
//		}
//		const auto next_pos = LoopIterator(pool, next(cur_pos));
//		pool.erase(cur_pos);
//		cur_pos = next_pos;
//		for (uint32_t step_index = 1; step_index < step_size; ++step_index) {
//			cur_pos = LoopIterator(pool, next(cur_pos));
//		}
//	}
//}

vector<int> MakeTestVector() {
	vector<int> numbers(10);
	iota(begin(numbers), end(numbers), 0);
	return numbers;
}

void TestIntVector() {
	const vector<int> numbers = MakeTestVector();
	{
		vector<int> numbers_copy = numbers;
		MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
		ASSERT_EQUAL(numbers_copy, numbers);
	}
	{
		vector<int> numbers_copy = numbers;
		MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
		ASSERT_EQUAL(numbers_copy, vector<int>({ 0, 3, 6, 9, 4, 8, 5, 2, 7, 1 }));
	}
}

// ��� ����������� ���, ������� ������� ��� ���������, ��� ���� ����������
// ������� MakeJosephusPermutation �� ��������� ����������� ��������.
// ������ ��, ��������, �� ��������� ��� �� �������, ������ �� ���������,
// ������ �� ������� ������ ���, ����� � ����� ��� move-��������� �
// � ����� ������������� �����

struct NoncopyableInt {
	int value;

	NoncopyableInt(int value) : value(value) {}

	NoncopyableInt(const NoncopyableInt&) = delete;
	NoncopyableInt& operator=(const NoncopyableInt&) = delete;

	NoncopyableInt(NoncopyableInt&&) = default;
	NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator == (const NoncopyableInt& lhs, const NoncopyableInt& rhs) {
	return lhs.value == rhs.value;
}

ostream& operator << (ostream& os, const NoncopyableInt& v) {
	return os << v.value;
}

void TestAvoidsCopying() {
	vector<NoncopyableInt> numbers;
	numbers.push_back({ 1 });
	numbers.push_back({ 2 });
	numbers.push_back({ 3 });
	numbers.push_back({ 4 });
	numbers.push_back({ 5 });

	MakeJosephusPermutation(begin(numbers), end(numbers), 2);

	vector<NoncopyableInt> expected;
	expected.push_back({ 1 });
	expected.push_back({ 3 });
	expected.push_back({ 5 });
	expected.push_back({ 4 });
	expected.push_back({ 2 });

	ASSERT_EQUAL(numbers, expected);
}

int main() {
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, TestIntVector);
	RUN_TEST(tr, TestAvoidsCopying);
	return 0;
}