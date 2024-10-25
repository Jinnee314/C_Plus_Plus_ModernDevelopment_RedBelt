#include "MyUnitTestFreimwork.hpp"

#include <algorithm>
#include <memory>
#include <vector>
#include <iterator>
#include <utility>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
	auto size = distance(range_begin, range_end);
	if (size < 2)
	{
		return;
	}

	vector<typename RandomIt::value_type> vec(make_move_iterator(range_begin), make_move_iterator(range_end));

	const size_t sizePart = size / 3;
	auto secondThird = next(begin(vec), sizePart);
	auto thirdThird = next(secondThird, sizePart);

	MergeSort(begin(vec), secondThird);
	MergeSort(secondThird, thirdThird);
	MergeSort(thirdThird, end(vec));

	vector<typename RandomIt::value_type> tmp;

	merge(
		make_move_iterator(begin(vec)),
		make_move_iterator(secondThird),
		make_move_iterator(secondThird),
		make_move_iterator(thirdThird),
		back_inserter(tmp)
	);

	merge(
		make_move_iterator(begin(tmp)),
		make_move_iterator(end(tmp)),
		make_move_iterator(thirdThird),
		make_move_iterator(end(vec)),
		range_begin
	);
}

void TestIntVector() {
	vector<int> numbers = { 6, 1, 3, 9, 1, 9, 8, 12, 1 };
	MergeSort(begin(numbers), end(numbers));
	ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, TestIntVector);
	return 0;
}
