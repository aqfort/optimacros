#include "test_runner.h"

using namespace std;

TestRunner::TestRunner()
{
    fail_count = 0;
}

TestRunner::~TestRunner()
{
    if (fail_count > 0)
    {
        cerr << fail_count << " unit tests failed" << endl;
        exit(1);
    }
}

// void Test() {
//     ASSERT(true);
//
//     ASSERT_EQUAL(1, 1);
// }
//
// void TestAll() {
//     TestRunner runner;
//
//     RUN_TEST(runner, Test);
// }
