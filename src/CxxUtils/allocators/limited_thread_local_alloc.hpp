#ifndef CXX_UTILS_ALLOCATORS_LIMITED_THREAD_LOCAL_ALLOC_HPP_
#define CXX_UTILS_ALLOCATORS_LIMITED_THREAD_LOCAL_ALLOC_HPP_

/* external includes */
#include <cassert>
#include <cstdlib>

template <typename ItemT>
class FastTheadLocalAllocator
{
    public:
    FastTheadLocalAllocator(const size_t num_threads, const size_t max_items_per_thread)
        : _num_threads(num_threads), _max_items_per_thread(max_items_per_thread)
    {
        _items    = new ItemT[num_threads * max_items_per_thread];
        _counters = new size_t[num_threads];
    }

    ~FastTheadLocalAllocator()
    {
        delete[] _items;
        delete[] _counters;
    }

    [[nodiscard]] ItemT *Alloc(const size_t thread_idx)
    {
        const size_t idx = thread_idx * _max_items_per_thread + _counters[thread_idx]++;
        assert(_counters[thread_idx] < _max_items_per_thread);

        return _items + idx;
    }

    [[nodiscard]] ItemT *Get(const size_t thread_idx, const size_t idx)
    {
        assert(idx < _max_items_per_thread);
        assert(thread_idx < _num_threads);
        assert(idx < _counters[thread_idx]);

        return _items + thread_idx * _max_items_per_thread + idx;
    }

    private:
    ItemT *_items{};
    size_t *_counters{};
    size_t _num_threads{};
    size_t _max_items_per_thread{};
};

#endif  // CXX_UTILS_ALLOCATORS_LIMITED_THREAD_LOCAL_ALLOC_HPP_
