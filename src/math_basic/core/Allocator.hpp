#pragma once
#include <cstdlib>
#include <stdlib.h>
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
            if (posix_memalign(&p, 32, n * sizeof(T)) != 0) p = nullptr;
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

template <typename T, typename U>
bool operator==(const AlignedAllocator<T>&, const AlignedAllocator<U>&) noexcept { return true; }
template <typename T, typename U>
bool operator!=(const AlignedAllocator<T>&, const AlignedAllocator<U>&) noexcept { return false; }
