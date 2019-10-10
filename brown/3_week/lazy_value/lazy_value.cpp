#include <functional>
#include <string>
using namespace std;

template <typename T>
class LazyValue {
public:
    explicit LazyValue(std::function<T()> init);

    bool HasValue() const;
    const T& Get() const ;

private:
    mutable std::optional<T> val__;
    std::function<T()> init__;
};

template <typename T>
LazyValue<T>::LazyValue(std::function<T()> init)
        : val__(nullopt), init__(init) {}

template <typename T>
bool LazyValue<T>::HasValue() const {
    return val__.has_value();
}

template <typename T>
const T& LazyValue<T>::Get() const {
    if (val__)
        return val__.value();
    else {
        val__.emplace(init__());
        return val__.value();
    }
}
