#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <memory>

#include <mutex>
#include <future>

using namespace std;

struct Result
{
    size_t line{};
    size_t position{};
    string word{};
};

struct Mask
{
    Mask(string_view const mask_string_view) : mask_word(mask_string_view)
    {
        bool starting_question_mark{true};

        for (auto const &mask_char : mask_string_view)
        {
            if (mask_char == '\n')
            {
                mask_word = {};
                starting_position = 0;
                break;
            }
            else if (starting_question_mark && mask_char == '?')
            {
                starting_position++;
            }
            else if (mask_char == '?')
            {
                starting_question_mark = false;
            }
        }
    }

    string_view mask_word{};
    size_t starting_position{};
};

class SearchSystem
{
private:
    struct IndexedString
    {
        size_t line_number{};
        string_view word{};
    };

public:
    SearchSystem(shared_ptr<ifstream> file_stream)
    {
        if (file_stream->good())
        {
            while (!file_stream->eof())
            {
                data_.push_back("");
                getline(*file_stream, data_.back());
            }
        }
    }

    map<size_t, Result> Search(Mask const &mask)
    {
        map<string, Result> filtered_result{};
        last_readed_index_ = 0;

        auto search_function = [this](map<string, Result> &filtered_result, Mask const &mask)
        {
            auto next_string = NextString();

            while (next_string.line_number != 0)
            {
                auto result = ProcessLine(next_string, mask);

                if (result.line != 0)
                {
                    const lock_guard<mutex> lock(mutex_);
                    filtered_result.emplace(make_pair(result.word, result));
                }

                next_string = NextString();
            }
        };

        auto const thread_count = thread::hardware_concurrency();

        for (size_t i = 0; i < thread_count; i++)
        {
            futures_.push_back(async(search_function, ref(filtered_result), ref(mask)));
        }

        for (auto &item : futures_)
        {
            item.get();
        }

        map<size_t, Result> sorted_res{};
        for (auto const &item : filtered_result)
        {
            sorted_res.emplace(item.second.line, item.second);
        }

        return sorted_res;
    }

private:
    Result ProcessLine(IndexedString const &line, Mask const &mask)
    {
        if (line.word.length() < mask.mask_word.length())
        {
            return {};
        }

        bool found{false};

        auto char_mask = line.word.cbegin() + mask.starting_position;
        auto char_data = mask.mask_word.cbegin() + mask.starting_position;

        auto position{line.word.cend()};

        while ((char_mask != line.word.cend()) && (char_data != mask.mask_word.cend()))
        {
            if (*char_data == '?')
            {
                found = true;
            }
            else
            {
                if (*char_mask == *char_data)
                {
                    if (position == line.word.cend())
                    {
                        found = true;
                        position = char_mask - mask.starting_position;
                    }
                }
                else
                {
                    found = false;
                }
            }

            if (found)
            {
                char_data++;
            }
            else
            {
                position = line.word.cend();
                char_data = mask.mask_word.cbegin() + mask.starting_position;
            }

            char_mask++;
        }

        if (found && !(*char_data))
        {
            Result result{};

            result.line = line.line_number;
            result.position = static_cast<size_t>(position - line.word.cbegin());
            result.word = string(char_mask - mask.mask_word.length(), mask.mask_word.length());

            return result;
        }

        return {};
    }

    IndexedString NextString()
    {
        IndexedString result{};
        lock_guard<mutex> lock(mutex_);

        if (last_readed_index_ < data_.size())
        {
            result.line_number = last_readed_index_ + 1;
            result.word = data_.at(last_readed_index_);

            last_readed_index_++;
        }

        return result;
    }

private:
    mutex mutex_{};
    vector<future<void>> futures_{};

    size_t last_readed_index_{};
    vector<string> data_{};
};

std::map<size_t, Result> Processor(shared_ptr<ifstream> file_stream,
                                   string_view const mask_string_view)
{
    Mask mask(mask_string_view);
    SearchSystem search_system(file_stream);

    return search_system.Search(mask);
}
