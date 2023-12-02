#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>

namespace day01 {
std::vector<std::string> read_file(std::string_view file_path)
{
    std::ifstream infile(file_path);
    std::vector<std::string> output;
    std::string line;
    while (std::getline(infile, line)) {
        output.push_back(line);
    }

    return output;
}

static constexpr std::array<std::pair<std::string_view, int>, 9> replacements{
    { { "one", 1 }, { "two", 2 }, { "three", 3 }, { "four", 4 }, { "five", 5 }, { "six", 6 }, { "seven", 7 }, { "eight", 8 }, { "nine", 9 } }
};

bool match(std::string::const_iterator curr, std::string::const_iterator end, std::string_view pattern)
{
    if (std::distance(curr, end) < std::ssize(pattern)) {
        return false;
    }
    for (auto c : pattern) {
        if (*curr != c) {
            return false;
        }
        ++curr;
    }
    return true;
}

int find_numbers(const std::string& line, bool day2)
{
    std::optional<int> first_number{};
    int last_number{};

    auto iter = std::begin(line);

    for (auto char_ : line) {
        if (std::isdigit(char_) != 0) {
            if (!first_number) {
                first_number = char_ - '0';
            }
            last_number = char_ - '0';
        }
        if (day2) {
            for (auto [pattern, replacement] : replacements) {
                if (match(iter, std::end(line), pattern)) {
                    if (!first_number) {
                        first_number = replacement;
                    }
                    last_number = replacement;
                    break;
                }
            }
        }
        ++iter;
    }
    return *first_number * 10 + last_number;
}

int64_t part1()
{
    using namespace std::placeholders;
    const auto input = read_file(INPUT_DIR "day01.txt");
    auto result = input | std::views::transform(std::bind(find_numbers, _1, false));

    return std::accumulate(result.begin(), result.end(), 0);
}

int64_t part2()
{
    using namespace std::placeholders;
    const auto input = read_file(INPUT_DIR "day01.txt");
    auto result = input | std::views::transform(std::bind(find_numbers, _1, true));

    return std::accumulate(result.begin(), result.end(), 0);
}

}// namespace day01

int main()
{
    std::cout << std::format("Day 01 Part 1: {}\n", day01::part1());
    std::cout << std::format("Day 01 Part 2: {}\n", day01::part2());
    return 0;
}
