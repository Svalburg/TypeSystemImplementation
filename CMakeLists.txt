project(eca2)
cmake_minimum_required(VERSION 2.8)

OPTION(BUILD_UNIT_TESTS "Enable unit testing" ON)
OPTION(NCONCURRENCY "Disable concurrency to speed up certain processes" ON)
if (NCONCURRENCY)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNCONCURRENCY")
endif (NCONCURRENCY)

OPTION(MEMORY_DEBUG "Use slow memory allocator and don't cache any memory allocation, for debug only" OFF)
if (MEMORY_DEBUG)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMEMORY_DEBUG")
endif (MEMORY_DEBUG)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pipe -g")

find_package (Threads)

##### LIBRARY

SET(lib
	lib/memory.cpp
	lib/exception.cpp
	lib/simplestring.cpp
	lib/queue.cpp
	lib/stack.cpp
	lib/shared_object.cpp
	lib/stringparse.cpp
	lib/parser.cpp
	)

INCLUDE_DIRECTORIES("lib")

if (MEMORY_DEBUG)
    set (lib ${lib} lib/memorypool.debug.cpp)
else (MEMORY_DEBUG)
    set (lib ${lib} lib/memorypool.cpp)
endif ()
    
add_library(libbp STATIC ${lib})

###### EXECUTABLE

SET(srcs
    main.cpp
    src/Rule.cpp
    src/RuleAssign.cpp
    src/RuleBinOp.cpp
    src/RuleCallCmpF.cpp
    src/RuleCallF.cpp
    src/RuleConst.cpp
    src/RuleExprAsStmt.cpp
    src/RuleExprConcat.cpp
    src/RuleFuncDef.cpp
    src/RuleIf.cpp
    src/RuleInput.cpp
    src/RuleRepeat.cpp
    src/RuleSkip.cpp
    src/RuleStmtConcat.cpp
    src/RuleVar.cpp
    src/RuleWhile.cpp
    src/StateTuple.cpp
    src/Function.cpp
    src/ComponentFunction.cpp
    src/TimeDependentEC.cpp
    src/Environment.cpp

    src/parse.cpp)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(${PROJECT_NAME} ${srcs})
target_link_libraries(${PROJECT_NAME} libbp)

if (BUILD_UNIT_TESTS)
    FILE(GLOB UNITTESTS_SRCS lib/*.test.cpp src/*.test.cpp)
    add_executable(${PROJECT_NAME}-test ${UNITTESTS_SRCS} src/test.cpp)
    target_link_libraries(${PROJECT_NAME}-test libbp)

    set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)
    add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
    add_dependencies(${PROJECT_NAME}-test googletest)
    INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})
    # need google-test
    target_link_libraries(${PROJECT_NAME}-test ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a)
endif (BUILD_UNIT_TESTS)
