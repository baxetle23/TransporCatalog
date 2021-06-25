#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <iostream>

struct Data {
    std::unordered_set<std::string> bus_stop_;
    std::unordered_set<std::string> bus_route_;
    void    Print(std::ostream& out) const;
    friend std::ostream& operator<< (std::ostream &out, const Data& data);
};

void    AddData(Data& data);
