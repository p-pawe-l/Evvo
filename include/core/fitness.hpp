#pragma once 

#include <limits>
#include <type_traits>

namespace evvo::fitness::tactics {
    struct higher_better {
    private:
        constexpr static double worst_fitness = -std::numeric_limits<double>::infinity();
    
    public:
        [[nodiscard]] constexpr static double get_worst_fit() noexcept { return worst_fitness; }
        constexpr bool operator()(const double f1, const double f2) const noexcept {
            return f1 > f2;
        }
    };

    struct higher_or_equal_better {
    private:
        constexpr static double worst_fitness = higher_better::get_worst_fit();

    public:
        [[nodiscard]] constexpr static double get_worst_fit() noexcept { return worst_fitness; }
        constexpr bool operator()(const double f1, const double f2) const noexcept {
            return f1 >= f2;
        }
    };
    struct lower_better  {
    private:
        constexpr static double worst_fitness = std::numeric_limits<double>::infinity();

    public:
        [[nodiscard]] constexpr static double get_worst_fit() noexcept { return worst_fitness; }
        constexpr bool operator()(const double f1, const double f2) const noexcept {
            return f1 < f2;
        }
    };

    struct lower_or_equal_better {
    private:
        constexpr static double worst_fitness = lower_better::get_worst_fit();

    public:
        [[nodiscard]] constexpr static double get_worst_fit() noexcept { return worst_fitness; }
        constexpr bool operator()(const double f1, const double f2) const noexcept {
            return f1 <= f2;
        }
    };
}

namespace evvo::fitness {

    template <typename Tactics> 
        requires std::is_default_constructible_v<Tactics> &&
                 std::is_invocable_r_v<bool, Tactics, double, double>
    constexpr bool is_better(const double first, const double second) {
        return Tactics{}(first, second);
    }

}