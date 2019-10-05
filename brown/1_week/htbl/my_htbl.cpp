#include <vector>
#include <forward_list>
#include <iterator>
#include <unordered_set>
#include <algorithm>

using namespace std;


template<typename Type, typename Hasher>
class HashSet {
public:
    using BucketList = forward_list<Type>;
    using ArrayOfBucket = vector<BucketList>;

public:
    explicit HashSet(
            size_t num_buckets,
            const Hasher &hasher = {}
    ) : buckets(num_buckets), num_buckets(num_buckets), hasher(hasher) {};

    void Add(const Type &value) {
        auto bucket_id = hasher(value) % num_buckets;
        BucketList &bucket = buckets[bucket_id];
        if (bucket.empty()) {
            bucket.push_front(value);
        } else {
            if (find(bucket.begin(), bucket.end(), value) == bucket.end())
                bucket.push_front(value);
        }
    };

    bool Has(const Type &value) const {
        auto bucket_id = hasher(value) % num_buckets;
        const BucketList &bucket = buckets[bucket_id];
        return find(bucket.begin(), bucket.end(), value) != bucket.end();
    }

    void Erase(const Type &value) {
        auto bucket_id = hasher(value) % num_buckets;
        BucketList &bucket = buckets[bucket_id];
        bucket.remove(value);
    }

    const BucketList &GetBucket(const Type &value) const {
        auto bucket_id = hasher(value) % num_buckets;
        const BucketList &bucket = buckets[bucket_id];
        return bucket;
    }

private:
    ArrayOfBucket buckets;
    size_t num_buckets;
    Hasher hasher;
};