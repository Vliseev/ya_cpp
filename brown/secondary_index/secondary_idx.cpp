#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

struct Record {
    string id;
    string title;
    string user;
    int timestamp;
    int karma;
};


// Реализуйте этот класс
class Database {
    using p_point = const Record *;
    multimap<int, p_point> __timestamp_map;
    multimap<int, p_point> __karma_map;
    unordered_multimap<string, p_point> __user_mmap;
    unordered_map<string, Record> __id_map;

    struct Iters {
        multimap<int, p_point>::iterator __ts_it;
        multimap<int, p_point>::iterator __k_it;
        unordered_multimap<string, p_point>::iterator __user_it;

        Iters(const multimap<int, p_point>::iterator &ts_it,
              const multimap<int, p_point>::iterator &k_it,
              const unordered_multimap<string, p_point>::iterator &user_it)
                : __ts_it(ts_it), __k_it(k_it), __user_it(user_it) {};
    };

    unordered_map<string, Iters> __it_map;

public:
    bool Put(const Record &record);

    const Record *GetById(const string &id) const;

    bool Erase(const string &id);

    template<typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const;

    template<typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const;

    template<typename Callback>
    void AllByUser(const string &user, Callback callback) const;
};

template<typename MapType, typename Val>
void remove_helper(MapType &map, Val &field, const string &id) {
    auto range_ts = map.equal_range(field);
    auto ts_it = find_if(range_ts.first, range_ts.second,
                         [&id](auto &el) { return el.second->second.id == id; });
    map.erase(ts_it);
}

const Record *Database::GetById(const string &id) const {
    //    cerr << "GetBy Id " << id << ";";
    //    auto record = ;
    if (__id_map.find(id) != __id_map.end()) {
        return &__id_map.at(id);
    } else
        return nullptr;
}

bool Database::Put(const Record &record) {
    //    cerr << "Put " << record << ";";
    auto it = __id_map.emplace(record.id, record);
    if (it.second) {
        auto ts_it = __timestamp_map.emplace(record.timestamp, &it.first->second);
        auto k_it = __karma_map.emplace(record.karma, &it.first->second);
        auto r_it = __user_mmap.emplace(record.user, &it.first->second);
        __it_map.insert({record.id, Iters(ts_it, k_it, r_it)});
        return true;
    } else
        return false;
}

bool Database::Erase(const string &id) {
    //    cerr << "Erase " << id << ";";
    auto item = __it_map.find(id);
    if (item != __it_map.end()) {
        __timestamp_map.erase(item->second.__ts_it);
        __karma_map.erase(item->second.__k_it);
        __user_mmap.erase(item->second.__user_it);
        __id_map.erase(id);
        __it_map.erase(item);
        return true;
    } else {
        return false;
    }
}

template<typename Callback>
void Database::RangeByTimestamp(int low, int high, Callback callback) const {
    //    cerr << "RangeByTimestamp " << low << " " << high << ";";
    auto lb = __timestamp_map.lower_bound(low);
    auto ub = __timestamp_map.upper_bound(high);
    for (; lb != ub; ++lb) {
        if (!callback(*(lb->second))) return;
    }
}

template<typename Callback>
void Database::RangeByKarma(int low, int high, Callback callback) const {
    //    cerr << "RangeByKarma " << low << " " << high << ";";
    auto lb = __karma_map.lower_bound(low);
    auto ub = __karma_map.upper_bound(high);
    for (; lb != ub; ++lb) {
        if (!callback(*(lb->second))) return;
    }
}

template<typename Callback>
void Database::AllByUser(const string &user, Callback callback) const {
    //    cerr << "AllByUser " << user << ";";
    auto range = __user_mmap.equal_range(user);
    for (; range.first != range.second; ++range.first) {
        if (!callback(*(range.first->second))) return;
    }
}