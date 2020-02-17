//
// Created by user on 10.02.2020.
//

#pragma once
#include <cmath>
#include "Stop.h"

static const double R = 6371000.0;
double ComputeDist(const Stop& lhs, const Stop& rhs);