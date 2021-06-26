#include "input_reader.h"

void AddData(Data& data) {
    std::string string_input;
    int count_;
    std::ifstream in("/Users/lcash/Desktop/Qt_project/lol/infile");
    in >> count_;
    while (count_-- >= 0 && std::getline(in, string_input)) {
        if (string_input[0] == 'S') {
            data.bus_stop_.insert(move(string_input));
        } else if (string_input[0] == 'B') {
            data.bus_route_.insert(move(string_input));
        }
    }
}

void    Data::Print(std::ostream& out) const {
    using namespace std::string_literals;
    out << "Print bus stop"s << std::endl;
    for (const auto& stop : bus_stop_) {
        out << stop << std::endl;
    }
    out << "Print bus route"s << std::endl;
    for (const auto& route : bus_route_) {
        out << route << std::endl;
    }
}

std::ostream& operator<<(std::ostream &out, const Data& data) {
    data.Print(out);
    return out;
}