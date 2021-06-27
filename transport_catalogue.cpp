#include "transport_catalogue.h"

TransportCatalogue::StopBus TransportCatalogue::ParserBusStopCord(const std::string& text) {
    auto end_name = std::find(text.begin(), text.end(), ':');
    auto comma_first = std::find(text.begin(), text.end(), ',');
    auto comma_second = std::find(comma_first, text.end(), ',');
    if (comma_first == comma_second) {
        comma_second = text.end();
    }
    return {
        {text.begin() + 5, end_name},
        std::stod(std::string{end_name + 1, comma_first}),
        std::stod(std::string{comma_first + 1, comma_second})
    };
}

void TransportCatalogue::ParserBusStopDistance(const std::string& text) {
    using namespace std::string_literals;
    auto end_name = std::find(text.begin(), text.end(), ':');
    std::string name_stop {text.begin() + 5, end_name};
    auto first = std::find_if(stops_.begin(), stops_.end(), [&](StopBus& stopbus) {
            return stopbus.name_ == name_stop;
    });

    std::string::size_type n;
    n = text.find(" to "s);
    while (n != std::string::npos) {
        auto it = text.begin() + n;
        while (*(it - 1) != ' ')
            it--;
        double distance = std::stod(std::string{it, text.begin() + n});
        it = text.begin() + n + 4;
        while (*(it) != ',' && it !=text.end())
            it++;
        std::string stop2(text.begin() + n + 4, it);
        auto second = std::find_if(stops_.begin(), stops_.end(), [&](StopBus& stopbus) {
                return stopbus.name_ == stop2;
        });
        auto element = std::make_pair(&(*first), &(*second));
        nearby_stop_distance_[element] = distance;
        element = std::make_pair(&(*second), &(*first));
        if (!nearby_stop_distance_.count(element))
            nearby_stop_distance_[element] = distance;
        n = text.find(" to "s, n + 4);
    }
}

TransportCatalogue::Bus TransportCatalogue::ParserBus(const std::string& text) {
    char separator;
    std::vector<StopBus *> result;
    auto end_name = std::find(text.begin(), text.end(), ':');
    if (std::find(text.begin(), text.end(), '>') != text.end()) {
        separator = '>';
    } else {
        separator = '-';
    }
    auto it_begin = end_name + 2;
    while (true) {
        auto it_end = std::find(it_begin, text.end(), separator);
        if (it_end == text.end()) {
            std::string name_stop {it_begin, it_end};
            auto it = std::find_if(stops_.begin(), stops_.end(), [&](StopBus& element) {
                  return element.name_ == name_stop;
                  });
            result.push_back(&(*it));
            break ;
        } else {
            std::string name_stop {it_begin, it_end - 1};
            auto it = std::find_if(stops_.begin(), stops_.end(), [&](StopBus& element) {
                  return element.name_ == name_stop;
                  });
            result.push_back(&(*it));
            it_begin = it_end + 2;
        }   
    }
    if (separator == '-')
        result.insert(result.end(), result.rbegin() + 1, result.rend());
    return {
        {text.begin() + 4, end_name},
        result
    };
}

// add information about stops and buses
void TransportCatalogue::AddBusAndStop(const Data& data) {
   for (auto& text : data.bus_stop_) {
       // add stops to storage
       stops_.push_back(ParserBusStopCord(text));
       // create container stop_name -> bus1, bus2 busN
       stop_buses_[stops_[stops_.size() - 1].name_];
   }

   for (auto& text : data.bus_stop_) {
       size_t pos = text.find(" to ");
       if (pos != std::string::npos) {
            ParserBusStopDistance(text);
       }
   }

   for (auto& text : data.bus_route_) {
       // add buses to storage
       buses_.push_back(ParserBus(text));
       // create container bus_name -> stop1 stop2 stopN
       bus_route_[buses_[buses_.size() - 1].name_] = &buses_[buses_.size() - 1];
   }
   // continue create container stop_name -> bus1, bus2 busN
   for (auto& bus : buses_) {
       for(auto& name_stop : bus.stops_)
           stop_buses_[name_stop->name_].insert(&bus);
   }
}

//need use std::next
void TransportCatalogue::GetLenRouteGeo(TransportCatalogue::BusInformation& bus) {
    for (auto it = bus_route_[bus.name]->stops_.begin(); it <  bus_route_[bus.name]->stops_.end() - 1; ++it) {
        bus.route_lenght_geo +=  ComputeDistance({(*it)->x, (*it)->y}, {(*(it + 1))->x, (*(it + 1))->y});
    }
}

void TransportCatalogue::GetLenRouteFac(TransportCatalogue::BusInformation &bus) {
    for (auto it = bus_route_[bus.name]->stops_.begin(); it < bus_route_[bus.name]->stops_.end() - 1; ++it) {
        auto key = std::make_pair(*it, *(it + 1));
        bus.route_lenght_fac += nearby_stop_distance_[key];
    }
}

//parser and method shoulb be separated
TransportCatalogue::BusInformation TransportCatalogue::GetBusRoute(const std::string& bus_query) {
    std::unordered_set<std::string> uniq_stop;
    BusInformation bus_info {"", 0, 0, 0, 0};
    bus_info.name = bus_query;
    bus_info.name.remove_prefix(4); //delete word "Bus "
    if (bus_route_.count(bus_info.name)) {
        bus_info.stops_on_route = bus_route_.at(bus_info.name)->stops_.size();
        for_each(bus_route_.at(bus_info.name)->stops_.begin(), bus_route_.at(bus_info.name)->stops_.end(), [&](auto stop) {
            uniq_stop.insert(stop->name_);
        });
        bus_info.unique_stops = uniq_stop.size();
        uniq_stop.clear();
        GetLenRouteGeo(bus_info);
        GetLenRouteFac(bus_info);
   }
   return bus_info;
}

TransportCatalogue::StopInformation TransportCatalogue::GetStopInfo(const std::string& stop_query) {
    StopInformation stop_info;
    stop_info.name = stop_query;
    stop_info.name.remove_prefix(5); // delete word "Stop "
    if (stop_buses_.count(stop_info.name)) {
        stop_info.existence = true;
        std::for_each(stop_buses_.at(stop_info.name).begin(), stop_buses_.at(stop_info.name).end(), [&](auto& Bus) {
            stop_info.bus_name_.insert(Bus->name_);
        });
    } else {
        stop_info.existence = false;
    }
    return stop_info;
}


void TransportCatalogue::ExecuteQuery(std::string& query) {
    if (query[0] == 'B') {
        PrintBusRoute(GetBusRoute(query));
    } else if (query[0] == 'S') {
        PrintStopInfo(GetStopInfo(query));
    }
}

void TransportCatalogue::PrintBusRoute(TransportCatalogue::BusInformation&& element) {
    using namespace std::string_literals;
    std::cout << "Bus "s << element.name << ": "s;
    if (element.unique_stops) {
        std::cout << element.stops_on_route << " stops on route, "s
                    << element.unique_stops << " unique stops, "s
                    << element.route_lenght_fac << " route length, "s
                    << element.route_lenght_fac / element.route_lenght_geo << " curvature"s << std::endl;
    } else {
        std::cout << "not found"s << std::endl;
    }
}

void TransportCatalogue::PrintStopInfo(TransportCatalogue::StopInformation&& element) {
    using namespace std::string_literals;
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
