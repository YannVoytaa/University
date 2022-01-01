#ifndef MEMBER_H
#define MEMBER_H

#include "treasure.h"

using strength_t_G = unsigned int;

template<IntegerType ValueType, bool IsArmed>
class Adventurer {
public:
    typedef strength_t_G strength_t;

    constinit static const bool isArmed = IsArmed;

    constexpr Adventurer() requires (!IsArmed): strength(0), lootValue(0) {};

    explicit constexpr Adventurer(strength_t strength) requires(IsArmed): strength(strength), lootValue(0) {};

    [[nodiscard]] constexpr strength_t getStrength() const requires IsArmed {
        return strength;
    }

    constexpr void loot([[maybe_unused]] Treasure<ValueType, true> &&treasure) requires (!IsArmed) {
    }

    constexpr void loot(Treasure<ValueType, true> &&treasure) requires IsArmed {
        if (strength > 0) {
            strength /= 2;
            lootValue += treasure.getLoot();
        }
    }

    constexpr void loot(Treasure<ValueType, false> &&treasure) {
        lootValue += treasure.getLoot();
    }

    constexpr ValueType pay() {
        ValueType sumLoot = lootValue;
        lootValue = 0;
        return sumLoot;
    }

private:
    strength_t strength;
    ValueType lootValue;

};

template<IntegerType ValueType>
using Explorer = Adventurer<ValueType, false>;


inline constexpr strength_t_G fib(std::size_t n) {
    strength_t_G next = 1;
    strength_t_G last = 0;

    for (std::size_t i = 0; i < n; i++) {
        next += last; //a_{n+1} = a_n + a_{n-1}
        last = next - last; //a_n = a_{n+1} - a_{n-1}
    }

    return last;
}

template<std::size_t expeditions>
concept PossibleExpeditionsNo = expeditions < 25;

template<IntegerType ValueType, std::size_t CompletedExpeditions> requires PossibleExpeditionsNo<CompletedExpeditions>
class Veteran {
public:
    typedef strength_t_G strength_t;

    constexpr Veteran() : strength(fib(CompletedExpeditions)), lootValue(0) {};

    constinit static const bool isArmed = true;

    constexpr void loot(Treasure<ValueType, true> &&treasure) {
        if (strength > 0) {
            lootValue += treasure.getLoot();
        }
    }

    constexpr void loot(Treasure<ValueType, false> &&treasure) {
        lootValue += treasure.getLoot();
    }

    constexpr ValueType pay() {
        ValueType loot = lootValue;
        lootValue = 0;
        return loot;
    }

    [[nodiscard]] constexpr strength_t getStrength() const {
        return strength;
    }

private:
    strength_t strength;
    ValueType lootValue;

};

#endif //MEMBER_H
