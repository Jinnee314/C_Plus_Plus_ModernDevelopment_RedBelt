#include "MyUnitTestFreimwork.hpp"
#include "LogDuration.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <random>
#include <utility>
#include <sstream>
#include <set>

using namespace std;



class RouteManager {
public:
	void AddRoute(int start, int finish) 
	{
		//reachable_lists_[start].push_back(finish);
		//reachable_lists_[finish].push_back(start);
		auto res1 = reachable_lists_[start].insert(finish);
		auto res2 = reachable_lists_[finish].insert(start);
	}

	int FindNearestFinish(int start, int finish) const 
	{
		int result = abs(start - finish);

		if (reachable_lists_.count(start) < 1) 
		{
			return result;
		}
		const set<int>& reachable_stations = reachable_lists_.at(start);
		const auto finish_pos = reachable_stations.lower_bound(finish);

		if (finish_pos != reachable_stations.end())
		{
			result = min(result, abs(finish - *finish_pos));
		}
		if (finish_pos != reachable_stations.begin())
		{
			result = min(result, abs(finish - *prev(finish_pos)));
		}

		/*const vector<int>& reachable_stations = reachable_lists_.at(start);
		if (!reachable_stations.empty()) 
		{
			result = min(
				result,
				abs(finish - *min_element
				(
					begin(reachable_stations), end(reachable_stations),
					[finish](int lhs, int rhs) 
					{ 
						return abs(lhs - finish) < abs(rhs - finish); 
					}
				)
				)
			);
		}*/
		return result;
	}
private:
	map<int, set<int>> reachable_lists_;
	//map<int, vector<int>> reachable_lists_;
};

void testTime1()
{
	RouteManager rm;
	mt19937 rnd(8);
	//uniform_int_distribution<int> dist(static_cast<int>(pow(-10, 9)), static_cast<int>(pow(10, 9)));
	uniform_int_distribution<int> dist(-10'000, 10'000);
	const size_t size = 100000;
	set<pair<int, int>> s;
	while(s.size() != size)
	{
		pair<int, int> p{ dist(rnd), dist(rnd) };
		if (!s.contains({ p.second, p.first }))
		{
			s.insert(move(p));
		}
	}
	/*for (const auto& e : s)
	{
		if (s.contains({ e.second, e.first }))
		{
			auto it = s.find({ e.second, e.first });
			if (it != s.find(e))
			{
				cout << "EQUAL PAIR" << endl;
				break;
			}
		}
	}*/
	/*for (int i = 0; i < size; ++i)
	{
		s.insert(move(make_pair(dist(rnd), dist(rnd))));
	}*/
	std::cout << "set full" << endl;

	const size_t point = 50000;
	auto it = s.begin();
	LOG_DURATION("all request");
	{
		LOG_DURATION("add route");
		for (size_t i = 0; i < point; ++i)
		{
			rm.AddRoute(it->first, it->second);
			++it;
		}
	}
	{
		LOG_DURATION("find nearest finish");
		ostringstream out;
		for (size_t i = point; i < size - 1; ++i)
		{
			out << rm.FindNearestFinish(it->first, it->second) << '\n';
			++it;
		}
	}
}

void testTime2()
{
	RouteManager rm;
	//const size_t size = 100000;
	const int point = 50000;
	
	{
		LOG_DURATION("add");
		for (int i = point / -2; i < point / 2; ++i)
		{
			rm.AddRoute(i, i + 2);
		}
	}
	{
		LOG_DURATION("find");
		for (int i = 0; i < point; ++i)
		{
			rm.FindNearestFinish(-1 * i, i);
		}
	}
}
int main() {
	testTime2();

	/*RouteManager routes;

	int query_count;
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) {
	  string query_type;
	  cin >> query_type;
	  int start, finish;
	  cin >> start >> finish;
	  if (query_type == "ADD") {
		routes.AddRoute(start, finish);
	  } else if (query_type == "GO") {
		cout << routes.FindNearestFinish(start, finish) << "\n";
	  }
	}*/

	return 0;
}
