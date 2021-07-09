add_executable(unit-tests
        main.cpp
        core/hash.cpp
        core/name.cpp
        core/transform2d.cpp
        core/memory_pool.cpp
        deser/xml_loader.cpp)

target_include_directories(unit-tests
        PRIVATE catch)

target_link_libraries(unit-tests
        PRIVATE core
        PRIVATE deser)

add_test(NAME unit COMMAND unit-tests)