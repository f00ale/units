#pragma once

#include <ostream>
#include <ratio>

#include "units.h"

namespace units {

template<typename Scale>
struct dumpratio {
    static_assert(detail::is_ratio_v<Scale>, "Scale needs to be a ratio");

    friend std::ostream &operator<<(std::ostream &os, const dumpratio &) {
        os << "(" << Scale::num << '/' << Scale::den << ")";
        return os;
    }
};

template<typename Scale>
struct prefix : public dumpratio<Scale> {};

template<>
struct prefix<std::ratio<1>> {
    friend std::ostream &operator<<(std::ostream &os, const prefix &) { return os; }
};

template<>
struct prefix<std::deci> {
    friend std::ostream &operator<<(std::ostream &os, const prefix &) { return (os << "d"); }
};

template<>
struct prefix<std::centi> {
    friend std::ostream &operator<<(std::ostream &os, const prefix &) { return (os << "c"); }
};

template<>
struct prefix<std::milli> {
    friend std::ostream &operator<<(std::ostream &os, const prefix &) { return (os << "m"); }
};

template<>
struct prefix<std::micro> {
    friend std::ostream &operator<<(std::ostream &os, const prefix &) { return (os << "µ"); }
};

template<typename Dim, typename Scale>
struct unitoutput {
    static_assert(dim::is_list_v<Dim>, "Dim needs to be a dimension");
    static_assert(detail::is_ratio_v<Scale>, "Scale needs to be a ratio");

    friend std::ostream &operator<<(std::ostream &os, const unitoutput &) {
        os << " <";
        dim::output<Dim>::put(os);
        os << "> " << Scale::num << '/' << Scale::den;
        return os;
    }
};

template<typename U, typename Dim, typename S>
std::ostream &operator<<(std::ostream &os, const unit<U, Dim, S> &u) {
    os << u.get() << unitoutput<Dim, S>();
    return os;
}

}
