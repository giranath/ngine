add_subdirectory(core)
add_subdirectory(deser)
add_subdirectory(asset)
add_subdirectory(gpu)
add_subdirectory(gameplay)
add_subdirectory(app)

add_library(runtime INTERFACE)
target_link_libraries(runtime
        INTERFACE core
        INTERFACE deser
        INTERFACE asset
        INTERFACE gpu
        INTERFACE gameplay
        INTERFACE app)