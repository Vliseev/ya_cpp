//
// Created by user on 10.02.2020.
//
#include "GeoFunction.h"
#include <cmath>
/*
 acos(sin(lhs.lat) * sin(rhs.lat) +
cos(lhs.lat) * cos(rhs.lat) *
cos(abs(lhs.lon - rhs.lon))
) * 6371000
 * */
static constexpr double PI = 3.1415926535;
double ComputeDist(const Stop& lhs, const Stop& rhs) {
    auto [lat_1, lon_1] = lhs.GetPos();
    auto [lat_2, lon_2] = rhs.GetPos();
    lat_1 = lat_1 * PI / 180;
    lon_1 = lon_1 * PI / 180;
    lat_2 = lat_2 * PI / 180;
    lon_2 = lon_2 * PI / 180;

    return std::acos(std::sin(lat_1) * std::sin(lat_2) +
                     std::cos(lat_1) * std::cos(lat_2) *
                         std::cos(std::abs(lon_1 - lon_2))) *
           R;
}
