#include "profile.h"
#include "test_runner.h"

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

void RunConcurrentUpdates(ConcurrentMap<int, int> &cm, size_t thread_count,
                          int key_count) {
  auto kernel = [&cm, key_count](int seed) {
    vector<int> updates(key_count);
    iota(begin(updates), end(updates), -key_count / 2);
    shuffle(begin(updates), end(updates), default_random_engine(seed));

    for (int i = 0; i < 2; ++i) {
      for (auto key : updates) {
        cm[key].ref_to_value++;
      }
    }
  };

  vector<future<void>> futures;
  for (size_t i = 0; i < thread_count; ++i) {
    futures.push_back(async(kernel, i));
  }
}

void TestConcurrentUpdate() {
  const size_t thread_count = 3;
  const size_t key_count = 50000;

  ConcurrentMap<int, int> cm(thread_count);
  RunConcurrentUpdates(cm, thread_count, key_count);

  const auto result = std::as_const(cm).BuildOrdinaryMap();
  ASSERT_EQUAL(result.size(), key_count);
  for (auto &[k, v] : result) {
    AssertEqual(v, 6, "Key = " + to_string(k));
  }
}

void TestReadAndWrite() {
  ConcurrentMap<size_t, string> cm(5);

  auto updater = [&cm] {
    for (size_t i = 0; i < 50000; ++i) {
      cm[i].ref_to_value += 'a';
    }
  };
  auto reader = [&cm] {
    vector<string> result(50000);
    for (size_t i = 0; i < result.size(); ++i) {
      result[i] = cm[i].ref_to_value;
    }
    return result;
  };

  auto u1 = async(updater);
  auto r1 = async(reader);
  auto u2 = async(updater);
  auto r2 = async(reader);

  u1.get();
  u2.get();

  for (auto f : {&r1, &r2}) {
    auto result = f->get();
    ASSERT(all_of(result.begin(), result.end(), [](const string &s) {
      return s.empty() || s == "a" || s == "aa";
    }));
  }
}

void TestSpeedup() {
  {
    ConcurrentMap<int, int> single_lock(1);

    LOG_DURATION("Single lock");
    RunConcurrentUpdates(single_lock, 4, 50000);
  }
  {
    ConcurrentMap<int, int> many_locks(100);

    LOG_DURATION("100 locks");
    RunConcurrentUpdates(many_locks, 4, 50000);
  }
}

void TestConstAccess() {
  const unordered_map<int, string> expected = {
      {1, "one"},
      {2, "two"},
      {3, "three"},
      {31, "thirty one"},
      {127, "one hundred and twenty seven"},
      {1598, "fifteen hundred and ninety eight"}};

  const ConcurrentMap<int, string> cm = [&expected] {
    ConcurrentMap<int, string> result(3);
    for (const auto &[k, v] : expected) {
      result[k].ref_to_value = v;
    }
    return result;
  }();

  vector<future<string>> futures;
  for (int i = 0; i < 10; ++i) {
    futures.push_back(async([&cm, i] {
      try {
        return cm.At(i).ref_to_value;
      } catch (exception &) {
        return string();
      }
    }));
  }
  futures.clear();

  ASSERT_EQUAL(cm.BuildOrdinaryMap(), expected);
}

void TestStringKeys() {
  const unordered_map<string, string> expected = {
      {"one", "ONE"},
      {"two", "TWO"},
      {"three", "THREE"},
      {"thirty one", "THIRTY ONE"},
  };

  const ConcurrentMap<string, string> cm = [&expected] {
    ConcurrentMap<string, string> result(2);
    for (const auto &[k, v] : expected) {
      result[k].ref_to_value = v;
    }
    return result;
  }();

  ASSERT_EQUAL(cm.BuildOrdinaryMap(), expected);
}

struct Point {
  int x, y;
};

struct PointHash {
  size_t operator()(Point p) const {
    std::hash<int> h;
    return h(p.x) * 3571 + h(p.y);
  }
};

bool operator==(Point lhs, Point rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

void TestUserType() {
  ConcurrentMap<Point, size_t, PointHash> point_weight(5);

  vector<future<void>> futures;
  for (int i = 0; i < 1000; ++i) {
    futures.push_back(async([&point_weight, i] {
      point_weight[Point{i, i}].ref_to_value = i;
    }));
  }

  futures.clear();

  for (int i = 0; i < 1000; ++i) {
    ASSERT_EQUAL(point_weight.At(Point{i, i}).ref_to_value, i);
  }

  const auto weights = point_weight.BuildOrdinaryMap();
  for (int i = 0; i < 1000; ++i) {
    ASSERT_EQUAL(weights.at(Point{i, i}), i);
  }
}

void TestHas() {
  ConcurrentMap<int, int> cm(2);
  cm[1].ref_to_value = 100;
  cm[2].ref_to_value = 200;

  const auto &const_map = std::as_const(cm);
  ASSERT(const_map.Has(1));
  ASSERT(const_map.Has(2));
  ASSERT(!const_map.Has(3));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestConcurrentUpdate);
  RUN_TEST(tr, TestReadAndWrite);
  RUN_TEST(tr, TestSpeedup);
  RUN_TEST(tr, TestConstAccess);
  RUN_TEST(tr, TestStringKeys);
  RUN_TEST(tr, TestUserType);
  RUN_TEST(tr, TestHas);
}
