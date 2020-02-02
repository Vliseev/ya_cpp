#pragma once
//
// Created by user on 02.02.2020.
//
#include <memory>
#include <vector>
#include "Stop.h"

struct Bus {
 private:
    size_t id_{};
    std::vector<ShPtrStop> rout_{};

 public:
    Bus(size_t id, const std::vector<ShPtrStop>& rout) : id_(id), rout_(rout) {}

    Bus() = default;

    size_t GetId() const {
        return id_;
    }
    const std::vector<ShPtrStop>& GetRout() const {
        return rout_;
    }
};

using ShPtrBus = std::shared_ptr<Bus>;