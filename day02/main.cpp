#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include <scn/scn.h>

namespace day02 {
std::vector<std::string> read_file(std::string_view file_path)
{
    std::ifstream infile(file_path);
    std::vector<std::string> output;
    std::string line;
    while (std::getline(infile, line)) {
        output.push_back(line);
    }
    return output;
}

static const std::unordered_map<std::string_view, int> bag_content = { { { "red", 12 }, { "green", 13 }, { "blue", 14 } } };

struct min_game
{
    int idx;
    int red{};
    int green{};
    int blue{};
};

min_game parse_line(std::string_view line)
{
    int idx{};
    auto result = scn::scan(line, "Game {}: ", idx);
    std::string_view games = result.reconstruct();

    min_game max_values{ .idx = idx };

    for (const auto game : games | std::views::split(';')) {
        for (const auto draw : game | std::views::split(',')) {
            int amount{};
            std::string_view color;
            (void)scn::scan(std::string_view{ draw }, "{} {}", amount, color);

            if(color == "red") { max_values.red = std::max(max_values.red, amount);
}
            if(color == "green") { max_values.green = std::max(max_values.green, amount);
}
            if(color == "blue") { max_values.blue = std::max(max_values.blue, amount);
}
        }
    }
    return max_values;
}

bool is_valid(const min_game& min_values)
{
    return bag_content.at("red") >= min_values.red && bag_content.at("green") >= min_values.green && bag_content.at("blue") >= min_values.blue;
}

std::pair<int, int> part1and2(const std::vector<std::string>& input)
{
    auto games = input | std::views::transform(parse_line);
    auto part1 = games | std::views::filter(is_valid) | std::views::transform([](auto x) { return x.idx; });
    auto part2 = games | std::views::transform([](auto x) { return x.red * x.green * x.blue; });

    return { 
        std::accumulate(part1.begin(), part1.end(), 0), 
        std::accumulate(part2.begin(), part2.end(), 0) 
        };
}

}// namespace day02

int main()
{
    const auto input = day02::read_file(INPUT_DIR "day02.txt");
    auto [part1, part2] = day02::part1and2(input);
    std::cout << std::format("Day 02 Part 1: {}\n", part1);
    std::cout << std::format("Day 02 Part 2: {}\n", part2);
}
