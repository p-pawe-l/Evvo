#pragma once 

#include <stdexcept>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <mutex>

namespace evvo::cache {

    template <typename Value>
    using ShrWrappedValue = std::shared_ptr<Value>;

    template <typename Key, 
              typename Value, 
              typename HashMap = std::unordered_map<Key, ShrWrappedValue<Value>>
    >
    class fixed_size_cache {
    public:
        using map_t = HashMap;
        using ops_thread_guard = typename std::lock_guard<std::mutex>;
    
        enum class cache_op_codes: std::int8_t {
            SUCCESS = 0,
            INVALID_CAPACITY_SIZE,
            INVALID_KEY, 
        };

    private:
        std::size_t cap_;
        map_t map_;
        ops_thread_guard thread_guard_;

        cache_op_codes remove();

    public:
        explicit fixed_size_cache(std::size_t cap);
    };
}
