#pragma once

#include <utility>
#include <ratio>
#include <numeric>

#include "dim.h"

namespace units {
namespace detail {
template<typename T> struct is_ratio : public std::false_type {};
template<auto N, auto D> struct is_ratio<std::ratio<N,D>> : public std::true_type {};

template<typename T>
constexpr bool is_ratio_v = is_ratio<T>::value;

template<typename R1, typename R2>
struct common_ratio {
    // a common ratio used for addition/subtraction
    // actually gcd of the two ratios
    static_assert(is_ratio<R1>::value);
    static_assert(is_ratio<R2>::value);
    using type = typename std::ratio<std::gcd(R1::num, R2::num),
                                     std::lcm(R1::den, R2::den)>::type;
};

template<typename fromS, typename toS>
struct scale_helper {
    static_assert(is_ratio<fromS>::value && is_ratio<toS>::value);
    using scale_div = typename std::ratio_divide<fromS, toS>::type;

    template<typename T>
    constexpr static T calc(const T & v)
    noexcept(noexcept((scale_div::num * v) / scale_div::den))
    {
        return (scale_div::num * v) / scale_div::den;
    }
};

} // namespace detail

template<typename U, typename Dim, typename S>
class unit {
    static_assert(dim::is_list<Dim>::value, "Dim needs to bee a dim::list");
    static_assert(detail::is_ratio<S>::value, "Scale needs to be a std::ratio");
    U value;
public:
    using Scale = S;
    using Dimension = Dim;
    //explicit
    constexpr
    unit(U &&u)
    noexcept(noexcept(U{std::move(u)}))
            : value{std::move(u)} {}

    template<typename U2, typename S2>
    constexpr unit(const unit<U2, Dim, S2> &u)
    noexcept(noexcept(U{detail::scale_helper<S2, S>::calc(static_cast<U>(u.get()))}))
            : value{detail::scale_helper<S2, S>::calc(static_cast<U>(u.get()))} {}

    constexpr const U &get() const noexcept { return value; }

    template<typename U2, typename S2>
    constexpr // ??
    const unit &operator+=(const unit<U2, Dim, S2> &o) {
        value += unit(o).get();
        return *this;
    }

    template<typename U2, typename S2>
    constexpr // ??
    const unit &operator-=(const unit<U2, Dim, S2> &o) {
        value -= unit(o).get();
        return *this;
    }

    constexpr
    const unit operator-() const
    noexcept(noexcept(unit(-value)))
    {
        return unit(-value);
    }

    template<typename U2, typename S2>
    friend constexpr auto operator==(const unit &u1, const unit<U2, Dim, S2> &u2)
    noexcept(noexcept(u1.value == unit(u2).value)) {
        return u1.value == unit(u2).value;
    }

    template<typename U2, typename S2>
    friend constexpr auto operator!=(const unit &u1, const unit<U2, Dim, S2> &u2)
    noexcept(noexcept(u1.value != unit(u2).value)) {
        return u1.value != unit(u2).value;
    }

    template<typename U2, typename S2>
    friend constexpr auto operator+(const unit &u1, const unit<U2, Dim, S2> &u2)
    // noexcept etc
    {
        using return_ratio = typename detail::common_ratio<S, S2>::type;
        using return_type = unit<decltype(u1.get() + u2.get()), Dim, return_ratio>;
        return return_type(return_type(u1).get() + return_type(u2).get());
    }

    template<typename U2, typename S2>
    friend constexpr auto operator-(const unit &u1, const unit<U2, Dim, S2> &u2)
    // noexcept etc
    {
        using return_ratio = typename detail::common_ratio<S, S2>::type;
        using return_type = unit<decltype(u1.get() - u2.get()), Dim, return_ratio>;
        return return_type(return_type(u1).get() - return_type(u2).get());
    }

    template<typename U2, typename Dim2, typename S2>
    friend constexpr auto operator*(const unit &u1, const unit<U2, Dim2, S2> &u2)
    // noexcept etc
    {
        using return_ratio = typename std::ratio_multiply<S, S2>::type;
        using return_type = unit<decltype(u1.get() * u2.get()), dim::add_t<Dim, Dim2>,
        return_ratio>;
        return return_type(u1.get() * u2.get());
    }

    template<typename U2, typename Dim2, typename S2>
    friend constexpr auto operator/(const unit &u1, const unit<U2, Dim2, S2> &u2)
    // noexcept etc
    {
        using return_ratio = typename std::ratio_multiply<S, S2>::type;
        using return_type = unit<decltype(u1.get() / u2.get()),
                dim::subtract_t<Dim, Dim2>, return_ratio>;
        return return_type(u1.get() / u2.get());
    }

};


}
