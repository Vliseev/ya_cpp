#pragma once
#include <charconv>
#include <memory>
#include <string_view>
#include "DbManager.h"

struct Request;
using RequestHolder = std::unique_ptr<Request>;

struct Request {
    enum class Type { BUS_ADD, STOP_ADD, BUS_INFO };

    Request(Type type) : type_(type) {}
    static RequestHolder Create(Type type);
    virtual void ParseFrom(std::string_view input) = 0;
    virtual ~Request() = default;

    const Type type_;
};

template <typename ResultType>
struct ReadRequest : Request {
    using Request::Request;
    virtual ResultType Process(const DbManager& manager) const = 0;
};

struct ModifyRequest : Request {
    using Request::Request;
    virtual void Process(DbManager& manager) const = 0;
};

template <class T>
T ConvertNumber(std::string_view text) {
    T val;
    if (auto [p, ec] =
            std::from_chars(text.data(), text.data() + text.size(), val);
        ec == std::errc()) {
        std::string err_str("invalid_arg: ");
        err_str += text;
        throw std::invalid_argument(err_str);
    }
    return val;
}

struct AddBusRequest : public ModifyRequest {
    AddBusRequest() : ModifyRequest(Type::BUS_ADD){};

    void ParseFrom(std::string_view input) override {

        //        date_from = Date::FromString(ReadToken(input));
        //        date_to = Date::FromString(ReadToken(input));
        //        percent = ConvertToInt(input);
    }

    std::vector<std::string> route;
    size_t id;
};