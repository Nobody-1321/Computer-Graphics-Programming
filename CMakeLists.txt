cmake_minimum_required(VERSION 3.25)

include(${CMAKE_BINARY_DIR}/generators/conan_toolchain.cmake)

project(graphics VERSION 2.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

set(LIBRARIES_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/utils
)

foreach(dir ${LIBRARIES_DIR})
    add_subdirectory(${dir})
endforeach()

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

set(SOURCE_MAIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SOURCE_SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders)

file(GLOB SOURCE_FILES ${SOURCE_MAIN_DIR}/*.cpp)
    
set(RUNTIME_OUTPUT_DIRECTORY_PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/programs)
set(RUNTIME_OUTPUT_DIRECTORY_SHADERS ${CMAKE_CURRENT_BINARY_DIR}/programs/shaders)

file(MAKE_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY_PROGRAMS})
file(MAKE_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY_SHADERS})

foreach(SOURCE_FILE ${SOURCE_FILES})
    get_filename_component(SOURCE_FILE_NAME ${SOURCE_FILE} NAME_WE)

    message(STATUS "Adding program: ${SOURCE_FILE_NAME}")

    add_executable(${SOURCE_FILE_NAME}
        ${SOURCE_FILE}
    )

    target_link_libraries(${SOURCE_FILE_NAME} PRIVATE glad utils glfw glm::glm ${OPENGL_LIBRARIES})

    set_target_properties(${SOURCE_FILE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/programs
)
endforeach()

# Copy .glsl files to the same directory as the executable
file(GLOB GLSL_FILES "${SOURCE_SHADERS_DIR}/*.glsl")
foreach(GLSL_FILE ${GLSL_FILES})
    file(COPY ${GLSL_FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/programs/shaders)
endforeach()
