#include <cassert>
#include <chrono>
#include <cstdint>
#include <deque>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <mdspan>
#include <stack>
#include <tuple>
#include <utility>
#include <vector>

namespace day10 {
auto read_file(const std::string& file_path)
{
    std::ifstream infile(file_path);
    std::vector<char> output;
    std::string line;
    int64_t line_length{};
    int64_t start{ 0 };
    int64_t counter{ 0 };
    while (std::getline(infile, line)) {
        line = std::format(".{}.", line);
        line_length = std::ssize(line);
        if (output.empty()) {
            for (int64_t i{}; i < line_length; ++i) {
                output.push_back('.');
            }
        }
        if (auto find = line.find('S'); find != line.npos) {
            start = line_length + counter + static_cast<int64_t>(find);
        }
        counter += line_length;
        output.append_range(line);
    }

    for (int64_t i{}; i < line_length; ++i) {
        output.push_back('.');
    }
    return std::make_tuple(output, line_length, start);
}

auto get_possible_next_moves(size_t row, size_t col, const auto& maze) -> std::vector<std::pair<size_t, size_t>>
{
    char current_symbol = maze[row, col];
    switch (current_symbol) {
    case 'S':
        return std::vector{ std::make_pair(row, col - 1),
            std::make_pair(row, col + 1),
            std::make_pair(row - 1, col),
            std::make_pair(row + 1, col) };
        break;
    case '|':
        return std::vector{ std::make_pair(row - 1, col), std::make_pair(row + 1, col) };
        break;
    case '-':
        return std::vector{ std::make_pair(row, col - 1), std::make_pair(row, col + 1) };
        break;
    case 'L':
        return std::vector{ std::make_pair(row, col + 1), std::make_pair(row - 1, col) };
        break;
    case 'J':
        return std::vector{ std::make_pair(row, col - 1), std::make_pair(row - 1, col) };
        break;
    case '7':
        return std::vector{ std::make_pair(row, col - 1), std::make_pair(row + 1, col) };
        break;
    case 'F':
        return std::vector{ std::make_pair(row, col + 1), std::make_pair(row + 1, col) };
        break;
    }

    return {};
}
auto solution(const auto& input)
{
    int64_t part1{};
    int64_t part2{};

    auto [data, length, starting_point] = input;
    std::vector<int64_t> distances(data.size(), -1);
    std::vector<int64_t> nodes(data.size(), -1);
    auto maze = std::mdspan(data.data(), std::ssize(data) / length, length);
    auto distance_map = std::mdspan(distances.data(), std::ssize(distances) / length, length);
    auto node_map = std::mdspan(nodes.data(), std::ssize(nodes) / length, length);

    assert(maze.is_exhaustive());
    assert(distance_map.is_exhaustive());
    assert(node_map.is_exhaustive());
    assert(data.size() == maze.size() && maze.size() == distance_map.size() && distance_map.size() == node_map.size());

    std::pair<size_t, size_t> start = std::make_pair(starting_point / length, starting_point % length);

    std::stack<std::pair<int64_t, decltype(start)>> queue;
    queue.emplace(0, start);

    std::map<decltype(start), decltype(start)> parent_node;

    decltype(start) last_node;

    while (!queue.empty()) {
        auto current = queue.top().second;
        int64_t previous_distance = queue.top().first;
        queue.pop();

        if (distance_map[current.first, current.second] == -1) {
            distance_map[current.first, current.second] = previous_distance + 1;
            auto next_moves = get_possible_next_moves(current.first, current.second, maze);
            for (auto& move : next_moves) {
                bool is_valid = false;
                auto back_move = get_possible_next_moves(move.first, move.second, maze);
                for (auto& back : back_move) {
                    if (back.first == current.first && back.second == current.second) is_valid = true;
                }
                if (!is_valid) continue;
                if (distance_map[move.first, move.second] == -1) {
                    parent_node[move] = current;
                    queue.emplace(distance_map[current.first, current.second], move);
                }
                if (previous_distance > 2 && maze[move.first, move.second] == 'S') {
                    last_node = current;
                    part1 = distance_map[current.first, current.second];
                }
            }
        }
    }

    // Map of all nodes that are part of the path
    node_map[start.first, start.second] = 1;
    while (last_node != start) {
        node_map[last_node.first, last_node.second] = 1;
        last_node = parent_node[last_node];
    }

    // Manually replacing S with proper symbol
    maze[start.first, start.second] = '|';

    // clean map of pipes not part of path
    for (size_t j{}; j < data.size(); ++j) {
        if (nodes[j] == -1) data[j] = '.';
    }

    for (std::size_t row = 0; row != node_map.extent(0); row++) {
        int pipe_breaks = { 0 };
        for (std::size_t col = 0; col != node_map.extent(1); col++) {
            char current = maze[row, col];
            if (current == 'F' || current == 'L') {
                char pipe_start = current;
                while (current != '7' && current != 'J') {
                    col++;
                    current = maze[row, col];
                }
                if ((pipe_start == 'L' && current == '7') || (pipe_start == 'F' && current == 'J')) {
                    ++pipe_breaks;
                }
            } else if (maze[row, col] == '|') {
                ++pipe_breaks;
            } else if (pipe_breaks % 2 == 1 && node_map[row, col] == -1) {
                part2++;
            }
        }
    }

    return std::make_pair(part1 / 2, part2);
}

}// namespace day10

int main()
{
    using time_scale = std::chrono::microseconds;

    auto start = std::chrono::high_resolution_clock::now();
    const auto input = day10::read_file(INPUT_DIR "day10.txt");
    auto io_time = std::chrono::high_resolution_clock::now();

    auto [part1, part2] = day10::solution(input);

    std::cout << std::format("Day 10 Part 1: {}\n", part1);
    std::cout << std::format("Day 10 Part 2: {}\n", part2);

    auto execution_time = std::chrono::high_resolution_clock::now();

    auto io_ms = std::chrono::duration_cast<time_scale>(io_time - start);
    auto execution_ms = std::chrono::duration_cast<time_scale>(execution_time - io_time);
    auto total_ms = std::chrono::duration_cast<time_scale>(execution_time - start);
    std::cout << std::format("IO: {}, Execution: {}, Total: {}\n", io_ms, execution_ms, total_ms);
}
