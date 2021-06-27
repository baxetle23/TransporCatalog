#include <iostream>
#include "transport_catalogue.h"
#include "stat_reader.h"

using namespace std::string_literals;

int main()
{
    Data data;
    AddData(data);
    TransportCatalogue transport_catalogue;
    transport_catalogue.AddBusAndStop(data); 
    Query query;
    AddQuery(query);
    for (auto element : transport_catalogue.GetBusRoute(query.bus_query_)) {
        std::cout << "Bus "s << element.name << ": "s; 
        if (element.unique_stops) {
            std::cout << element.stops_on_route << " stops on route, "s 
            << element.unique_stops << " unique stops, " << 
            element.route_lenght << " route length" << std::endl;
        } else {
            std::cout << "not found"s << std::endl; 
        }
    }

    for (auto& element : transport_catalogue.GetStopInfo(query.stop_query_)) {
        if (element.existence) {
            if (element.bus_name_.size()) {
                std::cout << "Stop "s << element.name << ": buses "s;
                for (const auto& bus : element.bus_name_) {
                    std::cout << bus << " "s;
                }
                std::cout << std::endl;
            } else {
                std::cout << "Stop "s << element.name << ": no buses"s << std::endl;
            }

        } else {
            std::cout << "Stop "s << element.name << ": not found"s << std::endl;
        }
    }
}
