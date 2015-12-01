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

#include "exception.h"
#include "simplestring.h"

namespace bitpowder {
namespace lib {

std::ostream& operator <<(std::ostream& out, const Exception& e) {
    out<< "Exception " << e.err << " (" << strerror(e.err) << ")";
    if (e.where) {
        out << " [" << e.where;
        if (e.sourceFile)
            out << " in " << e.sourceFile << ":" << e.lineNo;
        out << "]";
    }
    if (e.desc.size() > 0)
        out << " due to " << e.desc;
    return out;
}

String Exception::toString(MemoryPool& mp) const {
    return ("Exception "_S + err + " (" + strerror(err) + ")" +
            " ["_S + where + " in " + sourceFile + ":" + lineNo + "]" +
            " due to "_S + desc)(mp);
}

}
}
