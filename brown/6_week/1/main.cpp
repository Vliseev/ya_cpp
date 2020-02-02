//
// Created by user on 01.02.2020.
//
#include <memory>

int main() {
    std::shared_ptr<int> g = std::make_shared<int>(5);

    std::shared_ptr<int> b = g;
    return 0;
}