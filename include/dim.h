#pragma once

#include <utility>
#include <iostream>

namespace units {
namespace dim {

template<typename T, int I, int ... L>
struct list;

template<typename T>
struct is_list : public std::false_type {};
template<typename T, int ... I>
struct is_list<list<T, I...>> : public std::true_type {};

template<typename T>
constexpr bool is_list_v = is_list<T>::value;

namespace detail {
template<int, typename>
struct list_prepend;
template<int I, typename T, int ... L>
struct list_prepend<I, list < T, L...>> {
using type = list<T, I, L...>;
};

template<typename OP, typename T1, typename T2>
struct lists_op;
template<typename OP, typename T, int I1, int I2>
struct lists_op<OP, list < T, I1>, list <T, I2>> {
using type = list<T, OP()(I1, I2)>;
};

template<typename OP, typename T, int I1, int ... L1, int I2, int ... L2>
struct lists_op<OP, list < T, I1, L1...>, list<T, I2, L2...>> {
static_assert(sizeof...(L1) == sizeof...(L2), "Lists not the same size.");

using type = typename list_prepend<
        OP()(I1, I2),
        typename lists_op<OP, list < T, L1...>, list < T, L2...>>::type
>::type;

};

struct add {
    constexpr int operator()(int i1, int i2) const { return i1 + i2; }
};

struct sub {
    constexpr int operator()(int i1, int i2) const { return i1 - i2; }
};
}

template<typename L1, typename L2> using subtract_t = typename detail::lists_op<detail::sub,L1,L2>::type;
template<typename L1, typename L2> using add_t = typename detail::lists_op<detail::add,L1,L2>::type;



template<typename T>
struct output;

template<typename T, int I, int ... L>
struct output<list<T, I, L...>> {
    static void put(std::ostream & os) {
        os << I << ',';
        output<list<T,L...>>::put(os);
    }
};
template<typename T, int I>
struct output<list<T, I>> {
    static void put(std::ostream & os) {
        os << I;
    }
};

}
}
