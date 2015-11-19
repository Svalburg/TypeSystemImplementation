/**
Copyright 2014-2015 Bernard van Gastel, bvgastel@bitpowder.com.
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

#ifndef SHARED_OBJECT_H
#define SHARED_OBJECT_H

#include "exception.h"
#include <atomic>
#include <iostream>

/**
  * shared_object is a smart pointer using storage of the object pointed to, so it has minimal overhead.
  *
  * std::shared_ptr and shared_object are functional more or less equivalent. std::shared_ptr uses
  * two pointers: one to the control block and one to the data that gets pointed to. This makes all
  * the pointers double in size compared to shared_object.
  *
  * The aliasing std::shared_ptr functionality can be achieved with shared_object by using the
  * count_proxy facility. This adds an extra pointer field to the object that is referenced (but
  * shared_objects are half the size of std::shared_ptr).
  *
  * Because shared_objects use embedded reference counters, objects can be of dynamic size without
  * requiring additional allocations (std::shared_ptr can do the same, although the control block
  * needs to be allocated separatly).
  *
  * Depending on the needs, shared_object lets you choose the type of the reference counter. This
  * can be a concurrent refernece counter or just a plain 'int'.
  *
  * The default manner of std::shared_ptr to use std::make_shared to allocate at once can not be used
  * with CacheInstances, as only instances are cached, not control blocks. It also does not work with
  * the 'reserve' behaviour of CacheInstances.
  *
  * A major drawback of shared_object is that its use has the be properly envisioned during design
  * of the classes. It can not be added afterwards like std::shared_ptr.
  */
namespace bitpowder {
namespace lib {

class concurrent_count {
    std::atomic<int> refcount;
  public:
    concurrent_count(int refcount = 0) : refcount(refcount) {
    }
    concurrent_count(const concurrent_count& copy) : refcount(0) {
    }

    // prefix
    int operator++() {
        return refcount.fetch_add(1, std::memory_order_relaxed) + 1;
    }

    // postfix
    int operator++(int) {
        return refcount.fetch_add(1, std::memory_order_relaxed);
    }

    int operator+=(int value) {
        return refcount.fetch_add(value, std::memory_order_relaxed) + value;
    }

    // prefix
    int operator--() {
        return refcount.fetch_sub(1, std::memory_order_relaxed) - 1;
    }

    // postfix
    int operator--(int) {
        return refcount.fetch_sub(1, std::memory_order_relaxed);
    }

    int operator-=(int value) {
        return refcount.fetch_sub(value, std::memory_order_relaxed) - value;
    }

    operator int() {
        return refcount;
    }
};

template <class Action>
class ref_count_with_action {
    std::atomic<int> refcount;
    Action action;
  public:
    ref_count_with_action(Action&& action) : refcount(0), action(std::forward<Action>(action)) {
    }

    void operator++(int) {
        refcount++;
    }

    int operator--(int) {
        int count = refcount--;
        if (count == 1)
            action();
        return count;
    }

    operator int() {
        return refcount;
    }
};

template <class Type>
class count_proxy {
    Type* refcount;
  public:
    count_proxy(Type& refcount) : refcount(&refcount) {
    }
    count_proxy() : refcount(nullptr) {
    }
    int operator++(int) {
        checkAssert(refcount);
        return refcount ? int((*refcount)++) : 0;
    }

    int operator--(int) {
        checkAssert(refcount);
        return refcount ? int((*refcount)--) : 0;
    }

    int operator++() {
        checkAssert(refcount);
        return refcount ? int(++(*refcount)) : 0;
    }

    int operator--() {
        checkAssert(refcount);
        return refcount ? int(--(*refcount)) : 0;
    }

    operator int() {
        checkAssert(refcount);
        return refcount ? int(*refcount) : 0;
    }
};

template <class Object, class Base = Object, class RefCountType = decltype(Base::refcount), RefCountType Base::*ptr = &Base::refcount, void (*deleter)(Object*) = &performDelete<Object>>
class shared_object {
    Object* object = nullptr;
    template <class OtherObject, class OtherBase, class OtherRefCountType, OtherRefCountType OtherBase::*otherPtr, void (*otherDeleter)(OtherObject*)>
    friend class shared_object;
  public:
    shared_object() {
    }
    shared_object(Object* object) : object(object) {
        if (object != nullptr)
            (((Base*)object)->*ptr)++;
    }
    ~shared_object() {
        if (object != nullptr && (((Base*)object)->*ptr)-- == 1) {
            deleter(object);
            object = nullptr;
        }
    }
    template <class OtherObject, class OtherBase, class OtherRefCountType, OtherRefCountType OtherBase::*otherPtr = &OtherBase::refcount, void (*otherDeleter)(OtherObject*) = &performDelete<OtherObject>>
    shared_object(const shared_object<OtherObject, OtherBase, OtherRefCountType, otherPtr, otherDeleter>& o) : object(static_cast<Object*>(o.get())) {
        if (object != nullptr)
            (((Base*)object)->*ptr)++;
    }
    shared_object(const shared_object& o) : object(o.object) {
        if (object != nullptr)
            (((Base*)object)->*ptr)++;
    }
    shared_object(shared_object&& o) : object(std::move(o.object)) {
        o.object = nullptr;
    }

    shared_object& operator=(shared_object&& o) {
        if (this == &o)
            return *this;
        if (object && (((Base*)object)->*ptr)-- == 1)
            deleter(object);
        object = std::move(o.object);
        o.object = nullptr;
        return *this;
    }
    shared_object& operator=(const shared_object& o) {
        if (object && (((Base*)object)->*ptr)-- == 1)
            deleter(object);
        object = o.object;
        if (object)
            (((Base*)object)->*ptr)++;
        return *this;
    }
    operator Object* () const {
        return object;
    }
    Object& operator*() const {
        return *object;
    }
    Object* operator->() const {
        return object;
    }
    explicit operator bool() const {
        return object != nullptr;
    }
    bool operator==(const shared_object& o) const {
        return object == o.object;
    }
    bool operator==(const std::nullptr_t& o) const {
        return object == nullptr;
    }

    bool operator!=(const shared_object& o) const {
        return !(*this == o);
    }
    bool operator!=(const std::nullptr_t& o) const {
        return object != nullptr;
    }

    bool empty() const {
        return object == nullptr;
    }
    Object* get() const {
        return object;
    }
};

#define REFERENCE_COUNTED_INTERFACE(T) \
    lib::count_proxy<lib::concurrent_count> refcount; \
    typedef lib::shared_object<T> Ref; \
    T(lib::concurrent_count& refcount) : refcount(refcount) { \
    } \
    virtual ~T() { \
    }

class shared_object_owner_less {
  public:
    template <class Object, class Base, class RefCountType, RefCountType Base::*ptr, void (*deleter)(Object*)>
    bool operator()(const shared_object<Object,Base,RefCountType,ptr,deleter>& lhs, const shared_object<Object,Base,RefCountType,ptr,deleter>& rhs) const {
        if (!(bool)lhs || !(bool)rhs)
            return (bool)rhs;
        return *lhs < *rhs;
    }

    template <class Object, class Base, class RefCountType, RefCountType Base::*ptr, void (*deleter)(Object*), class T>
    bool operator()(const shared_object<Object,Base,RefCountType,ptr,deleter>& lhs, const T& rhs) const {
        return !(bool)lhs || T(*lhs) < rhs;
    }

    template <class T, class Object, class Base, class RefCountType, RefCountType Base::*ptr, void (*deleter)(Object*)>
    bool operator()(const T& lhs, const shared_object<Object,Base,RefCountType,ptr,deleter>& rhs) const {
        return !(bool)rhs || lhs < T(*rhs);
    }
    typedef void is_transparent;
};

}
}

namespace std {

template <class Object, class Base, class RefCountType, RefCountType Base::*ptr, void (*deleter)(Object*)>
void swap(bitpowder::lib::shared_object<Object, Base, RefCountType, *ptr, *deleter>& lhs, bitpowder::lib::shared_object<Object, Base, RefCountType, *ptr, *deleter>& rhs) {
    std::swap(lhs.object, rhs.object);
}

template <class Object, class Base, class RefCountType, RefCountType Base::*ptr, void (*deleter)(Object*)>
class remove_pointer<bitpowder::lib::shared_object<Object,Base,RefCountType,ptr,deleter>> {
  public:
    typedef Object type;
};

template <class Object, class Base, class RefCountType, RefCountType Base::*ptr, void (*deleter)(Object*)>
struct owner_less<bitpowder::lib::shared_object<Object,Base,RefCountType,ptr,deleter>> {
    bool operator()(const bitpowder::lib::shared_object<Object,Base,RefCountType,ptr,deleter>& lhs, const bitpowder::lib::shared_object<Object,Base,RefCountType,ptr,deleter>& rhs) const {
        if (!(bool)lhs || !(bool)rhs)
            return (bool)rhs;
        return *lhs < *rhs;
    }
};

}

#endif // SHARED_OBJECT_H
