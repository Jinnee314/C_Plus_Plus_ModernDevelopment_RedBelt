#include "MyUnitTestFreimwork.hpp"

#include <sstream>
#include <iostream>
#include <string>

using namespace std;

class Logger {
public:
	explicit Logger(ostream& output_stream) : os(output_stream) {
	}

	void SetLogLine(bool value) { log_line = value; }
	void SetLogFile(bool value) { log_file = value; }

	void Log(const string& message)
	{
		os << message << "\n";
	}

	bool getLogLine() const { return log_line; }
	bool getLogFile() const { return log_file; }

private:
	ostream& os;
	bool log_line = false;
	bool log_file = false;
};

#define LOG(logger, message)											\
{																		\
	std::string file = __FILE__;										\
	std::string line = std::to_string(__LINE__);						\
	if (logger.getLogLine() && logger.getLogFile())						\
	{																	\
		logger.Log(file + ":" + line + " " + message);					\
	}																	\
	else if (logger.getLogLine())										\
	{																	\
		logger.Log("Line " + line + " " + message);						\
	}																	\
	else if (logger.getLogFile())										\
	{																	\
		logger.Log(file + " " + message);								\
	}																	\
	else																\
	{																	\
		logger.Log(message);											\
	}																	\
}

void TestLog() {
	/* Для написания юнит-тестов в этой задаче нам нужно фиксировать конкретные
	 * номера строк в ожидаемом значении (см. переменную expected ниже). Если
	 * мы добавляем какой-то код выше функции TestLog, то эти номера строк меняются,
	 * и наш тест начинает падать. Это неудобно.
	 *
	 * Чтобы этого избежать, мы используем специальный макрос #line
	 * (http://en.cppreference.com/w/cpp/preprocessor/line), который позволяет
	 * переопределить номер строки, а также имя файла. Благодаря ему, номера
	 * строк внутри функции TestLog будут фиксированы независимо от того, какой
	 * код мы добавляем перед ней*/
#line 1 "logger.cpp"

	ostringstream logs;
	Logger l(logs);
	LOG(l, "hello");

	l.SetLogFile(true);
	LOG(l, "hello");

	l.SetLogLine(true);
	LOG(l, "hello");

	l.SetLogFile(false);
	LOG(l, "hello");

	string expected = "hello\n";
	expected += "E:\\C++ progi\\RedBeltWeek1LoggerAndMacrosLog\\RedBeltWeek1LoggerAndMacrosLog\\logger.cpp hello\n";
	expected += "E:\\C++ progi\\RedBeltWeek1LoggerAndMacrosLog\\RedBeltWeek1LoggerAndMacrosLog\\logger.cpp:10 hello\n";
	expected += "Line 13 hello\n";
	ASSERT_EQUAL(logs.str(), expected);
}

int main() {
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, TestLog);
}