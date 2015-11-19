/**
Copyright 2013-2015 Bernard van Gastel, bvgastel@bitpowder.com.
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

/**
      if (line.parse().split(Char<','>(),
                           nop,
                           span(space).toNumber(day)
                           .span(space).toNumber(month)
                           .accept("GMT").store(remainer)))
        // arrows in c++: GMT => 0; hh:mm => unsigned int (offset), deze integegreren in resultaat
        Parse(line).or(epxr, stmt, [optionally] lambda to integrate current result with new result).store(&expression)

                hoe met conditionals om te gaan, "S1;S2" kunnen parsen maar ook "S1", optioneel dus. Mogelijk
                een extra optie om te parsen met een combine functie
                Parse(line).something.opt(S2, [](Expression a, Expression b) { return Or(a, b) })

                linenumbers/posities onthouden voor elke operatie

                mooie error meldingen (misschien onError())

        struct {
            bool error;
            int position;

        };

    ParseResult<ResultClass>
    .result([]() {
        return TM::Cache(minutes, hours);
    })
*/

/**
  * TODO:
  * - add error messages (with positions in the text)
  * - split tokenizer and parsing (so the whole document does not need to be in memory!)
  * - accept(Token) should be possible, together with accept(object, object, object)
  * - make sure there is a streaming interface available
  * - make it easy to creating some sort of LL<n> parser
  */

#ifndef StringParser_H
#define StringParser_H

#include "simplestring.h"

namespace bitpowder {
namespace lib {

class StringParser {
    String left;
    const static int DEFAULT_ERROR = std::numeric_limits<int>::max();
    int err; // 0 = no error

  public:
    StringParser(const String& input, int error = 0) : left(input), err(0) {
    }
    StringParser(const StringParser& copy) : left(copy.left), err(copy.err) {
    }


    StringParser& error(int errorCodeIfDefaultErrorCodeIsCurrentlyActive) {
        if (err == DEFAULT_ERROR)
            err = errorCodeIfDefaultErrorCodeIsCurrentlyActive;
        return *this;
    }

    const String& remaining() const {
        return left;
    }

    StringParser& accept() {
        err = DEFAULT_ERROR;
        return *this;
    }

    template <typename T, typename... Args>
    StringParser& accept(T&& first, Args&&... args) {
        return choose([first](StringParser& cont) {
            return cont._accept(first);
        }, [args...](StringParser& cont) {
            return cont.accept(args...);
        });
    }

    StringParser& _accept(const String& str) {
        if (!err) {
            if (left.startsWith(str))
                left = left.substring(str.length());
            else
                err = DEFAULT_ERROR;
        }
        //std::cout << "accept() resulted in StringParser(" << left << ", " << error << ")" << std::endl;
        return *this;
    }

    StringParser& _accept(char c) {
        if (!err) {
            if (left.size() > 0 && left.pointer()[0] == c)
                left = left.substring(1);
            else
                err = DEFAULT_ERROR;
        }
        //std::cout << "accept() resulted in StringParser(" << left << ", " << error << ")" << std::endl;
        return *this;
    }

    StringParser& nonEmpty() {
        if (!err && left.empty())
            err = DEFAULT_ERROR;
        //std::cout << "nonEmpty() resulted in StringParser(" << left.length() << ", " << error << ") " << left.empty() << std::endl;
        return *this;
    }

    StringParser& empty() {
        if (!err && !left.empty())
            err = DEFAULT_ERROR;
        //std::cout << "empty() resulted in StringParser(" << left << ", " << error << ")" << std::endl;
        return *this;
    }

    template <class C>
    StringParser& span(const C& c) {
        if (!err)
            left.doSpan(c);
        return *this;
    }

    template <class Split>
    StringParser& span(String& prefix, const Split& span) {
        if (!err)
            prefix = left.doSpan<Split>(span);
        return *this;
    }

    template <class C>
    StringParser& rspan(const C& c) {
        if (!err)
            left.doRSpan(c);
        return *this;
    }

    StringParser& search(const String& text) {
        if (!err) {
            String remainder;
            String prefix = left.split(text, remainder);
            //std::cout << "search: prefix=" << prefix.length() << " original=" << left.length() << std::endl;
            if (prefix == left) {
                // not found
                err = DEFAULT_ERROR;
                //std::cout << "search: setErrorTo=" << setErrorTo << std::endl;
            } else {
                left = remainder;
            }
        }
        return *this;
    }

    StringParser& searchIgnoreCase(const String& text) {
        if (!err) {
            String remainder;
            String prefix = left.splitIgnoreCase(text, remainder);
            //std::cout << "search: prefix=" << prefix.length() << " original=" << left.length() << std::endl;
            if (prefix == left) {
                // not found
                err = DEFAULT_ERROR;
                //std::cout << "search: setErrorTo=" << setErrorTo << std::endl;
            } else {
                left = remainder;
            }
        }
        return *this;
    }

    template<typename... Args>
    StringParser& search(const String& text, Args... path2) {
        if (err)
            return *this;
        StringParser retval(*this);
        retval.search(path2...);
        _first(retval, [text](StringParser &cont) -> StringParser & { return cont.search(text); });
        return *this;
    }

    template<typename... Args>
    StringParser& searchIgnoreCase(const String& text, Args... path2) {
        if (err)
            return *this;
        StringParser retval(*this);
        retval.searchIgnoreCase(path2...);
        _first(retval, [text](StringParser &cont) -> StringParser & { return cont.searchIgnoreCase(text); });
        return *this;
    }

    StringParser& rsearch(const String& text) {
        if (!err) {
            String remainder;
            String prefix = left.rsplit(text, remainder);
            if (prefix == left) {
                // not found
                err = DEFAULT_ERROR;
            } else {
                left = remainder;
            }
        }
        return *this;
    }

    template <class Split>
    StringParser& splitOn(String& prefix, const Split& splitOn, char& delim) {
        if (!err)
            prefix = left.doSplitOn<Split>(splitOn, &delim);
        return *this;
    }

    template <class Split>
    StringParser& splitOn(String& prefix, const Split& splitOn) {
        if (!err)
            prefix = left.doSplitOn<Split>(splitOn);
        return *this;
    }

    template <class Split>
    StringParser& splitOn(const Split& splitOn, char& delim) {
        if (!err)
            left.doSplitOn<Split>(splitOn, &delim);
        return *this;
    }

    template <class Split>
    StringParser& splitOn(const Split& splitOn) {
        if (!err)
            left.doSplitOn<Split>(splitOn);
        return *this;
    }

    StringParser& split(String& prefix, const String& splitOn) {
        if (!err)
            prefix = left.doSplit(splitOn);
        return *this;
    }

    StringParser& splitIgnoreCase(String& prefix, const String& splitOn) {
        if (!err)
            prefix = left.doSplitIgnoreCase(splitOn);
        return *this;
    }

    template <class Split>
    StringParser& rsplitOn(String& prefix, const Split& splitOn, char* delim = nullptr) {
        if (!err)
            prefix = left.doRSplitOn<Split>(splitOn, delim);
        return *this;
    }

    StringParser& rsplit(String& prefix, const String& splitOn) {
        if (!err)
            prefix = left.doRSplit(splitOn);
        return *this;
    }

    StringParser& rsplitIgnoreCase(String& prefix, const String& splitOn) {
        if (!err)
            prefix = left.doRSplitIgnoreCase(splitOn);
        return *this;
    }

    template <class N>
    StringParser& parseNumber(N& n) {
        String remainder;
        N retval = left.toNumber<N>(remainder);
        if (remainder.size() == left.size()) {
            err = DEFAULT_ERROR;
        } else {
            n = retval;
            left = remainder;
        }
        return *this;
    }

    template <class N>
    StringParser& parseHexNumber(N& n) {
        String remainder;
        N retval = left.hexToNumber<N>(remainder);
        if (remainder.size() == left.size()) {
            err = DEFAULT_ERROR;
        } else {
            n = retval;
            left = remainder;
        }
        return *this;
    }

        template <class N>
    StringParser& parseMaxNumber(N& n) {
        String remainder;
        N retval = left.maxToNumber<N>(remainder);
        if (remainder.size() == left.size()) {
            err = DEFAULT_ERROR;
        } else {
            n = retval;
            left = remainder;
        }
        return *this;
    }

    template <typename Cont>
    StringParser& perform(Cont&& path) {
        if (!err)
            path(*this);
        return *this;
    }

/*
    StringParser& perform(std::function<StringParser & (StringParser& cont)> path) {
        if (!error)
            path(*this);
        return *this;
    }
*/

    template <typename Func, typename... Extra>
    StringParser& opt(const Func& path, Extra& ... extra) {
        if (!err) {
            String left = this->left;
            path(*this, extra...);
            if (err) {
                this->left = left;
                err = 0;
            }
        }
        return *this;
    }

    template <typename Cont>
    StringParser& choose(Cont&& path) {
        perform(path);
        return *this;
    }


    template <typename Cont, typename... Args>
    StringParser& choose(Cont&& path, Args... path2) {
        if (err)
            return *this;
        String left = this->left;
        perform(path);
        if (!err)
            return *this;
        this->left = left;
        err = 0;
        choose(std::forward<Args>(path2)...);
        return *this;
    }

    StringParser& _first(const StringParser& retval2, std::function<StringParser & (StringParser& cont)> path) {
        if (err)
            return *this;
        StringParser retval(*this);
        retval.perform(path);
        if (retval.err && retval2.err) {
            err = retval.err;
        } else if (retval2.err || retval.left.size() > retval2.left.size()) {
            left = retval.left;
        } else {
            left = retval2.left;
        }
        return *this;
    }

    template <typename Cont>
    StringParser& first(Cont&& path) {
        perform(path);
        return *this;
    }
    template <typename Cont, typename... Args>
    StringParser& first(Cont&& path, Args... path2) {
        if (err)
            return *this;
        StringParser retval2(*this);
        retval2.first(path2...);
        return (*this)._first(retval2, path);
    }

    StringParser& remainder(StringParser& remainder) {
        if (!err)
            remainder = *this;
        return *this;
    }

    StringParser& remainder(String& remainder) {
        if (!err)
            remainder = left;
        return *this;
    }

    operator bool() const {
        return !err;
    }
};

}
}

#endif // StringParser_H
