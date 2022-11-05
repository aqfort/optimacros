#pragma once

#include <mutex>
#include <future>
#include <vector>

class SearchSystem
{
public:
    SearchSystem() = default;
    ~SearchSystem() = default;

private:
    std::mutex mutex_;
    std::vector<std::future<void>> futures_;
};
