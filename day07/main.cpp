#include "scn/scan/scan.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>


namespace day07 {
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

enum class hand_type_t {
    FIVE_OF_A_KIND = 0, 
    FOUR_OF_A_KIND = 1,
    FULL_HOUSE = 2,
    THREE_OF_A_KIND = 3, 
    TWO_PAIR = 4,
    ONE_PAIR = 5,
    HIGH_CARD = 6,
    UNKNOWN = 1000
};

bool part2_active = false;
static const std::string deck1 = "23456789TJQKA";
static const std::string deck2 = "J23456789TQKA";

struct hand_t {
    std::string cards;
    int64_t bid;
    mutable int64_t hand_type_cache {-1};
    mutable int64_t hand_type_cache2 {-1};
    
    int64_t get_hand_type() const {
        if(cards.contains("JJJJJ")) return 0;
        if(!part2_active && hand_type_cache != -1) {
            return hand_type_cache;
        }
        if(part2_active && hand_type_cache2 != -1) {
            return hand_type_cache2;
        }
        
        int wild_count{};
        std::unordered_map<char, int64_t> card_count {};
        for(const auto& card: cards) {
            if(card == 'J' && part2_active) {
                wild_count++;
            } else {
                card_count[card]++;
            }
        }
        auto func =[&]() {
            auto values = std::views::values(card_count) | std::ranges::to<std::vector>();
            std::sort(values.begin(), values.end(), std::greater<>());
            values[0]+=wild_count;
            if(values[0] == 5) return 0;
            if(values[0] == 4) return 1;
            if(values[0] == 3 && values[1] == 2) return 2;
            if(values[0] == 3 && values[1] == 1) return 3;
            if(values[0] == 2 && values[1] == 2) return 4;
            if(values[0] == 2 && values[1] == 1) return 5;
            return 6;
        };

        if(part2_active) {
            hand_type_cache2 = func();
            return hand_type_cache2;
        }
        hand_type_cache = func();
        return hand_type_cache;
    }

    auto operator<=>(const hand_t& rhs) const {
        const std::string& deck = (part2_active)?deck2:deck1;
        if(get_hand_type() <rhs.get_hand_type()) return 1;
        else if(get_hand_type() > rhs.get_hand_type()) return -1;
        for(const auto& cmp : std::views::zip(cards, rhs.cards)) {
            if(cmp.first == cmp.second) continue;
            return static_cast<int>(deck.find(cmp.first)) - static_cast<int>(deck.find(cmp.second));
        }
        return 0;
    }
};


auto solution(const std::vector<std::string>& input, bool part2 = false)
{
    part2_active=part2;
    int64_t sum {0};
    std::vector<hand_t> hands;
    for(const auto& line : input) {
        hand_t hand;
        (void)scn::scan(line, "{} {}", hand.cards, hand.bid);
        hands.push_back(hand);
    }
    std::sort(hands.begin(), hands.end());

    int64_t i {1};
    for(const auto& hand: hands) {
        sum += hand.bid * i;
        i++;
    }

    return sum;
}

}// namespace day07

int main()
{
    using time_scale = std::chrono::microseconds;

    auto start = std::chrono::high_resolution_clock::now();
    const auto input = day07::read_file(INPUT_DIR "day07.txt");
    auto io_time = std::chrono::high_resolution_clock::now();


    std::cout << std::format("Day 07 Part 1: {}\n", day07::solution(input));
    std::cout << std::format("Day 07 Part 2: {}\n", day07::solution(input, true));
    
    auto execution_time = std::chrono::high_resolution_clock::now();

    auto io_ms = std::chrono::duration_cast<time_scale>(io_time - start);
    auto execution_ms = std::chrono::duration_cast<time_scale>(execution_time - io_time);
    auto total_ms = std::chrono::duration_cast<time_scale>(execution_time - start);
    std::cout << std::format("IO: {}, Execution: {}, Total: {}\n", io_ms, execution_ms, total_ms);
}
