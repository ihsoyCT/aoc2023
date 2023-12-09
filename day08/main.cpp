#include <boost/unordered/unordered_flat_map.hpp>
#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <scn/scn.h>
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

using element_t = std::string;

struct node_t {
    element_t left;
    element_t right;
};

auto parse_input(const std::vector<std::string>& input, std::string& instructions) {
    boost::unordered_flat_map<element_t, node_t> map;

    int skip = 2;
    for(const auto& line : input) {
        if(skip > 0) {
            if(skip == 2) instructions = line;
            --skip;
            continue;
        }
        element_t element {};
        node_t node {};
        (void) scn::scan(line, "{} = ({:3}, {:3})", element, node.left, node.right);
        map[element] = node;
    }

    return map;
}

auto solution(const std::vector<std::string>& input)
{
    std::string instructions;    
    auto map = parse_input(input, instructions);

    size_t part1{};
    { // Part 1
        element_t cursor = "AAA";
        while(cursor != "ZZZ") {
            if(instructions[part1 % instructions.size()] == 'R') {
                cursor = map[cursor].right;
            } else {
                cursor = map[cursor].left;
            }
            part1++;
        }
    }

    // Part2
    auto starting_points =  std::views::keys(map) | std::views::filter([](auto key) {
        return static_cast<bool>(key[key.size() - 1] == 'A');
    });

    std::vector<size_t> counts{};
    for(const auto& start_node : starting_points) {
        size_t instruction_cursor{};
        element_t cursor = start_node;
        while(static_cast<bool>(cursor[cursor.size() - 1] != 'Z')) {
            if(instructions[instruction_cursor % instructions.size()] == 'R') {
                cursor = map[cursor].right;
            } else {
                cursor = map[cursor].left;
            }
            instruction_cursor++;
        }
        counts.push_back(instruction_cursor);
    }

    size_t part2{1};
    while(!counts.empty()) {
        part2 = std::lcm(counts.back(), part2);
        counts.pop_back();
    }
    return std::make_pair(part1, part2);
}

}// namespace day08

int main()
{
    using time_scale = std::chrono::milliseconds;

    auto start = std::chrono::high_resolution_clock::now();
    const auto input = day08::read_file(INPUT_DIR "day08.txt");
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
