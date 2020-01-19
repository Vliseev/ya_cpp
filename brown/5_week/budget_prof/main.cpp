#include <array>
#include <charconv>
#include <cmath>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>

/*
        0,31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366
    0,31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365*/

using namespace std;
template <typename It>
class Range {
   public:
    Range(It begin, It end) : begin_(begin), end_(end) {
    }
    It begin() const {
        return begin_;
    }
    It end() const {
        return end_;
    }

   private:
    It begin_;
    It end_;
};

pair<string_view, optional<string_view>>
SplitTwoStrict(string_view s, string_view delimiter = " ") {
    const size_t pos = s.find(delimiter);
    if (pos == s.npos) {
        return {s, nullopt};
    } else {
        return {s.substr(0, pos), s.substr(pos + delimiter.length())};
    }
}

vector<string_view> Split(string_view s, string_view delimiter = " ") {
    vector<string_view> parts;
    if (s.empty()) {
        return parts;
    }
    while (true) {
        const auto [lhs, rhs_opt] = SplitTwoStrict(s, delimiter);
        parts.push_back(lhs);
        if (!rhs_opt) {
            break;
        }
        s = *rhs_opt;
    }
    return parts;
}

constexpr bool IsInterc(int y) {
    if (y % 4)
        return false;
    return !(y % 100 == 0 && y % 400 != 0);
}

constexpr array<int, 100> GetAccYear() {
    std::array<int, 100> val = {};
    int acc_sum = 0;
    int index = 0;
    for (auto y = 2000; y < 2100; ++y) {
        val[index++] = acc_sum;
        acc_sum += IsInterc(y) ? 366 : 365;
    }
    return val;
}

constexpr array<int, 100> AccYear = GetAccYear();
constexpr int AccMount[] = {0,   31,  59,  90,  120, 151, 181,
                            212, 243, 273, 304, 334, 365};
constexpr int AccMountInt[] = {0,   31,  60,  91,  121, 152, 182,
                               213, 244, 274, 305, 335, 366};

void GetVal(string_view s, int& val) {
    std::from_chars(s.data(), s.data() + s.size(), val);
}

class HomeBud {
    struct PaymentDb {
        PaymentDb() : earn_(36575, 0.0), spent_(36575, 0.0) {
        }
        std::vector<double> earn_{};
        std::vector<double> spent_{};
    };
    PaymentDb db_;

   public:
    HomeBud() = default;
    void Update(std::string_view s);

   private:
    void UpdateEarn(vector<string_view>);
    void UpdateInc(vector<string_view>);
    void UpdateTax(vector<string_view>);
    void Spend(vector<string_view>);
    pair<int, int> GetIdx(const vector<string_view>& line) const;
};
void HomeBud::Update(std::string_view s) {
    auto tokens = Split(s);
    if (tokens[0] == "Earn")
        UpdateEarn(tokens);
    else if (tokens[0] == "ComputeIncome")
        UpdateInc(tokens);
    else if (tokens[0] == "PayTax")
        UpdateTax(tokens);
    else
        Spend(tokens);
}
void HomeBud::UpdateEarn(vector<string_view> line) {
    auto [ind1, ind2] = GetIdx(line);
    double val;
    int v;
    GetVal(line[3], v);

    val = static_cast<double>(v) / (ind2 - ind1);
    for (int i = ind1; i < ind2; i++) {
        db_.earn_[i] += val;
    }
}

void HomeBud::UpdateInc(vector<string_view> line) {
    auto [ind1, ind2] = GetIdx(line);
    double tot_sum = 0;
    for (int i = ind1; i < ind2; i++) {
        tot_sum += db_.earn_[i];
        tot_sum -= db_.spent_[i];
    }

    cout << tot_sum << "\n";
}
void HomeBud::UpdateTax(vector<string_view> line) {
    auto [ind1, ind2] = GetIdx(line);

    int v;
    GetVal(line[3], v);
    double perc = static_cast<double>(v) / 100.0;

    for (int i = ind1; i < ind2; i++) {
        db_.earn_[i] *= (1.0 - perc);
    }
}

void HomeBud::Spend(vector<string_view> line) {
    auto [ind1, ind2] = GetIdx(line);
    double val;
    int v;
    GetVal(line[3], v);

    val = static_cast<double>(v) / (ind2 - ind1);
    for (int i = ind1; i < ind2; i++) {
        db_.spent_[i] += val;
    }
}

pair<int, int> HomeBud::GetIdx(const vector<string_view>& line) const {
    int ind1;
    int ind2;

    auto date1 = Split(line[1], "-");
    auto date2 = Split(line[2], "-");

    int y1, m1, d1;
    int y2, m2, d2;
    GetVal(date1[0], y1);
    GetVal(date1[1], m1);
    GetVal(date1[2], d1);

    GetVal(date2[0], y2);
    GetVal(date2[1], m2);
    GetVal(date2[2], d2);

    m1 = IsInterc(y1) ? AccMountInt[m1 - 1] : AccMount[m1 - 1];
    m2 = IsInterc(y2) ? AccMountInt[m2 - 1] : AccMount[m2 - 1];

    y1 = y1 - 2000;
    y2 = y2 - 2000;

    ind1 = AccYear[y1] + m1 + d1 - 1;
    ind2 = AccYear[y2] + m2 + d2 - 1;

    return {ind1, ind2 + 1};
}

void Solution(istream& iss) {
    int q;
    HomeBud hb;
    iss >> q;

    string _;
    getline(iss, _);

    for (int i = 0; i < q; ++i) {
        string s;
        getline(iss, s);
        //        std::cerr << s << "\n";
        hb.Update(s);
    }
}

int main() {
    istringstream iss(
        "100\n"
        "ComputeIncome 2001-02-08 2001-02-19\n"
        "ComputeIncome 2001-02-04 2001-02-12\n"
        "Earn 2001-02-18 2001-02-25 709441\n"
        "Spend 2001-02-11 2001-02-13 723037\n"
        "PayTax 2001-02-08 2001-02-14 33\n"
        "PayTax 2001-02-12 2001-02-24 76\n"
        "Spend 2001-02-07 2001-02-14 954000\n"
        "ComputeIncome 2001-02-19 2001-02-19\n"
        "PayTax 2001-02-11 2001-02-21 42\n"
        "Spend 2001-02-01 2001-02-15 774069\n"
        "PayTax 2001-02-03 2001-02-13 92\n"
        "Earn 2001-02-28 2001-02-28 455680\n"
        "PayTax 2001-02-20 2001-02-22 60\n"
        "Spend 2001-02-18 2001-02-21 471630\n"
        "ComputeIncome 2001-02-22 2001-02-26\n"
        "Spend 2001-02-11 2001-02-17 877836\n"
        "Spend 2001-02-01 2001-02-27 264048\n"
        "Earn 2001-02-01 2001-02-27 610429\n"
        "Earn 2001-02-13 2001-02-22 301095\n"
        "PayTax 2001-02-07 2001-02-10 98\n"
        "ComputeIncome 2001-02-11 2001-02-19\n"
        "Spend 2001-02-13 2001-02-19 975815\n"
        "Spend 2001-02-02 2001-02-13 531910\n"
        "ComputeIncome 2001-02-04 2001-02-18\n"
        "Spend 2001-02-07 2001-02-22 257535\n"
        "ComputeIncome 2001-02-11 2001-02-20\n"
        "ComputeIncome 2001-02-17 2001-02-19\n"
        "PayTax 2001-02-03 2001-02-19 10\n"
        "PayTax 2001-02-13 2001-02-25 29\n"
        "PayTax 2001-02-04 2001-02-18 46\n"
        "Spend 2001-02-03 2001-02-17 597043\n"
        "PayTax 2001-02-17 2001-02-26 85\n"
        "PayTax 2001-02-02 2001-02-24 5\n"
        "Spend 2001-02-02 2001-02-15 66077\n"
        "PayTax 2001-02-02 2001-02-11 12\n"
        "Earn 2001-02-21 2001-02-22 277038\n"
        "Earn 2001-02-04 2001-02-11 895689\n"
        "Spend 2001-02-04 2001-02-13 15053\n"
        "PayTax 2001-02-12 2001-02-13 62\n"
        "ComputeIncome 2001-02-04 2001-02-17\n"
        "PayTax 2001-02-12 2001-02-19 26\n"
        "PayTax 2001-02-01 2001-02-18 97\n"
        "Earn 2001-02-04 2001-02-23 95480\n"
        "PayTax 2001-02-09 2001-02-19 73\n"
        "Earn 2001-02-14 2001-02-23 713091\n"
        "ComputeIncome 2001-02-15 2001-02-28\n"
        "PayTax 2001-02-26 2001-02-27 16\n"
        "ComputeIncome 2001-02-07 2001-02-24\n"
        "Earn 2001-02-03 2001-02-22 353489\n"
        "PayTax 2001-02-27 2001-02-27 92\n"
        "Spend 2001-02-19 2001-02-24 818062\n"
        "PayTax 2001-02-13 2001-02-27 58\n"
        "ComputeIncome 2001-02-04 2001-02-20\n"
        "Spend 2001-02-02 2001-02-16 27986\n"
        "Earn 2001-02-11 2001-02-24 730933\n"
        "Spend 2001-02-14 2001-02-22 591553\n"
        "PayTax 2001-02-11 2001-02-23 85\n"
        "ComputeIncome 2001-02-10 2001-02-27\n"
        "PayTax 2001-02-14 2001-02-21 21\n"
        "PayTax 2001-02-06 2001-02-24 68\n"
        "Spend 2001-02-01 2001-02-15 799023\n"
        "PayTax 2001-02-13 2001-02-26 68\n"
        "PayTax 2001-02-11 2001-02-23 11\n"
        "PayTax 2001-02-04 2001-02-27 61\n"
        "Spend 2001-02-03 2001-02-24 181323\n"
        "ComputeIncome 2001-02-15 2001-02-21\n"
        "PayTax 2001-02-15 2001-02-21 53\n"
        "Spend 2001-02-13 2001-02-22 346631\n"
        "PayTax 2001-02-13 2001-02-25 56\n"
        "ComputeIncome 2001-02-16 2001-02-17\n"
        "ComputeIncome 2001-02-21 2001-02-24\n"
        "Earn 2001-02-03 2001-02-22 433972\n"
        "ComputeIncome 2001-02-12 2001-02-21\n"
        "PayTax 2001-02-01 2001-02-02 85\n"
        "Spend 2001-02-06 2001-02-23 211937\n"
        "Spend 2001-02-11 2001-02-24 244477\n"
        "Earn 2001-02-11 2001-02-22 776928\n"
        "Spend 2001-02-02 2001-02-20 694665\n"
        "ComputeIncome 2001-02-02 2001-02-09\n"
        "Spend 2001-02-10 2001-02-11 923829\n"
        "ComputeIncome 2001-02-03 2001-02-26\n"
        "Earn 2001-02-11 2001-02-22 688252\n"
        "Earn 2001-02-20 2001-02-27 203049\n"
        "PayTax 2001-02-02 2001-02-03 38\n"
        "ComputeIncome 2001-02-08 2001-02-09\n"
        "PayTax 2001-02-05 2001-02-22 55\n"
        "ComputeIncome 2001-02-11 2001-02-19\n"
        "PayTax 2001-02-19 2001-02-20 43\n"
        "Spend 2001-02-05 2001-02-11 820907\n"
        "Spend 2001-02-09 2001-02-13 749795\n"
        "PayTax 2001-02-05 2001-02-16 82\n"
        "PayTax 2001-02-02 2001-02-17 47\n"
        "Spend 2001-02-09 2001-02-11 519168\n"
        "PayTax 2001-02-02 2001-02-03 5\n"
        "Earn 2001-02-13 2001-02-15 708328\n"
        "ComputeIncome 2001-02-14 2001-02-19\n"
        "Spend 2001-02-08 2001-02-18 590141\n"
        "PayTax 2001-02-04 2001-02-22 41\n"
        "Earn 2001-02-03 2001-02-10 898006\n"
        "ComputeIncome 2001-02-11 2001-02-16");
    cout.precision(25);
    Solution(cin);

    return 0;
}
