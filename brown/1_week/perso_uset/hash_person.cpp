#include <limits>
#include <random>
#include <unordered_set>
#include <string>

using namespace std;

struct Address {
    string city, street;
    int building;

    bool operator==(const Address &other) const {
      // реализуйте оператор
      return tie(city, street, building)
             == tie(other.city, other.street, other.building);
    }
};

struct Person {
    string name;
    int height;
    double weight;
    Address address;

    bool operator==(const Person &other) const {
      // реализуйте оператор
      return tie(name, height, weight, address)
             == tie(other.name, other.height, other.weight, other.address);
    }
};

struct AddressHasher {
    const size_t coef = 2'946'901;
    // реализуйте структуру

    size_t operator()(const Address &address) const {

      return coef * (hash<string>()(address.city) +
                     coef * (hash<string>()(address.street) +
                             coef * hash<int>()(address.building)));
    }
};

struct PersonHasher {
    // реализуйте структуру
    const size_t coef = 2'946'901;
    // реализуйте структуру

    size_t operator()(const Person &person) const {

      return coef * (hash<string>()(person.name) +
                     coef * (hash<int>()(person.height) +
                             coef * (hash<double>()(person.weight) +
                                     coef * AddressHasher()(person.address))));
    }
};
