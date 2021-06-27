#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_set>

struct Query {
    std::vector<std::string> bus_query_;
    std::vector<std::string> stop_query_;

};
void AddQuery(Query& query);


