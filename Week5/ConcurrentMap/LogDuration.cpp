#include "LogDuration.hpp"

#include <iostream>

using namespace std::chrono;
LogDuration::LogDuration(const std::string& mes) : message(mes + ": "), start(steady_clock::now()) {}

LogDuration::~LogDuration()
{
	auto finish = steady_clock::now();
	auto dur = finish - start;
	std::cerr << message << duration_cast<milliseconds>(dur).count() << " ms" << "\n";
}