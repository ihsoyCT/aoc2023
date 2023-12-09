#include <charconv>
#include <chrono>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <utility>
#include <vector>

namespace day06 {
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

struct race
{
    int64_t time;
    int64_t distance;
};

auto parse_input(const std::vector<std::string>& input)
{
    auto isEmpty = [](const auto& s) { return s.size() > 0; };
    auto to_number = [](const auto& element) {
        int64_t value{};
        std::from_chars(element.data(), element.data() + element.size(), value);
        return value;
    };
    auto to_race = [](const std::pair<int64_t, int64_t>& pair) { return race(pair.first, pair.second); };

    auto parser =
        std::views::split(' ') | std::views::filter(isEmpty) | std::views::drop(1) | std::views::transform(to_number);
    auto times = input[0] | parser;
    auto distances = input[1] | parser;
    auto races = std::views::zip(times, distances) | std::views::transform(to_race) | std::ranges::to<std::vector>();

    race part2{};

    for (const auto& race : races) {
        if (part2.time == 0) {
            part2.time = race.time;
        } else {
            part2.time = std::stoll(std::to_string(part2.time) + std::to_string(race.time));
        }
        if (part2.distance == 0) {
            part2.distance = race.distance;
        } else {
            part2.distance = std::stoll(std::to_string(part2.distance) + std::to_string(race.distance));
        }
    }

    return std::make_pair(races, part2);
}


auto calculate_winning_race_times(const race& race)
{    
    return 1/2 * (race.time - std::sqrt(std::pow(race.time,2) - 4 * race.distance));
}

auto solution(const std::vector<std::string>& input)
{
    auto races = parse_input(input);

    int64_t part1 {1};
    for (const auto& race : races.first) {
        part1 *= calculate_winning_race_times(race);
    }

    int64_t part2 = calculate_winning_race_times(races.second);
    return std::make_pair(part1, part2);
}

}// namespace day06

int main()
{
    using time_scale = std::chrono::microseconds;

    auto start = std::chrono::high_resolution_clock::now();
    const auto input = day06::read_file(INPUT_DIR "day06.txt");
    auto io_time = std::chrono::high_resolution_clock::now();

    const auto [part1, part2] = day06::solution(input);
    auto execution_time = std::chrono::high_resolution_clock::now();

    std::cout << std::format("Day 05 Part 1: {}\n", part1);
    std::cout << std::format("Day 05 Part 2: {}\n", part2);

    auto io_ms = std::chrono::duration_cast<time_scale>(io_time - start);
    auto execution_ms = std::chrono::duration_cast<time_scale>(execution_time - io_time);
    auto total_ms = std::chrono::duration_cast<time_scale>(execution_time - start);
    std::cout << std::format("IO: {}, Execution: {}, Total: {}\n", io_ms, execution_ms, total_ms);
}
