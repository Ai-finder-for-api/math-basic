#pragma once
#include <cstdlib>
#include <new>

template <typename T>
class AlignedAllocator {
public:
    using value_type = T;
    T* allocate(std::size_t n) {
        if (n > std::size_t(-1) / sizeof(T)) throw std::bad_alloc();
        void* p = nullptr;
        #if defined(_MSC_VER)
            p = _aligned_malloc(n * sizeof(T), 32);
        #else
            posix_memalign(&p, 32, n * sizeof(T));
        #endif
        if (!p) throw std::bad_alloc();
        return static_cast<T*>(p);
    }
    void deallocate(T* p, std::size_t) noexcept {
        #if defined(_MSC_VER)
            _aligned_free(p);
        #else
            free(p);
        #endif
    }
};
