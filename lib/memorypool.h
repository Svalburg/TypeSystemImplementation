/**
Copyright 2010-2015 Bernard van Gastel, bvgastel@bitpowder.com.
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

/*****
 * MemoryPool
 * - with delayed function execution
 * - fast allocation of temporary memory
 * - fast saving and restoring of the current state
 */

#ifdef MEMORY_DEBUG
#include "memorypool.debug.h"
#endif

#ifndef _MEMORYPOOL_H_
#define _MEMORYPOOL_H_

#include <limits>
#include <cstddef>

#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <unistd.h>
#include "exception.h"
#include "lib-common.h"
#include "deque.h"
#include "stack.h"
#include "memory.h"
#include "shared_object.h"

namespace bitpowder {
namespace lib {

#define MEMORYPOOL_SIZE 4096
#define MEMORYPOOL_SIZE_DEFAULT_SIZE (MEMORYPOOL_SIZE - sizeof(MemoryPoolPageBase))

struct DelayedItemBase {
    DelayedItemBase* next = nullptr;
    virtual void execute() = 0;
};

template <typename F, typename... T>
struct DelayedItem : public DelayedItemBase {
    F func;
    std::tuple<T...> args;
    void* extra[0];
    DelayedItem(const F& f, const T& ... func) : func(f), args(func...) {
    }
    void execute() {
        callFunction(func, args);
    }
};

class MemoryPoolPageBase {
    friend class MemoryPool;
    template <unsigned int SIZE>
    friend class StaticMemoryPool;
  public:
    MemoryPoolPageBase* prev = nullptr;
    MemoryPoolPageBase* next = nullptr;
  public:
    MemoryPoolPageBase(void* data, unsigned int size, bool fixedSize);
  private:
    char* data;

    unsigned int size; // excluding this struct
    unsigned int available;
    bool fixedSize;

    void reset();
    static bool fitsIn(MemoryPoolPageBase* pool, size_t size, size_t alignOn, size_t& padding);
};

class MemoryPoolPageDefaultSize : public MemoryPoolPageBase, public CacheInstances<MemoryPoolPageDefaultSize> {
    friend class MemoryPool;
    template <unsigned int SIZE>
    friend class StaticMemoryPool;

    void* store[MEMORYPOOL_SIZE_DEFAULT_SIZE/sizeof(void*)]; // type is double so it is word aligned, according to the C spec

    MemoryPoolPageDefaultSize();
};

class MemoryPoolPageVariableSize : public MemoryPoolPageBase {
    friend class MemoryPool;
    template <unsigned int SIZE>
    friend class StaticMemoryPool;

    MemoryPoolPageVariableSize(unsigned int size);

    void* store[0]; // type is double so it is word aligned, according to the C spec
};

class MemoryPoolStatus {
    friend class MemoryPool;
    MemoryPoolPageBase* current = nullptr;
    DelayedItemBase* memoryObjects = nullptr;
    MemoryPoolPageBase* nextPage = nullptr;
    std::size_t available = 0;
  public:
    MemoryPoolStatus() {
    }
    MemoryPoolStatus(MemoryPoolPageBase* start, MemoryPoolPageBase* nextPage) : current(start), nextPage(nextPage) {}
};

class MemoryPool {
    NO_COPY(MemoryPool);
  protected:
    // NOT: sorted with minimal pools first, checks if space is available until it comes in a larger pool
    // NOT: a way to start searching in pools who have indeeed enough space, by making it an array, and the first item is for bytes < 16, second for < 32, etc
    // because otherwise MemoryPoolStatus struct becomes to large
    MemoryPoolPageBase* current = nullptr;
    Deque<MemoryPoolPageBase*> all;
    Stack<DelayedItemBase*> memoryObjects; // Delayed items that if executed will release memory. To avoid memory corruption if deallocating
    bool rewinding = false; // busy with rewinding
  public:
    static const size_t DEFAULT_ALIGN_ON = sizeof(void*);
    // alloc memory from the current memory pool (alloc new one if there is not enough memory). Only usable in the current autorelease pool
    void* alloc(size_t size, size_t alignOn = DEFAULT_ALIGN_ON, MemoryPoolPageBase** pool = NULL);

    void* allocAll(size_t& size, size_t alignOn = DEFAULT_ALIGN_ON);
    void* calloc(size_t size, size_t alignOn = DEFAULT_ALIGN_ON);

    // do not let memory allocations after this one cross into the next memory pool, restore after with mprestore. Should be done when allocatin a new autoreleasepool
    MemoryPoolStatus status();
    void restore(const MemoryPoolStatus& status);

    size_t available() {
        return current ? current->available : MEMORYPOOL_SIZE_DEFAULT_SIZE;
    }
    void* next(size_t alignOn = DEFAULT_ALIGN_ON) {
        return current ? &current->data[current->size - current->available] : nullptr;
    }

    // with spare memory add a memory pool. Do not forget to autorelease if memory is needed for something else!
    MemoryPool(size_t size, void* data);
    MemoryPool();
    MemoryPool(MemoryPool* from, size_t size);
    MemoryPool(MemoryPool& from, size_t size);
    ~MemoryPool();

    void clear();

    // true if memory is added (and therefore used)
    bool addMemory(void* data, size_t size);
    void addMemory(MemoryPool* mp);

    // some utility functions that uses memory
    void* memdup(const void* data, size_t length, size_t alignOn = DEFAULT_ALIGN_ON);
    char* strdup(const char* str);
    char* strndup(const char* str, size_t length);

    int printf(char** str, size_t max, const char* format, ...) __attribute__ ((format (printf, 4, 5)));
    int vprintf(char** str, size_t max, const char* format, va_list ap);
    // not terminating the string, useful for generating byte strings
    int printf_nt(char** str, size_t max, const char* format, ...) __attribute__ ((format (printf, 4, 5)));
    int vprintf_nt(char** str, size_t max, const char* format, va_list ap);

    // true if return succeeded (and the memory is used)
    bool returnUnused(char* buffer, size_t len);

    template <typename T>
    void delay(T&& delayedFunc, size_t extra = 0, void** extraPtr = nullptr) {
        MemoryPoolPageBase* p = NULL;
        DelayedItem<T>* obj = (DelayedItem<T>*)alloc(sizeof(DelayedItem<T>)+extra, DEFAULT_ALIGN_ON, &p);
        checkAssert(p != NULL);
        checkAssert(obj != NULL);
        new ((void*)obj) DelayedItem<T>(std::move(delayedFunc));
        memoryObjects.push_back(obj);
        if (extraPtr)
            *extraPtr = extra > 0 ? &obj->extra : nullptr;
    }
    template <typename T, typename Object>
    void* allocAndDelayOn(size_t size, const T& delayedFunc) {
        MemoryPoolPageBase* p = NULL;
        DelayedItem<T, Object*>* obj = (DelayedItem<T, Object*>*)alloc(sizeof(DelayedItem<T, Object*>) + size, DEFAULT_ALIGN_ON, &p);
        checkAssert(p != NULL);
        checkAssert(obj != NULL);
        Object* object = (Object*)&obj->extra[0];
        new((void*)obj) DelayedItem<T, Object*>(delayedFunc, object);
        memoryObjects.push_back(obj);
        return object;
    }

    template <class T>
    void retain(const shared_object<T>& ref) {
        new (*this, destroy<shared_object<T>>) shared_object<T>(ref);
    }

    void reserve(std::size_t chunkSize, size_t alignOn = DEFAULT_ALIGN_ON);

    static void vacuum();

    struct Any {};

    template <typename T = Any>
    class Allocator {
        struct Unused {
            Unused* next = nullptr;
            size_t bytes;
            Unused(size_t bytes) : bytes(bytes) {
            }
        };
        Stack<Unused*> unused;
      public:
        MemoryPool& mp;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T value_type;

        template <class U>
        struct rebind {
            typedef Allocator<U> other;
        };

        inline Allocator(MemoryPool& mp) : mp(mp) {
        }

        Allocator(const Allocator& copy) : mp(copy.mp) {
        }
        template <class U>
        inline Allocator(const Allocator<U>& mpa) : mp(mpa.mp) {
        }

        inline pointer address(reference x) const {
            return &x;
        }
        inline const_pointer address(const_reference x) const {
            return &x;
        }
        inline pointer allocate(size_type n) {
            size_t bytes = sizeof(value_type)*n;
            auto it = unused.begin();
            while (it != unused.end()) {
                if (it->bytes >= bytes)
                    return (pointer)it.erase();
                ++it;
            }
            pointer retval = (pointer)mp.alloc(bytes);
            return retval;
        }

        inline void deallocate(pointer p, size_type n) {
            deallocate((void*)p, sizeof(value_type)*n);
        }
        inline void deallocate(void* p, std::size_t bytes) {
            if (bytes >= sizeof(Unused) && (mp.rewinding || !mp.returnUnused((char*)p, bytes))) {
                unused.push_back(new(p) Unused(bytes));
            }
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

    Allocator<Any> allocator() {
        return allocator<Any>();
    }
  private:
    template <class T>
    Allocator<T> allocator() {
        return Allocator<T>(*this);
    }
    MemoryPoolPageBase* allocExtra();
};

template <class T, class U>
bool operator==(const MemoryPool::Allocator<T>& lhs, const MemoryPool::Allocator<U>& rhs) {
    return lhs.mp == rhs.mp;
}

template <class T, class U>
bool operator!=(const MemoryPool::Allocator<T>& lhs, const MemoryPool::Allocator<U>& rhs) {
    return lhs.mp != rhs.mp;
}

template <unsigned int SIZE>
class StaticMemoryPool : public MemoryPool {
    char buffer[SIZE+sizeof(MemoryPoolPageVariableSize)];
  public:
    StaticMemoryPool() : MemoryPool() {
        addMemory(buffer, SIZE+sizeof(MemoryPoolPageVariableSize));
    }
    void clear() {
        MemoryPool::clear();
        addMemory(buffer, SIZE+sizeof(MemoryPoolPageVariableSize));
    }
};

class EmbeddedMemoryPool : public MemoryPool {
    char* buffer;
    size_t size;
  public:
    EmbeddedMemoryPool(char* buffer, size_t size) : buffer(buffer), size(size) {
        addMemory(buffer, size);
    }
    EmbeddedMemoryPool(void* buffer, size_t size) : EmbeddedMemoryPool((char*)buffer, size) {
    }
    void clear() {
        MemoryPool::clear();
        addMemory(buffer, size);
    }
};

}
}

template <class T>
inline void* operator new(std::size_t nbytes, const bitpowder::lib::MemoryPool::Allocator<T>& alloc) {
    return alloc.mp->alloc(nbytes);
}

inline void* operator new(std::size_t nbytes, bitpowder::lib::MemoryPool& mp) {
    return mp.alloc(nbytes);
}

// do not rely on, as if the wrong order (!) of includes are used, MemoryPool* is cast to void* and a totally different semantic is used
void* operator new(std::size_t nbytes, bitpowder::lib::MemoryPool* mp) = delete;

template <class T>
inline void* operator new(std::size_t nbytes, bitpowder::lib::MemoryPool& mp, void (*f)(T*) = nullptr) {
    if (f == nullptr)
        return mp.alloc(nbytes);
    return mp.allocAndDelayOn<void (*)(T*), T>(nbytes, f);
}

// do not rely on, as if the wrong order (!) of includes are used, MemoryPool* is cast to void* and a totally different semantic is used
template <class T>
inline void* operator new(std::size_t nbytes, bitpowder::lib::MemoryPool* mp, void (*f)(T*) = nullptr) = delete;

/**
 * @brief Variable size allocations are supported
 *
 * Usages, with extra of std::size_t:
 * - new(extra) SomeType;
 * - new(extra, MemoryPool) SomeType;
 */

template <typename... Args>
void* operator new(std::size_t nbytes, std::size_t extra, Args&& ... args) {
    return operator new(nbytes+extra, std::forward<Args>(args)...);
}

template <typename... Args>
void operator delete(void* p, std::size_t extra, Args&& ... args) noexcept {
    return operator delete(p);
}

#endif
