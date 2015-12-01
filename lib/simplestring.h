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

#ifndef _BP_STRING_H_
#define _BP_STRING_H_

extern "C" {
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>

#ifdef __MINGW32__
    int strncasecmp (const char* sz1, const char* sz2, size_t sizeMaxCompare);
#endif
}

#include <cctype>
#include <cmath>
#include <limits>
#include <cstddef>
#include <string>
#include <tuple>
#include "lib-common.h"
#include "memorypool.h"
#include "memory.h"
#include "shared_object.h"

namespace bitpowder {
namespace lib {

class String;
class StringT;

template <class FirstString>
class Substring;

typedef std::size_t StringLength;
typedef ssize_t SignedStringLength;

class StringUtil {
public:
    static unsigned char FromHex(unsigned char c) {
        if ((c >= '0') && (c <= '9')) {
            return c - '0';
        } else if ((c >= 'a') && (c <= 'f')) {
            return c - 'a' + 10;
        } else if ((c >= 'A') && (c <= 'F')) {
            return c - 'A' + 10;
        }
        return 0;
    }
    static unsigned char FromHex(unsigned char first, unsigned char second) {
        return (FromHex(first) << 4) | FromHex(second);
    }
};

// Basic 'string'
class StringOperation {
  public:
    explicit constexpr StringOperation(StringLength _size) : _size(_size) {
    }
    StringLength _size;
    inline StringLength size() const {
        return _size;
    }
    virtual StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const = 0;

    String operator()(MemoryPool& mp) const;
    StringT c_str(MemoryPool& mp) const;

    template <StringLength N> String operator()(char (&buffer)[N]) const;
    template <StringLength N> StringT c_str(char (&buffer)[N]) const;

    String operator()(char* buffer, StringLength size) const;
    StringT c_str(char* buffer, StringLength size) const;

    String to(char*& buffer, StringLength& size) const;

    Substring<StringOperation> _substring(StringLength offset, StringLength maxLength = std::numeric_limits<StringLength>::max()) const;
    // some compile errors, like "lala" + String are harder to spot, as by autocasting this becomes "lala" + 1 -> "ala", instead of "lala"_S + String
    explicit operator bool() const {
        return _size > 0;
    }
    explicit operator std::string() const;
    const StringOperation& op() const {
        return *this;
    }
    static StringLength writeToHelper(char* __restrict__& buffer, StringLength& bufferLength, StringLength& offset, const char* src, StringLength len);
    static StringLength writeToHelper(char* __restrict__& buffer, StringLength& bufferLength, StringLength& offset, const StringOperation& op, StringLength max = std::numeric_limits<StringLength>::max());
    static StringLength writeToHelper(char* __restrict__& buffer, StringLength& bufferLength, StringLength& offset, const String& str, StringLength max = std::numeric_limits<StringLength>::max());
};

// support for String methods
template <typename A, typename B>
struct CharOr;

template <class T>
struct Complement {
    T t;
    constexpr Complement(const T& t) : t(t) {
    }
    constexpr bool operator()(char in) const {
        return !t(in);
    }
    template <typename B>
    constexpr CharOr<Complement,B> operator+(const B& b) const {
        return CharOr<Complement,B>(*this, b);
    }
    constexpr T operator !() const {
        return t;
    }
};

template <typename A, typename B>
struct CharOr {
    A a;
    B b;
    constexpr CharOr() : a(), b() {
    }
    constexpr CharOr(const A& a, const B& b) : a(a), b(b) {
    }
    constexpr bool operator()(char in) const {
        return a(in) || b(in);
    }
    template <typename C>
    constexpr CharOr<CharOr<A,B>,C> operator+(const C& c) const {
        return CharOr< CharOr<A,B>, C>(*this, c);
    }
    constexpr Complement<CharOr> operator !() const {
        return Complement<CharOr>(*this);
    }
};

template <char m>
struct Char {
    constexpr inline bool operator()(char in) const {
        return in == m;
    }
    template <typename B>
    constexpr CharOr<Char,B> operator+(const B& b) const {
        return CharOr<Char,B>(*this, b);
    }
    constexpr Complement<Char> operator !() const {
        return Complement<Char>(*this);
    }
};

struct Alpha {
    constexpr inline bool operator()(char in) const {
        return ('a' <= in && in <= 'z') ||
               ('A' <= in && in <= 'Z');
    }
    template <typename B>
    constexpr CharOr<Alpha,B> operator+(const B& b) const {
        return CharOr<Alpha,B>(*this, b);
    }
    constexpr Complement<Alpha> operator !() const {
        return Complement<Alpha>(*this);
    }
};

struct Num {
    constexpr inline bool operator()(char in) const {
        return ('0' <= in && in <= '9');
    }
    template <typename B>
    constexpr CharOr<Num,B> operator+(const B& b) const {
        return CharOr<Num,B>(*this, b);
    }
    constexpr Complement<Num> operator !() const {
        return Complement<Num>(*this);
    }
};

struct Whitespace {
    constexpr inline bool operator()(char in) const {
        return in == ' ' || in == '\t' || in == '\n' || in == '\r';
    }
    template <typename B>
    constexpr CharOr<Whitespace,B> operator+(const B& b) const {
        return CharOr<Whitespace,B>(*this, b);
    }
    constexpr Complement<Whitespace> operator !() const {
        return Complement<Whitespace>(*this);
    }
};

struct Hex {
    constexpr inline bool operator()(char in) const {
        return ('0' <= in && in <= '9') ||
               ('a' <= in && in <= 'f') ||
               ('A' <= in && in <= 'F');
    }
    template <typename B>
    constexpr CharOr<Hex,B> operator+(const B& b) const {
        return CharOr<Hex,B>(*this, b);
    }
    constexpr Complement<Hex> operator !() const {
        return Complement<Hex>(*this);
    }
};

template <class T>
constexpr Complement<T> CharComplement(const T& t) {
    return Complement<T>(t);
}

#define C(c) Char<c>()

class StringOperationString;

inline int toUpper(int c) {
    return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
}

inline int toLower(int c) {
    return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
}

#define SMALL_MEM_COPY_LIMIT 128
// avoids function call, can be faster for small copies
inline void smallmemcpy(char* __restrict__ dst, const char* __restrict__ src, StringLength length) {
    char* dstEnd = &dst[length];
    while (dst != dstEnd) {
        *dst = *src;
        ++dst;
        ++src;
    }
}

inline void memcpywrap(char* __restrict__ dst, const char* __restrict__ src, StringLength length) {
    if (length < SMALL_MEM_COPY_LIMIT)
        smallmemcpy(dst, src, length);
    else
        memcpy(dst, src, length);
}

template <class S> class Hashed;
template <class S> class HashedIgnoreCase;
template <class S> class StableHashed;
template <class S> class StableHashedIgnoreCase;

// the main string
class String {
  protected:
    const char* ptr;
    StringLength len;
  public:
    constexpr static StringLength UNKNOWN_LENGTH = std::numeric_limits<StringLength>::max();
    constexpr inline String(const char* pointer, StringLength len = UNKNOWN_LENGTH) : ptr(pointer), len(len < UNKNOWN_LENGTH ? len : pointer ? strlen(pointer) : 0) {
    }
    String(const std::string& str) : ptr(str.c_str()), len(str.length()) {
    }
    constexpr inline String() : ptr(nullptr), len(0) {
    }
    String(std::nullptr_t) : String() {
    }
    template <StringLength N>
    constexpr inline String(char (&str)[N]) : ptr(str), len(N-1) {
    }
    template <StringLength N>
    constexpr inline String(const char (&str)[N]) : ptr(str), len(N-1) {
    }
    constexpr inline String(const char* begin, const char* end) : ptr(begin < end ? begin : nullptr), len(begin < end ? end-begin : 0) {
    }
    constexpr inline String(const String& c) : ptr(c.ptr), len(c.len) {
    }
    String& operator=(const String& str) {
        ptr = str.ptr;
        len = str.len;
        return *this;
    }
    std::string stl() const;
    explicit operator std::string() const;

    void clear() {
        ptr = nullptr;
        len = 0;
    }
    typedef const char* const_iterator;
    inline const_iterator begin() const {
        return ptr;
    }
    inline const_iterator end() const {
        return ptr+len;
    }
    inline constexpr StringLength length() const __attribute__ ((pure)) {
        return len;
    }
    inline constexpr StringLength size() const __attribute__ ((pure)) {
        return len;
    }
    inline constexpr const char* pointer() const __attribute__ ((pure)) {
        return ptr;
    }
    // stable hashes are stable over execution runs and over multiple systems/installations
    unsigned int stableHash(unsigned int continueWith = 5381) const __attribute__ ((pure));
    unsigned int stableHashIgnoreCase(unsigned int continueWith = 5381) const __attribute__ ((pure));
    // in order to counter denial of service attacks, these hashes are only stable during a executing.
    unsigned int hash() const __attribute__ ((pure));
    unsigned int hashIgnoreCase() const __attribute__ ((pure));
    // It uses the randomizeStringHash to counter semi-collisions (last few bytes that are the same, so the same modulo is used)
    static unsigned int randomizeStringHash(unsigned int value);
    // in order to counter denial of service attacks, these hashes are only stable during a executing.
    inline bool empty() const __attribute__ ((pure)) {
        return !ptr || len == 0;
    }
    inline StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const {
        StringLength l = std::min((StringLength)(len - offset), bufferLength);
        memcpywrap(buffer, &ptr[offset], l);
        return l;
    }
    constexpr String substring(StringLength pos) const __attribute ((pure)) {
        return pos >= length() ? String() : String(pointer() + pos, length() - pos);
    }
    constexpr String substring(StringLength pos, StringLength len) const __attribute ((pure)) {
        return pos >= length() ? String() : String(pointer() + pos, std::min(length() - pos, len));
    }
    template <StringLength N>
    static StringLength printf(const char (&str)[N], const char* format, ...) {
        va_list ap;
        va_start(ap, format);
        int retval = snprintf(str, N, format, ap);
        va_end(ap);
        return retval;
    }
    bool startsWith(const String& str) const {
        if (str.length() > length())
            return false;
        return memcmp(pointer(), str.pointer(), str.length()) == 0;
    }
    bool startsWithIgnoreCase(const String& b) const {
        return len >= b.len && strncasecmp(ptr, b.ptr, b.len) == 0;
    }
    bool endsWith(const String& str) const {
        if (str.length() > length())
            return false;
        return memcmp(pointer()+length()-str.length(), str.pointer(), str.length()) == 0;
    }
    bool endsWithIgnoreCase(const String& b) const {
        return len >= b.len && strncasecmp(ptr+len-b.len, b.ptr, b.len) == 0;
    }
    String dir() const;
    String base() const;
    String extension() const;
    String trim() const;

    template <typename T>
    String span(const T& t, String* next = nullptr) const {
        const char* p = pointer();
        if (p) {
            for (StringLength l = 0; l < len; ++l) {
                if (!t(p[l])) {
                    String retval = substring(0, l);
                    if (next)
                        *next = substring(l);
                    return retval;
                }
            }
        }
        String retval = *this;
        if (next)
            *next = String();
        return retval;
    }
    template <typename T>
    String span(const T& t, String& next) const {
        return span(t, &next);
    }
    template <typename T>
    String doSpan(const T& t) {
        return span<T>(t, this);
    }
    template <typename T>
    String rspan(const T& t, String* next = nullptr) const {
        const char* p = pointer();
        if (p) {
            for (SignedStringLength l = len-1; l >= 0; --l) {
                if (!t(p[l])) {
                    String retval = substring(l+1);
                    if (next)
                        *next = substring(0, l+1);
                    return retval;
                }
            }
        }
        String retval = *this;
        if (next)
            *next = String();
        return retval;
    }
    template <typename T>
    String rspan(const T& t, String& next) const {
        return rspan(t, &next);
    }
    template <typename T>
    String doRSpan(const T& t) {
        return rspan<T>(t, this);
    }

    template <typename T>
    String splitOn(const T& t, String& next, char* delimPtr = nullptr) const {
        return splitOn(t, &next, delimPtr);
    }
    template <typename T>
    String splitOn(const T& t, String* next = nullptr, char* delimPtr = nullptr) const {
        const char* p = pointer();
        if (p) {
            for (StringLength l = 0; l < len; ++l) {
                if (t(p[l])) {
                    String retval = substring(0, l);
                    if (delimPtr)
                        *delimPtr = p[l];
                    if (next)
                        *next = substring(l+1);
                    return retval;
                }
            }
        }
        String retval = *this;
        if (delimPtr)
            *delimPtr = '\0';
        if (next)
            *next = String();
        return retval;
    }
    template <typename T>
    String doSplitOn(const T& t, char* delimPtr = nullptr) {
        return splitOn<T>(t, this, delimPtr);
    }

    template <typename T>
    String rsplitOn(const T& t, String& next, char* delimPtr = nullptr) const {
        return rsplitOn(t, &next, delimPtr);
    }
    template <typename T>
    String rsplitOn(const T& t, String* next = nullptr, char* delimPtr = nullptr) const {
        const char* p = pointer();
        if (p) {
            for (SignedStringLength l = len-1; l >= 0; --l) {
                if (t(p[l])) {
                    String retval = substring(l+1);
                    if (delimPtr)
                        *delimPtr = p[l];
                    if (next)
                        *next = substring(0, l);
                    return retval;
                }
            }
        }
        String retval = *this;
        if (delimPtr)
            *delimPtr = '\0';
        if (next)
            *next = String();
        return retval;
    }
    template <typename T>
    String doRSplitOn(const T& t, char* delimPtr = nullptr) {
        return rsplitOn<T>(t, this, delimPtr);
    }

    String doSplit(const String& s);
    String doSplitIgnoreCase(const String& s);

    String split(const String& s, String& next) const;
    String split(const String& s, String* next = nullptr) const;

    String splitIgnoreCase(const String& s, String& next) const;
    String splitIgnoreCase(const String& s, String* next = nullptr) const;

    String doRSplit(const String& s);
    String doRSplitIgnoreCase(const String& s);
    String rsplit(const String& s, String& next) const;
    String rsplit(const String& s, String* next = nullptr) const;
    String rsplitIgnoreCase(const String& s, String* next = nullptr) const;

    bool contains(const String& str) const;
    bool containsIgnoreCase(const String& str) const;
    char operator[](off_t off) const;

    enum Escape {
        LikeC,
        URL
    };

    String unescape(Escape escape, MemoryPool& mp) const;
    String escape(Escape escape, MemoryPool& mp) const;
    size_t lengthOfEscape(Escape escape) const;
    String base64decode(MemoryPool& mp) const;

    bool equalsIgnoreCase(const String& b) const;
  private:
    static bool lowerless(unsigned char a, unsigned char b) {
        return toLower(a) < toLower(b);
    }
  public:
    bool lessIgnoreCase(const String& b) const {
        if (length() != b.length())
            return length() < b.length();
        if (pointer() == b.pointer())
            return false;
        return std::lexicographical_compare(begin(), end(), b.begin(), b.end(), lowerless);
    }
    bool lessIgnoreCaseAlfa(const String& b) const {
        if (pointer() == b.pointer() && length() == b.length())
            return false;
        return std::lexicographical_compare(begin(), end(), b.begin(), b.end(), lowerless);
    }
    bool lessAlfa(const String& b) const {
        if (pointer() == b.pointer() && length() == b.length())
            return false;
        return std::lexicographical_compare(begin(), end(), b.begin(), b.end(), std::less<char>());
    }

    int compareUsingVersion(const String& rhs) const;

    template <typename T, int _base = 10>
    inline T toNumber(String* next = nullptr) const {
        if (length() == 0) {
            if (next)
                *next = *this;
            return 0;
        }
        T result = 0;
        bool isNegative = ptr[0] == '-';
        int i = isNegative;
        SignedStringLength l = length();
        while (i < l &&
                ((ptr[i] >= '0' && ptr[i] <= '9')
                 || (_base > 10 && ptr[i] >= 'a' && ptr[i] <= 'z')
                 || (_base > 10 && ptr[i] >= 'A' && ptr[i] <= 'Z')
                )) {
            result *= _base;
            if ((ptr[i] >= '0') && (ptr[i] <= '9')) {
                result += ptr[i] - '0';
            } else if ((ptr[i] >= 'a') && (ptr[i] <= 'z')) {
                result += ptr[i] - 'a' + 10;
            } else if ((ptr[i] >= 'A') && (ptr[i] <= 'Z')) {
                result += ptr[i] - 'A' + 10;
            }
            ++i;
        }
        if (std::is_floating_point<T>::value) {
            if (i < l && ptr[i] == '.') {
                ++i;
                T afterPoint = 1;
                while (i < l &&
                        ((ptr[i] >= '0' && ptr[i] <= '9')
                         || (_base > 10 && ptr[i] >= 'a' && ptr[i] <= 'z')
                         || (_base > 10 && ptr[i] >= 'A' && ptr[i] <= 'Z')
                        )) {
                    result *= _base;
                    if ((ptr[i] >= '0') && (ptr[i] <= '9')) {
                        result += ptr[i] - '0';
                    } else if ((ptr[i] >= 'a') && (ptr[i] <= 'z')) {
                        result += ptr[i] - 'a' + 10;
                    } else if ((ptr[i] >= 'A') && (ptr[i] <= 'Z')) {
                        result += ptr[i] - 'A' + 10;
                    }
                    ++i;
                    afterPoint *= _base;
                }
                result /= afterPoint;
            }
            if (i < l && toLower(ptr[i]) == 'e') {
                ++i;
                int e = 0;
                while (i < l &&
                        ((ptr[i] >= '0' && ptr[i] <= '9')
                         || (_base > 10 && ptr[i] >= 'a' && ptr[i] <= 'z')
                         || (_base > 10 && ptr[i] >= 'A' && ptr[i] <= 'Z')
                        )) {
                    e *= _base;
                    if ((ptr[i] >= '0') && (ptr[i] <= '9')) {
                        e += ptr[i] - '0';
                    } else if ((ptr[i] >= 'a') && (ptr[i] <= 'z')) {
                        e += ptr[i] - 'a' + 10;
                    } else if ((ptr[i] >= 'A') && (ptr[i] <= 'Z')) {
                        e += ptr[i] - 'A' + 10;
                    }
                    ++i;
                }
                result *= std::pow(10, e);
            }
        }
        if (next)
            *next = i > (isNegative ? 1 : 0) ? substring(i) : *this;
        return isNegative ? -result : result;
    }
    template <typename T, int _base = 10>
    inline T toNumber(String& next) const {
        return toNumber<T, _base>(&next);
    }

    template <typename T>
    inline T hexToNumber(String* next = nullptr) const {
        return toNumber<T, 16>(next);
    }
    template <typename T>
    inline T hexToNumber(String& next) const {
        return hexToNumber<T>(&next);
    }

    template <typename T, int _base = 64>
    inline T maxToNumber(String* next = nullptr) const {
        if (length() == 0) {
            if (next)
                *next = *this;
            return 0;
        }
        T result = 0;
        bool isNegative = ptr[0] == '-';
        int i = isNegative;
        SignedStringLength l = length();
        while (i < l &&
                ((ptr[i] >= '0' && ptr[i] <= '9')
                 || (_base > 10 && ptr[i] >= 'a' && ptr[i] <= 'z')
                 || (_base > 36 && ptr[i] >= 'A' && ptr[i] <= 'Z')
                 || (_base > 62 && ptr[i] == '_')
                 || (_base > 63 && ptr[i] == '.')
                )) {
            result *= _base;
            if ((ptr[i] >= '0') && (ptr[i] <= '9')) {
                result += ptr[i] - '0';
            } else if ((ptr[i] >= 'a') && (ptr[i] <= 'z')) {
                result += ptr[i] - 'a' + 10;
            } else if ((ptr[i] >= 'A') && (ptr[i] <= 'Z')) {
                result += ptr[i] - 'A' + 36;
            } else if (ptr[i] == '_') {
                result += 62;
            } else if (ptr[i] == '.') {
                result += 63;
            }
            i++;
        }
        if (next)
            *next = i > (int)isNegative ? substring(i) : *this;
        return isNegative ? -result : result;
    }

    template <typename T>
    inline T maxToNumber(String& next) const {
        return maxToNumber<T, 64>(&next);
    }

    template <typename T>
    inline T doToNumber() {
        return toNumber<T, 10>(this);
    }
    template <typename T>
    inline T doHexToNumber() {
        return hexToNumber<T>(this);
    }
    template <typename T>
    inline T doMaxToNumber() {
        return maxToNumber<T>(this);
    }

    String append(const String& a, MemoryPool& mp) const;

    template <class F>
    String filter(F& f, MemoryPool& mp) const {
        StringLength newSize = 0;
        for (char c : *this)
            if (f(c))
                newSize++;
        char* retval = (char*)mp.alloc(newSize, sizeof(char));
        newSize = 0;
        for (char c : *this)
            if (f(c))
                retval[newSize++] = c;
        return {retval, newSize};
    }

    explicit operator bool() const {
        return len > 0;
    }

    StringOperationString op() const;
    operator StringOperationString () const;
    explicit constexpr operator StableHashed<String> () const;
    explicit operator Hashed<String> () const;

    String operator()(MemoryPool& mp) const;
    StringT c_str(MemoryPool& mp) const;
    template <StringLength N> String operator()(char (&buffer)[N]) const;
    template <StringLength N> StringT c_str(char (&buffer)[N]) const;
    String operator()(char* buffer, StringLength size) const;
    String to(char*& buffer, StringLength& size) const;
    String string() const {
        return *this;
    }
};

class StringOperationString : public StringOperation {
    String str;
  public:
    constexpr StringOperationString(const String& str) : StringOperation(str.size()), str(str) {
    }
    StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
        return str.writeTo(buffer, bufferLength, offset);
    }
};

inline StringOperationString String::op() const {
    return StringOperationString(*this);
}
inline String::operator StringOperationString () const {
    return op();
}

// terminating string
class StringT : public String {
  public:
    constexpr inline StringT(const char* ptr, StringLength len = std::numeric_limits<StringLength>::max()) : String(ptr, len) {
    }
    inline StringT(const std::string& s) : String(s.c_str(), s.size()) {
    }
    constexpr inline StringT() : String() {
    }
    template <StringLength N>
    constexpr inline StringT(const char (&str)[N]) : String(str) {
    }

    StringT c_str(MemoryPool& mp) const {
        return String::c_str(mp);
    }
    const char* c_str() const {
        return pointer();
    }
};

template <StringLength N> String String::operator ()(char (&buffer)[N]) const {
    return op()(buffer);
}
template <StringLength N> StringT String::c_str(char (&buffer)[N]) const {
    return op().c_str(buffer);
}

// operators on strings
inline bool operator==(const String& lhs, const String& rhs) {
    return lhs.length() == rhs.length() && (lhs.pointer() == rhs.pointer() || memcmp(lhs.pointer(), rhs.pointer(), lhs.length()) == 0);
}
inline bool operator<(const String& lhs, const String& rhs) {
    if (lhs.length() != rhs.length())
        return lhs.length() < rhs.length();
    if (lhs.pointer() == rhs.pointer())
        return false;
    return memcmp(lhs.pointer(), rhs.pointer(), lhs.length()) < 0;
}
inline bool operator>(const String& lhs, const String& rhs) {
    if (lhs.length() != rhs.length())
        return lhs.length() > rhs.length();
    if (lhs.pointer() == rhs.pointer())
        return false;
    return memcmp(lhs.pointer(), rhs.pointer(), lhs.length()) > 0;
}

inline bool operator>=(const String& lhs, const String& rhs) {
    return !(lhs < rhs);
}
inline bool operator<=(const String& lhs, const String& rhs) {
    return !(lhs > rhs);
}
inline bool operator!=(const String& lhs, const String& rhs) {
    return !(lhs == rhs);
}

template <class S>
class StableHashed {
    S s;
    unsigned int hashCache;
    static constexpr unsigned int _hash(const S& str, unsigned int hash) {
        return str.length() == 0 ? hash : _hash(str.substring(1), hash*33 ^ str[0]);
    }
    static constexpr unsigned int _hashRaw(const char* str, StringLength length, unsigned int hash) {
        return length == 0 ? hash : _hashRaw(&str[1], length-1, hash*33 ^ str[0]);
    }
    static constexpr unsigned int hash(const S& str) {
        return _hashRaw(str.pointer(), str.length(), 5381);
    }
  public:
    constexpr StableHashed(const S& str) : s(str), hashCache(hash(str)) {
    }
    constexpr StableHashed(const char* value, StringLength size) : s(value, size), hashCache(hash(s)) {
    }
    StableHashed(const StableHashed<StringT>& s) : s(s.string()), hashCache(s.stableHash()) {
    }
    template <bool enabled = std::is_same<String,S>::value>
    StableHashed(const StableHashed& s, typename std::enable_if<enabled,StableHashed>::type* = nullptr) : s(s.s), hashCache(s.hashCache) {
    }
    StableHashed(const S& s, unsigned int hashCode) : s(s), hashCache(hashCode) {
    }
    template <StringLength N>
    constexpr inline StableHashed(const char (&str)[N]) : s(S(str, N-1)), hashCache(hash(this->s)) {
    }
    inline unsigned int stableHash() const {
        return hashCache;
    }
    StableHashed& operator=(const S& str) {
        this->s = str;
        hashCache = str.stableHash();
        return *this;
    }
    StableHashed& operator=(const StableHashed& str) {
        this->s = str.s;
        hashCache = str.hashCache;
        return *this;
    }
    template <StringLength N>
    StableHashed& operator=(const char (&str)[N]) {
        this->s = str;
        hashCache = hash(this->s);
        return *this;
    }
    explicit operator S& () {
        return s;
    }
    S* operator->() {
        return& s;
    }
    const S* operator->() const {
        return& s;
    }
    S& string() {
        return s;
    }
    operator const S& () const {
        return s;
    }
    const S& string() const {
        return s;
    }
    explicit operator Hashed<S> () const {
        return {*this};
    }
    bool equals(const StableHashed<String>& b) const {
        return hashCache == b.hashCache && s == b.string();
    }
    bool less(const StableHashed<String>& b) const {
        if (hashCache != b.hashCache)
            return hashCache < b.hashCache;
        return s < b.string();
    }
    StableHashed operator()(MemoryPool& mp) const {
        return {s(mp), hashCache};
    }
};

// for std::less<StableHashedString>
template <class LHS, class RHS>
inline bool operator<(const StableHashed<LHS>& lhs, const StableHashed<RHS>& rhs) {
    if (lhs.stableHash() != rhs.stableHash())
        return lhs.stableHash() < rhs.stableHash();
    return lhs.string() < rhs.string();
}
template <class LHS, class RHS>
inline bool operator>(const StableHashed<LHS>& lhs, const StableHashed<RHS>& rhs) {
    if (lhs.stableHash() != rhs.stableHash())
        return lhs.stableHash() > rhs.stableHash();
    return lhs.string() > rhs.string();
}
// for StableHashedString
template <class LHS, class RHS>
inline bool operator==(const StableHashed<LHS>& lhs, const StableHashed<RHS>& rhs) {
    return lhs.stableHash() == rhs.stableHash() && lhs.string() == rhs.string();
}

template <class LHS, class RHS>
inline bool operator>=(const StableHashed<LHS>& lhs, const StableHashed<RHS>& rhs) {
    return !(lhs < rhs);
}
template <class LHS, class RHS>
inline bool operator<=(const StableHashed<LHS>& lhs, const StableHashed<RHS>& rhs) {
    return !(lhs > rhs);
}
template <class LHS, class RHS>
inline bool operator!=(const StableHashed<LHS>& lhs, const StableHashed<RHS>& rhs) {
    return !(lhs == rhs);
}

template <class S>
class StableHashedIgnoreCase {
    S s;
    unsigned int hashCache;
    static constexpr unsigned int _hash(const S& str, unsigned int hash) {
        return str.length() == 0 ? hash : _hash(str.substring(1), hash*33 ^ toUpper(str[0]));
    }
    static constexpr unsigned int _hashRaw(const char* str, StringLength length, unsigned int hash) {
        return length == 0 ? hash : _hashRaw(&str[1], length-1, hash*33 ^ toUpper(str[0]));
    }
    static constexpr unsigned int hash(const S& str) {
        return _hashRaw(str.pointer(), str.length(), 5381);
    }
  public:
    constexpr StableHashedIgnoreCase(const S& str) : s(str), hashCache(hash(str)) {
    }
    constexpr StableHashedIgnoreCase(const char* value, StringLength size) : s(value, size), hashCache(hash(s)) {
    }
    StableHashedIgnoreCase(const StableHashedIgnoreCase<StringT>& s) : s(s.string()), hashCache(s.stableHashIgnoreCase()) {
    }
    template <bool enabled = std::is_same<String,S>::value>
    StableHashedIgnoreCase(const StableHashedIgnoreCase& s, typename std::enable_if<enabled,StableHashedIgnoreCase>::type* = nullptr) : s(s.s), hashCache(s.hashCache) {
    }
    StableHashedIgnoreCase(const S& s, unsigned int hashCode) : s(s), hashCache(hashCode) {
    }
    template <StringLength N>
    constexpr inline StableHashedIgnoreCase(const char (&str)[N]) : s(S(str, N-1)), hashCache(hash(this->s)) {
    }
    inline unsigned int stableHashIgnoreCase() const {
        return hashCache;
    }
    StableHashedIgnoreCase& operator=(const S& str) {
        this->s = str;
        hashCache = str.stableHashIgnoreCase();
        return *this;
    }
    StableHashedIgnoreCase& operator=(const StableHashedIgnoreCase& str) {
        this->s = str.s;
        hashCache = str.hashCache;
        return *this;
    }
    template <StringLength N>
    StableHashedIgnoreCase& operator=(const char (&str)[N]) {
        this->s = str;
        hashCache = hash(this->s);
        return *this;
    }
    explicit operator S& () {
        return s;
    }
    S* operator->() {
        return& s;
    }
    const S* operator->() const {
        return& s;
    }
    S& string() {
        return s;
    }
    operator const S& () const {
        return s;
    }
    const S& string() const {
        return s;
    }
    explicit operator HashedIgnoreCase<S> () const {
        return {*this};
    }
    bool equalsIgnoreCase(const StableHashedIgnoreCase& b) const {
        return hashCache == b.hashCache && s.equalsIgnoreCase(b.s);
    }
    bool lessIgnoreCase(const StableHashedIgnoreCase& b) const {
        if (hashCache != b.hashCache)
            return hashCache < b.hashCache;
        return string().lessIgnoreCase(b);
    }
    StableHashedIgnoreCase operator()(MemoryPool& mp) const {
        return {s(mp), hashCache};
    }
};

// for std::less<StableHashedString>
template <class LHS, class RHS>
inline bool operator<(const StableHashedIgnoreCase<LHS>& lhs, const StableHashedIgnoreCase<RHS>& rhs) {
    if (lhs.stableHashIgnoreCase() != rhs.stableHashIgnoreCase())
        return lhs.stableHashIgnoreCase() < rhs.stableHashIgnoreCase();
    return lhs.string() < rhs.string();
}
template <class LHS, class RHS>
inline bool operator>(const StableHashedIgnoreCase<LHS>& lhs, const StableHashedIgnoreCase<RHS>& rhs) {
    if (lhs.stableHashIgnoreCase() != rhs.stableHashIgnoreCase())
        return lhs.stableHashIgnoreCase() > rhs.stableHashIgnoreCase();
    return lhs.string() > rhs.string();
}
// for StableHashedString
template <class LHS, class RHS>
inline bool operator==(const StableHashedIgnoreCase<LHS>& lhs, const StableHashedIgnoreCase<RHS>& rhs) {
    return lhs.stableHashIgnoreCase() == rhs.stableHashIgnoreCase() && lhs.string() == rhs.string();
}

template <class LHS, class RHS>
inline bool operator>=(const StableHashedIgnoreCase<LHS>& lhs, const StableHashedIgnoreCase<RHS>& rhs) {
    return !(lhs < rhs);
}
template <class LHS, class RHS>
inline bool operator<=(const StableHashedIgnoreCase<LHS>& lhs, const StableHashedIgnoreCase<RHS>& rhs) {
    return !(lhs > rhs);
}
template <class LHS, class RHS>
inline bool operator!=(const StableHashedIgnoreCase<LHS>& lhs, const StableHashedIgnoreCase<RHS>& rhs) {
    return !(lhs == rhs);
}

template <class S>
class Hashed {
    S s;
    unsigned int hashCache;
  public:
    Hashed(const StableHashed<StringT>& s) : s(s), hashCache(String::randomizeStringHash(s.stableHash())) {
    }
    template <bool enabled = std::is_same<String,S>::value>
    Hashed(const StableHashed<S>& s, typename std::enable_if<enabled,Hashed>::type* = nullptr) : s(s), hashCache(String::randomizeStringHash(s.stableHash())) {
    }
    Hashed(const StringT& s) : Hashed(s, String::randomizeStringHash(s.stableHash())) {
    }
    template <bool enabled = std::is_same<String,S>::value>
    Hashed(const S& s, typename std::enable_if<enabled,Hashed>::type* = nullptr) : Hashed(s, String::randomizeStringHash(s.stableHash())) {
    }
    Hashed(const S& s, unsigned int hashCode) : s(s), hashCache(hashCode) {
    }
    Hashed(const Hashed<StringT>& s) : s(s.string()), hashCache(s.hash()) {
    }
    template <bool enabled = std::is_same<String,S>::value>
    Hashed(const Hashed& s, typename std::enable_if<enabled,Hashed>::type* = nullptr) : s(s.s), hashCache(s.hashCache) {
    }

    inline unsigned int hash() const {
        return hashCache;
    }
    Hashed& operator=(const S& s) {
        this->s = s;
        hashCache = s.hash();
        return *this;
    }
    Hashed& operator=(const StableHashed<S>& str) {
        this->s = s;
        hashCache = String::randomizeStringHash(s.stableHash());
        return *this;
    }
    Hashed& operator=(const Hashed& s) {
        this->s = s.s;
        hashCache = s.hashCache;
        return *this;
    }
    template <StringLength N>
    Hashed& operator=(const char (&str)[N]) {
        return *this = StringT(str);
    }
    explicit operator S& () {
        return s;
    }
    explicit operator bool() const {
        return (bool)string();
    }
    S* operator->() {
        return &s;
    }
    const S* operator->() const {
        return &s;
    }
    S& string() {
        return s;
    }
    operator const S& () const {
        return s;
    }
    const S& string() const {
        return s;
    }
    bool equals(const Hashed<String>& b) const {
        return hashCache == b.hash() && s == b.string();
    }
    bool less(const Hashed<String>& b) const {
        if (hashCache != b.hash())
            return hashCache < b.hash();
        return string() < b.string();
    }
    Hashed operator()(MemoryPool& mp) const {
        return {s(mp), hashCache};
    }
};


template <class LHS, class RHS>
bool operator<(const Hashed<LHS>& lhs, const Hashed<RHS>& rhs) {
    if (lhs.hash() != rhs.hash())
        return lhs.hash() < rhs.hash();
    return lhs.string() < rhs.string();
}

template <class LHS, class RHS>
bool operator>(const Hashed<LHS>& lhs, const Hashed<RHS>& rhs) {
    if (lhs.hash() != rhs.hash())
        return lhs.hash() > rhs.hash();
    return lhs.string() > rhs.string();
}

template <class LHS, class RHS>
bool operator==(const Hashed<LHS>& lhs, const Hashed<RHS>& rhs) {
    return lhs.hash() == rhs.hash() && lhs.string() == rhs.string();
}

template <class LHS, class RHS>
bool operator==(const LHS& lhs, const Hashed<RHS>& rhs) {
    return lhs.string().hash() == rhs.hash() && lhs.string() == rhs.string();
}

template <class LHS, class RHS>
bool operator==(const Hashed<LHS>& lhs, const RHS& rhs) {
    return lhs.hash() == rhs.string().hash() && lhs.string() == rhs.string();
}

template <class LHS, class RHS>
inline bool operator>=(const Hashed<LHS>& lhs, const Hashed<RHS>& rhs) {
    return !(lhs < rhs);
}
template <class LHS, class RHS>
inline bool operator<=(const Hashed<LHS>& lhs, const Hashed<RHS>& rhs) {
    return !(lhs > rhs);
}
template <class LHS, class RHS>
inline bool operator!=(const Hashed<LHS>& lhs, const Hashed<RHS>& rhs) {
    return !(lhs == rhs);
}

template <class S>
class HashedIgnoreCase {
    S s;
    unsigned int hashCache;
  public:
    HashedIgnoreCase(const StableHashedIgnoreCase<StringT>& s) : s(s), hashCache(String::randomizeStringHash(s.stableHashIgnoreCase())) {
    }
    template <bool enabled = std::is_same<String,S>::value>
    HashedIgnoreCase(const StableHashed<S>& s, typename std::enable_if<enabled,HashedIgnoreCase>::type* = nullptr) : s(s), hashCache(String::randomizeStringHash(s.stableHashIgnoreCase())) {
    }
    HashedIgnoreCase(const StringT& s) : HashedIgnoreCase(s, String::randomizeStringHash(s.stableHashIgnoreCase())) {
    }
    template <bool enabled = std::is_same<String,S>::value>
    HashedIgnoreCase(const S& s, typename std::enable_if<enabled,HashedIgnoreCase>::type* = nullptr) : HashedIgnoreCase(s, String::randomizeStringHash(s.stableHashIgnoreCase())) {
    }
    HashedIgnoreCase(const S& s, unsigned int hashCode) : s(s), hashCache(hashCode) {
    }
    HashedIgnoreCase(const HashedIgnoreCase<StringT>& s) : s(s.string()), hashCache(s.hashIgnoreCase()) {
    }
    template <bool enabled = std::is_same<String,S>::value>
    HashedIgnoreCase(const HashedIgnoreCase& s, typename std::enable_if<enabled,HashedIgnoreCase>::type* = nullptr) : s(s.s), hashCache(s.hashCache) {
    }

    inline unsigned int hashIgnoreCase() const {
        return hashCache;
    }
    HashedIgnoreCase& operator=(const S& s) {
        this->s = s;
        hashCache = s.hashIgnoreCase();
        return *this;
    }
    HashedIgnoreCase& operator=(const StableHashedIgnoreCase<S>& str) {
        this->s = s;
        hashCache = String::randomizeStringHash(s.stableHashIgnoreCase());
        return *this;
    }
    HashedIgnoreCase& operator=(const HashedIgnoreCase& s) {
        this->s = s.s;
        hashCache = s.hashCache;
        return *this;
    }
    template <StringLength N>
    HashedIgnoreCase& operator=(const char (&str)[N]) {
        return *this = StringT(str);
    }
    explicit operator S& () {
        return s;
    }
    explicit operator bool() const {
        return (bool)string();
    }
    S* operator->() {
        return &s;
    }
    const S* operator->() const {
        return &s;
    }
    S& string() {
        return s;
    }
    operator const S& () const {
        return s;
    }
    const S& string() const {
        return s;
    }
    bool equalsIgnoreCase(const HashedIgnoreCase<String>& b) const {
        return hashCache == b.hashIgnoreCase() && s.equalsIgnoreCase(b.string());
    }
    bool lessIgnoreCase(const HashedIgnoreCase<String>& b) const {
        if (hashCache != b.hashIgnoreCase())
            return hashCache < b.hashIgnoreCase();
        return string().lessIgnoreCase(b);
    }
    HashedIgnoreCase operator()(MemoryPool& mp) const {
        return {s(mp), hashCache};
    }
};

template <class LHS, class RHS>
bool operator<(const HashedIgnoreCase<LHS>& lhs, const HashedIgnoreCase<RHS>& rhs) {
    if (lhs.hashIgnoreCase() != rhs.hashIgnoreCase())
        return lhs.hashIgnoreCase() < rhs.hashIgnoreCase();
    return lhs.string() < rhs.string();
}

template <class LHS, class RHS>
bool operator>(const HashedIgnoreCase<LHS>& lhs, const HashedIgnoreCase<RHS>& rhs) {
    if (lhs.hashIgnoreCase() != rhs.hashIgnoreCase())
        return lhs.hashIgnoreCase() > rhs.hashIgnoreCase();
    return lhs.string() > rhs.string();
}

template <class LHS, class RHS>
bool operator==(const HashedIgnoreCase<LHS>& lhs, const HashedIgnoreCase<RHS>& rhs) {
    return lhs.hashIgnoreCase() == rhs.hashIgnoreCase() && lhs.string() == rhs.string();
}

template <class LHS, class RHS>
bool operator==(const LHS& lhs, const HashedIgnoreCase<RHS>& rhs) {
    return lhs.string().hashIgnoreCase() == rhs.hashIgnoreCase() && lhs.string() == rhs.string();
}

template <class LHS, class RHS>
bool operator==(const HashedIgnoreCase<LHS>& lhs, const RHS& rhs) {
    return lhs.hashIgnoreCase() == rhs.string().hashIgnoreCase() && lhs.string() == rhs.string();
}

template <class LHS, class RHS>
inline bool operator>=(const HashedIgnoreCase<LHS>& lhs, const HashedIgnoreCase<RHS>& rhs) {
    return !(lhs < rhs);
}
template <class LHS, class RHS>
inline bool operator<=(const HashedIgnoreCase<LHS>& lhs, const HashedIgnoreCase<RHS>& rhs) {
    return !(lhs > rhs);
}
template <class LHS, class RHS>
inline bool operator!=(const HashedIgnoreCase<LHS>& lhs, const HashedIgnoreCase<RHS>& rhs) {
    return !(lhs == rhs);
}

// STRING CONTAINER

struct StringContainerSharedContent {
    StringT s;
    concurrent_count refcount = 0;
    std::atomic<unsigned int> hash = {0};
    std::atomic<unsigned int> hashIgnoreCase = {0};
    std::atomic<bool> hashValid = {false};
    std::atomic<bool> hashIgnoreCaseValid = {false};
    friend class StringContainer;
    StringContainerSharedContent(StringLength bytes);
    StringContainerSharedContent(const Hashed<String>& str);
    StringContainerSharedContent(const HashedIgnoreCase<String>& str);
    StringContainerSharedContent(const StringOperation& op);

    typedef shared_object<StringContainerSharedContent> Ref;
    static Ref create(const StringOperation& op);
    static Ref create(const Hashed<String>& str);
    static Ref create(const HashedIgnoreCase<String>& str);
    static Ref create(StringLength bytes);

    StringT& string() {
        return s;
    }
    unsigned int getHash() {
        if (!hashValid) {
            unsigned h = s.hash();
            hash = h;
            hashValid = true;
            return h;
        }
        return hash;
    }
    unsigned int getHashIgnoreCase() {
        if (!hashIgnoreCaseValid) {
            unsigned h = s.hashIgnoreCase();
            hashIgnoreCase = h;
            hashIgnoreCaseValid = true;
            return h;
        }
        return hashIgnoreCase;
    }

    char extra[0];
};

static_assert(std::is_standard_layout<StringContainerSharedContent>::value, "StringContainerSharedContent should have a standard layout so it will use less memory (as the location of extra is known this memory can be used");

class StringContainer;

bool operator<(const StringContainer& lhs, const StringContainer& rhs);
bool operator==(const StringContainer& a, const StringContainer& b);

class StringContainer {
    static const Hashed<StringT> empty;
    static const HashedIgnoreCase<StringT> emptyIgnoreCase;
    StringContainerSharedContent::Ref ptr;
  public:
    class less {
      public:
        bool operator()(const Hashed<String>& lhs, const Hashed<String>& rhs) const {
            return lhs.less(rhs);
        }
        typedef void is_transparent;
    };
    class lessIgnoreCase {
      public:
        bool operator()(const HashedIgnoreCase<String>& lhs, const HashedIgnoreCase<String>& rhs) const {
            return lhs.lessIgnoreCase(rhs);
        }
        typedef void is_transparent;
    };
    class lessIgnoreCaseAlfa {
      public:
        bool operator()(const String& lhs, const String& rhs) const {
            return lhs.lessIgnoreCaseAlfa(rhs);
        }
        typedef void is_transparent;
    };
    StringContainer() {
    }
    StringContainer(const StringContainer& str) = default;
    StringContainer(StringLength bytes) : ptr(StringContainerSharedContent::create(bytes)) {
    }
    StringContainer(const StringOperation& string) : ptr(string.size() > 0 ? StringContainerSharedContent::create(string) : nullptr) {
    }
    template <class S>
    StringContainer(const Hashed<S>& string) : ptr(string->length() > 0 ? StringContainerSharedContent::create(string) : nullptr) {
    }
    template <class S>
    StringContainer(const HashedIgnoreCase<S>& string) : ptr(string->length() > 0 ? StringContainerSharedContent::create(string) : nullptr) {
    }

    StringContainer(const char* str) : StringContainer(String(str)) {
    }
    template <StringLength N>
    StringContainer(const char (&str)[N]) : StringContainer(String(str)) {
    }
    StringContainer(const String& string) : StringContainer(string.op()) {
    }
    template <class S>
    StringContainer(const StableHashed<S>& string) : StringContainer(Hashed<S>(string)) {
    }
    template <class S>
    StringContainer(const StableHashedIgnoreCase<S>& string) : StringContainer(HashedIgnoreCase<S>(string)) {
    }

    StringContainer& operator=(const StringContainer& string) = default;
    StringContainer& operator=(const char* str) {
        return *this = String(str);
    }
    template <StringLength N>
    StringContainer& operator=(const char (&str)[N]) {
        return *this = String(str);
    }
    StringContainer& operator=(const String& string) {
        return *this = string.op();
    }
    template <class S>
    StringContainer& operator=(const StableHashed<S>& string) {
        return *this = Hashed<S>(string);
    }
    template <class S>
    StringContainer& operator=(const Hashed<S>& string) {
        ptr = string->length() > 0 ? StringContainerSharedContent::create(string) : nullptr;
        return *this;
    }
    StringContainer& operator=(const StringOperation& string) {
        ptr = string.size() > 0 ? StringContainerSharedContent::create(string) : nullptr;
        return *this;
    }

    template <class S>
    bool equalsIgnoreCase(const HashedIgnoreCase<S>& b) const {
        return hashedStringIgnoreCase().equalsIgnoreCase(b);
    }
    bool equalsIgnoreCase(const StringContainer& b) const {
        return hashedStringIgnoreCase().equalsIgnoreCase(b.hashedStringIgnoreCase());
    }
    operator Hashed<StringT>() const {
        return hashedString();
    }
    operator Hashed<String>() const {
        return hashedString();
    }
    operator HashedIgnoreCase<StringT>() const {
        return hashedStringIgnoreCase();
    }
    operator HashedIgnoreCase<String>() const {
        return hashedStringIgnoreCase();
    }
    operator StringT() const {
        return string();
    }
    operator String() const {
        return string();
    }
    explicit operator bool() const {
        return (bool)string();
    }
    const StringT& string() const {
        return ptr ? ptr->string() : empty.string();
    }
    Hashed<StringT> hashedString() const {
        if (!ptr)
            return empty;
        return {ptr->string(), ptr->getHash()};
    }
    HashedIgnoreCase<StringT> hashedStringIgnoreCase() const {
        if (!ptr)
            return emptyIgnoreCase;
        return {ptr->string(), ptr->getHashIgnoreCase()};
    }
    const StringT& operator*() const {
        return string();
    }
    const StringT* operator->() const {
        return &string();
    }
    StringOperationString op() const {
        return string().op();
    }
    unsigned int hashIgnoreCase() const {
        if (!ptr)
            return emptyIgnoreCase.hashIgnoreCase();
        return ptr->getHashIgnoreCase();
    }
    unsigned int hash() const {
        if (!ptr)
            return empty.hash();
        return ptr->getHash();
    }
};

// CONVERSIONS
class precision {
    unsigned short p;
  public:
    precision(unsigned short p = 1) : p(p) {
    }
    unsigned short getPrecision() const {
        return p;
    }
};

class capital {
    bool capitals;
  public:
    capital(bool capitals) : capitals(capitals) {
    }
    bool getCapitals() const {
        return capitals;
    }
};

template <class T,
          unsigned int bufferSize = std::numeric_limits<T>::digits10+1+std::numeric_limits<T>::is_signed,
          int base = 10>
class NumberToStringObject : public StringOperation {
    char buffer[bufferSize];
    unsigned char pos;
    template <int v>
    struct Int2Type {
        enum { value = v };
    };

    static_assert(base >= 10, "base should be at least 10, otherwise the allocated storage could be too small (based on digits10)");
    static_assert(base <= 64, "base should be smaller or equal to 64");

    inline char chr(unsigned char r) {
        return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_."[r];
    }

    void doConversion(T i, unsigned int precision, char buffer[], unsigned char& pos, Int2Type<false>) {
        while (i != 0) {
            unsigned char r = i % base;
            buffer[--pos] = chr(r);
            i /= base;
        }
        while (pos > 0 && (bufferSize - pos) < precision)
            buffer[--pos] = '0';
    }

    void doConversion(T i, unsigned int precision, char buffer[], unsigned char& pos, Int2Type<true>) {
        if (i >= 0) {
            doConversion(i, precision, buffer, pos, Int2Type<false>());
            return;
        }
        while (i != 0) {
            unsigned char r = -(i % base);
            buffer[--pos] = chr(r);
            i /= base;
        }
        while (pos > 0 && (bufferSize - pos) < precision)
            buffer[--pos] = '0';
        buffer[--pos] = '-';
    }

  public:
    NumberToStringObject(T i, precision p = precision(1)) : StringOperation(0), /*buffer(),*/ pos(bufferSize) {
        // Potential helpful hints: http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
        doConversion(i, p.getPrecision(), buffer, pos, Int2Type<std::numeric_limits<T>::is_signed>());
        _size = bufferSize - pos;
    }
    virtual StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
        StringLength l = std::min((StringLength)(_size - offset), bufferLength);
        const char* representation = &this->buffer[pos+offset];
        smallmemcpy(buffer, representation, l);
        return l;
    }
};

template <class T,
          unsigned int bufferSize = std::numeric_limits<T>::digits10+1+std::numeric_limits<T>::is_signed>
class FloatToStringObject : public StringOperation {
    char buffer[bufferSize];

    template <class _CharT, class _Traits>
    class fixed_stringbuf
            : public std::basic_streambuf<_CharT, _Traits>
    {
        using Base = std::basic_streambuf<_CharT, _Traits>;
    public:
        typedef _CharT                         char_type;
        typedef _Traits                        traits_type;
        typedef typename traits_type::int_type int_type;
        typedef typename traits_type::pos_type pos_type;
        typedef typename traits_type::off_type off_type;

    private:
        char* addr = nullptr;
        size_t size = 0;
        mutable char_type* __hm_ = 0;
        std::ios_base::openmode __mode_ = std::ios_base::out;

    public:
        explicit fixed_stringbuf(char* addr, size_t size) : addr(addr), size(size)
        {
            Base::setp(&addr[0], &addr[size]);
        }

        String getString() {
            return {addr, StringLength(Base::pptr()-addr)};
        }
    };

  public:
    FloatToStringObject(T i, precision p = precision(1)) : StringOperation(0) {
        fixed_stringbuf<char, std::char_traits<char>> buf {buffer, bufferSize};
        std::ostream s {&buf};
        s.precision(99);
        s.unsetf(std::ios::floatfield);
        s << i;
        _size = buf.getString().length();
    }
    virtual StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
        StringLength l = std::min((StringLength)(_size - offset), bufferLength);
        const char* representation = &this->buffer[offset];
        smallmemcpy(buffer, representation, l);
        return l;
    }
};

static inline char chrToHex(unsigned char r) {
    checkAssert(r < 16);
    return "0123456789abcdef"[r];
}

template <int N, int bufferSize = 2*N>
class ArrayToHexStringObject : public StringOperation {
    char buffer[bufferSize];
  public:
    ArrayToHexStringObject(const unsigned char (&in)[N]) : StringOperation(0) {
        for (int i = 0; i < N; ++i) {
            buffer[_size++] = chrToHex(in[i] >> 4);
            buffer[_size++] = chrToHex(in[i] & 0xF);
        }
    }
    virtual StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
        StringLength l = std::min((StringLength)(_size - offset), bufferLength);
        const char* representation = &this->buffer[offset];
        memcpywrap(buffer, representation, l);
        return l;
    }
};

class StringToHexStringObject : public StringOperation {
    String str;
  public:
    StringToHexStringObject(const String& str) : StringOperation(str.size()*2), str(str) {
    }
    virtual StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
        StringLength l = std::min((StringLength)(_size - offset), bufferLength);
        for (StringLength i = 0; i < l; ++i) {
            StringLength pos = offset + i;
            if (pos % 2 == 0) {
                buffer[i] = chrToHex(((unsigned char)str[pos/2]) >> 4);
            } else {
                buffer[i] = chrToHex(((unsigned char)str[pos/2]) & 0xF);
            }
        }
        return l;
    }
};

class StringFromHexStringObject : public StringOperation {
    String str;

public:
    StringFromHexStringObject(const String& str) : StringOperation(str.size()/2), str(str) {
    }
    virtual StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
        StringLength l = std::min((StringLength)(_size - offset), bufferLength);
        for (StringLength i = 0; i < l; ++i) {
            StringLength pos = offset + i;
            buffer[i] = StringUtil::FromHex(str[pos*2], str[pos*2+1]);
        }
        return l;
    }
};


template <typename T>
class StringConversion {
  public:
    typedef T ConversionResult;
    template <typename... Args>
    static ConversionResult convert(const T& s, Args&& ... args) {
        return {s, std::forward<Args>(args)...};
    }
};

#define NUMBER_TO_STRING(type) \
template <> \
class StringConversion< type > { \
public: \
    typedef NumberToStringObject< type > ConversionResult; \
    template <typename... Args> \
    static ConversionResult convert(const type& s, Args&&... args) { \
        return {s, std::forward<Args>(args)...}; \
    } \
}

NUMBER_TO_STRING(char);
NUMBER_TO_STRING(unsigned char);
NUMBER_TO_STRING(short);
NUMBER_TO_STRING(unsigned short);
NUMBER_TO_STRING(int);
NUMBER_TO_STRING(unsigned int);
NUMBER_TO_STRING(long);
NUMBER_TO_STRING(unsigned long);
NUMBER_TO_STRING(long long);
NUMBER_TO_STRING(unsigned long long);

#define FLOAT_TO_STRING(type) \
template <> \
class StringConversion< type > { \
public: \
    typedef FloatToStringObject< type > ConversionResult; \
    template <typename... Args> \
    static ConversionResult convert(const type& s, Args&&... args) { \
        return {s, std::forward<Args>(args)...}; \
    } \
}

FLOAT_TO_STRING(float);
FLOAT_TO_STRING(double);
FLOAT_TO_STRING(long double);

template <>
class StringConversion<std::string> {
  public:
    class STLString : public StringOperation {
      public:
        const std::string& str;
        STLString(const std::string& str) : StringOperation(str.size()), str(str) {
        }

        virtual StringLength _size() const {
            return str.length();
        }
        virtual StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
            StringLength l = std::min((StringLength)(_size() - offset), bufferLength);
            const char* representation = &str.c_str()[offset];
            memcpywrap(buffer, representation, l);
            return l;
        }
    };

    typedef STLString ConversionResult;
    static ConversionResult convert(const std::string& s) {
        return s;
    }
};

class CharString : public StringOperation {
  public:
    String str;
    CharString(const char* str) : StringOperation(0), str(str) {
        _size = this->str.length();
    }
    virtual StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
        StringLength l = std::min((StringLength)(_size - offset), bufferLength);
        const char* representation = &str.pointer()[offset];
        memcpywrap(buffer, representation, l);
        return l;
    }
};

template <>
class StringConversion<const char*> {
  public:
    typedef CharString ConversionResult;
    static ConversionResult convert(const char* s) {
        return s;
    }
};

template <class S>
class StringConversion<Hashed<S>> {
  public:
    typedef String ConversionResult;
    static ConversionResult convert(const Hashed<S>& s) {
        return s;
    }
};


template <>
class StringConversion<char*> {
  public:
    typedef CharString ConversionResult;
    static ConversionResult convert(char* s) {
        return s;
    }
};

template <>
class StringConversion<StringContainer> {
  public:
    typedef String ConversionResult;
    static ConversionResult convert(const StringContainer& s) {
        return s.string();
    }
};

template <StringLength N>
class StringConversion<char[N]> {
  public:
    class FixedCharString : public StringOperation {
      public:
        const char (&str)[N];
        constexpr FixedCharString(const char (&str)[N]) : StringOperation(N-1), str(str) {
        }
        virtual StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
            StringLength l = std::min((StringLength)(N-1 - offset), bufferLength);
            const char* representation = &str[offset];
            memcpywrap(buffer, representation, l);
            return l;
        }
    };
    typedef FixedCharString ConversionResult;
    static ConversionResult convert(const char (&s)[N]) {
        return {s};
    }
};

template <>
class StringConversion<bool> {
  public:
    class BoolString : public StringOperation {
        bool value;
      public:
        constexpr BoolString(bool value) : StringOperation(value ? 4 : 5), value(value) {
        }

        virtual StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
            StringLength l = std::min((StringLength)(_size - offset), bufferLength);
            //const char *representation = &(value ? "true" : "false")[offset];
            const char* representation = (value ? "true" : "false");
            smallmemcpy(buffer, representation, l);
            return l;
        }
    };
    typedef BoolString ConversionResult;
    static ConversionResult convert(const bool& s) {
        return s;
    }
};

// conversion functions

template <class T>
typename StringConversion<T>::ConversionResult ToString(const T& t) {
    return StringConversion<T>::convert(t);
}

template <class T, typename... Args>
typename StringConversion<T>::ConversionResult ToString(const T& t, Args&& ... p) {
    return StringConversion<T>::convert(t, std::forward<Args...>(p...));
}

template <class T>
NumberToStringObject<T,std::numeric_limits<T>::digits/4+1+std::numeric_limits<T>::is_signed,16> ToHexString(const T& t, precision p = precision()) {
    return NumberToStringObject<T,std::numeric_limits<T>::digits/4+1+std::numeric_limits<T>::is_signed,16>(t, p);
}

template <int N>
ArrayToHexStringObject<N> ArrayToHexString(const char (&in)[N]) {
    return ArrayToHexStringObject<N>((const unsigned char(&)[N])in);
}

template <int N>
ArrayToHexStringObject<N> ArrayToHexString(const unsigned char (&in)[N]) {
    return ArrayToHexStringObject<N>(in);
}

inline StringToHexStringObject ToHexString(const String& str) {
    return StringToHexStringObject(str);
}

inline StringFromHexStringObject FromHexString(const String& str) {
    return StringFromHexStringObject(str);
}

template <class T>
NumberToStringObject<T,std::numeric_limits<T>::digits/6+1+std::numeric_limits<T>::is_signed,64> ToMaxString(const T& t, precision p = precision()) {
    return NumberToStringObject<T,std::numeric_limits<T>::digits/6+1+std::numeric_limits<T>::is_signed,64>(t, p);
}

// OPERATIONS
template <class A, class B>
class StringConcatCopy : public StringOperation {
    const A a;
    const B b;
  public:
    constexpr StringConcatCopy(const A& a, const B& b) : StringOperation(a.size() + b.size()), a(a), b(b) {
    }
    StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
        StringLength l = 0;
        StringLength lengthA = a.size();
        if (offset < lengthA) {
            l += a.writeTo(buffer, bufferLength, offset);
            l += b.writeTo(buffer + l, bufferLength - l, 0);
        } else {
            l += b.writeTo(buffer + l, bufferLength - l, offset - lengthA);
        }
        return l;
    }
};

template <class FirstString, class B>
class StringConcatRef : public StringOperation {
    const FirstString& a;
    const B b;
  public:
    constexpr StringConcatRef(const FirstString& a, const B& b) : StringOperation(a.size() + b.size()), a(a), b(b) {
    }
    StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
        StringLength l = 0;
        StringLength lengthA = a.size();
        if (offset < lengthA) {
            l += a.writeTo(buffer, bufferLength, offset);
            l += b.writeTo(buffer + l, bufferLength - l, 0);
        } else {
            l += b.writeTo(buffer + l, bufferLength - l, offset - lengthA);
        }
        return l;
    }
};

template <class B>
constexpr StringConcatRef<StringOperation, typename StringConversion<B>::ConversionResult> operator+(const StringOperation& a, const B& b) {
    return {a, ToString(b)};
}

template <class B>
constexpr StringConcatRef<String, typename StringConversion<B>::ConversionResult> operator+(const String& a, const B& b) {
    return {a, ToString(b)};
}

template <class FirstString, class State>
class StateConcat : public StringOperation {
    const FirstString&  t;
    State st;
  public:
    constexpr StateConcat(const FirstString& t, State st) : StringOperation(t.size()), t(t), st(st) {
    }
    StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
        return t.writeTo(buffer, bufferLength, offset);
    }
    operator State() const {
        return st;
    }
};

inline StateConcat<StringOperation, precision> operator+(const StringOperation& a, const precision& b) {
    return StateConcat<StringOperation, precision>(a, b);
}

inline StateConcat<StringOperation, capital> operator+(const StringOperation& a, const capital& b) {
    return StateConcat<StringOperation, capital>(a, b);
}

inline StateConcat<String, precision> operator+(const String& a, const precision& b) {
    return StateConcat<String, precision>(a, b);
}

inline StateConcat<String, capital> operator+(const String& a, const capital& b) {
    return StateConcat<String, capital>(a, b);
}

template <class FirstString, class State, class B>
constexpr StringConcatRef<StringOperation, typename StringConversion<B>::ConversionResult> operator+(const StateConcat<FirstString,State>& a, const B& b) {
    return {a, ToString(b, (State)a)};
}

/*
template <class FirstString, class State, class B>
constexpr StateConcat<StringConcat<StringOperation, typename StringConversion<B>::ConversionResult>, State> operator+(const StateConcat<FirstString,State>& a, const B& b) {
    return {StringConcat<StringOperation, typename StringConversion<B>::ConversionResult>(a, ToString(b, (State)a)), (State)a};
}
*/

template <class FirstString>
class StringMultiply : public StringOperation {
    const FirstString& a;
    const short i;
  public:
    StringMultiply(const FirstString& a, short i) : StringOperation(i* a._size), a(a), i(i) {
    }
    StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
        if (offset >= _size)
            return 0;
        short k = 0; // how many times the string is already written out
        if (offset < _size) {
            SignedStringLength aSize = a._size;
            k += offset / aSize;
            StringLength l = a.writeTo(buffer, bufferLength, offset % aSize);
            buffer = &buffer[l];
            bufferLength -= l;
        }
        // write once full
        StringLength first = a.writeTo(buffer, bufferLength, 0);
        k++;
        StringLength l = first;
        // repeat if needed
        for (; l < bufferLength && k < i; k++) {
            StringLength len = std::min(first, bufferLength - l);
            memcpy(&buffer[l], buffer, len);
            l += len;
        }
        return l;
    }
};
template <class FirstString>
StringMultiply<FirstString> operator*(const FirstString& a, short i) {
    return StringMultiply<FirstString>(a, i);
}

template <class FirstString>
class Substring : public StringOperation {
    const FirstString& a;
    StringLength offset;
  public:
    Substring(const FirstString& a, StringLength offset, StringLength length = std::numeric_limits<StringLength>::max()) : StringOperation(std::min(length, StringLength(a.size()-offset))), a(a), offset(offset) {
    }
    StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset2) const override {
        return a.writeTo(buffer, std::min(bufferLength, _size), offset+offset2);
    }
};

template <class FirstString>
class EscapeOperation : public StringOperation {
    const FirstString& a;
  public:
    constexpr EscapeOperation(const FirstString& a) : StringOperation(a.size()), a(a) {
    }
    StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
        StringLength size = a.writeTo(buffer, bufferLength, offset);
        char* end = buffer + size;
        for (char* it = buffer; it < end; ++it)
            if (*it == '\'')
                *it = '"';
        return size;
    }
};

template <class FirstString>
EscapeOperation<FirstString> Escape(const FirstString& str) {
    return EscapeOperation<FirstString>(str);
}

template <class FirstString>
class UppercaseOperation : public StringOperation {
    const FirstString& a;
  public:
    constexpr UppercaseOperation(const FirstString& a) : StringOperation(a.size()), a(a) {
    }
    StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
        StringLength size = a.writeTo(buffer, bufferLength, offset);
        char* end = buffer + size;
        for (char* it = buffer; it < end; ++it)
            *it = toUpper(*it);
        return size;
    }
};

template <class FirstString>
UppercaseOperation<FirstString> ToUppercase(const FirstString& str) {
    return UppercaseOperation<FirstString>(str);
}

template <class FirstString>
class LowercaseOperation : public StringOperation {
    const FirstString& a;
  public:
    constexpr LowercaseOperation(const FirstString& a) : StringOperation(a.size()), a(a) {
    }
    StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
        StringLength size = a.writeTo(buffer, bufferLength, offset);
        char* end = buffer + size;
        for (char* it = buffer; it < end; ++it)
            *it = toLower(*it);
        return size;
    }
};

template <class FirstString>
LowercaseOperation<FirstString> ToLowercase(const FirstString& str) {
    return LowercaseOperation<FirstString>(str);
}

/*
class StringEscape : public StringOperation {
    const StringOperation& a,
    off_t offset;
    StringLength len;
    StringLength extra;
    StringEscape(const StringOperation& a, off_t offset, StringLength length) : a(a), offset(offset), len(std::min(length, StringLength(a.length()-offset))), extra(0) {
        for (const_iterator it = begin(); it != end(); ++it) {
            switch (*it) {
            case '-':
            case '~':
            case '.':
            case '/':
            case ',':
            case '=':
            case '&':
            //case ' ':
                continue;
            }
            if ('a' <= *it && *it <= 'z')
                continue;
            if ('A' <= *it && *it <= 'Z')
                continue;
            if ('0' <= *it && *it <= '9')
                continue;
            extra++;
        }
    }
    SignedStringLength length() const {
        return len+extra;
    }
    StringLength writeTo(char *buffer, StringLength bufferLength, off_t offset2) const {
        return a.writeTo(buffer, std::min(bufferLength, len), offset+offset2);
    }
};
*/

// RAW stuff
class _RawByte : public StringOperation {
  public:
    unsigned char n;
    _RawByte(unsigned char n) : StringOperation(1), n(n) {
    }
    virtual StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
        StringLength retval = 0;
        retval += StringOperation::writeToHelper(buffer, bufferLength, offset, (char*)&n, 1);
        return retval;
    }
};

inline _RawByte ToRawU8(unsigned char n) {
    return {n};
}

template <class T>
class _RawNumber : public StringOperation {
  public:
    T n;
    _RawNumber(T n) : StringOperation(sizeof(T)) {
        this->n = hton(n);
    }
    virtual StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
        StringLength retval = 0;
        retval += StringOperation::writeToHelper(buffer, bufferLength, offset, (char*)&n, sizeof(T));
        return retval;
    }
};

inline _RawNumber<unsigned short> ToRawU16(unsigned short n) {
    return {n};
}

inline _RawNumber<unsigned int> ToRawU32(unsigned int n) {
    return {n};
}

inline _RawNumber<unsigned long long> ToRawU64(unsigned long long n) {
    return {n};
}

template <class T>
class _RawString : public StringOperation {
  public:
    T str;
    _RawString(const T& str) : StringOperation(0), str(str) {
        _size = this->str.size() + 4;
    }
    virtual StringLength writeTo(char* __restrict__ buffer, StringLength bufferLength, StringLength offset) const override {
        char sizeBuffer[4];
        *alias_cast<unsigned int*>(sizeBuffer) = htonl(this->str.size());
        StringLength retval = 0;
        retval += StringOperation::writeToHelper(buffer, bufferLength, offset, sizeBuffer, 4);
        retval += StringOperation::writeToHelper(buffer, bufferLength, offset, str);
        return retval;
    }
};

template <class T>
_RawString<typename StringConversion<T>::ConversionResult> ToRawString(const T& t) {
    return {ToString(t)};
}

template <class T>
StringConcatCopy<_RawNumber<unsigned int>, typename StringConversion<T>::ConversionResult> ToRawStringAlt(const T& t) {
    return {ToRawU32(t.size()), ToString(t)};
}


// remaining

template <StringLength N> String StringOperation::operator()(char (&buffer)[N]) const {
    StringLength len = writeTo(buffer, N, 0);
    return String(buffer, len);
}

template <StringLength N> StringT StringOperation::c_str(char (&buffer)[N]) const {
    StringLength len = writeTo(buffer, N-1, 0);
    buffer[len] = '\0';
    return StringT(buffer, len);
}

template <StringLength N>
const String ConstString(const char (&string)[N]) {
    return String((char*)string, N-1);
}

class StringNameCompare {
  public:
    bool operator()(const String& lhs, const String& rhs) const;
};

template <class T>
struct lessAlfa : std::binary_function<T, T, bool>, std::binary_function<unsigned char,unsigned char,bool> {
    bool operator() (const T& lhs, const T& rhs) const {
        return String(lhs).lessAlfa(rhs);
    }
};

template <class T>
struct lessIgnoreCaseAlfa : std::binary_function<T, T, bool>, std::binary_function<unsigned char,unsigned char,bool> {
    bool operator() (const T& lhs, const T& rhs) const {
        return String(lhs).lessIgnoreCaseAlfa(rhs);
    }
};

template <class T>
struct lessIgnoreCase {
    bool operator() (const T& lhs, const T& rhs) const {
        return String(lhs).lessIgnoreCase(rhs);
    }
};

template <class S>
struct lessIgnoreCase<HashedIgnoreCase<S>> : public lessIgnoreCase<String> {
    bool operator() (const HashedIgnoreCase<S>& lhs, const HashedIgnoreCase<S>& rhs) const {
        if (lhs.hashIgnoreCase() != rhs.hashIgnoreCase())
            return lhs.hashIgnoreCase() < rhs.hashIgnoreCase();
        return lessIgnoreCase<String>::operator()(lhs.string(), rhs.string());
    }
};

}
}

constexpr bitpowder::lib::StringT operator "" _S(const char* value, size_t size) {
    return {value, bitpowder::lib::StringLength(size)};
}

constexpr bitpowder::lib::StableHashed<bitpowder::lib::StringT> operator "" _HS(const char* value, size_t size) {
    return {value, bitpowder::lib::StringLength(size)};
}

constexpr bitpowder::lib::StableHashedIgnoreCase<bitpowder::lib::StringT> operator "" _HSIC(const char* value, size_t size) {
    return {value, bitpowder::lib::StringLength(size)};
}

namespace std {
inline ostream& operator<< (ostream& out, const bitpowder::lib::String& str) {
    out.write(str.pointer(), str.size());
    return out;
}
inline ostream& operator<< (ostream& out, const bitpowder::lib::StringOperation& op) {
    bitpowder::lib::StaticMemoryPool<16384> mp;
    return out << op(mp);
}

template <>
struct hash<bitpowder::lib::String> : public unary_function<bitpowder::lib::String, unsigned int> {
    unsigned int operator()(const bitpowder::lib::String& str) const {
        return str.hashIgnoreCase();
    }
};

template <class S>
struct hash<bitpowder::lib::Hashed<S>> : public unary_function<bitpowder::lib::Hashed<S>, unsigned int> {
    unsigned int operator()(const bitpowder::lib::Hashed<S>& str) const {
        return str.hash();
    }
};

template <class S>
struct hash<bitpowder::lib::HashedIgnoreCase<S>> : public unary_function<bitpowder::lib::HashedIgnoreCase<S>, unsigned int> {
    unsigned int operator()(const bitpowder::lib::HashedIgnoreCase<S>& str) const {
        return str.hashIgnoreCase();
    }
};

template <>
struct hash<bitpowder::lib::StringContainer> : public unary_function<bitpowder::lib::StringContainer, unsigned int> {
    unsigned int operator()(const bitpowder::lib::StringContainer& str) const {
        return str->hashIgnoreCase();
    }
};

}

#endif
