//
// Created by vlad on 08.10.2019.
//

#include <algorithm>
#include <future>
#include <iterator>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

template <typename K, typename V, typename Hash = std::hash<K>>
class ConcurrentMap {
public:
    using MapType = unordered_map<K, V, Hash>;

    struct WriteAccess {
        unique_lock<shared_mutex> lock;
        V &ref_to_value;
    };

    struct ReadAccess {
        shared_lock<shared_mutex> lock;
        const V &ref_to_value;
    };

    explicit ConcurrentMap(size_t bucket_count);
    WriteAccess operator[](const K &key);
    ReadAccess At(const K &key) const;
    bool Has(const K &key) const;
    MapType BuildOrdinaryMap() const;

private:
    Hash hasher;
    size_t bucket_count__;
    struct BucketType {
        MapType map{};
        mutable shared_mutex mutex{};
    };
    vector<BucketType> buckets__;
};

template <typename K, typename V, typename Hash>
ConcurrentMap<K, V, Hash>::ConcurrentMap(size_t bucket_count)
        : buckets__(bucket_count), bucket_count__(bucket_count) {}

template <typename K, typename V, typename Hash>
typename ConcurrentMap<K, V, Hash>::WriteAccess ConcurrentMap<K, V, Hash>::
operator[](const K &key) {
    auto num_bucket = hasher(key) % bucket_count__;
    BucketType &bucket = buckets__[num_bucket];

    WriteAccess wa{unique_lock(bucket.mutex), bucket.map[key]};
    return wa;
}

template <typename K, typename V, typename Hash>
typename ConcurrentMap<K, V, Hash>::ReadAccess ConcurrentMap<K, V, Hash>::At(
        const K &key) const {
    auto num_bucket = hasher(key) % bucket_count__;
    const BucketType &bucket = buckets__[num_bucket];

    ReadAccess ra{shared_lock(bucket.mutex), bucket.map.at(key)};
    return ra;
}

template <typename K, typename V, typename Hash>
bool ConcurrentMap<K, V, Hash>::Has(const K &key) const {
    auto num_bucket = hasher(key) % bucket_count__;
    const BucketType &bucket = buckets__[num_bucket];

    shared_lock<shared_mutex> lk(bucket.mutex);
    return bucket.map.find(key) != bucket.map.end();
}

template <typename K, typename V, typename Hash>
typename ConcurrentMap<K, V, Hash>::MapType
ConcurrentMap<K, V, Hash>::BuildOrdinaryMap() const {
    MapType res_map;
    vector<unique_lock<shared_mutex>> lk_v;
    for (auto &m : buckets__) lk_v.push_back(unique_lock<shared_mutex>(m.mutex));
    for (int i = 0; i < bucket_count__; ++i) {
        for (auto &el : buckets__[i].map) res_map.insert(el);
    }

    return res_map;
}
