#include "stat_reader.h"

std::vector<std::string> AddQuery() {
    std::vector<std::string> query;
    std::string string_input;
    int count_;
    std::ifstream in("/Users/lcash/Desktop/Qt_project/lol/infile");
    in >> count_;
    for (int i = 0; i <= count_; i++)
        std::getline(in, string_input);
    in >> count_;
    getline(in, string_input);
    while(count_-- >= 0 && getline(in, string_input)) {
        query.push_back(move(string_input));
    }
    return query;
}

void Print(std::vector<std::tuple<int, int, double>>& answer) {
    
}