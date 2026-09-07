#pragma once 

#include <unordered_map>
#include <algorithm>

template <typename T>
struct CacheEntry {
    T data;
    std::size_t counter_ = 0;
};

template <typename Key, typename Value> 
class Cache {
private:
    std::size_t cap_;
    std::unordered_map<Key, CacheEntry<Value>> hash_map_;

    void remove_min_counter() {
        auto min_item = std::ranges::min(
            hash_map_,
            [](const std::pair<Key, CacheEntry<Value>> item) {
                return item.second.counter_;
            }
        );
        hash_map_.erase(std::move(min_item.second));
    } 

public:
    Cache() = delete;
    explicit Cache(std::size_t cap): cap_{cap} {}

    Value get(Key key) {
        auto& kval = hash_map_.at(std::move(key));
        kval.counter_++;
        return kval;
    }

    void put(const Key& key, const Value& val) {
        if (hash_map_.size() == cap_) { remove_min_counter(); }
        hash_map_.insert({key, CacheEntry<Value>{val, 0}});    
    }

    void put(const Key& key, Value&& val) {
        if (hash_map_.size() == cap_) { remove_min_counter(); }
        hash_map_.insert({key, CacheEntry<Value>{val, 0}});
    }
};
