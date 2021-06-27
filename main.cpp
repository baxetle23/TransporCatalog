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

    int count;
    std::string query;
    std::cin >> count;
    std::getline(std::cin, query);
    while (count-- > 0) {
        getline(std::cin, query);
        transport_catalogue.ExecuteQuery(query);
    }
}
