#pragma once

#include "units.h"

namespace units {
namespace lmt {

namespace lmtdim {
struct lmt_tag;
template<int L, int M, int T>
using lmt = dim::list<lmt_tag, L, M, T>;

using Scalar = lmt<0, 0, 0>;

using Length = lmt<1, 0, 0>;
using Mass   = lmt<0, 1, 0>;
using Time   = lmt<0, 0, 1>;

using Area   = lmt<2, 0, 0>;
using Volume = lmt<3, 0, 0>;

using Velocity     = lmt<1, 0, -1>;
using Acceleration = lmt<1, 0, -2>;

using Force  = lmt<1, 1, -2>;
using Energy = lmt<2, 1, -2>;
using Power  = lmt<2, 1, -3>;
}

template<typename T> using Id = unit<T, lmtdim::Scalar, std::ratio<1>>;

template<typename T, typename S> using Length = unit<T, lmtdim::Length, S>;
template<typename T, typename S> using Mass   = unit<T, lmtdim::Mass,   S>;
template<typename T, typename S> using Time   = unit<T, lmtdim::Time,   S>;

template<typename T, typename S> using Area   = unit<T, lmtdim::Area,   S>;

template<typename T, typename S> using Velocity     = unit<T, lmtdim::Velocity,     S>;
template<typename T, typename S> using Acceleration = unit<T, lmtdim::Acceleration, S>;

template<typename T> using Meter    = Length<T, std::ratio<1>>;
template<typename T> using Kilogram = Mass<T, std::ratio<1>>;
template<typename T> using Second   = Time<T, std::ratio<1>>;

template<typename T> using SquareMeter = Area<T, std::ratio<1>>;

// scaled units
template<typename T> using Centimeter = Length<T, std::centi>;
template<typename T> using Millimeter = Length<T, std::milli>;
template<typename T> using Micrometer = Length<T, std::micro>;

template<typename T> using Kilometer = Length<T, std::kilo>;

template<typename T> using Inch = Length<T, std::ratio<254,10000>::type>;
template<typename T> using Foot = Length<T, std::ratio<3048,10000>::type>;
template<typename T> using Mile = Length<T, std::ratio<1609344,1000>::type>;

template<typename T> using Kmh = Velocity<T, std::ratio<1000,3600>::type>;
template<typename T> using Mph = Velocity<T, std::ratio<1609344,3600000>::type>;

/*
template<typename T> using Minute = Time<T, std::ratio<60>>;
template<typename T> using Hour   = Time<T, std::ratio<3600>>;
*/

inline namespace literals {

constexpr auto operator""_m(unsigned long long v) -> Meter<long long> { return v; }
constexpr auto operator""_m(long double v)        -> Meter<double> { return v; }

constexpr auto operator""_cm(unsigned long long v) -> Centimeter<long long> { return v; }
constexpr auto operator""_cm(long double v)        -> Centimeter<double> { return v; }

constexpr auto operator""_mm(unsigned long long v) -> Millimeter<long long> { return v; }
constexpr auto operator""_mm(long double v)        -> Millimeter<double> { return v; }

constexpr auto operator""_ft(unsigned long long v) -> Foot<long long> { return v; }
constexpr auto operator""_ft(long double v)        -> Foot<double> { return v; }

constexpr auto operator""_in(unsigned long long v) -> Inch<long long> { return v; }
constexpr auto operator""_in(long double v)        -> Inch<double> { return v; }

}

}
}
