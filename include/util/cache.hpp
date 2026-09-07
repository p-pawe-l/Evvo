#pragma once 

#include <unordered_map>
#include <memory>
#include <mutex>
#include <functional>
#include <list>

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
        using list_t = std::list<Key>; // Double-linked list
        using ops_thread_guard = std::lock_guard<std::mutex>;
        using on_miss_cb = std::function<void(const Key& k, const Value& v)>;
        using on_erase_cb = std::function<void(const Key& k, const Value& v)>;

        using const_iter = typename map_t::const_iterator; 

        enum class cache_op_code: std::int8_t {
            SUCCESS = 0,
            EMPTY,
            INVALID_CAPACITY_SIZE,
            INVALID_KEY, 
        };

    private:
        std::size_t cap_;
        map_t cache_map_;
        list_t cache_freq_list_;
        ops_thread_guard thread_guard_;
        std::mutex op_lock_;

        on_erase_cb erase_cb_ = [](const Key& k, const Value& v) { return; };
        on_miss_cb miss_cb_ = [](const Key&k, const Value& v) { return; };

        // Removes least-used element in the cache
        cache_op_code remove() noexcept {
            const ops_thread_guard lock(op_lock_);
            if (cache_freq_list_.size() == 0) {
                return cache_op_code::EMPTY;
            }

            auto back_key = cache_freq_list_.back();
            try {
                erase_cb(back_key, cache_map_.at(back_key).get()); 
            } catch (const std::exception& e) {
                return cache_op_code::ERASE_CALLBACK_FAILURE;
            }
            if (auto res = map_erase(std::move(back_key) != cache_op_code::SUCCESS)) {
                return res;
            }
            // Removing element only after 
            // element from the map 
            // was deleted
            cache_freq_list_.pop_back();
                
            return cache_op_code::SUCCESS;
        }

        // Removes element from the cache map
        cache_op_code map_erase(Key&& k) {
            try {
                cache_map_.erase(k);
                return cache_op_code::SUCCESS;
            } catch (const std::exception& e) {
                return cache_op_code::INVALID_KEY;
            }
        }

    protected:
        const_iter begin() {
            return cache_map_.begin();
        }

        const_iter end() {
            return cache_map_.end();
        }

    public:
        explicit fixed_size_cache(std::size_t cap);
    };
}
