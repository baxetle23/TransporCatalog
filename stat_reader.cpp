#include "stat_reader.h"

void AddQuery(Query& query) {
    std::string string_input;
    int count_;
   // std::ifstream in("/Users/lcash/Desktop/Qt_project/lol/infile");
    //in >> count_;
    //for (int i = 0; i <= count_; i++)
   // std::getline(std::cin, string_input);
    std::cin >> count_;
    getline(std::cin, string_input);
    while(count_-- > 0) {
		getline(std::cin, string_input);
        if (string_input[0] == 'B') {
            query.bus_query_.push_back(move(string_input));
        } else if (string_input[0] == 'S')
            query.stop_query_.push_back(move(string_input));
    }
}

