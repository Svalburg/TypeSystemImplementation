/*****
 * MemoryPool Debug Version
 * - with delayed function execution
 * - fast allocation of temporary memory
 * - fast saving and restoring of the current state
 *
 * TODO:
 * - make an indirection for pthread specific storage, an struct which get updated, with two members: currentMemoryPool and newPool
 * - also give the current value of the pthread specific storage to every function, it is faster (the function to get the value gets executed multiple times now)
 */

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
#include "stack.h"

#define DEFAULT_SIZE 1024
#define ALIGN_ON sizeof(void*)

namespace bitpowder {
namespace lib {

struct DelayedItemBase {
    DelayedItemBase* next;
    DelayedItemBase() : next(nullptr) {}
    virtual void execute() = 0;
    static void executeOn(void* _base) {
        DelayedItemBase* base = (DelayedItemBase*)_base;
        base->execute();
    }
};

template <typename T>
struct DelayedItem : public DelayedItemBase {
    T func;
    void* extra[0];
    DelayedItem(T&& func) : func(std::move(func)) {
    }
    void execute() {
        func();
    }
};

template <typename T, typename S>
struct DelayedItemObject : public DelayedItemBase {
    T t;
    S s;
    void* extra[0];
    DelayedItemObject(const T& t , const S& s) : t(t), s(s) {
    }
    void execute() {
        t(s);
    }
};


typedef void (DelayedFunc)(void* data);

class MemoryPoolItem {
    DelayedFunc* func = nullptr;
    void* data = nullptr;
  public:
    MemoryPoolItem* next = nullptr;
    MemoryPoolItem* prev = nullptr;

    friend class MemoryPool;
};

class MemoryPoolStatus {
    MemoryPoolItem* delayed = nullptr;

  public:
    MemoryPoolStatus(MemoryPoolItem* delayed = nullptr) : delayed(delayed) {
    }

    friend class MemoryPool;
};

class MemoryPool {
    MemoryPool(const MemoryPool&);
    MemoryPool& operator=(const MemoryPool&);
    Stack<MemoryPoolItem*> delayed;
  public:
    // alloc memory from the current memory pool (alloc new one if there is not enough memory). Only usable in the current autorelease pool
    void* alloc(size_t size, char alignOn = ALIGN_ON, MemoryPoolItem** pool = NULL);

    void* allocAll(size_t& size, char alignOn = ALIGN_ON);
    void* calloc(size_t size, char alignOn = ALIGN_ON);

    // do not let memory allocations after this one cross into the next memory pool, restore after with mprestore. Should be done when allocatin a new autoreleasepool
    MemoryPoolStatus status();
    void restore(MemoryPoolStatus status);

    void returnUnused(char* buffer, size_t len) {
    }
    void* next(char alignOn = ALIGN_ON) {
        return nullptr;
    }

    size_t available() {
        return DEFAULT_SIZE;
    }

    void reserve(size_t size);

    // with spare memory add a memory pool. Do not forget to autorelease if memory is needed for something else!
    MemoryPool(void* data, size_t size);
    MemoryPool();
    MemoryPool(MemoryPool* from, size_t size);
    MemoryPool(MemoryPool& from, size_t size);
    ~MemoryPool();

    void clear();

    void addMemory(void* data, size_t size);
    void addMemory(MemoryPool* mp);

    // some utility functions that uses memory
    void* memdup(const void* data, size_t length, char alignOn);
    char* strdup(const char* str);
    char* strndup(const char* str, size_t length);
    int printf(char** str, size_t max, const char* format, ...) __attribute__ ((format (printf, 4, 5)));
    int vprintf(char** str, size_t max, const char* format, va_list ap);
    // not terminating the string, useful for generating byte strings
    int printf_nt(char** str, size_t max, const char* format, ...) __attribute__ ((format (printf, 4, 5)));
    int vprintf_nt(char** str, size_t max, const char* format, va_list ap);

    void delay(DelayedFunc* func, void* object) {
        MemoryPoolItem* pool = new(malloc(sizeof(MemoryPoolItem))) MemoryPoolItem();
        checkAssert(pool);
        pool->func = func;
        pool->data = object;
        delayed.push_back(pool);
    }
    /*
           void *allocAndDelayOn(size_t size, DelayedFunc *func) {
           void *data = alloc(size);
           delay(func, data);
           return data;
           }
         */
    template <typename T, typename Object>
    void* allocAndDelayOn(size_t size, T delayedFunc) {
        MemoryPoolItem* p = NULL;
        DelayedItemObject<T, Object*>* obj = (DelayedItemObject<T, Object*>*)alloc(sizeof(DelayedItemObject<T, Object*>) /*+ size*/, ALIGN_ON, &p);
        checkAssert(p != NULL);
        //Object *object = (Object*)&obj->extra[0];
        Object* object = (Object*)alloc(size);
        new((void*)obj) DelayedItemObject<T, Object*>(delayedFunc, object);
        //std::cerr << "type " << typeid(T).name() << " on object " << typeid(Object).name() << "@" << object << std::endl;
        delay(DelayedItemBase::executeOn, obj);
        return object;
    }

    template <class T, void (*func)(T*)>
    inline void delay(T* obj) {
        delay((DelayedFunc*)func, obj);
    }
    template <class T, void (T::*method)()>
    inline void delay(T* obj) {
        DelayedFunc* func = (DelayedFunc*)(void (*)(T*))&invoke<T, method>;
        delay(func, obj);
    }

    static void vacuum();

    template <typename T>
    class Allocator {
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

        template <class U>
        inline Allocator(const Allocator<U>& mpa) : mp(mpa.mp) {
        }

        inline pointer address(reference x) const {
            return &x;
        }
        inline const_pointer address(const_reference x) const {
            return &x;
        }
        inline pointer allocate(size_type size, const void* hint = 0) {
            return (pointer)mp.calloc(sizeof(T)*size);
        }

        inline void deallocate(pointer p, size_type n) {
        }
        inline void deallocate(void* p, size_type n) {
        }
        inline size_type max_size() const {
            //return mp->available() / sizeof(value_type);
            return std::numeric_limits<size_type>::max();
        }
        void construct(pointer p,  const_reference val) {
            new(reinterpret_cast<void*>(p)) T(val);
        }
        template<class U, typename... Args>
        void construct(U* p,  Args&& ... args) {
            new((void*)p) U(std::forward<Args>(args)...);
        }
        inline void construct(pointer p) {
            new(reinterpret_cast<void*>(p)) T();
        }
        inline void destroy(pointer p) {
            p->T::~T();
        }
    };

    struct Any {};
    Allocator<Any> allocator() {
        return allocator<Any>();
    }
  private:
    template <class T>
    Allocator<T> allocator() {
        return Allocator<T>(*this);
    }

  private:
    void allocExtra();
};

template <unsigned int SIZE>
class StaticMemoryPool : public MemoryPool {
  public:
    StaticMemoryPool() {
    }
    operator MemoryPool* () {
        return this;
    }
    MemoryPool* operator*() {
        return this;
    }
    MemoryPool* operator->() {
        return this;
    }
};

class EmbeddedMemoryPool : public MemoryPool {
  public:
    EmbeddedMemoryPool(char* buffer, size_t size) {
    }
    operator MemoryPool* () {
        return this;
    }
    MemoryPool* operator*() {
        return this;
    }
    MemoryPool* operator->() {
        return this;
    }
    void clear() {
        MemoryPool::clear();
    }
};

}
}


template <class T>
inline void* operator new(size_t nbytes, bitpowder::lib::MemoryPool::Allocator<T> alloc) {
    return alloc.mp->alloc(nbytes);
}

inline void* operator new(size_t nbytes, bitpowder::lib::MemoryPool& mp) {
    return mp.alloc(nbytes);
}

// do not rely on, as if the wrong order (!) of includes are used, MemoryPool* is cast to void* and a totally different semantic is used
void* operator new(std::size_t nbytes, bitpowder::lib::MemoryPool* mp) = delete;

template <class T>
inline void* operator new(std::size_t nbytes, bitpowder::lib::MemoryPool& mp, void (*f)(T*) = nullptr) {
    return  mp.allocAndDelayOn<void (*)(T*), T>(nbytes, f); //(bitpowder::lib::DelayedFunc*)f);
}

// do not rely on, as if the wrong order (!) of includes are used, MemoryPool* is cast to void* and a totally different semantic is used
template <class T>
inline void* operator new(std::size_t nbytes, bitpowder::lib::MemoryPool* mp, void (*f)(T*) = nullptr) = delete;


/**
 *  * @brief Variable size allocations are supported
 *   *
 *    * Usages, with extra of std::size_t:
 *     * - new(extra) SomeType;
 *      * - new(extra, MemoryPool) SomeType;
 *       */

template <typename... Args>
void* operator new(std::size_t nbytes, std::size_t extra, Args&& ... args) {
    return operator new(nbytes+extra, std::forward<Args>(args)...);
}

template <typename... Args>
void operator delete(void* p, std::size_t extra, Args&& ... args) noexcept {
    return operator delete(p);
}

#endif

