add_library(jsoncpp
        include/json/allocator.h
        include/json/assertions.h
        include/json/config.h
        include/json/forwards.h
        include/json/json.h
        include/json/json_features.h
        include/json/reader.h
        include/json/value.h
        include/json/version.h
        include/json/writer.h
        src/json_reader.cpp
        src/json_tool.h
        src/json_value.cpp
        src/json_valueiterator.inl
        src/json_writer.cpp)

target_include_directories(jsoncpp
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
               $<INSTALL_INTERFACE:include>)