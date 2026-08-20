cmake_minimum_required(VERSION 3.16)
project(evvo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Library sources ---
file(GLOB EVVO_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM EVVO_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_library(evvo ${EVVO_SOURCES})
target_include_directories(evvo PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# --- Demo executable ---
add_executable(evvo-demo ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(evvo-demo PRIVATE evvo)

# --- Testing (Catch2) ---
option(EVVO_BUILD_TESTS "Build tests" ON)

if(EVVO_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.7.1
    )
    FetchContent_MakeAvailable(Catch2)

    enable_testing()

    file(GLOB EVVO_TEST_SOURCES ${CMAKE_SOURCE_DIR}/test/*.cpp)

    if(EVVO_TEST_SOURCES)
        add_executable(evvo-tests ${EVVO_TEST_SOURCES})
        target_link_libraries(evvo-tests PRIVATE evvo Catch2::Catch2WithMain)

        include(CTest)
        include(Catch)
        catch_discover_tests(evvo-tests)
    endif()
endif()
