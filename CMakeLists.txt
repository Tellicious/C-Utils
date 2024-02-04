# 
# This file provides set of variables for end user
# and also generates one (or more) libraries, that can be added to the project using target_link_libraries(...)
#
# Before this file is included to the root CMakeLists file (using include() function), user can set some variables:
#
# libName_COMPILE_OPTS: If defined, it provide compiler options for generated library.
# libName_COMPILE_DEFS: If defined, it provides "-D" definitions to the library build
#
cmake_minimum_required(VERSION 3.20)

set (libName ADVutils)

set(${libName}_src
    ${CMAKE_CURRENT_LIST_DIR}/src/basicMath.c
    ${CMAKE_CURRENT_LIST_DIR}/src/button.c
    ${CMAKE_CURRENT_LIST_DIR}/src/hashFunctions.c
    ${CMAKE_CURRENT_LIST_DIR}/src/IIRFilters.c
    ${CMAKE_CURRENT_LIST_DIR}/src/list.c
    ${CMAKE_CURRENT_LIST_DIR}/src/LKHashTable.c
    ${CMAKE_CURRENT_LIST_DIR}/src/LPHashTable.c
    ${CMAKE_CURRENT_LIST_DIR}/src/matrix.c
    ${CMAKE_CURRENT_LIST_DIR}/src/movingAvg.c
    ${CMAKE_CURRENT_LIST_DIR}/src/numMethods.c
    ${CMAKE_CURRENT_LIST_DIR}/src/PID.c
    ${CMAKE_CURRENT_LIST_DIR}/src/quaternion.c
    ${CMAKE_CURRENT_LIST_DIR}/src/queue.c
    ${CMAKE_CURRENT_LIST_DIR}/src/timer.c
)

# Setup include directories
set(${libName}_inc
    ${CMAKE_CURRENT_LIST_DIR}/inc/
)

# Register library to the system
add_library(${libName} STATIC)
target_sources(${libName} PRIVATE ${${libName}_src})
target_include_directories(${libName} PUBLIC ${${libName}_inc})
target_compile_options(${libName} PRIVATE ${${libName}_COMPILE_OPTS})
target_compile_definitions(${libName} PRIVATE ${${libName}_COMPILE_DEFS})
