#include <charconv>
#include <chrono>
#include <cstdint>
#include <deque>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <utility>
#include <vector>

namespace day08 {
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

std::deque<int64_t> to_deque(auto range)
{
    std::deque<int64_t> out;
    for (int64_t num : range) out.push_back(num);
    return out;
}

auto solution(const std::vector<std::string>& input)
{
    int64_t part1{};
    int64_t part2{};

    auto to_number = [](const auto& element) {
        int64_t value{};
        std::from_chars(element.data(), element.data() + element.size(), value);
        return value;
    };

    for (const auto& line : input) {
        std::vector<std::deque<int64_t>> lines;
        lines.push_back(to_deque(line | std::views::split(' ') | std::views::transform(to_number)));

        while (true) {
            auto check_if_zero = lines.back() | std::views::filter([](auto num) { return num != 0; });
            if (check_if_zero.begin() == check_if_zero.end()) break;

            lines.push_back(to_deque(lines.back() | std::views::slide(2) | std::views::transform([](auto iter) {
                return *std::next(iter.begin()) - *iter.begin();
            })));
        }

        for (size_t end = lines.size() - 1; end > 0; --end) {
            lines[end - 1].push_back(lines[end - 1].back() + lines[end].back());
            lines[end - 1].push_front(lines[end - 1].front() - lines[end].front());
        }
        part1 += lines[0].back();
        part2 += lines[0].front();
    }

    return std::make_pair(part1, part2);
}

}// namespace day08

int main()
{
    using time_scale = std::chrono::microseconds;

    auto start = std::chrono::high_resolution_clock::now();
    const auto input = day08::read_file(INPUT_DIR "day09.txt");
    auto io_time = std::chrono::high_resolution_clock::now();

    auto [part1, part2] = day08::solution(input);

    std::cout << std::format("Day 08 Part 1: {}\n", part1);
    std::cout << std::format("Day 08 Part 2: {}\n", part2);

    auto execution_time = std::chrono::high_resolution_clock::now();

    auto io_ms = std::chrono::duration_cast<time_scale>(io_time - start);
    auto execution_ms = std::chrono::duration_cast<time_scale>(execution_time - io_time);
    auto total_ms = std::chrono::duration_cast<time_scale>(execution_time - start);
    std::cout << std::format("IO: {}, Execution: {}, Total: {}\n", io_ms, execution_ms, total_ms);
}
