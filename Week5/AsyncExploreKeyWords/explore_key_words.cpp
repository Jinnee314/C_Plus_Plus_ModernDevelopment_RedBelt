#include "MyUnitTestFreimwork.hpp"
#include "LogDuration.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include <iterator>
#include <future>
#include <array>
#include <string_view>

using namespace std;

struct Stats {
	map<string, int> word_frequences;

	void operator += (const Stats& other)
	{
		for (const auto& [word, count] : other.word_frequences)
		{
			word_frequences[word] += count;
		}
	}
};

vector<string_view> getWords(string_view line)
{
	vector<string_view> words;

	auto beginWord = line.find_first_not_of(' ');
	line.remove_prefix(beginWord);
	while (!line.empty())
	{
		auto endWord = line.find(' ');
		words.push_back(line.substr(0, endWord));
		if (endWord == line.npos) {
			if (words.back().back() == '\n')
			{
				words.back().remove_suffix(1);
			}
			break;
		}
		else {
			line.remove_prefix(endWord + 1);
		}
	}

	return words;
}

template<size_t numThread>
array<set<string>, numThread> paginate(const set<string>& keyWords)
{
	size_t sizeSet = keyWords.size();
	int numPage = static_cast<int>(numThread);
	array<set<string>, numThread> result;
	auto beginPage = keyWords.begin();

	for (size_t i = 0; numPage != 0; --numPage, ++i)
	{
		size_t sizePage = sizeSet % numPage == 0 ?
			sizeSet / numPage :
			sizeSet / numPage + 1;

		auto endPage = next(beginPage, sizePage);
		result[i] = set<string>(beginPage, endPage);
		beginPage = endPage;
		sizeSet -= sizePage;
	}

	return result;
}


Stats ExploreLine(const set<string>& key_words, const string& line)
{
	Stats stat;

	for (auto& word : getWords(line))
	{
		auto key = key_words.find(string(move(word)));
		if (key != key_words.end())
		{
			++stat.word_frequences[*key];
		}
	}

	return stat;
}

Stats ExploreKeyWordsSingleThread(
	const set<string>& key_words, istream& input
) {
	Stats result;
	for (string line; getline(input, line); ) {
		result += ExploreLine(key_words, line);
	}
	return result;
}

Stats ExploreKeyWordsFromLines(
	const set<string>& key_words, const vector<string>& lines
) {
	Stats result;
	for (const auto& line : lines) {
		result += ExploreLine(key_words, line);
	}
	return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
	// Реализуйте эту функцию
	const size_t MAX_BATCH_SIZE = 5000;
	
	vector<string> batch;
	batch.reserve(MAX_BATCH_SIZE);

	vector<future<Stats>> resultsThread;

	for (string line; getline(input, line); )
	{
		batch.push_back(move(line));
		if (batch.size() >= MAX_BATCH_SIZE)
		{
			resultsThread.push_back(
				async(ExploreKeyWordsFromLines, ref(key_words), move(batch))
			);
			batch.reserve(MAX_BATCH_SIZE);
		}
	}

	Stats res;

	if (!batch.empty())
	{
		res += ExploreKeyWordsFromLines(key_words, move(batch));
	}

	for (auto& f : resultsThread)
	{
		res += f.get();
	}

	return res;
}

void testGetWords()
{
	{
		string str = "aaaa bbbb cccc";
		auto vec = getWords(str);
		vector<string_view> res{ "aaaa", "bbbb", "cccc" };
		ASSERT_EQUAL(vec, res);
	}
}

void testPaginate2Page()
{
	const size_t numThreads = 2;
	{
		const set<string> key_words = { "yangle", "rocks", "sucks", "all" };
		auto arr = paginate<numThreads>(key_words);
		array<set<string>, numThreads> res(
			set<string>(key_words.begin(), next(key_words.begin(), 2)),
			set<string>(next(key_words.begin(), 2), key_words.end())
		);
		ASSERT_EQUAL(arr, res);
	}
	{
		const set<string> key_words = { "yangle", "rocks", "sucks", "all", "degg" };
		auto arr = paginate<numThreads>(key_words);
		array<set<string>, numThreads> res(
			set<string>(key_words.begin(), next(key_words.begin(), 3)),
			set<string>(next(key_words.begin(), 3), key_words.end())
		);
		ASSERT_EQUAL(arr, res);
	}
	{
		const set<string> key_words = { "yangle", "rocks", "sucks" };
		auto arr = paginate<numThreads>(key_words);
		array<set<string>, numThreads> res(
			set<string>(key_words.begin(), next(key_words.begin(), 2)),
			set<string>(next(key_words.begin(), 2), key_words.end())
		);
		ASSERT_EQUAL(arr, res);
	}
}

void testExploreLine()
{
	{
		const set<string> key_words = { "yangle", "rocks", "sucks", "all" };
		string str = "this new yangle service really rocks\n";
		auto res = ExploreLine(key_words, str);
		const map<string, int> expected = {
			{"yangle", 1},
			{"rocks", 1}
		};
		ASSERT_EQUAL(res.word_frequences, expected);
	}
}

void TestBasic() {
	const set<string> key_words = { "yangle", "rocks", "sucks", "all" };

	stringstream ss;
	ss << "this new yangle service really rocks\n";
	ss << "It sucks when yangle isn't available\n";
	ss << "10 reasons why yangle is the best IT company\n";
	ss << "yangle rocks others suck\n";
	ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

	const auto stats = ExploreKeyWords(key_words, ss);
	const map<string, int> expected = {
	  {"yangle", 6},
	  {"rocks", 2},
	  {"sucks", 1}
	};
	ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, testGetWords);
	RUN_TEST(tr, testPaginate2Page);
	RUN_TEST(tr, testExploreLine);
	RUN_TEST(tr, TestBasic);
}
