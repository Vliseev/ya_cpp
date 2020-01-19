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
        PaymentDb() : earn_(36575, 0.0) {
        }
        std::vector<double> earn_{};
    };
    PaymentDb db_;

   public:
    HomeBud() = default;
    void Update(std::string_view s);

   private:
    void UpdateEarn(vector<string_view>);
    void UpdateInc(vector<string_view>);
    void UpdateTax(vector<string_view>);
    pair<int, int> GetIdx(const vector<string_view>& line) const;
};
void HomeBud::Update(std::string_view s) {
    auto tokens = Split(s);
    if (tokens[0] == "Earn")
        UpdateEarn(tokens);
    else if (tokens[0] == "ComputeIncome")
        UpdateInc(tokens);
    else
        UpdateTax(tokens);
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
    }

    cout << tot_sum << "\n";
}
void HomeBud::UpdateTax(vector<string_view> line) {
    auto [ind1, ind2] = GetIdx(line);

    for (int i = ind1; i < ind2; i++) {
        db_.earn_[i] *= 0.87;
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
        hb.Update(s);
    }
}

int main() {
    istringstream iss(
        "8\n"
        "Earn 2000-01-02 2000-01-06 20\n"
        "ComputeIncome 2000-01-01 2001-01-01\n"
        "PayTax 2000-01-02 2000-01-03\n"
        "ComputeIncome 2000-01-01 2001-01-01\n"
        "Earn 2000-01-03 2000-01-03 10\n"
        "ComputeIncome 2000-01-01 2001-01-01\n"
        "PayTax 2000-01-03 2000-01-03\n"
        "ComputeIncome 2000-01-01 2001-01-01");
    cout.precision(25);
    Solution(cin);

    return 0;
}
