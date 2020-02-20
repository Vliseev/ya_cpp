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

    void AddBus(const std::string& id, const std::vector<std::string>& route,
                bool is_circular);
    void AddStop(const std::string& name, double lat, double lon);

    size_t GetBusStop(const std::string& bus_id) const;
    size_t GetUniqueBusStop(const std::string& bus_id) const;
    double GetRouteLen(const std::string& bus_id) const;

    std::vector<std::string_view> GetStopBuses(const std::string& name) const;

    bool ContainBus(const std::string& bus_id) const;
    bool ContainStop(const std::string& stop) const;

   private:
    DbManager() = default;
    ~DbManager() = default;

    std::unordered_map<std::string, ShPtrStop> stops_;
    std::unordered_map<std::string, ShPtrBus> buses_;

    mutable std::unordered_map<std::string, size_t> bus_stop_cache_{};
    mutable std::unordered_map<std::string, size_t> unique_bus_stop_cache_{};
    mutable std::unordered_map<std::string, double> route_cache_{};
};
