#pragma once

#include "input_reader.h"
#include "stat_reader.h"
#include "geo.h"
#include <deque>
#include <unordered_map>
#include <string_view>
#include <algorithm>
#include <string>
#include <vector>
#include <set>

class TransportCatalogue {
public:
    //
    struct BusInformation {
        std::string_view name;
        int stops_on_route;
        int unique_stops;
        double route_lenght;
    };

    struct StopInformation {
        std::set<std::string_view> bus_name_;
        std::string_view name;
        bool existence; // this is costyl
    };

    void AddBusAndStop(const Data& data);
    void ExecuteQuery(std::string& query);

    StopInformation GetStopInfo(const std::string& query);
    BusInformation GetBusRoute(const std::string& query);

    void    PrintBusRoute(BusInformation&& info);
    void    PrintStopInfo(StopInformation&& info);

private:
    struct  StopBus {
        std::string name_;
        double  x;
        double  y;
    };

    struct Bus {
        std::string name_;
        std::vector<StopBus *> stops_;
    };

    //container Buses and stops
    std::deque<Bus> buses_;
    std::deque<StopBus> stops_;
    //name Bus -> Bus route
    std::unordered_map<std::string_view, Bus *> bus_route_;
    //name Stop -> missing bus
    std::unordered_map<std::string_view, std::unordered_set<Bus *>> stop_buses_;

    StopBus ParserBusStop(const std::string& text);
    Bus ParserBus(const std::string& text);
    void GetLenRoute(BusInformation& bus);
};
