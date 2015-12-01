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

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "lib-common.h"
#include "exception.h"
#include "assert.h"
#include <memory>
#include <iterator>
#include <type_traits>
#include "iterators.h"

namespace bitpowder {
namespace lib {

template <class T, class PtrContainer, T PtrContainer::*ptr> class Stack;

template <class T, class PtrContainer = typename std::remove_pointer<T>::type, T PtrContainer::*ptr = &PtrContainer::next >
class Queue {
    NO_COPY(Queue);
  public:
    typedef T value_type;
    typedef typename std::remove_pointer<T>::type X;
    friend class Stack<T, PtrContainer, ptr>;
    typedef Queue Container;
  private:
    T head = nullptr;
    T tail = nullptr;
  public:
    inline Queue(std::initializer_list<T> l) {
        for (auto& e : l)
            push(e);
    }
    inline Queue() {
    }
    inline ~Queue() {
        clear();
    }
    inline Queue(Queue&& q) : head(std::move(q.head)), tail(std::move(q.tail)) {
        q.head = nullptr;
        q.tail = nullptr;
    }
    Queue& operator=(Queue&& q) {
        head = std::move(q.head);
        tail = std::move(q.tail);
        q.head = q.tail = nullptr;
        return *this;
    }
    inline void clear() {
        head = nullptr;
        tail = nullptr;
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

    inline void push_front(const T& e) {
        if (!e)
            return;
        assert(!(bool)(e->*ptr));
        e->*ptr = head;
        if (empty())
            tail = e;
        head = e;
    }

    inline void push_back(const T& e) {
        if (!e)
            return;
        assert(!(bool)(e->*ptr));
        e->*ptr = nullptr;
        if (empty()) {
            head = e;
        } else {
            tail->*ptr = e;
        }
        tail = e;
    }

    inline T pop_front() {
        T e = std::move(head);
        if (e) {
            head = std::move((T)e->*ptr);
            e->*ptr = nullptr;
            if (!head)
                tail = nullptr;
        }
        return e;
    }

    inline Queue pop_all() {
        return std::move(*this);
    }

    inline T front() {
        return head;
    }
    inline T back() {
        return tail;
    }
    inline T front() const {
        return head;
    }
    inline T back() const {
        return tail;
    }

    inline bool push_back(T&& first, T&& last) {
        if (head == first || !first || !last)
            return false;
        (empty() ? head : tail->*ptr) = std::move(first);
        tail = std::move(last);
        return true;
    }

    inline bool push_back(Queue&& c) {
        bool retval = push_back(std::move(c.head), std::move(c.tail));
        c.head = nullptr;
        c.tail = nullptr;
        return retval;
    }

    inline bool push_back(const T& first, const T& last) {
        if (head == first || !first || !last)
            return false;
        (empty() ? head : tail->*ptr) = first;
        tail = last;
        return true;
    }
    /*
    template <class C>
    inline bool push_back(C& c) {
        bool retval = push_back(c.front(), c.back());
        if (retval)
            c.clear();
        return retval;
    }
    */

    inline void erase(const T& e) {
        if (!e || empty())
            return;
        if (head == e) {
            pop();
            return;
        }
        // it is not the first item, and there are items
        T current = head;
        while (current->*ptr && current->*ptr != e) {
            current = current->*ptr;
        }
        if (current->*ptr != nullptr) { // therefore we know current->*ptr == e, but this check is cheaper
            current->*ptr = std::move(e->*ptr);
            e->*ptr = nullptr;
            if (tail == e)
                tail = std::move(current);
        }
    }

    // information
    inline bool empty() const {
        return !(bool)head;
    }

    inline bool multipleItems() const {
        return !empty() && head != tail;
    }

    template <class Self, class PtrType>
    class iterator_base : public std::iterator<std::bidirectional_iterator_tag, X, unsigned int, PtrType> {
      protected:
        PtrType* value;
      public:
        iterator_base(PtrType* value) : value(value) {
        }
        iterator_base(const iterator_base& a) : value(a.value) {
        }
        iterator_base(iterator_base&& a) : value(nullptr) {
            std::swap(a.value, value);
        }
        ~iterator_base() {
        }
        inline T operator*() {
            return value ? *value : nullptr;
        }
        inline T operator->() {
            return value ? *value : nullptr;
        }
        inline Self operator+(int i) {
            Self retval = *static_cast<Self*>(this);
            while (retval && i-- > 0)
                ++retval;
            return retval;
        }
        inline bool operator!=(const iterator_base& b) const {
            return !(*this == b);
        }
        inline bool operator==(const iterator_base& b) const {
            if (value == b.value)
                return true;
            X* thisPointsTo = value && *value ? &** value : nullptr;
            X* bPointsTo = b.value && *b.value ? &** b.value : nullptr;
            return thisPointsTo == bPointsTo;
        }
        inline operator bool() const {
            return value && *value;
        }
    };
    class const_iterator : public iterator_base<const_iterator,const T> {
      public:
        const_iterator(const T* value) : iterator_base<const_iterator,const T>(value) {
        }
        const_iterator(const const_iterator& a) : iterator_base<const_iterator,const T>(a) {
        }
        const_iterator(const_iterator&& a) : iterator_base<const_iterator,const T>(std::move(a)) {
        }
        inline void operator++() {
            this->value = this->value && *this->value ? &((*this->value)->*ptr) : nullptr;
        }
    };

    class iterator : public iterator_base<iterator,T> {
        Queue& q;
        T prevItem; // needed for setting 'tail' to the correct value
      public:
        iterator(T* value, Queue& q, T prevItem = nullptr) : iterator_base<iterator,T>(value), q(q), prevItem(prevItem) {
        }
        iterator(const iterator& a) : iterator_base<iterator,T>(a), q(a.q), prevItem(a.prevItem) {
        }
        iterator(iterator&& a) : iterator_base<iterator,T>(std::move(a)), q(a.q), prevItem(nullptr) {
            std::swap(prevItem, a.prevItem);
        }
        inline void operator++() {
            prevItem = this->value && *this->value ? *this->value : nullptr;
            this->value = this->value && *this->value ? &((*this->value)->*ptr) : nullptr;
        }
        inline T erase() {
            if (!this->value || !*this->value)
                return nullptr;
            T retval = std::move(*this->value);
            if (retval) {
                *this->value = std::move(retval->*ptr);
                retval->*ptr = nullptr;
                if (*this->value == nullptr) { // alternative way of specifying q.tail == retval (but more defensive)
                    q.tail = std::move(prevItem);
                    prevItem = nullptr;
                }
            }
            return retval;
        }
        // insert before current item, but also progressing, so current item stays the same
        inline void insert(const T& a) {
            if (!this->value || !*this->value)
                return;
            if (*this->value == nullptr) {
                q.tail = a;
            }
            a->*ptr = std::move(*this->value);
            *this->value = a;
            operator++();
        }
    };


    inline iterator begin() {
        return {&head, *this};
    }
    inline iterator end() {
        return {nullptr, *this};
    }

    inline const_iterator begin() const {
        return {&head};
    }
    inline const_iterator end() const {
        return {nullptr};
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
    template <class T, class PtrContainer, T PtrContainer::*nextPtr>
    void swap(bitpowder::lib::Queue<T, PtrContainer, nextPtr>& lhs, bitpowder::lib::Queue<T, PtrContainer, nextPtr>& rhs) {
        std::swap(lhs.head, rhs.head);
        std::swap(lhs.tail, rhs.tail);
    }
}

#endif

