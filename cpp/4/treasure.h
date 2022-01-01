#ifndef TREASURE_H
#define TREASURE_H

#include<type_traits>

template<typename T>
concept IntegerType = std::is_integral<T>().operator bool();

template<IntegerType ValueType, bool IsTrapped>
class Treasure {
public:
    constinit static const bool isTrapped = IsTrapped;

    explicit constexpr Treasure(ValueType value) : value(value) {};

    constexpr ValueType evaluate() const {
        return value;
    }

    constexpr ValueType getLoot() {
        ValueType val = evaluate();
        value = 0;
        return val;
    }

private:
    ValueType value;
};

template<IntegerType ValueType>
using SafeTreasure = Treasure<ValueType, false>;

template<IntegerType ValueType>
using TrappedTreasure = Treasure<ValueType, true>;

template<typename>
struct isTreasure : std::false_type {
};

template<IntegerType T, bool val>
struct isTreasure<Treasure<T, val>> : std::true_type {
};
#endif //TREASURE_H
