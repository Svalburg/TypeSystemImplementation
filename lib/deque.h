/*/**
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

#ifndef _DEQUE_H_
#define _DEQUE_H_

#include "lib-common.h"
#include "exception.h"
#include "queue.h"

#include <memory>
#include <algorithm>
#include <iterator>
#include <assert.h>

namespace bitpowder {
namespace lib {

template <class T, class PtrContainer = typename std::remove_pointer<T>::type, T PtrContainer::*nextPtr = &PtrContainer::next, T PtrContainer::*prevPtr = &PtrContainer::prev>
class Deque {
    NO_COPY(Deque);
    T head;
    T tail;
  public:
    typedef T value_type;
    typedef typename std::remove_pointer<T>::type X;
    typedef Deque Container;

    typedef T reference;
    typedef const T const_reference;
    typedef size_t size_type;

    inline Deque(std::initializer_list<T> l) : head(nullptr), tail(nullptr) {
        for (auto e : l)
            push(e);
    }
    Deque() : head(nullptr), tail(nullptr) {
    }
    ~Deque() {
        clear();
    }
    inline Deque(Deque&& q) : head(std::move(q.head)), tail(std::move(q.tail)) {
        q.head = nullptr;
        q.tail = nullptr;
    }
    Deque& operator=(Deque&& q) {
        head = std::move(q.head);
        tail = std::move(q.tail);
        q.head = q.tail = nullptr;
        return *this;
    }

    void clear() {
        // if a smart pointer is used, a loop is needed to make sure all memory is deallocated (because items are double referenced by each other)
        if (std::is_pointer<T>::value) {
            head = nullptr;
            tail = nullptr;
        } else while (!empty())
                pop();
    }

    inline void push(const T& e) {
        push_back(e);
    }

    inline T pop() {
        return pop_front();
    }

    inline void unpop(const T& e) {
        push_front(e);
    }


    void swap(Deque& b) {
        using std::swap;
        swap(head, b.head);
        swap(tail, b.tail);
    }

    /*
    FIXME: Deque argument should have the pointer types swapped
    void swapReversed(Deque &reversed) {
        using std::swap;
        swap(reversed.head, tail);
        swap(reversed.tail, head);
    }
    */

    void push_front(const T& e) {
        if (!e)
            return;
        assert(e->*nextPtr == nullptr);
        assert(e->*prevPtr == nullptr);
        if (empty()) {
            tail = e;
            e->*nextPtr = nullptr;
        } else {
            head->*prevPtr = e;
            e->*nextPtr = head;
        }
        head = e;
    }
    void push_front(Deque& d) {
        if (d.empty())
            return;
        if (empty()) {
            swap(d);
            return;
        }
        head->*prevPtr = d.back();
        d.back()->*nextPtr = head;
        head = d.front();
        d.head = d.tail = nullptr;
    }
    void push_back(const T& e) {
        if (!e)
            return;
        assert(e->*nextPtr == nullptr);
        assert(e->*prevPtr == nullptr);
        if (empty()) {
            head = e;
            e->*prevPtr = nullptr;
        } else {
            tail->*nextPtr = e;
            e->*prevPtr = tail;
        }
        tail = e;
    }
    void push_back(Deque&& d) {
        if (d.empty())
            return;
        if (empty()) {
            swap(d);
            return;
        }
        tail->*nextPtr = d.front();
        d.front()->*prevPtr = tail;
        tail = d.back();
        d.head = d.tail = nullptr;
    }
    template <size_t N>
    void push_back_array(T (&d)[N]) {
        for (T& i : d)
            push_back(&i);
    }

    T pop_front() {
        T e = nullptr;
        std::swap(head, e);
        if (e) {
            std::swap(head, (T)e->*nextPtr);
            if (!head) {
                tail = nullptr;
            } else {
                head->*prevPtr = nullptr;
            }
            assert(e->*prevPtr == nullptr);
        }
        return e;
    }
    T pop_back() {
        T e = nullptr;
        std::swap(tail, e);
        if (e) {
            std::swap(tail, (T)e->*prevPtr);
            if (!tail) {
                head = nullptr;
            } else {
                tail->*nextPtr = nullptr;
            }
            assert(e->*nextPtr == nullptr);
        }
        return e;
    }

    inline Deque pop_all() {
        Deque retval;
        swap(retval);
        return retval;
    }

    // caution: if not in here, but in a similar datastructure, it removes the element from the similar datastructure
    void erase(const T& e) {
        //fprintf(stdout, "deque::erase; e=%p e->next=%p e->prev=%p head=%p tail=%p\n", &*e, &*(e->*nextPtr), &*(e->*prevPtr), &*head, &*tail);
        if (!e || (e->*prevPtr == nullptr && e->*nextPtr == nullptr && e != head))
            return;
        if (e == head) {
            assert(e->*prevPtr == nullptr);
            head = std::move(e->*nextPtr);
            if (head) {
                head->*prevPtr = nullptr;
            } else {
                assert(tail == e);
                tail = nullptr;
            }
        } else if (e == tail) {
            assert(e->*nextPtr == nullptr);
            tail = std::move(e->*prevPtr);
            if (tail) {
                tail->*nextPtr = nullptr;
            } else {
                assert(head == e);
                head = nullptr;
            }
        } else {
            assert(e->*prevPtr);
            assert(e->*nextPtr);
            (e->*nextPtr)->*prevPtr = e->*prevPtr;
            //std::swap((e->*nextPtr)->*prevPtr, e->*prevPtr);
            (e->*prevPtr)->*nextPtr = std::move(e->*nextPtr);
            //std::swap((e->*prevPtr)->*nextPtr, e->*nextPtr);
        }
        e->*prevPtr = nullptr;
        e->*nextPtr = nullptr;
        //fprintf(stderr, "after -> e: %p; head: %p; tail: %p\n", e, head, tail);
    }

    T front() {
        return head;
    }
    T back() {
        return tail;
    }
    const T front() const {
        return head;
    }
    const T back() const {
        return tail;
    }

    // information
    bool empty() const {
        return !head;
    }

    // iterators
    template <class Container = Deque>
    class _iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
        friend class Deque;
        Container* d;
        T value;
        bool increasing;
      public:
        _iterator(Container* d, const T& value) : d(d), value(value), increasing(true) {
        }
        _iterator(const _iterator& it) : d(it.d), value(it.value), increasing(it.increasing) {
        }
        T operator*() const {
            return value;
        }
        T operator->() const {
            return value;
        }
        _iterator& operator++() {
            if (value)
                value = next(value);
            else if (!increasing)
                value = &*d->head;
            increasing = true;
            return *this;
        }
        _iterator& operator--() {
            if (value)
                value = prev(value);
            else if (increasing)
                value = &*d->tail;
            increasing = false;
            return *this;
        }
        bool operator==(const _iterator& b) const {
            return value == b.value;
        }
        bool operator!=(const _iterator& b) const {
            return value != b.value;
        }
        operator bool() const {
            return value;
        }

        T erase() {
            T retval = value;
            value = next(value);
            d->erase(retval);
            return retval;
        }

        inline void insert(T a) {
            d->insert(value, a);
        }
    };
    template <class Container = Deque>
    class _riterator : public _iterator<Container> {
      public:
        _riterator(Container* d, const T& value) : _iterator<Container>(d, value) {
        }
        _riterator(const _iterator<Container>& value) : _iterator<Container>(value) {
        }
        _riterator& operator++() {
            _iterator<Container>::operator--();
            return *this;
        }
        _riterator& operator--() {
            _iterator<Container>::operator++();
            return *this;
        }
    };

    typedef _iterator<Container> iterator;
    typedef _iterator<const Container> const_iterator;

    const_iterator begin() const {
        return const_iterator(this, head);
    }
    const_iterator end() const {
        return ++const_iterator(this, tail);
    }

    iterator begin() {
        return iterator(this, &*head);
    }
    iterator end() {
        return ++iterator(this, &*tail);
    }

    typedef _riterator<Container> reverse_iterator;
    typedef _riterator<const Container> const_reverse_iterator;
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(this, tail);
    }
    const_reverse_iterator rend() const {
        return ++const_reverse_iterator(this, head);
    }
    reverse_iterator rbegin() {
        return reverse_iterator(this, &*tail);
    }
    reverse_iterator rend() {
        return ++reverse_iterator(this, &*head);
    }

    void insert(iterator it, const T& b) {
        T c = *it;
        insert(c, b);
    }

    template <class Func>
    void insertAt(X* object, Func&& f) {
        for (auto it : *this)
            if (f(it)) {
                insert(it, object);
                return;
            }
        push_back(object);
    }

    // insert before
    void insert(const T& pos, const T& b) {
        if (!b)
            return;
        if (!pos) {
            push_back(b);
            return;
        }

        b->*prevPtr = b;
        std::swap(b->*prevPtr, pos->*prevPtr);

        b->*nextPtr = b;
        if (b->*prevPtr) {
            assert(pos != head);
            // previous exists
            std::swap(b->*nextPtr, (b->*prevPtr)->*nextPtr);
        } else {
            assert(pos == head);
            // previous did not exists
            std::swap(b->*nextPtr, head);
        }
    }

    void insert(const iterator& it, Deque&& b) {
        T c = *it;
        insert(c, std::move(b));
    }

    void insert(const T& pos, Deque&& b) {
        if (b.empty())
            return;
        if (!pos) {
            push_back(std::move(b));
            return;
        }
        T a = pos->*prevPtr;
        if (a) {
            checkAssert(pos != head);
            a->*nextPtr = b.front();
        } else {
            checkAssert(pos == head);
            head = b.front();
        }
        b.front()->*prevPtr = a;
        b.back()->*nextPtr = pos;
        pos->*prevPtr = b.back();
        b.head = b.tail = nullptr;
    }

    static T next(const T& c) {
        return c ? c->*nextPtr : nullptr;
    }

    static T prev(const T& c) {
        return c ? c->*prevPtr : nullptr;
    }

    // including
    Deque split(const T& pos) {
        T retvalHead = nullptr;
        T retvalTail = nullptr;
        if (!empty() && pos != nullptr) {
            retvalHead = std::move(head);
            head = pos->*nextPtr;
            pos->*nextPtr = nullptr;
            if (head)
                head->*prevPtr = nullptr;
            retvalTail = pos;
        }
        Deque retval;
        retval.head = std::move(retvalHead);
        retval.tail = std::move(retvalTail);
        return retval;
    }

    template <class P>
    auto select(P&& func) const -> select_container<typename std::result_of<P(T)>::type::second_type, const Container, P> {
        return {*this, std::move(func)};
    }

    template <class P>
    auto select(P&& func) -> select_container<typename std::result_of<P(T)>::type::second_type, Container, P> {
        return {*this, std::move(func)};
    }

    template <class P>
    inline auto sbegin(P&& func) const -> select_iterator<typename std::result_of<P(T)>::type::second_type,P,decltype(this->begin())> {
        return {begin(), std::move(func)};
    }
    template <class P>
    inline auto sbegin(P&& func) -> select_iterator<typename std::result_of<P(T)>::type::second_type,P,decltype(this->begin())> {
        return {begin(), std::move(func)};
    }

    template <class P>
    inline int erase_if(P func) {
        int retval = 0;
        for (auto it = sbegin([&func](T e) {
        return std::make_pair(func(e), e);
        }); it; it.erase(), ++retval);
        return retval;
    }

    template <class P>
    inline auto first(P func) const -> decltype(*this->template select(std::ref(func)).begin()) {
        for (auto e : select(std::ref(func)))
            return e;
        return nullptr;
    }
};

}
}

namespace std {
    template <class T, class PtrContainer, T PtrContainer::*nextPtr, T PtrContainer::*prevPtr>
    void swap(bitpowder::lib::Deque<T, PtrContainer, nextPtr, prevPtr>& lhs, bitpowder::lib::Deque<T, PtrContainer, nextPtr, prevPtr>& rhs) {
        lhs.swap(rhs);
    }
}

#endif

