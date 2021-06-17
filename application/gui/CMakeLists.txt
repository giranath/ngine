macro(target_install_dll TARGET DLL_TARGET)
    add_custom_command(TARGET "${TARGET}" POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:${DLL_TARGET}> $<TARGET_FILE_DIR:${TARGET}>)
endmacro()

find_package(SDL2 REQUIRED CONFIG)

add_executable(ng
        main.cpp
        )

target_link_libraries(ng
        PRIVATE runtime
        PRIVATE SDL2::SDL2
        PRIVATE SDL2::main)

target_install_dll(ng
        SDL2::SDL2)