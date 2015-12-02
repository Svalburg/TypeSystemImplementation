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
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _COMMON_H_
#define _COMMON_H_

#define BP_MAX(a, b) ((a) > (b) ? (a) : (b))
#define BP_MIN(a, b) ((a) < (b) ? (a) : (b))

#ifndef MAX
#define MAX BP_MAX
#endif
#ifndef MIN
#define MIN BP_MIN
#endif

extern "C" {
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <strings.h>

#if defined(__linux__)
#  include <endian.h>
#  define ntohll(x) be64toh(x)
#  define htonll(x) htobe64(x)
#elif defined(__FreeBSD__) || defined(__NetBSD__)
#  include <sys/endian.h>
#  define ntohll(x) be64toh(x)
#  define htonll(x) htobe64(x)
#elif defined(__OpenBSD__)
#  include <sys/types.h>
#  define ntohll(x) betoh64(x)
#  define htonll(x) htobe64(x)
#elif defined(__APPLE__)
#  include <arpa/inet.h>
#elif defined(_WIN32) || defined(__CYGWIN__)
#  if BYTE_ORDER == LITTLE_ENDIAN
#    define ntohs(x) __builtin_bswap16(x)
#    define htons(x) __builtin_bswap16(x)
#    define ntohl(x) __builtin_bswap32(x)
#    define htonl(x) __builtin_bswap32(x)
#    define ntohll(x) __builtin_bswap64(x)
#    define htonll(x) __builtin_bswap64(x)
#  else
#    define ntohs(x) (x)
#    define htons(x) (x)
#    define ntohl(x) (x)
#    define htonl(x) (x)
#    define ntohll(x) (x)
#    define htonll(x) (x)
#  endif
#else
#  warning "no 64-bits ntoh/hton byte operations"
#endif

#ifndef __WORDSIZE
#  if _WIN32 || _WIN64
#    if _WIN64
#      define __WORDSIZE 64
#    else
#      define __WORDSIZE 32
#    endif
#  endif
#endif

}
#include <memory>
#include <functional>
#include <utility>
#include <type_traits>
#include <algorithm>
#include <tuple>

namespace bitpowder {
namespace lib {

#if (__cplusplus <= 199711L) && !defined(__GXX_EXPERIMENTAL_CXX0X__)
#define nullptr 0
#endif

//#undef offsetof
//#define offsetof offset_of
//#define offsetof(type, attribute) ((size_t)&(((type*)0x10)->attribute) - 0x10)

//#define EXTENDED_DEBUG
#ifdef EXTENDED_DEBUG
//#define eprint(msg) ::fprintf(stderr, "[%s:%i] %s\n", __FILE__, __LINE__, msg)
#define eprintf(format, ...) ::fprintf(stderr, "[%s:%i] " format "\n", __FILE__, __LINE__, ## __VA_ARGS__)
#define CRASH {*((int*)0x02) = 1;}
#else
//#define eprint(...)
#define eprintf(...)
#define CRASH
#endif

//#if (__cplusplus <= 199711L) && !defined(__GXX_EXPERIMENTAL_CXX0X__)
//#define override
//#endif

// invoke is alternative to mem_fun, because this is statically, so it interfaces better with C (e.g. thread.cpp / pthread_create).
// For the same reason uses less memory (8 bytes instead of 16 bytes of a std::mem_fn)
template <class T, void (T::*method)()>
inline void invoke() {
    T::method();
}

template <class T, void (T::*method)()>
inline void invoke(T* object) {
    (object->*method)();
}

template <class T, class A, void (T::*method)(A)>
inline void invoke(T* object, A arg) {
    (object->*method)(arg);
}

template <class T, class A, class B, void (T::*method)(A, B)>
inline void invoke(T* object, A arg, B arg2) {
    (object->*method)(arg, arg2);
}

template <class T, class A, class B, class C, void (T::*method)(A, B, C)>
inline void invoke(T object, A arg, B arg2, C arg3) {
    (object->*method)(arg, arg2, arg3);
}

template <class T>
inline void destroy(T* object) {
    object->~T();
}

template <class T>
inline void performDelete(T* object) {
    delete object;
}

template <class T, class... Args>
inline void nothing(T, Args...) {
}

template <class T, class A>
inline void deleteSecond(T, A* snd) {
    delete snd;
}

#define NO_COPY(Class) \
    Class(const Class&) = delete; \
    Class& operator=(const Class&) = delete

// usage for in container classes
// usage: Test *bla = _new(mp->allocator<Test>(), Test(42));
template <class Alloc, class T> inline T* _new(Alloc a, const T& val = T()) {
    T* obj = a.allocate(1);
    a.construct(obj, val);
    return obj;
}

template <class T> inline T* _new(const T& val = T()) {
    std::allocator<T> a;
    T* obj = a.allocate(1);
    a.construct(obj, val);
    return obj;
}

template <class T>
class LinkedList {
    T content;
  public:
    LinkedList<T>* next;

    LinkedList() : content(), next(NULL) {
    }
    LinkedList(const T& c) : content(c), next(NULL) {
    }
    /*
    LinkedList(const LinkedList<T> &c) : content(c.content), next(nullptr) {
        //next = c.next ? new LinkedList<T>(*c.next) : nullptr;
    }
    */
    LinkedList(const LinkedList<T>& c) {
        content = c ? c->content : T();
        next = nullptr;
        //next = c && c->next ? new LinkedList<T>(c->next) : nullptr;
    }
    //LinkedList(T c) : next(NULL), content(c) {};
    virtual ~LinkedList() {
        /*
        // can be used to minimise stack usage
        LinkedList<T> *current = next;
        while (current) {
            LinkedList<T> *next = current->next;
            current->next = NULL;
            delete current;
            current = next;
        }
        */
    }
    inline T* operator->() {
        return &content;
    }
    inline T& operator*() {
        return content;
    }
    inline T* get() {
        return &content;
    }
    template <void (*f)(T*)>
    inline void execute() {
        f(&content);
    }
    template <void (T::*f)()>
    inline void execute() {
        ((&content)->*f)();
    }

    template <void (*f)(T*)>
    inline void forEach() {
        LinkedList<T>* e = this;
        while (e) {
            f(e->get());
            e = e->next;
        }
    }
    template <void (T::*f)()>
    void forEach() {
        forEach< &invoke<T,f> >();
    }

    template <void (*f)(T*)>
    static void forEachIn(T* e) {
        while (e) {
            f(e);
            e = e->next;
        }
    }
    template <void (T::*f)()>
    static void forEachIn(T* e) {
        forEachIn<&invoke<T,f> >(e);
    }
    static void forEachIn(T* e, void (*f)(T*)) {
        while (e) {
            f(e);
            e = e->next;
        }
    }
};

struct evaluateHelper {
    evaluateHelper(std::initializer_list<int>) {
    }
};
#define evaluate(x) evaluateHelper{(x,1)...}

// with help from http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
template <std::size_t...>
struct CountDown { };

template <int current, std::size_t... partialCountDown>
struct GenerateCountDown : GenerateCountDown<current-1, current-1, partialCountDown...> {
};

template <std::size_t... partialCountDown>
struct GenerateCountDown<0, partialCountDown...> {
    typedef CountDown<partialCountDown...> countDown;
};

template <typename Func, typename Args, std::size_t... countdown>
void callFunctionHelper(Func&& f, Args&& args, CountDown<countdown...>) {
    f(std::get<countdown>(args)...);
}
template <typename Func, typename Args>
void callFunction(Func&& f, Args&& args) {
    callFunctionHelper<Func, Args>(std::forward<Func>(f), std::forward<Args>(args), typename GenerateCountDown<std::tuple_size<typename std::decay<Args>::type>::value>::countDown());
}

template <typename Func, typename Args, std::size_t... countdown>
auto evaluateFunctionHelper(Func f, Args&& args, CountDown<countdown...>) -> decltype(f(std::get<countdown>(std::forward<Args>(args))...)) {
    return f(std::get<countdown>(args)...);
}
template <typename Func, typename Args>
auto evaluateFunction(Func f, Args&& args) -> decltype(evaluateFunctionHelper<Func, Args>(std::forward<Func>(f), std::forward<Args>(args), typename GenerateCountDown<std::tuple_size<typename std::decay<Args>::type>::value>::countDown())) {
    return evaluateFunctionHelper<Func, Args>(std::forward<Func>(f), std::forward<Args>(args), typename GenerateCountDown<std::tuple_size<typename std::decay<Args>::type>::value>::countDown());
}

template <std::size_t... countdown, typename... Args>
std::tuple<Args& ...> _convertToReferences(std::tuple<Args...>& args, CountDown<countdown...>) {
    return std::tuple<Args& ...>(std::ref(std::get<countdown>(args))...);
}

template <typename... Args>
std::tuple<Args& ...> convertToReferences(std::tuple<Args...>& args) {
    return _convertToReferences(args, typename GenerateCountDown<std::tuple_size<typename std::decay<std::tuple<Args...>>::type>::value>::countDown());
}




/**
  Usage:
        template <typename In>
        static typename In::type getArg() {
            return Type<typename In::type>::from(L, In::index);
        }
        template <typename... Args>
        static std::tuple<In...> convert(TypeWrapper<Args...> &&type) {
            return std::tuple<In...>(getArg<Args>()...);
        }

        convert(typename Zip<In>::type());
*/

template <int current, typename T>
struct ZippedNumber {
    typedef T type;
    static const int index = current;
};
template <typename... Args>
struct TypeWrapper {
};

template <typename T, typename... Args>
struct Unpack {
};
template <typename T, typename Tail, typename... Args>
struct Unpack<TypeWrapper<T,Tail>, Args...> : Unpack<Tail, T, Args...> {
};
template <typename... Args>
struct Unpack<TypeWrapper<>, Args...> {
    typedef TypeWrapper<Args...> type;
};

template <int current, typename Acc, typename... Args>
struct ZipNumbers {
};
template <int current, typename Acc, typename First, typename... Args>
struct ZipNumbers<current, Acc, First, Args...> : ZipNumbers<current+1, TypeWrapper<ZippedNumber<current, First>, Acc>, Args...> {
};
template <int current, typename Acc, typename First>
struct ZipNumbers<current, Acc, First> {
    //typedef Reverse<Acc> type;
    typedef typename Unpack<TypeWrapper<ZippedNumber<current, First>, Acc>>::type type;
};

template <typename... Args>
struct Zip : ZipNumbers<0, TypeWrapper<>, Args...> {
};

template <>
struct Zip<> {
    typedef typename Unpack<TypeWrapper<>>::type type;
};




///// select only one value of the arguments and throw away the others
template <size_t N, class Arg, class... Args>
struct nth_wrap {
    static auto get(Arg a, Args... args) -> decltype(nth_wrap<N-1, Args...>::get(std::forward<Args>(args)...)) {
        return nth_wrap<N-1, Args...>::get(std::forward<Args>(args)...);
    }
};

template <class Arg, class... Args>
struct nth_wrap<0, Arg, Args...> {
    static Arg get(Arg a, Args... args) {
        return std::forward<Arg>(a);
    }
};

template <class Arg>
struct nth_wrap<0, Arg> {
    static Arg get(Arg a) {
        return std::forward<Arg>(a);
    }
};

template <size_t N, class... Args>
auto nth(Args... a) -> decltype(nth_wrap<N,Args...>::get(std::forward<Args>(a)...)) {
    return nth_wrap<N,Args...>::get(std::forward<Args>(a)...);
}



template <class T>
T hton(T n);

template <>
inline unsigned short hton<unsigned short>(unsigned short n) {
    return htons(n);
}

template <>
inline unsigned int hton<unsigned int>(unsigned int n) {
    return htonl(n);
}

template <>
inline unsigned long long hton<unsigned long long>(unsigned long long n) {
    return htonll(n);
}


template <class T>
T ntoh(T n);

template <>
inline unsigned short ntoh<unsigned short>(unsigned short n) {
    return ntohs(n);
}

template <>
inline unsigned int ntoh<unsigned int>(unsigned int n) {
    return ntohl(n);
}

template <>
inline unsigned long long ntoh<unsigned long long>(unsigned long long n) {
    return ntohll(n);
}


// http://stackoverflow.com/questions/4163126/dereferencing-type-punned-pointer-will-break-strict-aliasing-rules-warning
template<typename T, typename F>
struct alias_cast_t
{
    union
    {
        F raw;
        T data;
    };
    static_assert(sizeof(F) == sizeof(T), "Cannot cast types of different sizes");
};
template<typename T, typename F>
T alias_cast(F raw_data)
{
    alias_cast_t<T, F> ac;
    ac.raw = raw_data;
    return ac.data;
}


}
}

#endif

