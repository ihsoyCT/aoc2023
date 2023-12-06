#include <algorithm>
#include <charconv>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string_view>
#include <utility>
#include <vector>


#include <scn/scn.h>

namespace day04 {
auto read_file(const std::string& file_path)
{
    std::ifstream infile(file_path);
    std::vector<std::string> output;
    std::string line;
    while (std::getline(infile, line)) {
        output.push_back(line);
    }
    return output;
}


auto solution(const std::vector<std::string>& input)
{
    std::vector<int64_t> points_store;
    std::map<size_t, int64_t> winning_counts{};

    for (size_t i = 1; i <= input.size(); ++i) {
        winning_counts[i] = 1;
    }

    for (std::string_view stratchcard : input) {
        size_t card_idx{};
        auto result = scn::scan(stratchcard, "Card {}:", card_idx);

        auto rest = result.reconstruct();

        auto test = rest | std::views::split('|')
                    | std::views::transform(std::views::split(' ') | std::views::filter([](auto s) {
                          return s.size() > 0;
                      }) | std::views::transform([](auto s) -> int64_t {
                          int64_t value{};
                          std::from_chars(s.begin(), s.end(), value);
                          return value;
                      }));

        auto winning_numbers = test | std::views::take(1) | std::views::join;
        auto numbers_won =
            test | std::views::drop(1) | std::views::join | std::views::filter([&winning_numbers](const auto& number) {
                return std::ranges::find(winning_numbers, number) != winning_numbers.end();
            });

        int64_t points{ 0 };
        size_t card_tracker = card_idx;

        for (auto n : numbers_won) {
            (void)n;
            ++card_tracker;
            winning_counts[card_tracker] += winning_counts[card_idx];
            if (points == 0) {
                points = 1;
            } else {
                points *= 2;
            }
        }

        points_store.push_back(points);
    }
    int64_t part1 = std::accumulate(points_store.begin(), points_store.end(), 0LL);
    int64_t part2 = std::accumulate(
        winning_counts.begin(), winning_counts.end(), 0, [](int64_t sum, decltype(winning_counts)::value_type& values) {
            return sum + values.second;
        });
    return std::make_pair(part1, part2);
}

}// namespace day04

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    const auto input = day04::read_file(INPUT_DIR "day04.txt");
    auto io_time = std::chrono::high_resolution_clock::now();

    const auto [part1, part2] = day04::solution(input);
    auto execution_time = std::chrono::high_resolution_clock::now();

    std::cout << std::format("Day 04 Part 1: {}\n", part1);
    std::cout << std::format("Day 04 Part 2: {}\n", part2);

    auto io_ms = std::chrono::duration_cast<std::chrono::microseconds>(io_time - start);
    auto execution_ms = std::chrono::duration_cast<std::chrono::microseconds>(execution_time - io_time);
    auto total_ms = std::chrono::duration_cast<std::chrono::microseconds>(execution_time - start);
    std::cout << std::format("IO: {}, Execution: {}, Total: {}\n", io_ms, execution_ms, total_ms);
}
