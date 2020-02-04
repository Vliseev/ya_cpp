//
// Created by user on 01.02.2020.
//
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include "StringOp.h"

/*
Stop Tolstopaltsevo: 55.611087, 37.20829
Stop Marushkino: 55.595884, 37.209755
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya >
Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
Bus 750: Tolstopaltsevo -
Marushkino - Rasskazovka
 */

int main() {
    std::string s =
        "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo "
        "Tovarnaya >\n"
        "Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye";

    std::string_view sw = s;
    auto tok = ReadToken(sw);
    sw = SkipSymbol(sw);
    std::cout << sw;

    return 0;
}