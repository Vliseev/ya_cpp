#pragma once
#include <iostream>
#include <memory>
#include <string_view>
#include "DbManager.h"
#include "StringOp.h"

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

enum class ReqConverterType { READ_REQUEST_TYPE, APPEND_REQUES_TYPE };

extern const std::unordered_map<std::string_view, Request::Type>
    STR_TO_APPEND_REQUEST_TYPE;
extern const std::unordered_map<std::string_view, Request::Type>
    STR_TO_READ_REQUEST_TYPE;

template <typename ResultType>
struct ReadRequest : Request {
    using Request::Request;
    virtual ResultType Process(const DbManager& manager) const = 0;
};

struct ModifyRequest : Request {
    using Request::Request;
    virtual void Process(DbManager& manager) const = 0;
};

struct AddBusRequest : public ModifyRequest {
    AddBusRequest() : ModifyRequest(Type::BUS_ADD){};

    void ParseFrom(std::string_view input) override;
    void Process(DbManager& manager) const override;

    std::vector<std::string> route{};
    std::string id{};
    bool is_circular{};

   private:
    static bool IsCircularCheck(std::string_view);
};

struct AddStopRequest : public ModifyRequest {
    AddStopRequest() : ModifyRequest(Type::STOP_ADD){};

    void ParseFrom(std::string_view input) override;
    void Process(DbManager& manager) const override;

    std::string name{};
    double lat{};
    double lon{};
};

struct BusInfoRequest : public ReadRequest<std::string> {
    BusInfoRequest() : ReadRequest<std::string>(Type::BUS_INFO){};
    void ParseFrom(std::string_view input) override;
    std::string Process(const DbManager& manager) const override;

    std::string id{};
};

RequestHolder ParseRequest(std::string_view request_str,
                           ReqConverterType req_type);

std::vector<RequestHolder> ReadRequests(
    std::istream& in_stream = std::cin,
    ReqConverterType req_type = ReqConverterType::APPEND_REQUES_TYPE);