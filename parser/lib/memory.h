/**
Copyright 2012-2015 Bernard van Gastel, bvgastel@bitpowder.com.
This file is part of Bit Powder Libraries.

Bit Powder Libraries is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Bit Powder Libraries is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Bit Powder Libraries.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MEMORY_H
#define MEMORY_H

#ifndef NCONCURRENCY
#include "spinlock.h"
#endif
#include "stack.h"
#include "queue.h"
#include "exception.h"
#ifndef NDEBUG
#endif
#include <set>

#ifndef NO_CPP_DEMANGLE
#include <cxxabi.h>
#ifdef __cplusplus
using __cxxabiv1::__cxa_demangle;
#endif
#endif

#include <cstddef>
#include <type_traits>

//#ifdef __linux__
//#define MEMORY_DEBUG_MEMORY_H
//#endif

#ifdef MEMORY_DEBUG
#define MEMORY_DEBUG_MEMORY_H
#endif

namespace bitpowder {
namespace lib {

void* malloc_with_minimal_size(std::size_t& size);

struct CachedInstance {
    CachedInstance* next = nullptr;
};

class CacheOfInstancesBase {
  public:
    static std::set<CacheOfInstancesBase*>* getAllInstanceCaches();
    static void purgeAll();
    static void statsAll();
    virtual void purge() const = 0;
    virtual void stats() const = 0;
};

template <class T>
class CacheOfInstances : CacheOfInstancesBase {
  public:
#ifndef NCONCURRENCY
    mutable SpinLock instanceCacheLock;
#endif
    mutable Stack< CachedInstance* > instanceCache;
    mutable unsigned long long instanceCount = 0;

    mutable unsigned long long activeCount = 0;

    void purge() const override {
#ifndef NCONCURRENCY
        auto l = instanceCacheLock.locker();
#endif
        auto local = instanceCache.pop_all();
        instanceCount = 0;
#ifndef NCONCURRENCY
        l.unlock();
#endif
        while (!local.empty()) {
            CachedInstance* instance = local.pop_back();
            free(instance);
        }
    }
    void stats() const override {
        size_t objectSize = sizeof(T);
#ifndef NCONCURRENCY
        auto l = instanceCacheLock.locker();
#endif
#ifndef NO_CPP_DEMANGLE
        char buffer[2048];
        size_t length = 2048;
        int status = 0;
        const char* displayName = __cxa_demangle(typeid(T).name(), buffer, &length, &status);
        if (!displayName)
            displayName = typeid(T).name();
#else
        char* displayName = typeid(T).name();
#endif
        std::cout << displayName << " has " << activeCount << " active objects = " << (activeCount * objectSize)/1024 << " KiB (cached " << instanceCount << " objects = " << (instanceCount * objectSize)/1024 << " KiB)" << std::endl;
    }
    CacheOfInstances() {
        getAllInstanceCaches()->insert(this);
    }
};

template <class T>
class CacheInstances {
#ifndef MEMORY_DEBUG_MEMORY_H
    static CacheOfInstances<T> InstanceCache;
#ifndef NDEBUG
    static std::set<void*> instances;
#endif
  public:
    ~CacheInstances() {
    }
    static void purge() {
        InstanceCache.purge();
    }

    template <class C>
    static void reuse(C& objects) {
        unsigned long long objectCount = 0;
        Queue<CachedInstance*> convertedObjects;
        while (!objects.empty()) {
            T* pointer = static_cast<T*>(objects.pop());
            destroy<T>(pointer);
            convertedObjects.push(new (pointer) CachedInstance());
            ++objectCount;
        }

#ifndef NCONCURRENCY
        auto l = InstanceCache.instanceCacheLock.locker();
#endif

#ifndef NDEBUG
        for (auto pointer : convertedObjects)
            instances.erase(pointer);
#endif

        InstanceCache.instanceCache.push_back(std::move(convertedObjects));
        InstanceCache.instanceCount += objectCount;
        InstanceCache.activeCount -= objectCount;
    }

    class Reserved {
        T* pointer = nullptr;
        void clear() {
            if (pointer) {
                T* c = pointer;
#ifndef NCONCURRENCY
                auto l = InstanceCache.instanceCacheLock.locker();
#endif
                InstanceCache.instanceCache.push_back(new (c) CachedInstance());
                ++InstanceCache.instanceCount;
                --InstanceCache.activeCount;
#ifndef NDEBUG
                instances.erase(pointer);
#endif
                pointer = nullptr;
            }
        }

      public:
        Reserved(T* pointer = nullptr) : pointer(pointer) {
        }
        Reserved(Reserved&& r) : pointer(r.pointer) {
            r.pointer = nullptr;
        }
        Reserved& operator=(Reserved &&r) {
            if (this == &r)
                return *this;
            clear();
            pointer = r.pointer;
            r.pointer = nullptr;
            return *this;
        }
        ~Reserved() {
            clear();
        }

        void* get() {
            T* retval = pointer;
            pointer = nullptr;
            return retval;
        }

        void reserve() {
            if (pointer)
                return;
#ifndef NCONCURRENCY
            auto l = InstanceCache.instanceCacheLock.locker();
#endif
            pointer = (T*)InstanceCache.instanceCache.pop_back();
            if (pointer)
                --InstanceCache.instanceCount;
            ++InstanceCache.activeCount;
#ifndef NCONCURRENCY
            l.unlock();
#endif
            if (!pointer)
                pointer = (T*)malloc(sizeof(T));
#ifndef NDEBUG
#ifndef NCONCURRENCY
            l.lock(InstanceCache.instanceCacheLock);
#endif
            instances.insert((void*)pointer);
#endif
        }
    };

    static Reserved reserve() {
        Reserved retval;
        retval->reserve();
        return retval;
    }

    static void* operator new(std::size_t size, Reserved& r) {
        if (sizeof(T) != size)
            return malloc(size);
        void* retval = r.get();
        return retval ? retval : malloc(sizeof(T));
    }

    static void* operator new(std::size_t size) {
        assert(sizeof(T) == size);
#ifndef NCONCURRENCY
        auto l = InstanceCache.instanceCacheLock.locker();
#endif
        void* retval = InstanceCache.instanceCache.pop_back();
        if (retval)
            --InstanceCache.instanceCount;
        ++InstanceCache.activeCount;
#ifndef NCONCURRENCY
        l.unlock();
#endif
        if (!retval)
            retval = (T*)malloc(size);

#ifndef NDEBUG
#ifndef NCONCURRENCY
        l.lock(InstanceCache.instanceCacheLock);
#endif
        instances.insert(retval);
#endif
        return retval;
    }

    static void operator delete(void* pointer, std::size_t size) {
        assert(sizeof(T) == size);
#ifndef NCONCURRENCY
        auto l = InstanceCache.instanceCacheLock.locker();
#endif

        InstanceCache.instanceCache.push_back(new (pointer) CachedInstance());
        ++InstanceCache.instanceCount;
        --InstanceCache.activeCount;

#ifndef NDEBUG
        // to avoid the assert in Stack ensuring the next pointer is always nullptr
        instances.erase(pointer);
#endif
    }

#ifndef NDEBUG
    static void showInstances() {
        fprintf(stderr, "%lu instances\n", instances.size());
        for (std::set<void*>::iterator it = instances.begin(); it != instances.end(); ++it) {
            fprintf(stderr, "* %p\n", *it);
        }
    }
    static void showCachedInstancesHelper(CachedInstance* pointer) {
        fprintf(stderr, "* %p\n", pointer);
    }
    static void showCachedInstances() {
        std::for_each(InstanceCache.instanceCache.begin(), InstanceCache.instanceCache.end(), showCachedInstancesHelper);
    }
#endif
#else
  public:

    template <class C>
    static void reuse(C& objects) {
        while (!objects.empty()) {
            T* object = static_cast<T*>(objects.pop());
            delete object;
        }
    }

    static void purge() {
    }
    class Reserved {
      public:
        void reserve() {
        }
    };
    static void* operator new(size_t size) {
        void* pointer = malloc(size);
        return pointer;
    }
    static void* operator new(size_t size, Reserved& r) {
        void* pointer = malloc(size);
        return pointer;
    }
    static void operator delete(void* pointer, std::size_t size) {
        free(pointer);
    }
    static void showInstances() {
    }
    static void showCachedInstances() {
    }
#endif

    static void* operator new(std::size_t size, void* p) {
        return p;
    }

    static void operator delete(void* p, std::size_t size, void* p2) {
    }

    // only objects of one size are supported (as the operator delete does not know the size information)
    template <typename... Args>
    static void* operator new(std::size_t nbytes, std::size_t extra, Args&& ... args) = delete;
};
#ifndef MEMORY_DEBUG_MEMORY_H
template <typename T> CacheOfInstances<T> CacheInstances<T>::InstanceCache;

#ifndef NDEBUG
template <typename T> std::set<void*> CacheInstances<T>::instances;
#endif
#endif

struct AllocCounter {
    std::size_t bytes = 0;
    std::size_t count = 0;

    std::size_t getBytes() const {
        return bytes;
    }
    std::size_t getCount() const {
        return count;
    }
};

template <bool StdLayout, class T>
struct OverheadOfHelper {
    static const std::size_t value = 0;
};
template <class T>
struct OverheadOfHelper<true, T> {
    static const std::size_t value = sizeof(T)-offsetof(T, extra);
};

template <class T>
struct OverheadOf {
    static const std::size_t value = OverheadOfHelper<std::is_standard_layout<T>::value, T>::value;
};

struct Any {};

template <typename T = Any>
class CountingAllocator {
  public:
    AllocCounter* counter = nullptr;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    template <class U>
    struct rebind {
        typedef CountingAllocator<U> other;
    };

    CountingAllocator() {
    }
    CountingAllocator(AllocCounter* allocCounter) : counter(allocCounter) {
    }

    CountingAllocator(const CountingAllocator& copy) : counter(copy.counter) {
    }
    template <class U>
    inline CountingAllocator(const CountingAllocator<U>& copy) : counter(copy.counter) {
    }

    inline pointer address(reference x) const {
        return &x;
    }
    inline const_pointer address(const_reference x) const {
        return &x;
    }
    inline pointer allocate(size_type n) {
        std::size_t bytes = sizeof(value_type)*n;
        void* retval = malloc_with_minimal_size(bytes);
        if (counter) {
            counter->bytes += bytes;
            ++counter->count;
        }
        return (pointer)retval;
    }

    inline void deallocate(pointer p, size_type n) {
        deallocate((void*)p, sizeof(value_type)*n);
    }
    inline void deallocate(void* p, std::size_t bytes) {
        if (counter) {
            counter->bytes -= bytes;
            --counter->count;
        }
        free(p);
    }
    size_type max_size() const {
        return std::numeric_limits<size_type>::max();
    }
    void construct(pointer p,  const_reference val) {
        new((void*)p) T(val);
    }
    template<class U, typename... Args>
    void construct(U* p,  Args&& ... args) {
        new((void*)p) U(std::forward<Args>(args)...);
    }
    void construct(pointer p) {
        new((void*)p) T();
    }
    void destroy(pointer p) {
        p->T::~T();
    }
};

struct CachingAny {
    char _padding[__WORDSIZE/8];
};

template <typename T = CachingAny, int N = (4096-sizeof(void*))/sizeof(T)>
class CachingAllocator {
  public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    template <class U>
    struct rebind {
        typedef CachingAllocator<U> other;
    };
  protected:
    struct value_proxy {
        char placeholder[sizeof(value_type)];
    };
    struct Chunk {
        value_proxy objects[BP_MAX(1,N)];
        Chunk* next = nullptr;
        static_assert(sizeof(CachedInstance) <= sizeof(value_type), "internal requirement");
    };

  public:
    Stack<CachedInstance*> cache;
    Stack<Chunk*> freeList;

    CachingAllocator() {
    }
    ~CachingAllocator() {
        while (!freeList.empty()) {
            Chunk* instance = freeList.pop_back();
            delete instance;
        }
    }

    CachingAllocator(const CachingAllocator& copy) {
    }
    CachingAllocator(CachingAllocator&& copy) : cache(std::move(copy.cache)), freeList(std::move(copy.freeList)) {
    }
    template <class U>
    inline CachingAllocator(const CachingAllocator<U>& copy) {
    }

    inline pointer address(reference x) const {
        return &x;
    }
    inline const_pointer address(const_reference x) const {
        return &x;
    }
    inline pointer allocate(size_type n) {
        if (n != 1) {
            std::size_t bytes = sizeof(value_type)*n;
            return (pointer)malloc(bytes);
        }
        void* retval = cache.pop_back();
        if (!retval) {
            Chunk* chunk = new Chunk();
            freeList.push_back(chunk);
            // save unused objects
            for (unsigned int i = N-1; i-- > 0; )
                cache.push_back(new (&chunk->objects[i]) CachedInstance());
            // return last object
            retval = (pointer)&chunk->objects[N-1];
        }
        return (pointer)retval;
    }

    inline void deallocate(pointer p, size_type n) {
        deallocate((void*)p, sizeof(value_type)*n);
    }
    inline void deallocate(void* p, std::size_t bytes) {
        if (bytes == sizeof(value_type))
            cache.push_back(new (p) CachedInstance());
        else
            free(p);
    }
    size_type max_size() const {
        return std::numeric_limits<size_type>::max();
    }
    void construct(pointer p,  const_reference val) {
        new((void*)p) T(val);
    }
    template<class U, typename... Args>
    void construct(U* p,  Args&& ... args) {
        new((void*)p) U(std::forward<Args>(args)...);
    }
    void construct(pointer p) {
        new((void*)p) T();
    }
    void destroy(pointer p) {
        p->T::~T();
    }
};


}
}

#endif // MEMORY_H
