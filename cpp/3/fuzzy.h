#ifndef FUZZY_H
#define FUZZY_H

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

using real_t = double;

class TriFuzzyNum {

public:

    constexpr TriFuzzyNum() = delete;

    constexpr TriFuzzyNum(real_t a, real_t b, real_t c) :
            l(trunc(min(min(a, b), c))),
            m(trunc(a + b + c - min(min(a, b), c) - max(max(a, b), c))),
            u(trunc(max(max(a, b), c))) {};

    constexpr TriFuzzyNum(TriFuzzyNum const &ref) = default;

    constexpr TriFuzzyNum(TriFuzzyNum &&ref) = default;

    [[nodiscard]] constexpr real_t lower_value() const { return l; };

    [[nodiscard]] constexpr real_t modal_value() const { return m; };

    [[nodiscard]] constexpr real_t upper_value() const { return u; };

    constexpr TriFuzzyNum &operator=(TriFuzzyNum const &ref) = default;

    constexpr TriFuzzyNum &operator=(TriFuzzyNum &&ref) noexcept {
        l = move(ref.l);
        m = move(ref.m);
        u = move(ref.u);
        return *this;
    }

    constexpr TriFuzzyNum operator+(TriFuzzyNum const &ref) const {
        return {l + ref.l, m + ref.m, u + ref.u};
    }

    constexpr TriFuzzyNum operator-(TriFuzzyNum const &ref) const {
        return {l - ref.u, m - ref.m, u - ref.l};
    }

    constexpr TriFuzzyNum operator*(TriFuzzyNum const &ref) const {
        return {l * ref.l, m * ref.m, u * ref.u};
    }

    TriFuzzyNum &operator+=(TriFuzzyNum const &ref) {
        l += ref.l;
        m += ref.m;
        u += ref.u;
        return *this;
    }

    TriFuzzyNum &operator-=(TriFuzzyNum const &ref) {
        l -= ref.u;
        m -= ref.m;
        u -= ref.l;
        return *this;
    }

    TriFuzzyNum &operator*=(TriFuzzyNum const &ref) {
        l *= ref.l;
        m *= ref.m;
        u *= ref.u;
        sortNumbers();
        return *this;
    }

    constexpr bool operator==(TriFuzzyNum const &ref) const {
        return l == ref.l && m == ref.m && u == ref.u;
    }

    constexpr bool operator!=(TriFuzzyNum const &ref) const {
        return !(*this == ref);
    }

    constexpr bool operator<=(TriFuzzyNum const &ref) const {
        real_t x = getRangeX(), y = getRangeY(), z = getRangeZ();
        real_t ref_x = ref.getRangeX(), ref_y = ref.getRangeY(), ref_z = ref.getRangeZ();
        return x < ref_x || (x == ref_x && (y < ref_y || (y == ref_y && z <= ref_z)));
    }

    constexpr bool operator<(TriFuzzyNum const &ref) const {
        return *this <= ref && (
                getRangeX() != ref.getRangeX() ||
                getRangeY() != ref.getRangeY() ||
                getRangeZ() != ref.getRangeZ());
    }

    constexpr bool operator>=(TriFuzzyNum const &ref) const {
        return !(*this < ref);
    }

    constexpr bool operator>(TriFuzzyNum const &ref) const {
        return !(*this <= ref);
    }

    constexpr int operator<=>(TriFuzzyNum const &ref) const {
        return *this < ref ? -1 :
               *this == ref ? 0 : 1;
    }

private:

    real_t l, m, u;

    void sortNumbers() {
        if (l > m) {
            swap(l, m);
        }
        if (m > u) {
            swap(m, u);
        }
        if (l > m) {
            swap(l, m);
        }
    }

    [[nodiscard]] constexpr real_t getRangeX() const {
        return getX() - getY() / 2;
    }

    [[nodiscard]] constexpr real_t getRangeY() const {
        return 1 - getY();
    }

    [[nodiscard]] constexpr real_t getRangeZ() const {
        return m;
    }

    [[nodiscard]] constexpr real_t getX() const {
        return ((u - l) * m +
                sqrt(1 + (u - m) * (u - m)) * l +
                sqrt(1 + (m - l) * (m - l)) * u) / getZ();
    }

    [[nodiscard]] constexpr real_t getY() const {
        return (u - l) / getZ();
    }

    [[nodiscard]] constexpr real_t getZ() const {
        return (u - l) +
               sqrt(1 + (u - m) * (u - m)) +
               sqrt(1 + (m - l) * (m - l));
    }

    constexpr static real_t trunc(real_t val) {
        return abs(val) < 1e-15 ? 0 : val;
    }

};

inline consteval TriFuzzyNum crisp_number(real_t v) {
    return {v, v, v};
}

inline constinit const TriFuzzyNum crisp_zero = crisp_number(0);

inline ostream &operator<<(ostream &os, const TriFuzzyNum &obj) {
    os << "(" << obj.lower_value() << ", " << obj.modal_value() << ", " << obj.upper_value() << ")";
    return os;
}


class TriFuzzyNumSet {

public:

    TriFuzzyNumSet(initializer_list<TriFuzzyNum> collection = {}) : nums(collection) {}

    TriFuzzyNumSet(TriFuzzyNumSet const &set) = default;

    TriFuzzyNumSet(TriFuzzyNumSet &&set) = default;

    TriFuzzyNumSet &operator=(TriFuzzyNumSet const &ref) = default;

    TriFuzzyNumSet &operator=(TriFuzzyNumSet &&ref) noexcept {
        nums = move(ref.nums);
        return *this;
    }

    void insert(TriFuzzyNum const &num) {
        nums.emplace_back(num);
    }

    void insert(TriFuzzyNum &&num) {
        nums.push_back(move(num));
    }

    void remove(const TriFuzzyNum &num) {
        auto it = find(nums.begin(), nums.end(), num);
        if (it != nums.end()) {
            nums.erase(it);
        }
    }

    [[nodiscard]] TriFuzzyNum arithmetic_mean() const {
        if (nums.empty()) {
            throw length_error("TriFuzzyNumSet::arithmetic_mean - the set is empty.");
        }
        real_t size = 0;
        TriFuzzyNum res = crisp_number(0);
        for (const TriFuzzyNum &num: nums) {
            res += num;
            size++;
        }
        return {res.lower_value() / size, res.modal_value() / size, res.upper_value() / size};
    }

private:

    vector<TriFuzzyNum> nums;
};


#endif //FUZZY_H
