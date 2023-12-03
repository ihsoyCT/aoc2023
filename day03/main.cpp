#include <cctype>
#include <cstddef>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <mdspan>
#include <numeric>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace day03 {
std::vector<int8_t> read_file(std::string_view file_path, size_t& line_length)
{
    std::ifstream infile(file_path);
    std::vector<int8_t> output;
    std::string line;
    while (std::getline(infile, line)) {
        output.insert(output.end(), line.begin(), line.end());
        output.push_back('.');
        line_length = line.size() + 1;
    }
    return output;
}

bool check_surrounding(std::mdspan<const int8_t, std::dextents<size_t, 2>> data,
    size_t row,
    size_t column,
    std::set<std::pair<size_t, size_t>>& adjacent_gears)
{
    static const std::array<const std::pair<int64_t, int64_t>, 8> options = {
        { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 } }
    };

    bool return_value = false;

    for (const auto& direction : options) {
        int64_t adj_row = row + direction.first;
        int64_t adj_column = column + direction.second;
        if (adj_row >= 0 && adj_column >= 0 && adj_row < data.extent(0) && adj_column < data.extent(1)
            && (std::isdigit(data[adj_row, adj_column]) == 0) && data[adj_row, adj_column] != '.') {
            if (data[adj_row, adj_column] == '*') {
                adjacent_gears.emplace(adj_row, adj_column);
            }
            return_value = true;
        }
    }
    return return_value;
}

auto solution(const std::vector<int8_t>& input, size_t line_length)
{
    auto data = std::mdspan(input.data(), input.size() / line_length, line_length);
    int64_t part1{};

    std::map<std::pair<size_t, size_t>, std::vector<int64_t>> gears_and_numbers;

    for (size_t i{}; i != data.extent(0); ++i) {
        int64_t current_number{};
        std::set<std::pair<size_t, size_t>> adjacent_gears{};

        bool valid{ false };
        for (size_t j{}; j != data.extent(1); ++j) {
            if (std::isdigit(data[i, j]) != 0) {
                current_number = current_number * 10 + data[i, j] - '0';
                valid |= check_surrounding(data, i, j, adjacent_gears);
            } else {
                if (current_number != 0) {
                    for (auto gear : adjacent_gears) {
                        gears_and_numbers[gear].push_back(current_number);
                    }
                    if (valid) {
                        part1 += current_number;
                        adjacent_gears.clear();
                    }
                    current_number = 0;
                    valid = false;
                }
            }
        }
    }

    auto part2 = std::accumulate(gears_and_numbers.begin(),
        gears_and_numbers.end(),
        0,
        [](int64_t sum, decltype(gears_and_numbers)::value_type& numbers) {
            if (numbers.second.size() != 2) {
                return sum;
            } else {
                return sum + numbers.second[0] * numbers.second[1];
            }
        });

    return std::make_pair(part1, part2);
}

}// namespace day03

int main()
{
    size_t line_length{};
    const auto input = day03::read_file(INPUT_DIR "day03.txt", line_length);

    const auto [part1, part2] = day03::solution(input, line_length);

    std::cout << std::format("Day 03 Part 1: {}\n", part1);
    std::cout << std::format("Day 03 Part 2: {}\n", part2);
}
