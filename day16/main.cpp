#include <algorithm>
#include <chrono>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <mdspan>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <boost/unordered/unordered_flat_set.hpp>

namespace day16 {
auto read_file(const std::string& file_path)
{
    size_t line_length{};
    std::ifstream infile(file_path);
    std::vector<char> data{};
    std::string line;
    while (std::getline(infile, line)) {
        data.append_range(line);
        line_length = std::size(line);
    }
    return std::pair{ data, line_length };
}

bool in_bounds(int64_t row, int64_t col, const auto& map)
{
    auto row_x = static_cast<size_t>(row);
    auto col_x = static_cast<size_t>(col);
    return !(row_x >= map.extent(0) || col_x >= map.extent(1));
}

void insert(auto& queue, int64_t row, int64_t col, int64_t row_speed, int64_t col_speed)
{
    queue.push(std::pair{ std::pair{ row, col }, std::pair{ row_speed, col_speed } });
}

auto solution(const std::pair<std::vector<char>, size_t>& input, bool part2 = false)
{
    int64_t output{};
    std::mdspan map = std::mdspan(input.first.data(), std::size(input.first) / input.second, input.second);

    // Keep track of which starting points where visited. If we already visited a starting point + movement vector
    // before, we know we cant get bigger
    // boost::unordered_flat_set<std::pair<std::pair<int64_t, int64_t>, std::pair<int64_t, int64_t>>> start_points_visited;

    // row col col speed row speed

    std::queue<std::pair<std::pair<int64_t, int64_t>, std::pair<int64_t, int64_t>>> starting_tiles = {};
    if (part2) {
        for (size_t i{}; i < map.extent(0); ++i) {
            insert(starting_tiles, static_cast<int64_t>(i), 0, 0, 1);
            insert(starting_tiles, static_cast<int64_t>(i), static_cast<int64_t>(map.extent(1)) - 1, 0, -1);
        }
        for (size_t i{}; i < map.extent(1); ++i) {
            insert(starting_tiles, 0, static_cast<int64_t>(i), 1, 0);
            insert(starting_tiles, static_cast<int64_t>(map.extent(0)) - 1, static_cast<int64_t>(i), -1, 0);
        }
    } else {
        insert(starting_tiles, 0, 0, 0, 1);
    }

    while (!starting_tiles.empty()) {
        std::queue<std::pair<std::pair<int64_t, int64_t>, std::pair<int64_t, int64_t>>> queue = {};
        auto start = starting_tiles.front();
        starting_tiles.pop();
        //if (start_points_visited.contains(start)) continue;

        insert(queue, start.first.first, start.first.second, start.second.first, start.second.second);
        std::vector<char> nodes(map.size(), '.');
        auto visited = std::mdspan(nodes.data(), std::size(input.first) / input.second, input.second);

        // store which tile + the movement vector to exit out of loops
        boost::unordered_flat_set<std::pair<std::pair<int64_t, int64_t>, std::pair<int64_t, int64_t>>> tiles_visited;

        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop();
            if (tiles_visited.contains(current)) continue;
            tiles_visited.insert(current);
            //start_points_visited.insert(current);

            auto [row, col] = current.first;
            auto [row_speed, col_speed] = current.second;
            if (!in_bounds(row, col, map)) continue;
            char location = map[row, col];
            visited[row, col] = '#';

            switch (location) {
            case '.':
                insert(queue, row + row_speed, col + col_speed, row_speed, col_speed);
                break;
            case '-':
                if (row_speed == 0) {
                    insert(queue, row + row_speed, col + col_speed, row_speed, col_speed);
                } else {
                    insert(queue, row, col + 1, 0, 1);
                    insert(queue, row, col - 1, 0, -1);
                }
                break;
            case '|':
                if (col_speed == 0) {
                    insert(queue, row + row_speed, col + col_speed, row_speed, col_speed);
                } else {
                    insert(queue, row + 1, col, 1, 0);
                    insert(queue, row - 1, col, -1, 0);
                }
                break;
            case '/':
                if (row_speed == 0) {
                    if (col_speed > 0) {
                        insert(queue, row - 1, col, -1, 0);
                    } else {
                        insert(queue, row + 1, col, +1, 0);
                    }
                } else {
                    if (row_speed > 0) {
                        insert(queue, row, col - 1, 0, -1);
                    } else {
                        insert(queue, row, col + 1, 0, +1);
                    }
                }
                break;
            case '\\':
                if (row_speed == 0) {
                    if (col_speed > 0) {
                        insert(queue, row + 1, col, +1, 0);
                    } else {
                        insert(queue, row - 1, col, -1, 0);
                    }
                } else {
                    if (row_speed > 0) {
                        insert(queue, row, col + 1, 0, +1);
                    } else {
                        insert(queue, row, col - 1, 0, -1);
                    }
                }
                break;
            default:
                std::unreachable();
            }
        }
        output = std::max(output, std::count_if(nodes.begin(), nodes.end(), [](char c) { return c == '#'; }));
    }
    return output;
}

}// namespace day16

int main()
{
    using time_scale = std::chrono::milliseconds;

    auto start = std::chrono::high_resolution_clock::now();
    const auto input = day16::read_file(INPUT_DIR "day16.txt");
    auto io_time = std::chrono::high_resolution_clock::now();
    std::cout << std::format("Day 16 Part 1: {}\n", day16::solution(input));
    std::cout << std::format("Day 16 Part 2: {}\n", day16::solution(input, true));

    auto execution_time = std::chrono::high_resolution_clock::now();

    auto io_ms = std::chrono::duration_cast<time_scale>(io_time - start);
    auto execution_ms = std::chrono::duration_cast<time_scale>(execution_time - io_time);
    auto total_ms = std::chrono::duration_cast<time_scale>(execution_time - start);
    std::cout << std::format("IO: {}, Execution: {}, Total: {}\n", io_ms, execution_ms, total_ms);
}
