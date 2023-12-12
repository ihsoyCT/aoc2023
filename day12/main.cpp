#include <charconv>
#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <mdspan>
#include <utility>
#include <vector>
#include <ranges>
#include <map>

namespace day12 {
auto read_file(const std::string& file_path)
{
    std::ifstream infile(file_path);
    std::vector<std::string> data;
    std::vector<std::vector<size_t>> numbers;
    std::string line;
    
    auto to_number = [](const auto& element) {
        size_t value{};
        std::from_chars(element.data(), element.data() + element.size(), value);
        return value;
    };

    while (std::getline(infile, line)) {
        auto split = line | std::views::split(' ');
        data.push_back(split | std::views::take(1) | std::views::join | std::ranges::to<std::string>());
        numbers.push_back(split | std::views::drop(1) | std::views::transform(std::views::split(',') | std::views::transform(to_number)) | std::views::join | std::ranges::to<std::vector>());
    }
    return std::make_pair(data, numbers);
}

size_t count_options(std::span<size_t> hints, std::string_view data, std::map<std::pair<size_t, size_t>, size_t>& cache) {
    if(hints.empty()) return 1;
    if(data.empty()) return 0;
    
    if(const auto it = cache.find(std::make_pair(hints.size(), data.size()));
        it != cache.end()) {
        return it->second;
    }
    size_t count{};
    size_t current_hint = hints.front();

    if(current_hint > data.size()) return 0;

    for(size_t i{}; i <= data.size() - current_hint; ++i) {
        bool valid{true};
        for(size_t j{}; j < current_hint; ++j) {
            if(data[i + j] == '.') {
                valid = false;
                
            }
        }
        // invalid if not at end of line and next element #
        if(valid && i + current_hint < data.size() && data[i + current_hint] == '#') {
            valid = false;
        }
        // invalid if prior element '#'
        if(valid && i > 0 && data[i-1] == '#') {
            valid = false;
        }

        // if we are on last hint and not all '#' are covered
        if(valid && hints.size() == 1) {
            for(size_t j{i + current_hint + 1}; j < data.size(); ++j) {
                if(data[j] == '#') {
                    valid = false;
                }
            }
        }

        if(valid) {
            std::string_view next {};
            if(i + current_hint + 1 < data.size()) {
                next=data.substr(i + current_hint + 1, data.size());
            }
            count += count_options(
                    std::span(std::next(hints.begin()), hints.end()),
                    next,
                    cache
                );
            cache[std::make_pair(hints.size(), data.size())] = count;
        }
        
        // if we are currently looking at a #, we know for certain that no valid range beyond this will be possible
        if(data[i] == '#') {
            break;
        }
    }
    return count;
}

auto solution(const auto& input)
{
    size_t part1{};
    size_t part2{};

    auto data = input.first;
    auto hints = input.second;
    for(size_t i{}; i < data.size(); ++i) {
        std::map<std::pair<size_t, size_t>, size_t> cache;
        part1 += count_options(hints[i], data[i], cache);
    }

    for(size_t i{}; i < data.size(); ++i) {
        std::string current_data = data[i];
        auto current_hints = hints[i];
        for(size_t j{1}; j < 5; ++j) {
            current_data.append("?").append(data[i]);
            current_hints.append_range(hints[i]);
        }

        std::map<std::pair<size_t, size_t>, size_t> cache;
        part2 += count_options(current_hints, current_data, cache);
    }

    return std::make_pair(part1, part2);
}

}// namespace day12

int main()
{
    using time_scale = std::chrono::milliseconds;

    auto start = std::chrono::high_resolution_clock::now();
    const auto input = day12::read_file(INPUT_DIR "day12.txt");
    auto io_time = std::chrono::high_resolution_clock::now();

    auto [part1, part2] = day12::solution(input);

    std::cout << std::format("Day 12 Part 1: {}\n", part1);
    std::cout << std::format("Day 12 Part 2: {}\n", part2);

    auto execution_time = std::chrono::high_resolution_clock::now();

    auto io_ms = std::chrono::duration_cast<time_scale>(io_time - start);
    auto execution_ms = std::chrono::duration_cast<time_scale>(execution_time - io_time);
    auto total_ms = std::chrono::duration_cast<time_scale>(execution_time - start);
    std::cout << std::format("IO: {}, Execution: {}, Total: {}\n", io_ms, execution_ms, total_ms);
}
