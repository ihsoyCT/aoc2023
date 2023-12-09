#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    int num;
    string _;
    ifstream file(INPUT_DIR "day06.txt");

    string line0, line1;

    getline(file, line0);
    getline(file, line1);

    vector<int> times;
    stringstream tt(line0);
    tt >> _;
    while (tt >> num) times.push_back(num);

    vector<int> records;
    stringstream tl(line1);
    tl >> _;
    while (tl >> num) records.push_back(num);


    for (int64_t k = 0; k < 100000; ++k) {
        
    int time, record, numWins, travelled;
    int volatile score = 1;
        for (int i{}; i < times.size(); ++i) {
            numWins = 0;
            time = times[i];
            record = records[i];

            for (int t{ 1 }; t <= time; ++t) {
                travelled = t * (time - t);
                if (travelled > record) numWins += 1;
            }
            score *= numWins;
        }
    }
    
    int time, record, numWins, travelled;
    int score = 1;
    for (int i{}; i < times.size(); ++i) {
        numWins = 0;
        time = times[i];
        record = records[i];

        for (int t{ 1 }; t <= time; ++t) {
            travelled = t * (time - t);
            if (travelled > record) numWins += 1;
        }
        score *= numWins;
    }
    cout << score << '\n';
}