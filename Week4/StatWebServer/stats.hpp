#pragma once

#include "http_request.hpp"

#include <string_view>
#include <map>
#include <array>
using namespace std;

class Stats {
private:
	inline static array<string, 5> methods{ "GET", "PUT", "POST", "DELETE", "UNKNOWN" };
	//array<int, 5> mCount{ 0, 0, 0, 0, 0 };
	inline static array<string, 6> uris{ "/", "/order", "/product", "/basket", "/help", "unknown" };
	//array<int, 6> uCount{ 0, 0, 0, 0, 0, 0 };
	map<string_view, int> mStats, uStats;

public:
	Stats();

	void AddMethod(string_view method);
	void AddUri(string_view uri);
	const map<string_view, int>& GetMethodStats() const;
	const map<string_view, int>& GetUriStats() const;
};

HttpRequest ParseRequest(string_view line);
