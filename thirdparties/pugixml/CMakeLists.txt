add_library(pugixml
        src/pugixml.cpp
        include/pugixml.hpp
        include/pugiconfig.hpp)

target_include_directories(pugixml
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
               $<INSTALL_INTERFACE:include>)