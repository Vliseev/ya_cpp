//
// Created by vlad on 14.10.2019.
//
#include "Common.h"
class Num : public Expression {
    int val__;

public:
    int Evaluate() const override;
    explicit Num(int val) : val__(val){};
    std::string ToString() const override;
};
int Num::Evaluate() const { return val__; }
std::string Num::ToString() const { return std::to_string(val__); }

class BinOp : public Expression {
protected:
    ExpressionPtr left__;
    ExpressionPtr right__;

public:
    virtual int Evaluate() const = 0;
    std::string ToString() const = 0;
    explicit BinOp(ExpressionPtr left, ExpressionPtr right)
            : left__(std::move(left)), right__(std::move(right)){};
};

class Plus : public BinOp {
public:
    int Evaluate() const override {
        return left__->Evaluate() + right__->Evaluate();
    }
    std::string ToString() const override {
        auto s =
                "(" + left__->ToString() + ")" + "+" + "(" + right__->ToString() + ")";
        return s;
    }
    explicit Plus(ExpressionPtr left, ExpressionPtr right)
            : BinOp(std::move(left), std::move(right)){};
};

class Mult : public BinOp {
public:
    int Evaluate() const override {
        return left__->Evaluate() * right__->Evaluate();
    }
    std::string ToString() const override {
        auto s =
                "(" + left__->ToString() + ")" + "*" + "(" + right__->ToString() + ")";
        return s;
    }
    explicit Mult(ExpressionPtr left, ExpressionPtr right)
            : BinOp(std::move(left), std::move(right)){};
};

// Функции для формирования выражения
ExpressionPtr Value(int value) { return std::make_unique<Num>(value); };
ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
    return std::make_unique<Plus>(std::move(left), std::move(right));
};
ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
    return std::make_unique<Mult>(std::move(left), std::move(right));
}