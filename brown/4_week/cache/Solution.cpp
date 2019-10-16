#include <iostream>
#include <list>
#include <mutex>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>
#include "Common.h"

using namespace std;

class LruCache : public ICache {
  struct Node {
    std::string key;
    BookPtr val;
    Node(std::string _key, BookPtr _val)
        : key(std::move(_key)), val(std::move(_val)){};
  };

  typedef std::list<Node> node_list;
  typedef std::unordered_map<std::string_view, node_list::iterator> node_map;

 public:
  LruCache(shared_ptr<IBooksUnpacker> books_unpacker, const Settings &settings)
      : books_unpacker__(books_unpacker),
        settings__(settings),
        current_weight__(0) {
    // реализуйте метод
  }

  BookPtr GetBook(const string &book_name) override {
    lock_guard<mutex> lock(mut__);
    auto ptr = Touch(book_name);  // side-effect: move items
    if (ptr != nullptr) {         // cache hit
      return ptr;
    }
    ptr = books_unpacker__->UnpackBook(book_name);
    Insert(book_name, ptr);
    return ptr;
  }

 private:
  BookPtr Touch(const string &name) const {
    using namespace std;
    auto find_el = mapping__.find(name);
    if (find_el != mapping__.end()) {
      Node node = move(*(find_el->second));
      mapping__.erase(find_el);
      nodes__.erase(find_el->second);

      nodes__.push_front(node);
      mapping__.insert({name, nodes__.begin()});

      return node.val;
    }
    return nullptr;
  }
  void Insert(const string &name, const BookPtr &ptr) {
    if (ptr->GetContent().length() > settings__.max_memory) {
      Clear();
      return;
    }
    while (ptr->GetContent().length() + current_weight__ >
               settings__.max_memory &&
           !nodes__.empty()) {
      PopMin();
    }
    nodes__.emplace_front(name, ptr);
    mapping__.insert({name, nodes__.begin()});
    current_weight__ += ptr->GetContent().size();
  }

  void Clear() {
    mapping__.clear();
    nodes__.clear();
    current_weight__ = 0;
  }

  void PopMin() {
    using namespace std;
    auto node = move(nodes__.back());
    nodes__.pop_back();
    mapping__.erase(node.key);
    current_weight__ -= node.val->GetContent().size();
  }

 private:
  shared_ptr<IBooksUnpacker> books_unpacker__;
  const Settings &settings__;
  mutable int current_weight__;

  mutable node_list nodes__;
  mutable node_map mapping__;

  mutex mut__;
};

unique_ptr<ICache> MakeCache(shared_ptr<IBooksUnpacker> books_unpacker,
                             const ICache::Settings &settings) {
  // реализуйте функцию
  return make_unique<LruCache>(books_unpacker, settings);
}
