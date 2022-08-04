#include "robin_hood_map.hpp"

template <typename Key, typename Val, typename Hash>
using fast_map_hash = robin_hood::unordered_map<Key, Val, Hash>;

template <typename Key, typename Val>
using fast_map = robin_hood::unordered_map<Key, Val>;

template <typename Key>
using fast_set = robin_hood::unordered_set<Key>;

// back to old hash table
// #include <ext/pb_ds/assoc_container.hpp>
// template <typename Key, typename Val, typename Hash>
// using fast_map_hash = __gnu_pbds::gp_hash_table<Key, Val, Hash>;

// template <typename Key, typename Val>
// using fast_map = __gnu_pbds::gp_hash_table<Key, Val>;

// template <typename Key>
// using fast_set = __gnu_pbds::gp_hash_table<Key, __gnu_pbds::null_type>;