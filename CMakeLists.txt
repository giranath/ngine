cmake_minimum_required(VERSION 3.14)

project(ngine
        VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 17)

find_package(glm REQUIRED)

add_subdirectory(thirdparties)
add_subdirectory(modules)
add_subdirectory(application)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()