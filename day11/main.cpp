#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <mdspan>
#include <utility>
#include <vector>

namespace day11 {
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

auto solution(const auto& input)
{
    std::vector<bool> empty_row(input.back().size(), true);
    std::vector<bool> empty_col(input.size(), true);
    std::vector<std::pair<size_t, size_t>> galaxies;

    for (size_t row{}; row < input.size(); ++row) {
        for (size_t col{}; col < input.back().size(); ++col) {
            if (input[row][col] != '.') {
                empty_row[row] = false;
                empty_col[col] = false;
                galaxies.emplace_back(row, col);
            }
        }
    }

    auto do_math = [&](size_t expand_factor) {
        // boost::unordered_flat_set<std::tuple<size_t, size_t,size_t, size_t>> pairs;
        size_t result{};
        for (const auto& g1 : galaxies) {
            for (const auto& g2 : galaxies) {
                if (g1 == g2) continue;
                // if(pairs.contains(std::make_tuple(g1.first, g1.second, g2.first, g2.second))) continue;
                // else pairs.insert(std::make_tuple(g2.first, g2.second, g1.first, g1.second));
                size_t min_row = std::min(g1.first, g2.first);
                size_t max_row = std::max(g1.first, g2.first);
                size_t distance_row{ max_row - min_row };

                for (; min_row < max_row; min_row++) {
                    if (empty_row[min_row]) {
                        distance_row += 1 * (expand_factor - 1);
                    }
                }

                size_t min_col = std::min(g1.second, g2.second);
                size_t max_col = std::max(g1.second, g2.second);
                size_t distance_col{ max_col - min_col };

                for (; min_col < max_col; min_col++) {
                    if (empty_col[min_col]) {
                        distance_col += 1 * (expand_factor - 1);
                    }
                }
                result += distance_col + distance_row;
            }
        }
        return result;
    };

    size_t part1{ do_math(2) };
    size_t part2{ do_math(1'000'000) };

    return std::make_pair(part1, part2);
}

}// namespace day11

int main()
{
    using time_scale = std::chrono::milliseconds;

    auto start = std::chrono::high_resolution_clock::now();
    const auto input = day11::read_file(INPUT_DIR "day11.txt");
    auto io_time = std::chrono::high_resolution_clock::now();

    auto [part1, part2] = day11::solution(input);

    std::cout << std::format("Day 11 Part 1: {}\n", part1);
    std::cout << std::format("Day 11 Part 2: {}\n", part2);

    auto execution_time = std::chrono::high_resolution_clock::now();

    auto io_ms = std::chrono::duration_cast<time_scale>(io_time - start);
    auto execution_ms = std::chrono::duration_cast<time_scale>(execution_time - io_time);
    auto total_ms = std::chrono::duration_cast<time_scale>(execution_time - start);
    std::cout << std::format("IO: {}, Execution: {}, Total: {}\n", io_ms, execution_ms, total_ms);
}
