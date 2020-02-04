//
// Created by vlad on 03.02.2020.
//

#pragma once
#include <optional>
#include <string>
#include <string_view>
#include <vector>

std::pair<std::string_view, std::optional<std::string_view>>
SplitTwoStrict(std::string_view s, std::string_view delimiter = " ");
std::pair<std::string_view, std::string_view>
SplitTwo(std::string_view s, std::string_view delimiter = " ");
std::string_view ReadToken(std::string_view& s,
                           std::string_view delimiter = " ");
std::string_view SkipSymbol(std::string_view s, char symbol = ' ');
std::string_view Strip(std::string_view s, char symbol = ' ');