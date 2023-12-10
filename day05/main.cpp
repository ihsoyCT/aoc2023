#include <charconv>
#include <chrono>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <thread>
#include <utility>
#include <vector>
#include <ranges>
#include <algorithm>

namespace day05 {
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

struct seed_range
{
    int64_t start;
    int64_t length;
};

struct map_element
{
    int64_t destination;
    int64_t source;
    int64_t range;
};

using mapType = std::vector<map_element>;
using seedType = seed_range;

auto parse_input(const std::vector<std::string>& input,
    std::vector<int64_t>& seeds,
    std::vector<seed_range>& seeds2,
    std::vector<mapType>& maps)
{
    auto to_number = [](const auto& element) {
        int64_t value{};
        std::from_chars(element.data(), element.data() + element.size(), value);
        return value;
    };

    seeds = input[0] | std::views::split(' ') | std::views::drop(1) | std::views::transform(to_number)
            | std::ranges::to<std::vector>();

    for (std::size_t i{}; i < seeds.size(); i += 2) {
        seeds2.emplace_back(seeds[i], seeds[i + 1]);
    }

    for (const auto& line : std::span{ std::next(input.begin()), input.end() }) {
        if (line.contains("map")) {
            maps.emplace_back();
        } else if (!line.empty()) {
            auto range =
                line | std::views::split(' ') | std::views::transform(to_number) | std::ranges::to<std::vector>();
            maps.back().emplace_back(range[0], range[1], range[2]);
        }
    }
}

int64_t get_new_value(int64_t seed, const std::vector<map_element>& map)
{
    int64_t new_seed = seed;
    for (const auto& conversion : map) {
        if (conversion.source <= seed && conversion.source + conversion.range > seed) {
            new_seed = conversion.destination + seed - conversion.source;
        }
    }
    return new_seed;
}

auto solution(const std::vector<std::string>& input)
{
    (void)input;

    std::vector<int64_t> seeds{};
    std::vector<seed_range> seeds2{};
    std::vector<mapType> maps;

    parse_input(input, seeds, seeds2, maps);

    std::vector<int64_t> locations{};
    std::atomic<int64_t> min_part2 = INT64_MAX;

    for (auto seed : seeds) {
        for (const auto& map : maps) {
            seed = get_new_value(seed, map);
        }
        locations.push_back(seed);
    }

    // Part 2
    std::vector<std::thread> threads;
    threads.reserve(seeds2.size());
    for (const auto& seed2 : seeds2) {
        threads.emplace_back([&]() {
            auto seed2n = seed2.start;
            while (seed2n < seed2.start + seed2.length) {
                auto seed = seed2n;
                for (const auto& map : maps) {
                    seed = get_new_value(seed, map);
                }
                min_part2 = std::min(min_part2.load(), seed);
                ++seed2n;
            }
            std::cout << std::format("seed_range: [{},{}] -> Current min: {}\n",seed2.start, seed2.length, min_part2.load());
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return std::make_pair(*std::min_element(locations.begin(), locations.end()), min_part2.load());
}

}// namespace day05

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    const auto input = day05::read_file(INPUT_DIR "day05.txt");
    auto io_time = std::chrono::high_resolution_clock::now();

    const auto [part1, part2] = day05::solution(input);
    auto execution_time = std::chrono::high_resolution_clock::now();

    std::cout << std::format("Day 05 Part 1: {}\n", part1);
    std::cout << std::format("Day 05 Part 2: {}\n", part2);

    auto io_ms = std::chrono::duration_cast<std::chrono::seconds>(io_time - start);
    auto execution_ms = std::chrono::duration_cast<std::chrono::seconds>(execution_time - io_time);
    auto total_ms = std::chrono::duration_cast<std::chrono::seconds>(execution_time - start);
    std::cout << std::format("IO: {}, Execution: {}, Total: {}\n", io_ms, execution_ms, total_ms);
}
