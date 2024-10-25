#pragma once
#include <chrono>
#include <string>

class LogDuration
{
private:
	std::string message;
	std::chrono::steady_clock::time_point start;

public:
	explicit LogDuration(const std::string& mes);

	~LogDuration();
};

#define GET_UNIQ_ID(a) varUniqId##a
#define UNIQ_ID(lineno) GET_UNIQ_ID(lineno)

#define LOG_DURATION(message) \
	LogDuration UNIQ_ID(__LINE__)(message);
