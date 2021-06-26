#include "transport_catalogue.h"

TransportCatalogue::StopBus TransportCatalogue::ParserBusStop(const std::string& text) {
    auto end_name = std::find(text.begin() + 5, text.end(), ':');
    auto comma = std::find(end_name, text.begin(), ',');
    return {
        {text.begin() + 5, end_name},
        std::stod(std::string{end_name + 2, comma}),
        std::stod(std::string{comma + 2, text.end()})
    };
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

// ??????? ??????? ???????????
void TransportCatalogue::AddBusAndStop(const Data& data) {
   // ????????? ????????? ?????????
   for (auto& text : data.bus_stop_) {
       stops_.push_back(ParserBusStop(text));
       stop_buses_[stops_[stops_.size() - 1].name_];
   }
   // ????????? ????????? ????????? ? ?????? ? ???????? ???????? ?? ?????
   for (auto& text : data.bus_route_) {
       buses_.push_back(ParserBus(text));
       bus_route_[buses_[buses_.size() - 1].name_] = &buses_[buses_.size() - 1];
   }
   // ????????? ?????? ? ?????????? ????????? ????? ??? ?????????
   for (auto& bus : buses_) {
       for(auto& name_stop : bus.stops_)
           stop_buses_[name_stop->name_].insert(&bus);
   }
}

//??? ???????? ?????????? ???????????? std::next
void TransportCatalogue::GetLenRoute(TransportCatalogue::BusInformation& bus) {
    for (auto it = bus_route_[bus.name]->stops_.begin(); it <  bus_route_[bus.name]->stops_.end() - 1; ++it) {
        bus.route_lenght +=  ComputeDistance({(*it)->x, (*it)->y}, {(*(it + 1))->x, (*(it + 1))->y});
    }
}


std::vector<TransportCatalogue::BusInformation> TransportCatalogue::GetBusRoute(const std::vector<std::string>& query) {
    std::vector<BusInformation> result;
    std::unordered_set<std::string> uniq_stop;
    for (auto& bus_query : query) {
        BusInformation bus_info {"", 0, 0, 0}; 
        bus_info.name = bus_query;
        bus_info.name.remove_prefix(4);
        if (bus_route_.count(bus_info.name)) {
            bus_info.stops_on_route = bus_route_.at(bus_info.name)->stops_.size();
            for_each(bus_route_.at(bus_info.name)->stops_.begin(), bus_route_.at(bus_info.name)->stops_.end(), [&](auto stop) {
                uniq_stop.insert(stop->name_);
            });
            bus_info.unique_stops = uniq_stop.size();
            uniq_stop.clear();
            GetLenRoute(bus_info);
        }
        result.push_back(bus_info);
    }
    return result;
}
























