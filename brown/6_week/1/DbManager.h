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

    void AddBus(size_t id, const std::vector<std::string>& route,
                bool is_circular);
    void AddStop(const std::string& name, double lat, double lon);

 private:
    DbManager() = default;
    ~DbManager() = default;

    std::unordered_map<std::string, ShPtrStop> stops_;
    std::unordered_map<size_t, ShPtrBus> buses_;
};
