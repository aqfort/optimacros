#include "test_runner.h"
#include "profile.h"

#include "search_system.h"

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <memory>

using namespace std;

int main(int argc, char **argv)
{
    try
    {
        ios_base::sync_with_stdio(false);
        cin.tie(nullptr);

        if (argc != 3)
        {
            throw invalid_argument("Exactly 2 arguments are required: input.txt \"mask\"");
        }

        string path = static_cast<string>(argv[1]);
        string_view mask_string_view(argv[2]);

        shared_ptr<ifstream> file_stream = make_shared<ifstream>(path);

        if (!file_stream->is_open())
        {
            throw invalid_argument("Can not open the file");
        }

        auto const &result = Processor(file_stream, mask_string_view);

        cout << result.size() << endl;
        for (auto const &item : result)
        {
            cout << item.second.line << ' ' << item.second.position + 1 << ' ' << item.second.word << endl;
        }
    }
    catch (const exception &this_exception)
    {
        cerr << this_exception.what() << '\n';
        return 1;
    }
    catch (...)
    {
        cerr << "(...) exception\n";
        return 1;
    }

    return 0;
}
