#include "test_runner.h"
#include "profile.h"

#include "search_system.h"

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>

using namespace std;

int main(int argc, char **argv, char **envp)
{
    try
    {
        ios_base::sync_with_stdio(false);
        cin.tie(nullptr);

        if (argc != 3)
        {
            throw invalid_argument("exactly 2 arguments are required: input.txt \"mask\"");
        }

        const string path = "../" + static_cast<string>(argv[1]);
        const string_view mask(argv[2]);

        ifstream file(path);

        if(!file.is_open()) {
            throw invalid_argument("can not open the file");
        }

        // cout << file.rdbuf() << endl;

        SearchSystem(file, mask);
    }
    catch (const exception &e)
    {
        cerr << e.what() << '\n';
        return 1;
    }
    catch (...) {
        cerr << "(...) exception\n";
        return 1;
    }

    return 0;
}
