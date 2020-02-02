//
// Created by user on 01.02.2020.
//

#include "Request.h"
RequestHolder Request::Create(Request::Type type) {
    switch (type) {
        case Request::Type::BUS_ADD:
            //            return make_unique<ComputeIncomeRequest>();
        case Request::Type::STOP_ADD:
            //            return make_unique<EarnRequest>();
        case Request::Type::BUS_INFO:
            //            return make_unique<PayTaxRequest>();
        default:
            return nullptr;
    }
}
