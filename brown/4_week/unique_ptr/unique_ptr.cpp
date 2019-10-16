#include "test_runner.h"

#include <cstddef>  // нужно для nullptr_t

using namespace std;
// Реализуйте шаблон класса UniquePtr
template <typename T>
class UniquePtr {
 private:
  using pointer_type = T*;
  pointer_type ptr_;

 public:
  UniquePtr() : ptr_(nullptr){};
  UniquePtr(T* ptr) : ptr_(ptr){};
  UniquePtr(const UniquePtr&) = delete;
  UniquePtr(UniquePtr&& other);

  UniquePtr& operator=(const UniquePtr&) = delete;
  UniquePtr& operator=(nullptr_t);
  UniquePtr& operator=(UniquePtr&& other);
  ~UniquePtr();

  T& operator*() const;
  T* operator->() const;
  T* Release();
  void Reset(T* ptr);
  void Swap(UniquePtr& other);
  T* Get() const;
};
template <typename T>
UniquePtr<T>::UniquePtr(UniquePtr&& other) {
  if (addressof(other) != this) {
    ptr_ = other.ptr_;
    other.ptr_ = nullptr;
  }
}
template <typename T>
UniquePtr<T>& UniquePtr<T>::operator=(nullptr_t) {
  if (ptr_) {
    delete ptr_;
    ptr_ = nullptr;
  }
  return *this;
}
template <typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& other) {
  if (ptr_) {
    delete ptr_;
  }
  ptr_ = other.ptr_;
  other.ptr_ = nullptr;
  return *this;
}
template <typename T>
UniquePtr<T>::~UniquePtr() {
  if (ptr_) {
    delete ptr_;
    ptr_ = nullptr;
  }
}
template <typename T>
T& UniquePtr<T>::operator*() const {
  return *ptr_;
}
template <typename T>
T* UniquePtr<T>::operator->() const {
  return ptr_;
}
template <typename T>
T* UniquePtr<T>::Release() {
  pointer_type tmp_ptr = ptr_;
  ptr_ = nullptr;
  return tmp_ptr;
}
template <typename T>
void UniquePtr<T>::Reset(T* ptr) {
  if (ptr_) {
    delete ptr_;
  }
  ptr_ = ptr;
}
template <typename T>
void UniquePtr<T>::Swap(UniquePtr& other) {
  std::swap(ptr_, other.ptr_);
}
template <typename T>
T* UniquePtr<T>::Get() const {
  return ptr_;
}

struct Item {
  static int counter;
  int value;
  Item(int v = 0) : value(v) {
    ++counter;
  }
  Item(const Item& other) : value(other.value) {
    ++counter;
  }
  ~Item() {
    --counter;
  }
};

int Item::counter = 0;

void TestLifetime() {
  Item::counter = 0;
  {
    UniquePtr<Item> ptr(new Item);
    ASSERT_EQUAL(Item::counter, 1);

    ptr.Reset(new Item);
    ASSERT_EQUAL(Item::counter, 1);
  }
  ASSERT_EQUAL(Item::counter, 0);

  {
    UniquePtr<Item> ptr(new Item);
    ASSERT_EQUAL(Item::counter, 1);

    auto rawPtr = ptr.Release();
    ASSERT_EQUAL(Item::counter, 1);

    delete rawPtr;
    ASSERT_EQUAL(Item::counter, 0);
  }
  ASSERT_EQUAL(Item::counter, 0);
}

void TestGetters() {
  UniquePtr<Item> ptr(new Item(42));
  ASSERT_EQUAL(ptr.Get()->value, 42);
  ASSERT_EQUAL((*ptr).value, 42);
  ASSERT_EQUAL(ptr->value, 42);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestLifetime);
  RUN_TEST(tr, TestGetters);
}
