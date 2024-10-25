#include "MyUnitTestFreimwork.hpp"
#include "LogDuration.hpp"

#include <iostream>
#include <list>
#include <map>

#include <fstream>
#include <random>
#include <algorithm>

using namespace std;

void createTimeTest()
{
	ofstream out("test.txt");
	const int numReq = 100'000;
	out << numReq << '\n';

	mt19937 rnd(7);
	uniform_int_distribution<int> dist(0, 100'000);
	vector<int> athls(100'000);

	for (size_t i = 1; i <= numReq; ++i)
	{
		athls[i - 1] = static_cast<int>(i);
	}

	shuffle(athls.begin(), athls.end(), rnd);

	for (size_t i = 0; i < numReq; ++i)
	{
		out << athls[i] << " " << dist(rnd) << "\n";
	}

	out.close();
}

int main()
{
	//createTimeTest();
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	size_t numReq = 0;
	cin >> numReq;
	LogDuration log("time work");
	list<int> queue;

	const int MAX_REQ = 100'000;
	vector<list<int>::iterator> poss(MAX_REQ + 1, queue.end());

	for (size_t i = 0; i < numReq; ++i)
	{
		int numAthl = 0, numBeforAthl = 0;
		cin >> numAthl >> numBeforAthl;
		poss[numAthl] = queue.insert(poss[numBeforAthl], numAthl);
	}

	for (const int athl : queue)
	{
		cout << athl << "\n";
	}

	log.~LogDuration();

	/*const int MAX_ATHLETES = 100'000;
	using Position = list<int>::iterator;

	int n_athletes;
	cin >> n_athletes;
	LogDuration log("time work");
	list<int> row;
	vector<Position> athlete_pos(MAX_ATHLETES + 1, row.end());

	for (int i = 0; i < n_athletes; ++i) {
		int athlete, next_athlete;
		cin >> athlete >> next_athlete;
		athlete_pos[athlete] = row.insert(
			athlete_pos[next_athlete],
			athlete
		);
	}

	for (int x : row) {
		cout << x << '\n';
	}
	log.~LogDuration();*/

	return 0;
}
//7
//77870 15168
//65859 40488
//4028 69498
//99145 99533
//11701 70988
//89106 11682
//7743 60706
