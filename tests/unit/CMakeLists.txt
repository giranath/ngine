add_executable(unit-tests
        main.cpp
        core/hash.cpp)

target_include_directories(unit-tests
        PRIVATE catch)

target_link_libraries(unit-tests
        PRIVATE core)

add_test(NAME unit COMMAND unit-tests)