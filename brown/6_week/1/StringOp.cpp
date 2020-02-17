//
// Created by vlad on 03.02.2020.
//

#include "StringOp.h"
std::string_view ReadToken(std::string_view& s, std::string_view delimiter) {
    const auto [lhs, rhs] = SplitTwo(s, delimiter);
    s = rhs;
    return lhs;
}
std::pair<std::string_view, std::optional<std::string_view>> SplitTwoStrict(
    std::string_view s, std::string_view delimiter) {
    const size_t pos = s.find(delimiter);
    if (pos == s.npos) {
        return {s, std::nullopt};
    } else {
        return {s.substr(0, pos), s.substr(pos + delimiter.length())};
    }
}
std::pair<std::string_view, std::string_view> SplitTwo(
    std::string_view s, std::string_view delimiter) {
    const auto [lhs, rhs_opt] = SplitTwoStrict(s, delimiter);
    return {lhs, rhs_opt.value_or("")};
}

std::string_view SkipSymbol(std::string_view s, char symbol) {
    auto it = s.begin();
    while (*it == symbol && it != s.end())
        it++;

    return s.substr(std::distance(s.begin(), it));
}

std::string_view Strip(std::string_view s, char symbol) {
    auto it = s.rbegin();
    while (*it == symbol && it != s.rend())
        it++;

    auto dist = s.size() - std::distance(s.rbegin(), it);
    return s.substr(0, dist);
}
