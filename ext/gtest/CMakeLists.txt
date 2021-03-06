cmake_minimum_required(VERSION 2.8)
project(gtest_builder C CXX)
include(ExternalProject)

ExternalProject_Add(googletest
    #SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk
    #URL gtest-1.7.0.zip
    URL https://googletest.googlecode.com/files/gtest-1.7.0.zip
    URL_HASH SHA1=f85f6d2481e2c6c4a18539e391aa4ea8ab0394af
    CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
    -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_FLAGS=${MSVC_COMPILER_DEFS}
    -Dgtest_force_shared_crt=ON
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
    # Disable install step
    INSTALL_COMMAND ""
    )

# Specify include dir
ExternalProject_Get_Property(googletest source_dir)
set(GTEST_INCLUDE_DIRS ${source_dir}/include PARENT_SCOPE)

# Specify MainTest's link libraries
ExternalProject_Get_Property(googletest binary_dir)
set(GTEST_LIBS_DIR ${binary_dir} PARENT_SCOPE)
