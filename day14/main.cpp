#include <algorithm>
#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>
#include <map>


namespace day14 {
auto read_file(const std::string& file_path)
{
    std::ifstream infile(file_path);
    std::vector<std::string> data{};
    std::string line;

    while (std::getline(infile, line)) {
        data.push_back(line);
    }
    return data;
}

// std::vector<std::string> rotate(const std::vector<std::string>& data)
// {
//     std::vector<std::string> trans;
//     size_t cols = data.size();
//     size_t rows = data.back().size();
//     for (size_t i = 0; i < cols; i++) {
//         for (size_t j = 0; j < rows; j++) {
//             trans[i][j] = data[rows - 1 - j][i];
//         }
//     }
//     return trans;
// }

auto part1(std::vector<std::string> input)
{
    size_t value{};
    // input = rotate(input);
    for (const auto& line : input) {
        std::queue<size_t> empties;

        size_t i{};
        for (char c : line) {
            if (c == '.')
                empties.push(i);
            else if (c == '#')
                empties = {};
            else {
                if (!empties.empty()) {
                    value += line.size() - empties.front();
                    empties.pop();
                    empties.push(i);
                } else {
                    value += line.size() - i;
                }
            }
            ++i;
        }
    }
    return value;
}

void print_data(std::vector<std::string> data)
{
    for (const auto& line : data) {
        std::cout << line << '\n';
    }
    std::cout << "-----------------\n";
}

void move_north(std::vector<std::string>& map)
{
    std::vector<int32_t> movement_vector(map[0].length(), 0);

    uint32_t y = 0;
    for (auto& row : map) {
        // iterate over the row moving any rocks by the amount in the movement vector
        for (uint32_t i = 0; i < row.length(); ++i) {
            if (row[i] == '.') {
                ++movement_vector[i];
                continue;
            }// increment the movement for this column
            if (row[i] == '#') {
                movement_vector[i] = 0;
                continue;
            }// reset the movement
            // we've not got to move the 'O' by the amount in the movement vector
            if (const uint32_t destination_row = y - movement_vector[i];
                destination_row != y)// don't set this if the row is the current one
            {
                map[destination_row][i] = 'O';
                row[i] = '.';
            }
        }
        ++y;
    }
}

void move_west(std::vector<std::string>& map)
{
    for (auto& row : map) {
        uint32_t movement = 0;
        // iterate over the row moving any rocks by the amount in the movement vector
        for (uint32_t i = 0; i < row.length(); ++i) {
            if (row[i] == '.') {
                ++movement;
                continue;
            }// increment the movement for this column
            if (row[i] == '#') {
                movement = 0;
                continue;
            }// reset the movement
            // we've not got to move the 'O' by the amount in the movement vector
            if (const uint32_t destination = i - movement;
                destination != i)// don't set this if the row is the current one
            {
                row[destination] = 'O';
                row[i] = '.';
            }
        }
    }
}

void move_south(std::vector<std::string>& map)
{
    std::vector<int32_t> movement_vector(map[0].length(), 0);

    for (int32_t r = map.size() - 1; r >= 0; --r) {
        int32_t y = r;
        std::string& row = map[r];
        // iterate over the row moving any rocks by the amount in the movement vector
        for (int32_t i = 0; i < row.length(); ++i) {
            if (row[i] == '.') {
                ++movement_vector[i];
                continue;
            }// increment the movement for this column
            if (row[i] == '#') {
                movement_vector[i] = 0;
                continue;
            }// reset the movement
            // we've not got to move the 'O' by the amount in the movement vector
            if (const int32_t destination_row = y + movement_vector[i];
                destination_row != y)// don't set this if the row is the current one
            {
                map[destination_row][i] = 'O';
                row[i] = '.';
            }
        }
    }
}

void move_east(std::vector<std::string>& map)
{
    for (auto& row : map) {
        int32_t movement = 0;
        // iterate over the row moving any rocks by the amount in the movement vector
        for (int32_t i = row.length() - 1; i >= 0; --i) {
            if (row[i] == '.') {
                ++movement;
                continue;
            }// increment the movement for this column
            if (row[i] == '#') {
                movement = 0;
                continue;
            }// reset the movement
            // we've not got to move the 'O' by the amount in the movement vector
            if (const uint32_t destination = i + movement;
                destination != i)// don't set this if the row is the current one
            {
                row[destination] = 'O';
                row[i] = '.';
            }
        }
    }
}

size_t part2(std::vector<std::string> map, uint64_t total_cycles = 1000000000 )
{
    std::map<std::vector<std::string>, std::pair<uint64_t, uint64_t>> cycle_map = {};
    // rotate the level N, W, S, E a billion times!
    // This will either take forever or there's a cycle! Rotate the level until a cycle is detected
    uint64_t cycle_count = 1;
    uint64_t lookup_index = 0;
    while (lookup_index == 0) {
        // undertake a cycle
        move_north(map);
        move_west(map);
        move_south(map);
        move_east(map);

        // does this output exist in the cycle_map
        if (auto result = cycle_map.find(map); result != cycle_map.end()) {
            // this item is in the map already work out the offset from the start
            auto [counter, load_sum] = result->second;
            const uint64_t loop_length = cycle_count - counter;
            lookup_index = counter + (total_cycles - counter) % loop_length;
            continue;
        }
        // calculate the load on the north wall
        //  now go through the map and calculate load
        uint64_t row_load = map.size();
        uint64_t load_sum = 0;
        for (auto row : map) {
            const uint64_t num_rocks = std::ranges::count(row.begin(), row.end(), 'O');
            load_sum += num_rocks * row_load;
            --row_load;
        }
        // record the outcome in a dictionary
        cycle_map[map] = std::make_pair(cycle_count, load_sum);
        ++cycle_count;
    }

    for (const auto& [key, value] : cycle_map) {
        if (value.first == lookup_index) {
            return value.second;
        }
    }

    return 0LLU;
}

}// namespace day14

int main()
{
    using time_scale = std::chrono::microseconds;

    auto start = std::chrono::high_resolution_clock::now();
    const auto input = day14::read_file(INPUT_DIR "day14.txt");
    auto io_time = std::chrono::high_resolution_clock::now();
    std::cout << std::format("Day 14 Part 1: {}\n", day14::part1(input));
    std::cout << std::format("Day 14 Part 2: {}\n", day14::part2(input));

    auto execution_time = std::chrono::high_resolution_clock::now();

    auto io_ms = std::chrono::duration_cast<time_scale>(io_time - start);
    auto execution_ms = std::chrono::duration_cast<time_scale>(execution_time - io_time);
    auto total_ms = std::chrono::duration_cast<time_scale>(execution_time - start);
    std::cout << std::format("IO: {}, Execution: {}, Total: {}\n", io_ms, execution_ms, total_ms);
}
