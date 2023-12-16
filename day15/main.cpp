#include <algorithm>
#include <chrono>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>


namespace day15 {
auto read_file(const std::string& file_path)
{
    std::ifstream infile(file_path);
    std::vector<std::string> data{};
    std::string line;
    while (std::getline(infile, line, ',')) {
        data.push_back(line);
    }
    return data;
}


size_t calculate_hash(std::string_view data)
{
    size_t value{};
    for (char c : data) {
        value += static_cast<size_t>(c);
        value *= 17;
        value = value % 256;
    }
    return value;
}

auto part1(const std::vector<std::string>& input)
{
    int64_t value{};

    for (const auto& line : input) {
        value += calculate_hash(line);
    }

    return value;
}

template<auto hashFn> class hashmap
{
  public:
    void insert(std::string_view key, int8_t value)
    {
        auto& bucket = buckets.at(hashFn(key));
        if (auto it = find(bucket, key); it != bucket.end()) {
            *it = std::make_pair(key, value);
        } else {
            bucket.emplace_back(key, value);
        }
    }
    void remove(std::string_view key)
    {
        auto& bucket = buckets.at(hashFn(key));
        if (auto it = find(bucket, key); it != bucket.end()) {
            bucket.erase(it);
        }
    }
    size_t calculate_fp()
    {
        size_t fp{};
        for (size_t box{ 1 }; box <= buckets.size(); ++box) {
            const auto& bucket = buckets.at(box - 1);
            size_t slot_i{ 1 };
            for (const auto& slot : bucket) {
                fp += box * slot_i * static_cast<size_t>(slot.second);
                ++slot_i;
            }
        }
        return fp;
    }

  private:
    std::array<std::vector<std::pair<std::string_view, int8_t>>, 256> buckets{};
    decltype(buckets)::value_type::iterator find(decltype(buckets)::value_type& bucket, std::string_view key)
    {
        return std::find_if(bucket.begin(), bucket.end(), [&key](auto& el) { return el.first == key; });
    }
};

auto part2(const std::vector<std::string>& input)
{
    hashmap<calculate_hash> map;

    for (const auto& line : input) {
        if (auto pos = line.find('-'); pos != std::string::npos) {
            map.remove(line.substr(0, line.size() - 1));
        } else {
            auto pos2 = line.find('=');
            std::string_view key = std::string_view(line).substr(0, pos2);
            int8_t value = static_cast<int8_t>(std::stoi(line.substr(pos2 + 1)));
            map.insert(key, value);
        }
    }
    return map.calculate_fp();
}

}// namespace day15

int main()
{
    using time_scale = std::chrono::microseconds;

    auto start = std::chrono::high_resolution_clock::now();
    const auto input = day15::read_file(INPUT_DIR "day15.txt");
    auto io_time = std::chrono::high_resolution_clock::now();
    std::cout << std::format("Day 15 Part 1: {}\n", day15::part1(input));
    std::cout << std::format("Day 15 Part 2: {}\n", day15::part2(input));

    auto execution_time = std::chrono::high_resolution_clock::now();

    auto io_ms = std::chrono::duration_cast<time_scale>(io_time - start);
    auto execution_ms = std::chrono::duration_cast<time_scale>(execution_time - io_time);
    auto total_ms = std::chrono::duration_cast<time_scale>(execution_time - start);
    std::cout << std::format("IO: {}, Execution: {}, Total: {}\n", io_ms, execution_ms, total_ms);
}
