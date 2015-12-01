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

#include "simplestring.h"
#include <random>

/* djb2 algorithm */
/*
unsigned int strhash(const char *str) {
    unsigned char c;
    unsigned int hash = 5381;
    while ((c = *str++))
        hash = ((hash << 5) + hash) ^ c;
    return hash;
    //return *str == 0 ? 1 : *str;
}
*/

char fromDigit(char tmp);
char toDigit(unsigned char tmp);

/*
const char *base(const char *str) {
    const char *last = str ? strrchr(str, '/') : NULL;
    return last ? &last[1] : str;
}

const char *extension(const char *str) {
    str = base(str);
    const char *last = str ? strrchr(str, '.') : NULL;
    return last ? &last[1] : NULL;
}

char *strsep2(char **s, const char *delim, char *seperator) {
    char *retval = *s;
    StringLength pos = strcspn(*s, delim);
    if (seperator != NULL)
        *seperator = (*s)[pos];
    if ((*s)[pos] == '\0') {
        *s = NULL;
    } else {
        (*s)[pos] = '\0';
        *s = (*s)[pos+1] ? &(*s)[pos+1] : NULL;
    }
    return retval;
}

char asciHexToChar(const char* hex) {
    return digit(hex[0]) << 4 | digit(hex[1]);
}

void unescapeString(char *queryString) {
    if (queryString == NULL)
        return;
    char *src = queryString;
    // faster loop till a % is encounterd
    while (*src != '\0' && *src != '%') {
        if (*src == '+')
            *src = ' ';
        src++;
    }
    char *dst = src;
    while (*src != '\0') {
        // convert '+' to a space
        if (*src == '+')
            *dst = ' ';
        // covert something with % to their real value
        else if (*src == '%' && *(src+1) != 0 && *(src+2) != 0) {
            *dst = digit(src[1]) << 4 | digit(src[2]);
            src+=2;
            // ohterwise copy character
        } else
            *dst = *src;
        src++;
        dst++;
    }
    *dst = '\0';
}
*/

unsigned char fromDigit(unsigned char tmp) {
    if (tmp >= 'A' && tmp <= 'F')
        return 10 + tmp - 'A';
    else if (tmp >= 'a' && tmp <= 'f')
        return 10 + tmp - 'a';
    else if (tmp >= '0' && tmp <= '9')
        return tmp - '0';
    return 0;
}

char toDigit(unsigned char value) {
    return "0123456789ABCDEF"[value];
}

namespace bitpowder {
namespace lib {
/*
 * Hashing: good overview: http://programmers.stackexchange.com/questions/49550/which-hashing-algorithm-is-best-for-uniqueness-and-speed
 *
    // SuperFastHash: http://www.azillionmonkeys.com/qed/hash.html BUT "SuperFastHash has very poor collision properties, which have been documented elsewhere"
    // Murmur2: http://www.team5150.com/~andrew/noncryptohashzoo/Murmur2.html
    inline unsigned int Murmur2( const unsigned char *key, unsigned int len, unsigned int seed ) {
            const unsigned int m = 0x5bd1e995;
            const unsigned char r = 24;
            unsigned int h = len + seed;
            const unsigned char * data = (const unsigned char *)key;

            for ( ; len >= 4; len -= 4, data += 4 ) {
                    unsigned int k = *(unsigned int *)data * m;
                    k ^= k >> r;
                    k *= m;
                    h = ( h * m ) ^ k;
            }

            switch ( len ) {
                    case 3: h ^= data[2] << 16;
                    case 2: h ^= data[1] << 8;
                    case 1: h ^= data[0];
                                    h *= m;
                    default:;
            }

            h ^= h >> 13;
            h *= m;
            h ^= h >> 15;
            return h;
    }
    typedef u_int32_t u32;
    typedef u_int8_t u8;
    typedef u_int64_t u64;
    // CrapWow: http://www.team5150.com/~andrew/noncryptohashzoo/CrapWow.html
    inline u32  CrapWow( const u8 *key, u32 len, u32 seed ) {
    #if !defined(__LP64__) && !defined(_MSC_VER) && ( defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) )
            // esi = k, ebx = h
            u32 hash;
            asm(
                    "leal 0x5052acdb(%%ecx,%%esi), %%esi\n"
                    "movl %%ecx, %%ebx\n"
                    "cmpl $8, %%ecx\n"
                    "jb DW%=\n"
            "QW%=:\n"
                    "movl $0x5052acdb, %%eax\n"
                    "mull (%%edi)\n"
                    "addl $-8, %%ecx\n"
                    "xorl %%eax, %%ebx\n"
                    "xorl %%edx, %%esi\n"
                    "movl $0x57559429, %%eax\n"
                    "mull 4(%%edi)\n"
                    "xorl %%eax, %%esi\n"
                    "xorl %%edx, %%ebx\n"
                    "addl $8, %%edi\n"
                    "cmpl $8, %%ecx\n"
                    "jae QW%=\n"
            "DW%=:\n"
                    "cmpl $4, %%ecx\n"
                    "jb B%=\n"
                    "movl $0x5052acdb, %%eax\n"
                    "mull (%%edi)\n"
                    "addl $4, %%edi\n"
                    "xorl %%eax, %%ebx\n"
                    "addl $-4, %%ecx\n"
                    "xorl %%edx, %%esi\n"
            "B%=:\n"
                    "testl %%ecx, %%ecx\n"
                    "jz F%=\n"
                    "shll $3, %%ecx\n"
                    "movl $1, %%edx\n"
                    "movl $0x57559429, %%eax\n"
                    "shll %%cl, %%edx\n"
                    "addl $-1, %%edx\n"
                    "andl (%%edi), %%edx\n"
                    "mull %%edx\n"
                    "xorl %%eax, %%esi\n"
                    "xorl %%edx, %%ebx\n"
            "F%=:\n"
                    "leal 0x5052acdb(%%esi), %%edx\n"
                    "xorl %%ebx, %%edx\n"
                    "movl $0x5052acdb, %%eax\n"
                    "mull %%edx\n"
                    "xorl %%ebx, %%eax\n"
                    "xorl %%edx, %%esi\n"
                    "xorl %%esi, %%eax\n"
                    : "=a"(hash), "=c"(len), "=S"(len), "=D"(key)
                    : "c"(len), "S"(seed), "D"(key)
                    : "%ebx", "%edx", "cc"
            );
            return hash;
    #else
            #define cwfold( a, b, lo, hi ) { p = (u32)(a) * (u64)(b); lo ^= (u32)p; hi ^= (u32)(p >> 32); }
            #define cwmixa( in ) { cwfold( in, m, k, h ); }
            #define cwmixb( in ) { cwfold( in, n, h, k ); }

            const u32 m = 0x57559429, n = 0x5052acdb, *key4 = (const u32 *)key;
            u32 h = len, k = len + seed + n;
            u64 p;

            while ( len >= 8 ) { cwmixb(key4[0]) cwmixa(key4[1]) key4 += 2; len -= 8; }
            if ( len >= 4 ) { cwmixb(key4[0]) key4 += 1; len -= 4; }
            if ( len ) { cwmixa( key4[0] & ( ( 1 << ( len * 8 ) ) - 1 ) ) }
            cwmixb( h ^ (k + n) )
            return k ^ h;
    #endif
    }
    */
/*
//////// BEGIN MurmurHash3_x86_32 from https://code.google.com/p/smhasher/source/browse/trunk/MurmurHash3.cpp
#define FORCE_INLINE inline __attribute__((always_inline))
inline uint32_t rotl32 ( uint32_t x, int8_t r )
{
  return (x << r) | (x >> (32 - r));
}
inline uint64_t rotl64 ( uint64_t x, int8_t r )
{
  return (x << r) | (x >> (64 - r));
}
#define ROTL32(x,y)     rotl32(x,y)
#define ROTL64(x,y)     rotl64(x,y)
#define BIG_CONSTANT(x) (x##LLU)
//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here
FORCE_INLINE uint32_t getblock32 ( const uint32_t * p, int i )
{
  return p[i];
}
FORCE_INLINE uint64_t getblock64 ( const uint64_t * p, int i )
{
  return p[i];
}
//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche
FORCE_INLINE uint32_t fmix32 ( uint32_t h )
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}
//----------
FORCE_INLINE uint64_t fmix64 ( uint64_t k )
{
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xff51afd7ed558ccd);
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
  k ^= k >> 33;

  return k;
}
//-----------------------------------------------------------------------------
void MurmurHash3_x86_32 ( const void * key, int len,
                          uint32_t seed, void * out )
{
  const uint8_t * data = (const uint8_t*)key;
  const int nblocks = len / 4;

  uint32_t h1 = seed;

  const uint32_t c1 = 0xcc9e2d51;
  const uint32_t c2 = 0x1b873593;

  //----------
  // body

  const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

  for(int i = -nblocks; i; i++)
  {
    uint32_t k1 = getblock32(blocks,i);

    k1 *= c1;
    k1 = ROTL32(k1,15);
    k1 *= c2;

    h1 ^= k1;
    h1 = ROTL32(h1,13);
    h1 = h1*5+0xe6546b64;
  }

  //----------
  // tail

  const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

  uint32_t k1 = 0;

  switch(len & 3)
  {
  case 3: k1 ^= tail[2] << 16;
  case 2: k1 ^= tail[1] << 8;
  case 1: k1 ^= tail[0];
          k1 *= c1; k1 = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= len;

  h1 = fmix32(h1);

  *(uint32_t*)out = h1;
}
//////// END MurmurHash3_x86_32
*/

std::string String::stl() const {
    return std::string(ptr, len);
}

bitpowder::lib::String::operator std::string() const {
    return this->stl();
}

unsigned int String::stableHash(unsigned int hash) const {
    /* djb2a algorithm */
    //unsigned int hash = 5381;
    const unsigned char* last = (const unsigned char*)end();
    const unsigned char* current = (const unsigned char*)begin();
    while (current < last) {
        unsigned char c = *current++;
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash;
}
unsigned int String::stableHashIgnoreCase(unsigned int hash) const {
    /* djb2a algorithm */
    //unsigned int hash = 5381;
    const unsigned char* last = (const unsigned char*)end();
    const unsigned char* current = (const unsigned char*)begin();
    while (current < last) {
        unsigned char c = toUpper(*current++);
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash;
}

template <typename T>
T rotate_r(T value, int bits) {
    return (value >> ((-bits) & (sizeof(T)*8-1))) | (value << bits);
}

unsigned int String::randomizeStringHash(unsigned int value) {
#if defined(NDEBUG) && !defined(PROFILING)
    constexpr unsigned int bits = sizeof(value)*8;
    static unsigned int xorValue = std::random_device()();
    static unsigned int rotate = xorValue % bits;
    return rotate_r(value, rotate) ^ xorValue;
#else
#warning "not randomizing strings, only use in non production mode to avoid worst-case triggering attacks based on hash values"
    return value;
#endif
}

String String::dir() const {
    String retval;
    rsplitOn(Char<'/'>(), retval);
    if (!retval.empty())
        return retval;
    // empty so the part after "/" was empty, this can mean two things:
    // first: /filename (in the root of the filesystem)
    // second: filename (in the current dir)
    if (length() > 0 && ptr[0] == '/')
        return "/";
    return ".";
}

String String::base() const {
    return rsplitOn(Char<'/'>(), nullptr);
}

String String::extension() const {
    String b = base();
    String retval = b.doRSplitOn(Char<'.'>());
    return b.empty() ? String() : retval;
}

String String::trim() const {
    String retval;

    static Char<' '> space;
    span(space, &retval);

    retval.doRSpan(space);
    return retval;
}

String String::doSplit(const String& s) {
    return split(s, this);
}

String String::doSplitIgnoreCase(const String& s) {
    return splitIgnoreCase(s, this);
}

String String::split(const String& s, String& next) const {
    return split(s, &next);
}

String String::split(const String& s, String* next) const {
    if (!s.empty() && s.length() <= length() && ptr) {
        const char* sPtr = s.pointer();
        char sFirst = sPtr[0];
        StringLength sLen = s.length();
        SignedStringLength end = len - sLen;
        for (SignedStringLength l = 0; l <= end; ++l) {
            if (ptr[l] == sFirst && memcmp(&ptr[l], sPtr, sLen) == 0) {
                String retval = substring(0, l);
                if (next)
                    *next = substring(l+s.length());
                return retval;
            }
        }
    }
    String retval = *this;
    if (next)
        *next = String();
    return retval;
}

String String::splitIgnoreCase(const String& s, String& next) const {
    return splitIgnoreCase(s, &next);
}

String String::splitIgnoreCase(const String& s, String* next) const {
    if (!s.empty() && s.length() <= length() && ptr) {
        const char* sPtr = s.pointer();
        char sFirst = toLower(sPtr[0]);
        StringLength sLen = s.length();
        SignedStringLength end = len - sLen;
        for (SignedStringLength l = 0; l <= end; ++l) {
            if (toLower(ptr[l]) == sFirst && strncasecmp(&ptr[l], sPtr, sLen) == 0) {
                String retval = substring(0, l);
                if (next)
                    *next = substring(l+s.length());
                return retval;
            }
        }
    }
    String retval = *this;
    if (next)
        *next = String();
    return retval;
}

String String::doRSplit(const String& s) {
    return rsplit(s, this);
}

String String::doRSplitIgnoreCase(const String& s) {
    return rsplitIgnoreCase(s, this);
}

String String::rsplit(const String& s, String& next) const {
    return rsplit(s, &next);
}

String String::rsplit(const String& s, String* next) const {
    if (!s.empty() && s.length() <= length() && ptr) {
        const char* sPtr = s.pointer();
        StringLength sLen = s.length();
        char sFirst = sPtr[0];
        for (SignedStringLength l = len-sLen; l >= 0; --l) {
            if (ptr[l] == sFirst && memcmp(&ptr[l], sPtr, sLen) == 0) {
                String retval = substring(l+sLen);
                if (next)
                    *next = substring(0, l);
                return retval;
            }
        }
    }
    String retval = *this;
    if (next)
        *next = String();
    return retval;
}

String String::rsplitIgnoreCase(const String& s, String* next) const {
    if (!s.empty() && s.length() <= length() && ptr) {
        const char* sPtr = s.pointer();
        StringLength sLen = s.length();
        for (SignedStringLength l = len-sLen; l >= 0; --l) {
            if (strncasecmp(&ptr[l], sPtr, sLen) == 0) {
                String retval = substring(l+sLen);
                if (next)
                    *next = substring(0, l);
                return retval;
            }
        }
    }
    String retval = *this;
    if (next)
        *next = String();
    return retval;
}

bool String::contains(const String& str) const {
    String first = split(str);
    return first.length() < length();
}

bool String::containsIgnoreCase(const String& str) const {
    String first = splitIgnoreCase(str);
    return first.length() < length();
}

char String::operator[](off_t off) const {
    if (length() == 0)
        throw std::exception();
    if (off < 0)
        off += length();
    off %= length();
    if (off >= 0)
        return ptr[off];
    return 0;
}

unsigned int String::hashIgnoreCase() const {
    return randomizeStringHash(stableHashIgnoreCase());
}
unsigned int String::hash() const {
    return randomizeStringHash(stableHash());
}
/*
 StringConcat operator+(const StringOperation &a, const StringOperation &b) {
 return StringConcat(a, b);
 }
 */



/*
 StringConstConcat operator+(const DataBase &a, const std::string &b) {
 return StringConstConcat(a, b);
 }

 StringConst2Concat operator+(const std::string &a, const DataBase &b) {
 return StringConst2Concat(a, b);
 }
 */

String String::unescape(Escape escape, MemoryPool& mp) const {
    if (empty())
        return String();
    char* dst = (char*)mp.alloc(len);
    unsigned int dst_i = 0;
    unsigned int ptr_i = 0;
    if (escape == Escape::URL) {
        while (ptr_i < len) {
            // convert '+' to a space
            if (ptr[ptr_i] == '+')
                dst[dst_i] = ' ';
            // covert something with % to their real value
            else if (ptr[ptr_i] == '%' && ptr_i+2 < len) {
                dst[dst_i] = fromDigit((unsigned char)ptr[ptr_i+1]) << 4 | fromDigit((unsigned char)ptr[ptr_i+2]);
                ptr_i += 2;
            } else
                dst[dst_i] = ptr[ptr_i];
            ++dst_i;
            ++ptr_i;
        }
    } else if (escape == Escape::LikeC) {
        bool previousCharWasEscapeChar = false;
        while (ptr_i < len) {
            char in = ptr[ptr_i++];
            if (previousCharWasEscapeChar) {
                switch (in) {
                case 'a':
                    dst[dst_i++] = '\a';
                    break;
                case 'b':
                    dst[dst_i++] = '\b';
                    break;
                case 'f':
                    dst[dst_i++] = '\f';
                    break;
                case 'n':
                    dst[dst_i++] = '\n';
                    break;
                case 'r':
                    dst[dst_i++] = '\r';
                    break;
                case 't':
                    dst[dst_i++] = '\t';
                    break;
                case 'v':
                    dst[dst_i++] = '\v';
                    break;
                case '\\':
                    dst[dst_i++] = '\\';
                    break;
                case '\'':
                    dst[dst_i++] = '\'';
                    break;
                case '"':
                    dst[dst_i++] = '"';
                    break;
                case '?':
                    dst[dst_i++] = '?';
                    break;
                case '0':
                    dst[dst_i++] = '\0';
                    break;
                case 'x': {
                    // implement hexidecimal encoding like \xhh (with h hex numbers)
                    char first = ptr_i < len ? ptr[ptr_i++] : 0;
                    char second = ptr_i < len ? ptr[ptr_i++] : 0;
                    dst[dst_i++] = StringUtil::FromHex(first, second);
                    break;
                }
                default:
                    dst[dst_i++] = in;
                }
                previousCharWasEscapeChar = false;
            } else {
                previousCharWasEscapeChar = in == '\\';
                if (!previousCharWasEscapeChar)
                    dst[dst_i++] = in;
            }
        }
    } else {
        (*this)(dst, len);
    }
    mp.returnUnused(&dst[dst_i], len-dst_i);
    return String((char*)dst, dst_i);
}

//a-z, A-Z, 0-9, '-._~/'
String String::escape(Escape escape, MemoryPool& mp) const {
    if (empty())
        return String();
    size_t dstLength = lengthOfEscape(escape);
    char* dst = (char*)mp.alloc(dstLength);

    if (escape == Escape::URL) {
        int i = 0;
        for (const_iterator it = begin(); it != end(); ++it) {
            switch (*it) {
            case '-':
            case '~':
            case '.':
            case '/':
            case ',':
            case '=':
            case '&':
            case ':':
                dst[i++] = *it;
                continue;
                /*
            case ' ':
            dst[i++] = '+';
            continue;
            */
            }
            if ('a' <= *it && *it <= 'z') {
                dst[i++] = *it;
                continue;
            }
            if ('A' <= *it && *it <= 'Z') {
                dst[i++] = *it;
                continue;
            }
            if ('0' <= *it && *it <= '9') {
                dst[i++] = *it;
                continue;
            }
            dst[i++] = '%';
            dst[i++] = toDigit(((unsigned char)*it) >> 4);
            dst[i++] = toDigit(((unsigned char)*it) & 0xF);
        }
        assert(i == dstLength);
    } else if (escape == Escape::LikeC) {
        // really convert
        int i = 0;
        for (const_iterator it = begin(); it != end(); ++it) {
            switch (*it) {
            case '\a':
                dst[i++] = '\\';
                dst[i++] = 'a';
                break;
            case '\b':
                dst[i++] = '\\';
                dst[i++] = 'b';
                break;
            case '\f':
                dst[i++] = '\\';
                dst[i++] = 'f';
                break;
            case '\n':
                dst[i++] = '\\';
                dst[i++] = 'n';
                break;
            case '\r':
                dst[i++] = '\\';
                dst[i++] = 'r';
                break;
            case '\t':
                dst[i++] = '\\';
                dst[i++] = 't';
                break;
            case '\v':
                dst[i++] = '\\';
                dst[i++] = 'v';
                break;
            case '\\':
                dst[i++] = '\\';
                dst[i++] = '\\';
                break;
            //case '\'': UNNEEDED
            case '"':
                dst[i++] = '\\';
                dst[i++] = '"';
                break;
            //case '?': UNNEEDED
            case '\0':
                dst[i++] = '\\';
                dst[i++] = '0';
                break;
            default:
                dst[i++] = *it;
            }
        }
        assert(i == dstLength);
    }
    return String(dst, dstLength);
}

size_t String::lengthOfEscape(String::Escape escape) const
{
    int extra = 0;
    if (escape == Escape::URL) {
        for (const_iterator it = begin(); it != end(); ++it) {
            switch (*it) {
            case '-':
            case '~':
            case '.':
            case '/':
            case ',':
            case '=':
            case '&':
            case ':':
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
        // allocate
        extra *= 2;
    } else if (escape == Escape::LikeC) {
        // count the needed extra charachters
        for (const_iterator it = begin(); it != end(); ++it) {
            switch (*it) {
            case '\a':
            case '\b':
            case '\f':
            case '\n':
            case '\r':
            case '\t':
            case '\v':
            case '\\':
            //case '\'':
            case '"':
            //case '\?':
            case '\0':
                ++extra;
                break;
            }
        }
    }
    return len+extra;
}

#define BASE_64_BAD -1
#define BASE_64_PADDING -2
#define FROM_BASE_64(x) ( \
    'A' <= (x) && (x) <= 'Z' ? (x) - 'A' : \
    'a' <= (x) && (x) <= 'z' ? (x) - 'a' + 26 : \
    '0' <= (x) && (x) <= '9' ? (x) - '0' + 52 : \
    (x) == '+' ? 62 : \
    (x) == '/' ? 63 : \
    (x) == '=' ? BASE_64_PADDING : /* padding */ \
    BASE_64_BAD \
    )
String String::base64decode(MemoryPool& mp) const {
    int size = length() * 3 / 4;
    char* retval = (char*)mp.alloc(size, sizeof(char));
    int o = 0;
    unsigned int i;
    for (i = 0; i+4 < length(); i += 4) {
        char digit[4];
        for (int j = 0; j < 4; ++j) {
            digit[j] = FROM_BASE_64(ptr[i+j]);
            if (digit[j] == BASE_64_PADDING || digit[j] == BASE_64_BAD)
                goto end;
        }
        retval[o++] = (unsigned char)(digit[0] << 2 | digit[1] >> 4);
        retval[o++] = (unsigned char)((digit[1] & 0xf) << 4 | digit[2] >> 2);
        retval[o++] = (unsigned char)((digit[2] & 0x3) << 6 | digit[3]);
    }
end:
    if (i < length()) {
        char digit[4];
        for (int j = 0; j < 4; ++j) {
            digit[j] = i+j < length() ? FROM_BASE_64(ptr[i+j]) : -1;
            if (digit[j] == BASE_64_BAD)
                goto end2;
        }
        if (digit[1] >= 0) {
            retval[o++] = (unsigned char)(digit[0] << 2 | digit[1] >> 4);
            if (digit[2] >= 0) {
                retval[o++] = (unsigned char)((digit[1] & 0xf) << 4 | digit[2] >> 2);
                if (digit[3] >= 0)
                    retval[o++] = (unsigned char)((digit[2] & 0x3) << 6 | digit[3]);
            }
        }
    }
end2:
    return String(retval, o);
}

bool String::equalsIgnoreCase(const String& b) const {
    if (size() != b.size())
        return false;
    const char* aIt = begin();
    const char* bIt = b.begin();
    if (aIt == bIt)
        return true;
    while (aIt != end()) {
        if (toUpper(*aIt) != toUpper(*bIt))
            return false;
        ++aIt;
        ++bIt;
    }
    return true;
}

Substring<StringOperation> StringOperation::_substring(StringLength offset, StringLength length) const {
    return Substring<StringOperation>(*this, offset, length);
}

StringLength StringOperation::writeToHelper(char* __restrict__ &buffer, StringLength& bufferLength, StringLength& offset, const StringOperation& op, StringLength max) {
    StringLength len = std::min(op._size, max);
    if (offset >= len) {
        offset -= len;
        return 0;
    }
    len = std::min(bufferLength, len);
    len = op.writeTo(buffer, len, offset);
    buffer += len;
    bufferLength -= len;
    offset = 0;
    return len;
}

StringLength StringOperation::writeToHelper(char* __restrict__ &buffer, StringLength& bufferLength, StringLength& offset, const String& str, StringLength max) {
    StringLength len = std::min(str.size(), max);
    if (offset >= len) {
        offset -= len;
        return 0;
    }
    len = std::min(bufferLength, len);
    len = str.writeTo(buffer, len, offset);
    buffer += len;
    bufferLength -= len;
    offset = 0;
    return len;
}

StringLength StringOperation::writeToHelper(char* __restrict__ &buffer, StringLength& bufferLength, StringLength& offset, const char* src, StringLength len) {
    if (offset >= len) {
        offset -= len;
        return 0;
    }
    len = std::min(bufferLength, len);
    len = len - offset;
    memcpywrap(buffer, src + offset, len);
    buffer += len;
    bufferLength -= len;
    offset = 0;
    return len;
}

String StringOperation::operator()(MemoryPool& mp) const {
    StringLength l = _size;
    char* __restrict__ buffer = (char* __restrict__)mp.alloc(l, sizeof(char));
    StringLength actual_length = writeTo(buffer, l, 0);
    if (actual_length < l)
        mp.returnUnused(buffer+actual_length, l-actual_length);
    return {buffer, actual_length};
}

StringT StringOperation::c_str(MemoryPool& mp) const {
    StringLength l = _size+1;
    char* __restrict__ buffer = (char* __restrict__)mp.alloc(l, sizeof(char));
    StringLength actual_length = writeTo(buffer, l-1, 0);
    buffer[actual_length++] = '\0';
    if (actual_length < l)
        mp.returnUnused(buffer+actual_length, l-actual_length);
    return {buffer, actual_length-1};
}

StringOperation::operator std::string() const {
    StaticMemoryPool<128> smp;
    return (*this)(smp).stl();
}

//FROM: http://opensource.apple.com/source/gcc/gcc-5470.3/libiberty/strverscmp.c
/* states: S_N: normal, S_I: comparing integral part, S_F: comparing
           fractional parts, S_Z: idem but with leading Zeroes only */
#define  S_N    0x0
#define  S_I    0x4
#define  S_F    0x8
#define  S_Z    0xC

/* result_type: CMP: return diff; LEN: compare using len_diff/diff */
#define  CMP    2
#define  LEN    3
/* Compare S1 and S2 as strings holding indices/version numbers,
   returning less than, equal to or greater than zero if S1 is less than,
   equal to or greater than S2 (for more info, see the Glibc texinfo doc).  */
int String::compareUsingVersion(const String& rhs) const {
    const char* s1 = pointer();
    const char* s2 = rhs.pointer();

    const unsigned char* p1 = (const unsigned char*) s1;
    const unsigned char* p1end = p1+size();
    const unsigned char* p2 = (const unsigned char*) s2;
    const unsigned char* p2end = p2+rhs.size();
    unsigned char c1, c2;
    int state;
    int diff;

    /* Symbol(s)    0       [1-9]   others  (padding)
         Transition   (10) 0  (01) d  (00) x  (11) -   */
    static const unsigned int next_state[] = {
        /* state    x    d    0    - */
        /* S_N */  S_N, S_I, S_Z, S_N,
        /* S_I */  S_N, S_I, S_I, S_I,
        /* S_F */  S_N, S_F, S_F, S_F,
        /* S_Z */  S_N, S_F, S_Z, S_Z
    };

    static const int result_type[] = {
        /* state   x/x  x/d  x/0  x/-  d/x  d/d  d/0  d/-
                             0/x  0/d  0/0  0/-  -/x  -/d  -/0  -/- */

        /* S_N */  CMP, CMP, CMP, CMP, CMP, LEN, CMP, CMP,
        CMP, CMP, CMP, CMP, CMP, CMP, CMP, CMP,
        /* S_I */  CMP, -1,  -1,  CMP, +1,  LEN, LEN, CMP,
        +1,  LEN, LEN, CMP, CMP, CMP, CMP, CMP,
        /* S_F */  CMP, CMP, CMP, CMP, CMP, LEN, CMP, CMP,
        CMP, CMP, CMP, CMP, CMP, CMP, CMP, CMP,
        /* S_Z */  CMP, +1,  +1,  CMP, -1,  CMP, CMP, CMP,
        -1,  CMP, CMP, CMP
    };

    if (p1 == p2)
        return 0;

    c1 = p1 == p1end ? '\0' : *p1++;
    c2 = *p2++;
    /* Hint: '0' is a digit too.  */
    state = S_N | ((c1 == '0') + (isdigit (c1) != 0));

    while ((diff = c1 - c2) == 0 && c1 != '\0')
        //while ((diff = c1 - c2) == 0 && p1 != p1end)
    {
        state = next_state[state];
        c1 = p1 == p1end ? '\0' : *p1++;
        c2 = p2 == p2end ? '\0' : *p2++;
        state |= (c1 == '0') + (isdigit (c1) != 0);
    }

    state = result_type[state << 2 | (((c2 == '0') + (isdigit (c2) != 0)))];

    switch (state) {
    case CMP:
        return diff;

    case LEN:
        while (isdigit (p1 == p1end ? '\0' : *p1++))
            if (!isdigit (p2 == p2end ? '\0' : *p2++))
                return 1;

        return isdigit (p2 == p2end ? '\0' : *p2++) ? -1 : diff;

    default:
        return state;
    }
}

bool StringNameCompare::operator()(const String& lhs, const String& rhs) const {
    int retval = lhs.compareUsingVersion(rhs);
    return retval < 0;
}

String String::append(const String& a, MemoryPool& mp) const {
    // is the tail the same as where we were going to be?
    const char* next = pointer()+length();
    if (mp.next(sizeof(char)) == next && mp.available() >= a.size()) {
        MemoryPoolStatus status = mp.status();
        // try append
        String retval = a(mp);
        if (retval.pointer() == next)
            return String(pointer(), length()+a.length());
        // apperantly broken MemoryPools, or it did not fit in, so restore the MemoryPool
        mp.restore(status);
        // fallback on default always correct behaviour
    }
    return (*this + a)(mp);
}

String::operator Hashed<String>() const {
    return {*this};
}

constexpr String::operator StableHashed<String>() const {
    return {*this};
}

String String::to(char*& buffer, StringLength& size) const {
    StringLength len = writeTo(buffer, size, 0);
    String retval(buffer, len);
    buffer += len;
    size -= len;
    return retval;
}

String String::operator()(char* buffer, StringLength size) const {
    StringLength len = writeTo(buffer, size, 0);
    return String(buffer, len);
}

String bitpowder::lib::StringOperation::operator()(char* buffer, StringLength size) const {
    StringLength len = writeTo(buffer, size, 0);
    return {buffer, len};
}

StringT bitpowder::lib::StringOperation::c_str(char* buffer, StringLength size) const {
    StringLength len = writeTo(buffer, size-1, 0);
    buffer[len] = '\0';
    return {buffer,len};
}

String bitpowder::lib::StringOperation::to(char*& buffer, StringLength& size) const {
    StringLength len = writeTo(buffer, size, 0);
    String retval(buffer,len);
    buffer += len;
    size -= len;
    return retval;
}

StringT bitpowder::lib::String::c_str(MemoryPool& mp) const {
    return op().c_str(mp);
}

String bitpowder::lib::String::operator()(MemoryPool& mp) const {
    return op()(mp);
}

bool operator<(const StringContainer& lhs, const StringContainer& rhs) {
    return lhs.string() < rhs.string();
}

bool operator==(const StringContainer& a, const StringContainer& b) {
    return a.string() == b.string();
}

const Hashed<StringT> StringContainer::empty = ""_HS;
const HashedIgnoreCase<StringT> StringContainer::emptyIgnoreCase = ""_HSIC;

StringContainerSharedContent::StringContainerSharedContent(StringLength bytes) : s((char*)&extra[0], bytes) {
}

StringContainerSharedContent::StringContainerSharedContent(const Hashed<String>& str) : s(str->op().c_str((char*)&extra[0], str->size()+1)), hash(str.hash()), hashValid(true) {
}

StringContainerSharedContent::StringContainerSharedContent(const HashedIgnoreCase<String>& str) : s(str->op().c_str((char*)&extra[0], str->size()+1)), hash(str.hashIgnoreCase()), hashIgnoreCaseValid(true) {
}

StringContainerSharedContent::StringContainerSharedContent(const StringOperation& op) : s(op.c_str((char*)&extra[0], op.size()+1)) {
}


StringContainerSharedContent::Ref StringContainerSharedContent::create(const StringOperation& op) {
    // using OverheadOf: savings of 1.1 Mb on 150k strings
    return new(op.size()+1-OverheadOf<StringContainerSharedContent>::value) StringContainerSharedContent(op);
}

StringContainerSharedContent::Ref StringContainerSharedContent::create(const Hashed<String>& str) {
    return new(str->size()+1-OverheadOf<StringContainerSharedContent>::value) StringContainerSharedContent(str);
}

StringContainerSharedContent::Ref StringContainerSharedContent::create(StringLength bytes) {
    return new(bytes+1-OverheadOf<StringContainerSharedContent>::value) StringContainerSharedContent(bytes);
}

StringContainerSharedContent::Ref StringContainerSharedContent::create(const HashedIgnoreCase<String>& str) {
    return new(str->size()+1-OverheadOf<StringContainerSharedContent>::value) StringContainerSharedContent(str);
}

}
}

