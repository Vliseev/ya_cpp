//
// Created by user on 01.02.2020.
//

#include "DbManager.h"
#include "GeoFunction.h"

void DbManager::AddBus(std::string id, const std::vector<std::string>& route,
                       bool is_circular) {
    if (!buses_.count(id)) {
        std::vector<ShPtrStop> cur_route{};
        for (const auto& stop_name : route) {
            auto stop_it = stops_.find(stop_name);
            if (stop_it != stops_.end()) {
                cur_route.push_back(stop_it->second);
            } else {
                const auto& [it, flag] = stops_.emplace(
                    stop_name,
                    std::make_shared<Stop>(
                        stop_name, std::numeric_limits<double>::infinity(),
                        std::numeric_limits<double>::infinity()));
                cur_route.push_back(it->second);
            }
        }
        if (is_circular) {
            for (auto it = route.crbegin() + 1; it != route.crend(); ++it) {
                auto stop_name = *it;
                auto stop_it = stops_.find(stop_name);
                cur_route.push_back(stop_it->second);
            }
        }
        buses_.emplace(id, std::make_shared<Bus>(id, std::move(cur_route)));
    }
}
void DbManager::AddStop(const std::string& name, double lat, double lon) {
    auto stop_it = stops_.find(name);
    if (stop_it == stops_.end()) {
        stops_.emplace(name, std::make_shared<Stop>(name, lat, lon));
    } else {
        auto stop_ptr = stop_it->second;
        if (stop_ptr->GetPos().lat == std::numeric_limits<double>::infinity() &&
            stop_ptr->GetPos().lon == std::numeric_limits<double>::infinity())
            stop_ptr->SetPos({lat, lon});
    }
}

size_t DbManager::GetBusStop(std::string bus_id) const {
    auto it = bus_stop_cache_.find(bus_id);
    if (it != bus_stop_cache_.end()) {
        return it->second;
    } else {
        auto const& bus = buses_.at(bus_id);
        auto stop_size = bus->GetRout().size();
        bus_stop_cache_.emplace(bus_id, stop_size);
        return stop_size;
    }
}

size_t DbManager::GetUniqueBusStop(std::string bus_id) const {
    auto it = unique_bus_stop_cache_.find(bus_id);
    if (it != unique_bus_stop_cache_.end()) {
        return it->second;
    } else {
        auto const& bus = buses_.at(bus_id);
        std::unordered_set<std::string_view> unique_stop;

        for (const auto& stop : bus->GetRout()) {
            unique_stop.insert(stop->GetName());
        }
        auto unique_stop_size = unique_stop.size();

        bus_stop_cache_.emplace(bus_id, unique_stop_size);
        return unique_stop_size;
    }
}

double DbManager::GetRouteLen(std::string bus_id) const {
    auto it = route_cache_.find(bus_id);
    if (it != route_cache_.end()) {
        return it->second;
    } else {
        auto route = buses_.at(bus_id)->GetRout();
        auto f_it = route.begin();
        auto s_it = f_it + 1;
        double dist = 0;

        for (; s_it != route.end(); ++s_it, ++f_it) {
            dist += ComputeDist(**f_it, **s_it);
        }
        route_cache_.emplace(bus_id, dist);

        return dist;
    }
}
bool DbManager::ContainBus(  std::string bus_id) const {
    return buses_.count(bus_id);
}
