#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <queue>
#include <set>
#include <map>
#include <utility>
#include <vector>

using namespace std;

template<typename T>
class PriorityCollection {

public:
    using Id = size_t;/* тип, используемый для идентификаторов */;

    struct Item {
        Item(T &&val, Id id, size_t prior) : val(move(val)), id(id), prior(prior) {}

        Item(Item &&) = default;

        T val;
        Id id;
        size_t prior;

    };


    // Добавить объект с нулевым приоритетом
    // с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        auto it = p_set.insert(Item(move(object),max_id, 0));
        id_map[max_id++] = it;
        return max_id - 1;
    }

    // Добавить все элементы диапазона [range_begin, range_end)
    // с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template<typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
        while (range_begin != range_end) {
            *(ids_begin++) = Add(move(*(range_begin++)));
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        return id_map.find(id) != id_map.end();
    }

    // Получить объект по идентификатору
    // Получить объект по идентификатору
    const T &Get(Id id) const {
        return id_map.at(id)->val;
    }


    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        auto el_it = id_map[id];
        auto el = move(p_set.extract(*el_it).value());
        el.prior++;
        auto iter = p_set.emplace(move(el));
        id_map[id] = iter;
    }

    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T &, int> GetMax() const {
        const auto& el = *p_set.crbegin();
        return {el.val, el.prior};
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax() {
        auto el = move(p_set.extract(*p_set.rbegin()).value());
        id_map.erase(el.id);
        return make_pair(move(el.val), el.prior);
    }

private:
    struct PriorComp {
        bool operator()(const Item &lhs, const Item &rhs) const {
            return rhs.prior != lhs.prior ? lhs.prior < rhs.prior : lhs.id < rhs.id;
        }
    };
    // Приватные поля и методы
    using prior_set_type = multiset<Item, PriorComp>;
    using id_map_type = map<Id, typename set<Item, PriorComp>::iterator>;
    prior_set_type p_set;
    id_map_type id_map;
    size_t max_id{};
};

