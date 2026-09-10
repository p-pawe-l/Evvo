#pragma once 

#include <limits>
#include <concepts>

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

namespace evvo::fitness::tactics::concepts {
    template <typename T>
    concept FitnessTactic = requires(const T& tactic, double a, double b) {
        { tactic(a, b) } -> std::same_as<bool>;
        { tactic.get_worst_fit() } -> std::same_as<double>;
    };
}

namespace evvo::fitness {

    // Informs whether first fitness is better than second one.
    template <tactics::concepts::FitnessTactic Tactics> 
        requires std::default_initializable<Tactics>
    constexpr bool is_better(const double first, const double second) {
        return Tactics{}(first, second);
    }

}