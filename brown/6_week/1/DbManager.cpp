//
// Created by user on 01.02.2020.
//

#include "DbManager.h"
void DbManager::AddBus(size_t id, const std::vector<std::string>& route,
                       bool is_circular) {
    if (!buses_.count(id)) {
        std::vector<ShPtrStop> cur_route{};
        for (const auto& stop_name : route) {
            auto stop_it = stops_.find(stop_name);
            if (stop_it != stops_.end()) {
                cur_route.push_back(stop_it->second);
            } else {
                stops_.emplace(
                    stop_name,
                    std::make_shared<Stop>(
                        stop_name, std::numeric_limits<double>::infinity(),
                        std::numeric_limits<double>::infinity()));
            }
        }
        if (is_circular) {
            for (auto it = route.crbegin() - 1; it != route.crend(); ++it) {
                auto stop_name = *it;
                auto stop_it = stops_.find(stop_name);
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
