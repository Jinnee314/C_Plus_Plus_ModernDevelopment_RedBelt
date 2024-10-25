#include "LogDuration.hpp"
#include "MyUnitTestFreimwork.hpp"

#include <string>
#include <vector>
#include <string_view>
#include <deque>
#include <map>

using namespace std;

using dict = map<string_view, string_view>;

class Translator {
public:
	void Add(string_view source, string_view target)
	{
		const string_view f(saveString(source));
		const string_view b(saveString(target));
		forward[f] = b;
		backward[b] = f;
	}
	string_view TranslateForward(string_view source) const
	{
		return forward.contains(source) ? forward.at(source) : "";
	}
	string_view TranslateBackward(string_view target) const
	{
		return backward.contains(target) ? backward.at(target) : "";
	}

private:
	string_view saveString(string_view sv)
	{
		for (const auto* m : {&forward, &backward})
		{
			auto it = m->find(sv);
			if (it != m->end())
			{
				return it->first;
			}
		}

		return dataBase.emplace_back(sv);
	}

	deque<string> dataBase;
	dict forward;
	dict backward;
};

void TestSimple() {
	Translator translator;
	ASSERT_EQUAL(translator.TranslateForward("okno"), "");
	ASSERT_EQUAL(translator.TranslateBackward("table"), "");

	translator.Add(string("okno"), string("window"));
	translator.Add(string("stol"), string("table"));

	ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
	ASSERT_EQUAL(translator.TranslateForward("window"), "");
	ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
	ASSERT_EQUAL(translator.TranslateBackward("stol"), "");

	translator.Add(string("okno"), string("otherWindow"));
	translator.Add(string("otherStol"), string("table"));
	ASSERT_EQUAL(translator.TranslateForward("okno"), "otherWindow");
	ASSERT_EQUAL(translator.TranslateBackward("table"), "otherStol");
}

int main() {
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, TestSimple);
	return 0;
}
