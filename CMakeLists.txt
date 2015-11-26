cmake_minimum_required(VERSION 2.8.11)
#include(${CMAKE_CURRENT_SOURCE_DIR}/parser/CMakeLists.txt)



project(Internship)



include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include

)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  -O0")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" -L\".\" -L\"Debug\" ")

# Define the CXX sources
set ( CXX_SRCS
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
    src/StateTuple.cpp
    src/Function.cpp
    src/ComponentFunction.cpp
    src/TimeDependentEC.cpp
    src/Environment.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -g -Wall -fexceptions")

add_executable(Internship ${CXX_SRCS} ${C_SRCS})

