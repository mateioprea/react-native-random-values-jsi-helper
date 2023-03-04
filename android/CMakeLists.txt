project(reactnativerandomvaluesjsihelper)
cmake_minimum_required(VERSION 3.9.0)

set (PACKAGE_NAME "react-native-random-values-jsi-helper")
set (BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)

include_directories(
        "../cpp"
)

add_library(reactnativerandomvaluesjsihelper
        SHARED
        cpp-adapter.cpp
        "../cpp/TypedArray.cpp"
)
set_target_properties(
        reactnativerandomvaluesjsihelper PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        POSITION_INDEPENDENT_CODE ON
)

find_package(ReactAndroid REQUIRED CONFIG)
find_library(
        log-lib
        log
)

target_link_libraries(
        reactnativerandomvaluesjsihelper
        ${log-lib}
        ReactAndroid::jsi 
        android
)
