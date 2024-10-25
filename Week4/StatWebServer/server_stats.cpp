#include "MyUnitTestFreimwork.hpp"
#include "http_request.hpp"
#include "stats.hpp"

#include <map>
#include <string_view>
using namespace std;

Stats ServeRequests(istream& input) {
	Stats result;
	for (string line; getline(input, line); ) {
		const HttpRequest req = ParseRequest(line);
		result.AddUri(req.uri);
		result.AddMethod(req.method);
	}
	return result;
}

void TestBasic() {
	const string input =
		R"(GET / HTTP/1.1
    POST /order HTTP/1.1
    POST /product HTTP/1.1
    POST /product HTTP/1.1
    POST /product HTTP/1.1
    GET /order HTTP/1.1
    PUT /product HTTP/1.1
    GET /basket HTTP/1.1
    DELETE /product HTTP/1.1
    GET / HTTP/1.1
    GET / HTTP/1.1
    GET /help HTTP/1.1
    GET /upyachka HTTP/1.1
    GET /unexpected HTTP/1.1
    HEAD / HTTP/1.1)";

	const map<string_view, int> expected_method_count = {
	  {"GET", 8},
	  {"PUT", 1},
	  {"POST", 4},
	  {"DELETE", 1},
	  {"UNKNOWN", 1},
	};
	const map<string_view, int> expected_url_count = {
	  {"/", 4},
	  {"/order", 2},
	  {"/product", 5},
	  {"/basket", 1},
	  {"/help", 1},
	  {"unknown", 2},
	};

	istringstream is(input);
	const Stats stats = ServeRequests(is);

	ASSERT_EQUAL(stats.GetMethodStats(), expected_method_count);
	ASSERT_EQUAL(stats.GetUriStats(), expected_url_count);
}

void TestAbsentParts() {
	// Методы GetMethodStats и GetUriStats должны возвращать словари
	// с полным набором ключей, даже если какой-то из них не встречался

	const map<string_view, int> expected_method_count = {
	  {"GET", 0},
	  {"PUT", 0},
	  {"POST", 0},
	  {"DELETE", 0},
	  {"UNKNOWN", 0},
	};
	const map<string_view, int> expected_url_count = {
	  {"/", 0},
	  {"/order", 0},
	  {"/product", 0},
	  {"/basket", 0},
	  {"/help", 0},
	  {"unknown", 0},
	};
	const Stats default_constructed;

	ASSERT_EQUAL(default_constructed.GetMethodStats(), expected_method_count);
	ASSERT_EQUAL(default_constructed.GetUriStats(), expected_url_count);
}

int main() {
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, TestBasic);
	RUN_TEST(tr, TestAbsentParts);
}

//#include <iostream>
//#include <string>
//#include <string_view>
//#include <vector>
//
//using namespace std;
//
//struct SV
//{
//	string_view fHalf;
//	string_view sHalf;
//
//	SV(const string_view& str)
//	{
//		fHalf = str.substr(0, str.size() / 2);
//		sHalf = str.substr(str.size() / 2);
//	}
//};
//
//int just(int a)
//{
//	int b = a;
//	int c = b;
//	int d = c;
//
//	return d;
//}
//int main()
//{
//	char a = 'a';
//	string_view sv("sssssssssssss");
//	char b = 'b';
//
//	const char* pf = sv.data();
//	char* pa = &a;
//	
//	cout << pf << endl;
//	for (int i = 0; i < 30; ++i)
//	{
//		cout << *(pa - i) << endl;
//	}
//	cout << b << endl;
//	for (int i = 0; i < 30; ++i)
//	{
//		cout << *(pa + i) << endl;
//	}
//
//	return 0;
//}