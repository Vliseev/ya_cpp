//
// Created by user on 01.02.2020.
//

#include "Request.h"
#include <sstream>

const std::unordered_map<std::string_view, Request::Type>
    STR_TO_APPEND_REQUEST_TYPE = {{"Bus", Request::Type::BUS_ADD},
                                  {"Stop", Request::Type::STOP_ADD}};

const std::unordered_map<std::string_view, Request::Type>
    STR_TO_READ_REQUEST_TYPE = {{"Bus", Request::Type::BUS_INFO}};

RequestHolder Request::Create(Request::Type type) {
    switch (type) {
        case Request::Type::BUS_ADD:
            return std::make_unique<AddBusRequest>();
        case Request::Type::STOP_ADD:
            return std::make_unique<AddStopRequest>();
        case Request::Type::BUS_INFO:
            return std::make_unique<BusInfoRequest>();
        default:
            return nullptr;
    }
}

std::optional<Request::Type> ConvTypeFromStr(std::string_view type_str,
                                             ReqConverterType req_type) {
    const std::unordered_map<std::string_view, Request::Type>* req_map;
    if (req_type == ReqConverterType::APPEND_REQUES_TYPE)
        req_map = &STR_TO_APPEND_REQUEST_TYPE;
    else if (req_type == ReqConverterType::READ_REQUEST_TYPE)
        req_map = &STR_TO_READ_REQUEST_TYPE;
    else
        return std::nullopt;

    if (const auto it = req_map->find(type_str); it != req_map->end()) {
        return it->second;
    } else {
        return std::nullopt;
    }
}

std::vector<RequestHolder> ReadRequests(std::istream& in_stream,
                                        ReqConverterType req_type) {
    auto request_count = ReadNumberOnLine<size_t>(in_stream);

    std::cerr << request_count << "\n";

    std::vector<RequestHolder> requests{};
    requests.reserve(request_count);
    for (size_t i = 0; i < request_count; ++i) {
        std::string request_str;
        getline(in_stream, request_str);

        std::cerr << request_str << "\n";

        if (auto request = ParseRequest(request_str, req_type)) {
            requests.push_back(move(request));
        }
    }

    return requests;
}

RequestHolder ParseRequest(std::string_view request_str,
                           ReqConverterType req_type) {
    const auto request_type = ConvTypeFromStr(ReadToken(request_str), req_type);
    if (!request_type) {
        return nullptr;
    }
    RequestHolder request = Request::Create(*request_type);
    if (request) {
        request->ParseFrom(request_str);
    };
    return request;
}

void AddBusRequest::ParseFrom(std::string_view input) {
    id = ReadToken(input,":");
    SkipSymbol(input);
    is_circular = IsCircularCheck(input);
    auto delimeter = is_circular ? "-" : ">";

    for (std::string_view t = ReadToken(input, delimeter); !t.empty();
         t = ReadToken(input, delimeter)) {
        t = SkipSymbol(t);
        t = Strip(t);
        route.emplace_back(t);
    }
}

void AddBusRequest::Process(DbManager& manager) const {
    manager.AddBus(id, route, is_circular);
}

bool AddBusRequest::IsCircularCheck(std::string_view s) {
    for (char it : s) {
        if (it == '-')
            return true;
        if (it == '>')
            return false;
    }
    return false;
}

void AddStopRequest::ParseFrom(std::string_view input) {
    name = ReadToken(input, ":");
    SkipSymbol(input);
    lat = std::stod(ReadToken(input, ", ").data());
    lon = std::stod(input.data());
}

void AddStopRequest::Process(DbManager& manager) const {
    manager.AddStop(name, lat, lon);
}

void BusInfoRequest::ParseFrom(std::string_view input) {
    //    id = ConvertNumber<size_t>(input);
    id = input;
}
std::string BusInfoRequest::Process(const DbManager& manager) const {
    std::ostringstream oss;
    if (!manager.ContainBus(id)) {
        oss << "Bus " << id << ": not found";
        return oss.str();
    }

    oss << "Bus " << id << ": " << manager.GetBusStop(id)
        << " stops on route, ";
    oss << manager.GetUniqueBusStop(id) << " unique stops, ";
    oss << manager.GetRouteLen(id) << " route length";

    return oss.str();
}
