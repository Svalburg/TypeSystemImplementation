#include <iostream>
#include <tuple>
#include <set>
#include <algorithm>
#include <vector>
#include <map>
#include <chrono>

#include "parse.h"

#include <sys/stat.h>
#include <fcntl.h>

using namespace std;
using namespace bitpowder::lib;

void ParseFile(const StringT& filename) {
    MemoryPool mp;

    struct stat st;
    if (stat(filename.c_str(), &st)) {
	std::cerr << "error: " << strerror(errno) << std::endl;
	return;
    }
#ifdef __MINGW32__
    int fd = open(filename.c_str(), O_RDONLY | O_BINARY);
#else
    int fd = open(filename.c_str(), O_RDONLY);
#endif
    if (!fd) {
	std::cerr << "error: " << strerror(errno) << std::endl;
	return;
    }
    char *buffer = (char *)mp.alloc(st.st_size);
    int size = read(fd, buffer, st.st_size);
    if (size != st.st_size) {
	std::cerr << "wrong number of bytes read: " << size << " instead of "
	    << st.st_size << std::endl;
	abort();
    }
    close(fd);

    String current(buffer, size);

    auto result = ParseECA(current, mp);
    if (!result) {
	std::cerr << "error: " << result.error() << " at " << result.position() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
	std::cerr << argv[0] << " [filename]" << std::endl;
	return -1;
    }
    ParseFile(argv[1]);
#ifndef __MINGW32__
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
#ifdef __APPLE__
    std::cout << usage.ru_maxrss << " bytes of memory used at max" << std::endl;
    std::cout << (usage.ru_maxrss/1024/1024) << " megabytes of memory used at max" << std::endl;
#else
    std::cout << usage.ru_maxrss << " kilobytes of memory used at max" << std::endl;
    std::cout << (usage.ru_maxrss/1024) << " megabytes of memory used at max" << std::endl;
#endif
#endif
    bitpowder::lib::MemoryPool::vacuum();
    return 0;
}

