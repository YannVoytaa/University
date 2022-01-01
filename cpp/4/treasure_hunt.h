#ifndef TREASURE_HUNT_H
#define TREASURE_HUNT_H

#include <utility>
#include "member.h"
#include "treasure.h"
#include <cassert>

template<typename T>
concept TreasureType = isTreasure<T>().operator bool();

template<typename T>
concept sharesStrengthT = requires(T a) {
    typename T::strength_t;
};

template<typename T>
concept hasIsArmed = requires(T a) {
    { [] () constexpr { return T::isArmed; }() };
    requires std::is_convertible<decltype(T::isArmed), bool>::value;
};

template<typename T>
concept hasPayMethod = requires(T a) {
    a.pay();
    requires IntegerType<decltype(a.pay())>;
};

template<typename T>
concept hasLootMethod = requires(T a) {
    a.loot(Treasure<decltype(a.pay()), false>(0));
    a.loot(Treasure<decltype(a.pay()), true>(0));
};

template<typename T>
concept MemberType = sharesStrengthT<T> && hasIsArmed<T> && hasPayMethod<T> && hasLootMethod<T>;


template<typename T>
concept EncounterSide = TreasureType<T> || MemberType<T>;

template<EncounterSide sideA, EncounterSide sideB>
class Encounter {
public:
    constexpr Encounter(sideA & a, sideB & b):a(a), b(b) {};
    constexpr void run() {
        if constexpr (TreasureType<sideA> && MemberType<sideB>) {
            b.loot(std::move(a));
            return;
        }
        else if constexpr (TreasureType<sideB> && MemberType<sideA>) {
            a.loot(std::move(b));
            return;
        }
        else if constexpr (TreasureType<sideA> && TreasureType<sideB>) {
            return;
        }
        else {
            if constexpr (sideA::isArmed && !sideB::isArmed) {
                a.loot(Treasure<decltype(a.pay()), false>(b.pay()));
                return;
            }
            else if constexpr (sideB::isArmed && !sideA::isArmed) {
                b.loot(Treasure<decltype(b.pay()), false>(a.pay()));
                return;
            }
            else if constexpr (!sideB::isArmed && !sideA::isArmed) {
                return;
            }
            else {
                if (a.getStrength() > b.getStrength()) {
                    a.loot(Treasure<decltype(a.pay()), false>(b.pay()));
                    return;
                }
                else if (b.getStrength() > a.getStrength()) {
                    b.loot(Treasure<decltype(b.pay()), false>(a.pay()));
                    return;
                }
            }
        }
    }

private:
    sideA &a;
    sideB &b;
};

template<EncounterSide sideA, EncounterSide sideB> requires (!TreasureType<sideA> || !TreasureType<sideB>)
constexpr void run(Encounter<sideA, sideB> e) {
    e.run();
}

template<typename... Encounters>
constexpr void expedition(Encounters... encounters) {
    (run(std::forward<Encounters>(encounters)),...);
}
#endif //TREASURE_HUNT_H
