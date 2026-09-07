#pragma once 

#include <limits>
#include <type_traits>

namespace evvo::fitness::tactics {
    struct higher_better {
    private:
        const double worst_fitness = -std::numeric_limits<double>::infinity();
    
    public:
        [[nodiscard]] double get_worst_fit() const noexcept { return worst_fitness; }
        constexpr bool operator()(const double f1, const double f2) {
            return f1 > f2;
        }
    };

    struct lower_better  {
    private:
        const double worst_fitness = std::numeric_limits<double>::infinity();

    public:
        [[nodiscard]] double get_worst_fit() const noexcept { return worst_fitness; }
        constexpr bool operator()(const double f1, const double f2) {
            return f1 < f2;
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