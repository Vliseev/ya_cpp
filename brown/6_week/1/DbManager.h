#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Bus.h"

class DbManager {
 public:
    static DbManager& GetInstance() {
        static DbManager instance;
        return instance;
    }
    DbManager(const DbManager&) = delete;
    DbManager& operator=(const DbManager&) = delete;

    void AddBus(std::string id, const std::vector<std::string>& route,
                bool is_circular);
    void AddStop(const std::string& name, double lat, double lon);

    size_t GetBusStop(std::string bus_id) const;
    size_t GetUniqueBusStop(std::string bus_id) const;
    double GetRouteLen(  std::string bus_id) const;
    bool ContainBus(  std::string bus_id) const;

 private:
    DbManager() = default;
    ~DbManager() = default;

    std::unordered_map<std::string, ShPtrStop> stops_;
    std::unordered_map<std::string, ShPtrBus> buses_;

    mutable std::unordered_map<std::string, size_t> bus_stop_cache_{};
    mutable std::unordered_map<std::string, size_t> unique_bus_stop_cache_{};
    mutable std::unordered_map<std::string, double> route_cache_{};
};
