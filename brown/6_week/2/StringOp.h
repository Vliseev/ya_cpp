//
// Created by vlad on 03.02.2020.
//

#pragma once
#include <charconv>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

std::pair<std::string_view, std::optional<std::string_view>> SplitTwoStrict(
    std::string_view s, std::string_view delimiter = " ");
std::pair<std::string_view, std::string_view> SplitTwo(
    std::string_view s, std::string_view delimiter = " ");
std::string_view ReadToken(std::string_view& s,
                           std::string_view delimiter = " ");
std::string_view SkipSymbol(std::string_view s, char symbol = ' ');
std::string_view Strip(std::string_view s, char symbol = ' ');

template <typename Number>
Number ReadNumberOnLine(std::istream& stream) {
    Number number;
    stream >> number;
    std::string dummy;
    getline(stream, dummy);
    return number;
}

template <class T>
T ConvertNumber(std::string_view text) {
    T val;
    if (auto [p, ec] =
            std::from_chars(text.data(), text.data() + text.size(), val);
        ec != std::errc()) {
        std::string err_str("invalid_arg: ");
        err_str += text;
        throw std::invalid_argument(err_str);
    }
    return val;
}