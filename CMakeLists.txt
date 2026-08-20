cmake_minimum_required(VERSION 3.16)
project(evo-core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Library sources ---
file(GLOB EVO_CORE_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM EVO_CORE_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_library(evo-core ${EVO_CORE_SOURCES})
target_include_directories(evo-core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# --- Demo executable ---
add_executable(evo-core-demo ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(evo-core-demo PRIVATE evo-core)

# --- Testing (Catch2) ---
option(EVO_CORE_BUILD_TESTS "Build tests" ON)

if(EVO_CORE_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.7.1
    )
    FetchContent_MakeAvailable(Catch2)

    enable_testing()

    file(GLOB EVO_CORE_TEST_SOURCES ${CMAKE_SOURCE_DIR}/test/*.cpp)

    if(EVO_CORE_TEST_SOURCES)
        add_executable(evo-core-tests ${EVO_CORE_TEST_SOURCES})
        target_link_libraries(evo-core-tests PRIVATE evo-core Catch2::Catch2WithMain)

        include(CTest)
        include(Catch)
        catch_discover_tests(evo-core-tests)
    endif()
endif()
