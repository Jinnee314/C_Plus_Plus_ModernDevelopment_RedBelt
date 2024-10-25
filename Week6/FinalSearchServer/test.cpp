#include "test.hpp"

#include "SearchServer.hpp"
#include "parse.hpp"
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

#include <filesystem>
#include <future>
#include <chrono>
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

void testTime()
{
	cout << "start\n";
	{
		ifstream docs("docs.txt");
		ifstream qyeris("queris.txt");
		ostringstream sout;
		LOG_DURATION("time test");
		TestSearchServer(docs, qyeris, sout);
	}
	{
		ifstream docs("docs.txt");
		ifstream qyeris("queris.txt");
		ostringstream sout;
		LOG_DURATION("time test");
		TestSearchServer(docs, qyeris, sout);
	}
	{
		ifstream docs("docs.txt");
		ifstream qyeris("queris.txt");
		ostringstream sout;
		LOG_DURATION("time test");
		TestSearchServer(docs, qyeris, sout);
	}
	{
		ifstream docs("docs.txt");
		ifstream qyeris("queris.txt");
		ostringstream sout;
		LOG_DURATION("time test");
		TestSearchServer(docs, qyeris, sout);
	}
	{
		ifstream docs("docs.txt");
		ifstream qyeris("queris.txt");
		ostringstream sout;
		LOG_DURATION("time test");
		TestSearchServer(docs, qyeris, sout);
	}
	{
		ifstream docs("docs.txt");
		ifstream qyeris("queris.txt");
		ostringstream sout;
		LOG_DURATION("time test");
		TestSearchServer(docs, qyeris, sout);
	}
	{
		ifstream docs("docs.txt");
		ifstream qyeris("queris.txt");
		ostringstream sout;
		LOG_DURATION("time test");
		TestSearchServer(docs, qyeris, sout);
	}
	{
		ifstream docs("docs.txt");
		ifstream qyeris("queris.txt");
		ostringstream sout;
		LOG_DURATION("time test");
		TestSearchServer(docs, qyeris, sout);
	}
	{
		ifstream docs("docs.txt");
		ifstream qyeris("queris.txt");
		ostringstream sout;
		LOG_DURATION("time test");
		TestSearchServer(docs, qyeris, sout);
	}
	{
		ifstream docs("docs.txt");
		ifstream qyeris("queris.txt");
		ostringstream sout;
		LOG_DURATION("time test");
		TestSearchServer(docs, qyeris, sout);
	}
}

const size_t MAX_NUM_DOC = 1'000; //50'000
const size_t MAX_NUM_WORDS = 500; // 1'000
const size_t MAX_SIZE_WORD = 100;
const size_t MAX_NUM_DIFF_WORDS_IN_ALL_DOCS = 15000;
const size_t MAX_NUM_QUERY = 10'000; // 500'000
const size_t MAX_WORDS_IN_QUERY = 10;
const size_t NUM_LETTERS = 10;
const char firstSymb = '0';
vector<string> createDocs(size_t shift = 0)
{
	vector<string> docs(MAX_NUM_DOC);

	for (size_t i = 0; i < MAX_NUM_DOC; ++i)
	{
		docs[i] += string(MAX_SIZE_WORD, firstSymb + (i % NUM_LETTERS));
		for (size_t j = 1; j < MAX_NUM_WORDS; ++j)
		{
			docs[i] += " " + string(MAX_SIZE_WORD, firstSymb + ((j + i + shift) % NUM_LETTERS));
		}
	}

	return docs;
}

vector<string> createQueris(size_t shift = 0)
{
	vector<string> querys(MAX_NUM_QUERY);
	for (size_t i = 0; i < MAX_NUM_QUERY; ++i)
	{
		querys[i] += string(MAX_SIZE_WORD, firstSymb + (i % NUM_LETTERS));
		for (size_t j = 1; j < MAX_WORDS_IN_QUERY; ++j)
		{
			querys[i] += " " + string(MAX_SIZE_WORD, firstSymb + ((j + i + shift) % NUM_LETTERS));
		}
	}

	return querys;
}

namespace fs = std::filesystem;

void writeData(const fs::path& file, const std::vector<string>& data)
{
	ofstream out(file);
	bool first = true;
	for (size_t i = 0; i < data.size(); ++i)
	{
		if (!first)
		{
			out << '\n';
		}
		first = false;
		out << data[i];
	}

	out.close();
}

void testMultiThreads()
{
	//{
	//	/*
	//	* single stream qyeris update qyeris: 13536 ms
	//	*/

	//	ifstream docs1("docs1.txt");
	//	ifstream docs2("docs2.txt");
	//	SearchServer s(docs1);

	//	ifstream qyeris1("queris1.txt");
	//	ifstream qyeris2("queris2.txt");

	//	ostringstream sout1;
	//	ostringstream sout2;

	//	{
	//		LOG_DURATION("single stream qyeris update qyeris");
	//		s.AddQueriesStream(qyeris1, sout1);
	//		s.UpdateDocumentBase(docs2);
	//		s.AddQueriesStream(qyeris2, sout2);
	//	}

	//	ofstream out("out1.txt");
	//	out << sout1.str();
	//	out.close();
	//	out.open("out2.txt");
	//	out << sout2.str();
	//	out.close();
	//}

	{
		/*
			
		*/
		ifstream docs1("docs1.txt");
		ifstream docs2("docs2.txt");
		SearchServer s(docs1);

		ifstream qyeris1("queris1.txt");
		ifstream qyeris2("queris2.txt");

		ostringstream sout1;
		ostringstream sout2;

		{
			LOG_DURATION("multi stream qyeris update qyeris");
			s.UpdateDocumentBase(docs2);
			this_thread::sleep_for(5000ms);
			s.AddQueriesStream(qyeris1, sout1);
			s.AddQueriesStream(qyeris2, sout2);

			//s.endTasks();
		}
		this_thread::sleep_for(5000ms);
		ofstream out("out1.txt");
		out << sout1.str();
		out.close();
		out.open("out2.txt");
		out << sout2.str();
		out.close();
	}
	/*{
		ifstream qyeris1("queris1.txt");
		ifstream qyeris2("queris2.txt");
		ostringstream sout1;
		ostringstream sout2;
		LOG_DURATION("multi stream");
		s.AddQueriesStream(qyeris1, sout1);
		s.AddQueriesStream(qyeris2, sout2);

		s.endTasks();
	}
	{
		ifstream qyeris1("queris1.txt");
		ifstream qyeris2("queris2.txt");
		ostringstream sout1;
		ostringstream sout2;
		LOG_DURATION("multi stream");
		s.AddQueriesStream(qyeris1, sout1);
		s.AddQueriesStream(qyeris2, sout2);

		s.endTasks();
	}
	{
		ifstream qyeris1("queris1.txt");
		ifstream qyeris2("queris2.txt");
		ostringstream sout1;
		ostringstream sout2;
		LOG_DURATION("multi stream");
		s.AddQueriesStream(qyeris1, sout1);
		s.AddQueriesStream(qyeris2, sout2);

		s.endTasks();
	}
	{
		ifstream qyeris1("queris1.txt");
		ifstream qyeris2("queris2.txt");
		ostringstream sout1;
		ostringstream sout2;
		LOG_DURATION("multi stream");
		s.AddQueriesStream(qyeris1, sout1);
		s.AddQueriesStream(qyeris2, sout2);

		s.endTasks();
	}*/

	//future f1 = async([&s, &qyeris, &sout]() { s.AddQueriesStream(qyeris, sout); });
	////this_thread::sleep_for(1000ms);
	//s.UpdateDocumentBase(docs2);

	//f1.get();
	//ofstream out("out.txt");
	//out << sout.str();
}

void testAll() {
	//MyUnitTest::TestRunner tr;
	//RUN_TEST(tr, TestSerpFormat);
	//RUN_TEST(tr, TestTop5);
	//RUN_TEST(tr, TestHitcount);
	//RUN_TEST(tr, TestRanking);
	//RUN_TEST(tr, TestBasicSearch);
	//testTime();

	//writeData("queris2.txt", createQueris());

	testMultiThreads();
}
