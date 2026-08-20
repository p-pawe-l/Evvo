cmake_minimum_required(VERSION 3.16)
project(areo-evo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Library sources ---
file(GLOB AREO_SOURCES ${CMAKE_SOURCE_DIR}/areo-src/*.cpp)

add_library(areo-evo ${AREO_SOURCES})
target_include_directories(areo-evo PUBLIC
    ${CMAKE_SOURCE_DIR}/areo-include
    ${CMAKE_SOURCE_DIR}/include
)

# --- Demo executable ---
add_executable(areo-evo-demo
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/std_policy.cpp
    ${CMAKE_SOURCE_DIR}/src/print_callback.cpp
    ${CMAKE_SOURCE_DIR}/src/save_to_file_callback.cpp
)
target_link_libraries(areo-evo-demo PRIVATE areo-evo)

# --- Testing (Catch2) ---
option(AREO_BUILD_TESTS "Build tests" ON)

if(AREO_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.7.1
    )
    FetchContent_MakeAvailable(Catch2)

    enable_testing()

    file(GLOB AREO_TEST_SOURCES ${CMAKE_SOURCE_DIR}/test/*.cpp)

    if(AREO_TEST_SOURCES)
        add_executable(areo-evo-tests ${AREO_TEST_SOURCES})
        target_link_libraries(areo-evo-tests PRIVATE areo-evo Catch2::Catch2WithMain)

        include(CTest)
        include(Catch)
        catch_discover_tests(areo-evo-tests)
    endif()
endif()
