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
    std::vector<std::string> query = AddQuery();
    for (auto element : transport_catalogue.GetBusRoute(query)) {
        std::cout << "Bus "s << element.name << ": "s; 
        if (element.unique_stops) {
            std::cout << element.stops_on_route << " stops on route, "s 
            << element.unique_stops << " unique stops, " << 
            element.route_lenght << " route length" << std::endl;
        } else {
            std::cout << "not found"s << std::endl; 
        }
    }
}
