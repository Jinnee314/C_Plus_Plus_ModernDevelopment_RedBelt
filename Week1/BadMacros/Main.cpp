#include "MyUnitTestFreimwork.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

// bad
//#define PRINT_VALUES(out, x, y) out << (x) << endl; out << (y) << endl

// good
#define PRINT_VALUES(out, x, y) out << (x) << "\n" << (y) << "\n"

int main()
{	MyUnitTest::TestRunner tr;
	tr.RunTest([] {
		ostringstream output;
		PRINT_VALUES(output, 5, "red belt");
		ASSERT_EQUAL(output.str(), "5\nred belt\n");
		},
		"PRINT_VALUES usage example");

    tr.RunTest([]
        {
            std::ostringstream output;
            int x = 6;
            if (false)
                PRINT_VALUES(output, x, x - 3);
            else
                PRINT_VALUES(output, --x, x + 3);
            ASSERT_EQUAL(output.str(), "5\n8\n");
        }, "PRINT_VALUES IF (FALSE) 6 6 ELSE 5 6");
}