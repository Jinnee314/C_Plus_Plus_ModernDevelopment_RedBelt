#include "search_server.h"
#include "parse.h"
#include "MyUnitTestFreimwork.hpp"
#include "LogDuration.hpp"

#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <random>
#include <thread>
using namespace std;

void TestFunctionality(
	const vector<string>& docs,
	const vector<string>& queries,
	const vector<string>& expected
) {
	istringstream docs_input(Join('\n', docs));
	istringstream queries_input(Join('\n', queries));

	SearchServer srv;
	srv.UpdateDocumentBase(docs_input);
	ostringstream queries_output;
	srv.AddQueriesStream(queries_input, queries_output);

	const string result = queries_output.str();
	const auto lines = SplitBy(Strip(result), '\n');
	ASSERT_EQUAL(lines.size(), expected.size());
	for (size_t i = 0; i < lines.size(); ++i) {
		ASSERT_EQUAL(lines[i], expected[i]);
	}
}

void TestSerpFormat() {
	const vector<string> docs = {
	  "london is the capital of great britain",
	  "i am travelling down the river"
	};
	const vector<string> queries = { "london", "the" };
	const vector<string> expected = {
	  "london: {docid: 0, hitcount: 1}",
	  Join(' ', vector{
		"the:",
		"{docid: 0, hitcount: 1}",
		"{docid: 1, hitcount: 1}"
	  })
	};

	TestFunctionality(docs, queries, expected);
}

void TestTop5() {
	const vector<string> docs = {
	  "milk a",
	  "milk b",
	  "milk c",
	  "milk d",
	  "milk e",
	  "milk f",
	  "milk g",
	  "water a",
	  "water b",
	  "fire and earth"
	};

	const vector<string> queries = { "milk", "water", "rock" };
	const vector<string> expected = {
	  Join(' ', vector{
		"milk:",
		"{docid: 0, hitcount: 1}",
		"{docid: 1, hitcount: 1}",
		"{docid: 2, hitcount: 1}",
		"{docid: 3, hitcount: 1}",
		"{docid: 4, hitcount: 1}"
	  }),
	  Join(' ', vector{
		"water:",
		"{docid: 7, hitcount: 1}",
		"{docid: 8, hitcount: 1}",
	  }),
	  "rock:",
	};
	TestFunctionality(docs, queries, expected);
}

void TestHitcount() {
	const vector<string> docs = {
	  "the river goes through the entire city there is a house near it",
	  "the wall",
	  "walle",
	  "is is is is",
	};
	const vector<string> queries = { "the", "wall", "all", "is", "the is" };
	const vector<string> expected = {
	  Join(' ', vector{
		"the:",
		"{docid: 0, hitcount: 2}",
		"{docid: 1, hitcount: 1}",
	  }),
	  "wall: {docid: 1, hitcount: 1}",
	  "all:",
	  Join(' ', vector{
		"is:",
		"{docid: 3, hitcount: 4}",
		"{docid: 0, hitcount: 1}",
	  }),
	  Join(' ', vector{
		"the is:",
		"{docid: 3, hitcount: 4}",
		"{docid: 0, hitcount: 3}",
		"{docid: 1, hitcount: 1}",
	  }),
	};
	TestFunctionality(docs, queries, expected);
}

void TestRanking() {
	const vector<string> docs = {
	  "london is the capital of great britain",
	  "paris is the capital of france",
	  "berlin is the capital of germany",
	  "rome is the capital of italy",
	  "madrid is the capital of spain",
	  "lisboa is the capital of portugal",
	  "bern is the capital of switzerland",
	  "moscow is the capital of russia",
	  "kiev is the capital of ukraine",
	  "minsk is the capital of belarus",
	  "astana is the capital of kazakhstan",
	  "beijing is the capital of china",
	  "tokyo is the capital of japan",
	  "bangkok is the capital of thailand",
	  "welcome to moscow the capital of russia the third rome",
	  "amsterdam is the capital of netherlands",
	  "helsinki is the capital of finland",
	  "oslo is the capital of norway",
	  "stockgolm is the capital of sweden",
	  "riga is the capital of latvia",
	  "tallin is the capital of estonia",
	  "warsaw is the capital of poland",
	};

	const vector<string> queries = { "moscow is the capital of russia" };
	const vector<string> expected = {
	  Join(' ', vector{
		"moscow is the capital of russia:",
		"{docid: 7, hitcount: 6}",
		"{docid: 14, hitcount: 6}",
		"{docid: 0, hitcount: 4}",
		"{docid: 1, hitcount: 4}",
		"{docid: 2, hitcount: 4}",
	  })
	};
	TestFunctionality(docs, queries, expected);
}

void TestBasicSearch() {
	const vector<string> docs = {
	  "we are ready to go",
	  "come on everybody shake you hands",
	  "i love this game",
	  "just like exception safety is not about writing try catch everywhere in your code move semantics are not about typing double ampersand everywhere in your code",
	  "daddy daddy daddy dad dad dad",
	  "tell me the meaning of being lonely",
	  "just keep track of it",
	  "how hard could it be",
	  "it is going to be legen wait for it dary legendary",
	  "we dont need no education"
	};

	const vector<string> queries = {
	  "we need some help",
	  "it",
	  "i love this game",
	  "tell me why",
	  "dislike",
	  "about"
	};

	const vector<string> expected = {
	  Join(' ', vector{
		"we need some help:",
		"{docid: 9, hitcount: 2}",
		"{docid: 0, hitcount: 1}"
	  }),
	  Join(' ', vector{
		"it:",
		"{docid: 8, hitcount: 2}",
		"{docid: 6, hitcount: 1}",
		"{docid: 7, hitcount: 1}",
	  }),
	  "i love this game: {docid: 2, hitcount: 4}",
	  "tell me why: {docid: 5, hitcount: 2}",
	  "dislike:",
	  "about: {docid: 3, hitcount: 2}",
	};
	TestFunctionality(docs, queries, expected);
}

void TestSearchServer(istream& document_input, istream& query_input,
	ostream& search_results_output) {
	SearchServer srv(document_input);
	srv.AddQueriesStream(query_input, search_results_output);
}

const size_t MAX_NUM_DOC = 1'000; //50'000
const size_t MAX_NUM_WORDS = 500; // 1'000
const size_t MAX_SIZE_WORD = 100;
const size_t MAX_NUM_DIFF_WORDS_IN_ALL_DOCS = 15000;
const size_t MAX_NUM_QUERY = 200; // 500'000
const size_t MAX_WORDS_IN_QUERY = 5;
const size_t NUM_LETTERS = 25;

vector<string> createDocs()
{
	vector<string> docs(MAX_NUM_DOC);

	for (size_t i = 0; i < MAX_NUM_DOC; ++i)
	{
		docs[i] += string(MAX_SIZE_WORD, 'a' + (i % 25));
		for (size_t j = 1; j < MAX_NUM_WORDS; ++j)
		{
			docs[i] += " " + string(MAX_SIZE_WORD, 'a' + ((j + i)  % 25));
		}
	}

	return docs;
}

vector<string> createQueris()
{
	vector<string> querys(MAX_NUM_QUERY);

	for (size_t i = 0; i < MAX_NUM_QUERY; ++i)
	{
		querys[i] += string(MAX_SIZE_WORD, 'a' + (i % 25));
		for (size_t j = 1; j < MAX_WORDS_IN_QUERY; ++j)
		{
			querys[i] += " " + string(MAX_SIZE_WORD, 'a' + ((j + i) % 25));
		}
	}

	return querys;
}

void testTime()
{
	cout << "start\n";
	/*{
		ifstream docs("docs.txt");
		SearchServer s(docs);

		{
			ifstream qyeris("queris.txt");
			ostringstream sout;
			LOG_DURATION("find");
			s.AddQueriesStream(qyeris, sout);
		}
		{
			ifstream qyeris("queris.txt");
			ostringstream sout;
			LOG_DURATION("find");
			s.AddQueriesStream(qyeris, sout);
		}
		{
			ifstream qyeris("queris.txt");
			ostringstream sout;
			LOG_DURATION("find");
			s.AddQueriesStream(qyeris, sout);
		}
		{
			ifstream qyeris("queris.txt");
			ostringstream sout;
			LOG_DURATION("find");
			s.AddQueriesStream(qyeris, sout);
		}
		{
			ifstream qyeris("queris.txt");
			ostringstream sout;
			LOG_DURATION("find");
			s.AddQueriesStream(qyeris, sout);
		}

		ifstream qyeris("queris.txt");
		ostringstream sout;
		s.AddQueriesStream(qyeris, cout);
	}*/
	/*{
		ifstream docs("docs.txt");
		LOG_DURATION("create list");
		SearchServer s(docs);
	}
	{
		ifstream docs("docs.txt");
		LOG_DURATION("create list");
		SearchServer s(docs);
	}
	{
		ifstream docs("docs.txt");
		LOG_DURATION("create list");
		SearchServer s(docs);
	}
	{
		ifstream docs("docs.txt");
		LOG_DURATION("create list");
		SearchServer s(docs);
	}
	{
		ifstream docs("docs.txt");
		LOG_DURATION("create list");
		SearchServer s(docs);
	}
	{
		ifstream docs("docs.txt");
		LOG_DURATION("create list");
		SearchServer s(docs);
	}*/
	{
		ifstream docs("docs.txt");
		ifstream qyeris("queris.txt");
		ostringstream sout;
		LOG_DURATION("time all");
		TestSearchServer(docs, qyeris, sout);
	}
	{
		ifstream docs("docs.txt");
		ifstream qyeris("queris.txt");
		ostringstream sout;
		LOG_DURATION("time all");
		TestSearchServer(docs, qyeris, sout);
	}
	{
		ifstream docs("docs.txt");
		ifstream qyeris("queris.txt");
		ostringstream sout;
		LOG_DURATION("time all");
		TestSearchServer(docs, qyeris, sout);
	}
	{
		ifstream docs("docs.txt");
		ifstream qyeris("queris.txt");
		ostringstream sout;
		LOG_DURATION("time all");
		TestSearchServer(docs, qyeris, sout);
	}
	{
		ifstream docs("docs.txt");
		ifstream qyeris("queris.txt");
		ostringstream sout;
		LOG_DURATION("time all");
		TestSearchServer(docs, qyeris, sout);
	}
}

/*
	create 1000 docs 500 words 100 symbs in word: 12187 ms

	create 1000 docs 500 words 100 symbs in word after use string_view: 6411 ms

	create 1000 docs 500 words 100 symbs in word after use string_view and vector: 6164 ms


	find 200 query with 5 words 100 sums in word: 7316

	find 200 query with 5 words 100 sums in word my version: 601

	time test: 29474 ms
	
	time test after optimization first: 7159
	
	time test after optimization second: 6258

	time test my realization: 7818 ms
*/

int main() {
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, TestSerpFormat);
	RUN_TEST(tr, TestTop5);
	RUN_TEST(tr, TestHitcount);
	RUN_TEST(tr, TestRanking);
	RUN_TEST(tr, TestBasicSearch);
	
	testTime();

	/*auto docs(createQueris());
	ofstream out("queris.txt");
	bool first = true;
	for (size_t i = 0; i < docs.size(); ++i)
	{
		if (!first)
		{
			out << '\n';
		}
		first = false;
		out << docs[i];
	}

	out.close();*/
}
