#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

namespace day13 {
auto read_file(const std::string& file_path)
{
    std::ifstream infile(file_path);
    std::vector<std::vector<std::string>> data{};
    std::string line;


    data.emplace_back();
    while (std::getline(infile, line)) {
        if (!line.empty()) {
            data.back().push_back(line);
        } else {
            data.emplace_back();
        }
    }
    return data;
}

std::vector<std::string> transpose_matrix(const std::vector<std::string>& mat)
{
    std::vector<std::string> trans(mat.back().size());

    for (const auto& el : mat) {
        size_t i{};
        for (char c : el) {
            trans[i] += c;
            ++i;
        }
    }
    return trans;
}

void are_same(std::string_view a, std::string_view b, int64_t& errors_allowed)
{
    for (size_t i{}; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            --errors_allowed;
        }
    }
}


bool is_horizontal_reflection(const std::vector<std::string>& data, size_t index_l, int64_t errors_allowed)
{
    size_t index_r = index_l + 1;
    while (index_l >= 0 && index_r < data.size()) {
        are_same(data[index_l], data[index_r], errors_allowed);
        if (index_l == 0) break;
        --index_l;
        ++index_r;
    }

    return errors_allowed == 0;
}

auto solution(const std::vector<std::vector<std::string>>& input, int64_t errors_allowed)
{
    size_t value {};
    for (const auto& mat : input) {
        auto transposed = transpose_matrix(mat);
        for (size_t i{ 0 }; i < mat.size() - 1; ++i) {
            if (is_horizontal_reflection(mat, i, errors_allowed)) {
                value += (i+1) * 100;
            }
        }
        for (size_t i{ 0 }; i < transposed.size() - 1; ++i) {
            if (is_horizontal_reflection(transposed, i, errors_allowed)) {
                value += (i+1);
            }
        }
    }
    return value;
}

}// namespace day13

int main()
{
    using time_scale = std::chrono::microseconds;

    auto start = std::chrono::high_resolution_clock::now();
    const auto input = day13::read_file(INPUT_DIR "day13.txt");
    auto io_time = std::chrono::high_resolution_clock::now();

    std::cout << std::format("Day 13 Part 1: {}\n", day13::solution(input, 0));
    std::cout << std::format("Day 13 Part 2: {}\n", day13::solution(input, 1));

    auto execution_time = std::chrono::high_resolution_clock::now();

    auto io_ms = std::chrono::duration_cast<time_scale>(io_time - start);
    auto execution_ms = std::chrono::duration_cast<time_scale>(execution_time - io_time);
    auto total_ms = std::chrono::duration_cast<time_scale>(execution_time - start);
    std::cout << std::format("IO: {}, Execution: {}, Total: {}\n", io_ms, execution_ms, total_ms);
}
