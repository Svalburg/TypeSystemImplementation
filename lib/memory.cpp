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

#include "memory.h"

#ifdef __FreeBSD__
#include <malloc_np.h> // FreeBSD
#elif defined(__APPLE__)
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

// FIXME: make some way to purge cached instances at regular intervals (like once per 10 minutes)
namespace bitpowder {
namespace lib {

std::set<CacheOfInstancesBase*>* CacheOfInstancesBase::getAllInstanceCaches() {
    static std::set<CacheOfInstancesBase*> retval;
    return &retval;
}

void CacheOfInstancesBase::purgeAll() {
    for (auto it = getAllInstanceCaches()->begin(); it != getAllInstanceCaches()->end(); ++it) {
        (*it)->purge();
    }
}

void CacheOfInstancesBase::statsAll()
{
    for (auto it = getAllInstanceCaches()->begin(); it != getAllInstanceCaches()->end(); ++it) {
        (*it)->stats();
    }
}

void* malloc_with_minimal_size(std::size_t& bytes) {
#ifdef __APPLE__
    bytes = malloc_good_size(bytes);
#endif
    void* data = malloc(bytes);
#ifdef __FreeBSD__
    bytes = malloc_usable_size(data);
#endif
#ifdef __linux__
    bytes = malloc_usable_size(data);
#endif
    return data;
}

}
}
