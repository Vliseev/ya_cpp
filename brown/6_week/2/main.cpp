//
// Created by user on 01.02.2020.
//
#include <sstream>
#include <string_view>
#include "Request.h"

/*"10\n"
"Stop Tolstopaltsevo: 55.611087, 37.20829\n"
"Stop Marushkino: 55.595884, 37.209755\n"
"Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo "
"Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n"
"Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n"
"Stop Rasskazovka: 55.632761, 37.333324\n"
"Stop Biryulyovo Zapadnoye: 55.574371, 37.6517\n"
"Stop Biryusinka: 55.581065, 37.64839\n"
"Stop Universam: 55.587655, 37.645687\n"
"Stop Biryulyovo Tovarnaya: 55.592028, 37.653656\n"
"Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n"
"3\n"
"Bus 256\n"
"Bus 750\n"
"Bus 751"
 */

int main() {
    auto& manager = DbManager::GetInstance();

    auto append_req =
        ReadRequests(std::cin, ReqConverterType::APPEND_REQUES_TYPE);

    for (auto& req : append_req) {
        const auto& request = static_cast<const ModifyRequest&>(*req);
        request.Process(manager);
    }

    auto read_req = ReadRequests(std::cin, ReqConverterType::READ_REQUEST_TYPE);
    for (auto& req : read_req) {
        const auto& request = static_cast<const BusInfoRequest&>(*req);
        std::cout << request.Process(manager) << std::endl;
    }

    return 0;
}
