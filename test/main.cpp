#include <units.h>
#include <io.h>
#include <lmt.h>

#include <iostream>

namespace units {
template<int ... L, typename S>
struct unitoutput<dim::list<lmt::lmtdim::lmt_tag, L...>, S> {
    friend std::ostream &operator<<(std::ostream & os, const unitoutput &) {
        os << " " << dumpratio<S>() << " lmt<";
        dim::output<dim::list<lmt::lmtdim::lmt_tag, L...>>::put(os);
        os << ">";
        return os;
    }
};

template<typename S>
struct unitoutput<lmt::lmtdim::Length, S> {
    friend std::ostream &operator<<(std::ostream & os, const unitoutput &) {
        os << " " << prefix<S>() << "m";
        return os;
    }
};

template<>
struct unitoutput<lmt::lmtdim::Length, std::ratio<254,10000>::type> {
    friend std::ostream &operator<<(std::ostream & os, const unitoutput &) {
        os << " in";
        return os;
    }
};

namespace {
using ft_ratio = std::ratio<3048,10000>::type;
}

template<>
struct unitoutput<lmt::lmtdim::Length, ft_ratio> {
    friend std::ostream &operator<<(std::ostream & os, const unitoutput &) {
        os << " ft";
        return os;
    }
};

template<>
struct unitoutput<lmt::lmtdim::Area, std::ratio<1>> {
    friend std::ostream &operator<<(std::ostream & os, const unitoutput &) {
        os << " m²";
        return os;
    }
};

template<>
struct unitoutput<lmt::lmtdim::Area, std::ratio_multiply<ft_ratio, ft_ratio>> {
    friend std::ostream &operator<<(std::ostream & os, const unitoutput &) {
        os << " ft²";
        return os;
    }
};



}

template<typename> struct constants;
template<> struct constants<int> {
    constexpr static const units::lmt::Acceleration<int, std::centi> g{982};
};
template<> struct constants<double> {
    constexpr static const units::lmt::Acceleration<double, std::ratio<1>> g{9.82};
};

template<typename T> constexpr auto g = constants<T>::g;

int main() {
    using namespace units::lmt;

    static_assert(Meter<int>(2)+Foot<int>(2)  == Micrometer<int>(2609600));
    static_assert(Meter<int>(2)*Meter<int>(2) == SquareMeter<int>(4));
    static_assert(g<int> * Second<int>(3) == Velocity<int, std::centi>(3*982));
    Meter<double> m{1};
    Foot<double> f{1};

    std::cout << m << std::endl;
    std::cout << f << std::endl;
    std::cout << (m+f) << std::endl;

    Meter<int> mi(1);
    Foot<int> fi(1);

    std::cout << (mi+fi) << " m+ft" << std::endl;
    std::cout << (mi-fi) << " m-ft" << std::endl;
    std::cout << Micrometer<int>(mi+fi) << " m+ft" << std::endl;
    std::cout << Micrometer<int>(mi-fi) << " m-ft" << std::endl;

    std::cout << (mi+f) << std::endl;

    std::cout << m << std::endl;
    m+=fi;
    std::cout << m << std::endl;

    std::cout << Centimeter<double>(m+f) << std::endl;

    using c = units::detail::common_ratio<std::ratio<1>, std::ratio<304,1000>>::type;
    std::cout << c::num << '/' << c::den << std::endl;

    using c2 = units::detail::common_ratio<std::mega, std::milli>::type;
    std::cout << c2::num << '/' << c2::den << std::endl;

    std::cout << (Meter<int>(2)*Meter<int>(2)) << std::endl;

    {
        Mph<double> mph(55);
        Kmh<double> kmh(mph);
        std::cout << mph << std::endl;
        std::cout << kmh << std::endl;
    }

    std::cout << g<int> << std::endl;
    std::cout << g<double> << std::endl;


    std::cout << Inch<int>(1) << std::endl;
    std::cout << Foot<int>(1) << std::endl;

    std::cout << (Foot<int>(1) + Inch<int>(12)) << std::endl;
    std::cout << (Inch<int>(12) + Inch<int>(12)) << std::endl;
    std::cout << (Foot<int>(1) + Foot<int>(1)) << std::endl;

    {
        constexpr Foot<int> f1(1);
        constexpr Inch<int> i12(12);
        static_assert(f1 == i12);

        constexpr Meter<int> m1(1);
        constexpr Centimeter<int> c100(100);
        static_assert(m1 == c100);

        constexpr auto c = Inch<int>(5) + Centimeter<int>(8);
        static_assert(c == Millimeter<int>(207));

        static_assert(1_ft == 12_in);
        static_assert(1_m == 100_cm);
        static_assert((5_in + 8_cm) == 207_mm);
    }


    {
        auto t = Second<int>(3);
        auto d = t*t*g<int>/Id<int>(2);
        assert(d == Centimeter<int>(4419));
        std::cout << d << std::endl;
    }
/*
    std::cout << (Meter<int>(1) * unit(3)) << std::endl;
    std::cout << (unit(3) * Meter<int>(1)) << std::endl;
    std::cout << (Id<int>(3) * Meter<int>(1)) << std::endl;
*/

    {
        struct svt_tag;
        using namespace units::dim;
        using Length   = list<svt_tag, 1,0,0>;
        using Time     = list<svt_tag, 0,0,1>;
        using Velocity = subtract_t<Length, Time>;

        static_assert(std::is_same<list<svt_tag,1,0,0>, add_t<list<svt_tag,-1,2,1>,list<svt_tag,2,-2,-1>>>::value, "");
        static_assert(std::is_same<list<svt_tag,1,0,-1>, Velocity>::value, "");
    }

    std::cout << (Meter<int>(2) + Foot<int>(3)) << std::endl;
    std::cout << Foot<int>(3) << std::endl;


    auto area = Foot<int>(3) * Foot<int>(4);
    std::cout << area << std::endl;
    std::cout << SquareMeter<double>(area) << std::endl;

}
