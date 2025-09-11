cmake_minimum_required (VERSION 3.25)
include(${CMAKE_BINARY_DIR}/generators/conan_toolchain.cmake)

project(Utils VERSION 1.0.0 LANGUAGES CXX)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

set(LIBRARY_HEADERS_UTILS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Utils.hpp
)

set(LIBRARY_SOURCE_UTILS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils.cpp
)

add_library(utils STATIC ${LIBRARY_SOURCE_UTILS})
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(utils PRIVATE glfw glad glm::glm ${OPENGL_LIBRARIES})