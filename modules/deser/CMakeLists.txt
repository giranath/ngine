add_library(deser
        public/ng/deser/object_deserializer.hpp
        private/object_deserializer.cpp
        private/object_xml_deserializer.hpp
        private/object_xml_deserializer.cpp
        public/ng/deser/object_archive_traits.hpp
        )

target_include_directories(deser
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
               $<INSTALL_INTERFACE:include>
        PRIVATE private
        PRIVATE public/ng/deser)

target_link_libraries(deser
        PUBLIC core
        PRIVATE jsoncpp
        PRIVATE toml++
        PRIVATE pugixml)

set_target_properties(deser PROPERTIES
        OUTPUT_NAME ngdeser)