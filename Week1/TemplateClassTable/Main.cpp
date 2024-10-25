#include "MyUnitTestFreimwork.hpp"

#include <vector>

using namespace std;

// Реализуйте здесь шаблонный класс Table
template<typename T>
class Table
{
private:
	vector<vector<T>> table;

public:
	Table(size_t numStr, size_t numCol) : table(vector<vector<T>>(numStr, vector<T>(numCol))) {}

	vector<T>& operator[](size_t i)
	{
		return table[i];
	}

	const vector<T>& operator[](size_t i) const
	{
		return table[i];
	}

	void Resize(size_t newNumStr, size_t newNumCol)
	{
		for (auto& str : table)
		{
			str.resize(newNumCol);
		}

		table.resize(newNumStr, vector<T>(newNumCol));
	}

	pair<size_t, size_t> Size() const
	{
		return make_pair(table.size(), table.empty() ? 0 : table.front().size());
	}
};

void TestTable() {
	Table<int> tt(0,0);
	ASSERT_EQUAL(tt.Size().first, 0);
	ASSERT_EQUAL(tt.Size().second, 0);
	Table<int> t(1, 1);
	ASSERT_EQUAL(t.Size().first, 1u);
	ASSERT_EQUAL(t.Size().second, 1u);
	t[0][0] = 42;
	ASSERT_EQUAL(t[0][0], 42);
	t.Resize(3, 4);
	ASSERT_EQUAL(t.Size().first, 3u);
	ASSERT_EQUAL(t.Size().second, 4u);
	ASSERT_EQUAL(t[0][0], 42);
}

int main() {
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, TestTable);
	return 0;
}
