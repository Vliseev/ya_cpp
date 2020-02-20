#pragma once
#include <memory>
#include <unordered_set>
#include <vector>
#include "Stop.h"

struct Stop {
    struct Position {
        double lat{};
        double lon{};
        Position(const double lat, const double lon) : lat(lat), lon(lon){};
        Position() = default;
    };
    Stop() = default;
    Stop(const std::string& name, const Position& pos)
        : name_(name), pos_(pos) {
    }
    Stop(const std::string& name, const double lat, const double lon)
        : name_(name), pos_(lat, lon) {
    }

   private:
    std::string name_{};

   public:
    void SetPos(const Position& pos) {
        pos_ = pos;
    }
    void SetPos(const Position&& pos) {
        pos_ = pos;
    }

   private:
    Position pos_{};
    std::unordered_set<std::string> buses_{};

   public:
    const std::string& GetName() const {
        return name_;
    }
    const Position& GetPos() const {
        return pos_;
    }
    void AddBus(const std::string& bus) {
        buses_.insert(bus);
    }

    const std::unordered_set<std::string>& GetBuses() const {
        return buses_;
    }
};
using ShPtrStop = std::shared_ptr<Stop>;