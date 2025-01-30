#ifndef CXX_UTILS_ALLOCATORS_BIG_CHUNK_THREAD_LOCAL_ALLOC_HPP_
#define CXX_UTILS_ALLOCATORS_BIG_CHUNK_THREAD_LOCAL_ALLOC_HPP_

#include <cinttypes>
#include <cstdlib>

class BigChunkAllocator
{
    struct MemNode {
        MemNode() = default;

        ~MemNode() = default;

        uint8_t *mem_chunk{};
        MemNode *next{};
    };

    public:
    BigChunkAllocator(const size_t chunk_size, const size_t num_threads)
        : _chunk_size(chunk_size), _num_threads(num_threads)
    {
        _thread_tops = new size_t[num_threads]{};
        _heads       = new MemNode *[num_threads];

        for (size_t thread_idx = 0; thread_idx < num_threads; ++thread_idx) {
            _heads[thread_idx]            = new MemNode();
            _heads[thread_idx]->mem_chunk = new uint8_t[chunk_size];
        }
    }

    ~BigChunkAllocator()
    {
        delete[] _thread_tops;

        for (size_t thread_idx = 0; thread_idx < _num_threads; ++thread_idx) {
            const MemNode *node = _heads[thread_idx];

            while (node) {
                const MemNode *next = node->next;
                delete node->mem_chunk;
                delete node;
                node = next;
            }
        }

        delete[] _heads;
    }

    template <typename T, typename... Args>
    [[nodiscard]] T *Alloc(const size_t thread_idx, Args... args)
    {
        const size_t size_needed = sizeof(T);
        const size_t cur_top     = _thread_tops[thread_idx];

        if (cur_top + size_needed > _chunk_size) {
            auto *new_node      = new MemNode();
            new_node->mem_chunk = new uint8_t[_chunk_size];
            new_node->next      = _heads[thread_idx];

            _heads[thread_idx]       = new_node;
            _thread_tops[thread_idx] = size_needed;
            return new (new_node->mem_chunk) T(args...);
        }

        MemNode *cur_node = _heads[thread_idx];
        _thread_tops[thread_idx] += size_needed;

        return new (cur_node->mem_chunk + cur_top) T(args...);
    }

    protected:
    size_t _chunk_size{};
    size_t _num_threads{};

    size_t *_thread_tops{};
    MemNode **_heads{};
};

#endif  // CXX_UTILS_ALLOCATORS_BIG_CHUNK_THREAD_LOCAL_ALLOC_HPP_
