#include "test.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <sstream>

#include <set>

using namespace std;

/*
	time test: 7818 ms
*/

int main()
{
	/*set<RelevAndPoss> s;
	for (size_t i = 0; i < 5; i++)
	{
		s.insert({ 0, i });
	}
	
	for (auto e : s)
	{
		cout << "{" << e.relev << ", " << e.pos << "}, ";
	}*/
    testAll();
}