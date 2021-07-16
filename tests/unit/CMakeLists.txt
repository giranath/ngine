add_executable(unit-tests
        main.cpp
        core/hash.cpp
        core/name.cpp
        core/transform2d.cpp
        core/memory_pool.cpp
        core/endian.cpp
        deser/xml_loader.cpp
        gameplay/node2d.cpp
        gameplay/node_path.cpp
        gameplay/node_tree.cpp
        asset/package.cpp)

target_include_directories(unit-tests
        PRIVATE catch)

target_link_libraries(unit-tests
        PRIVATE core
        PRIVATE deser
        PRIVATE gameplay
        PRIVATE asset)

add_test(NAME unit COMMAND unit-tests)