add_library(gameplay
        private/game_instance.cpp
        public/ng/gameplay/game_instance.hpp
        public/ng/gameplay/node.hpp
        private/node.cpp
        public/ng/gameplay/node_tree.hpp
        private/node_tree.cpp
        public/ng/gameplay/node2d.hpp
        private/node2d.cpp
        )

target_include_directories(gameplay
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
               $<INSTALL_INTERFACE:include>
        PRIVATE private
        PRIVATE public/ng/gameplay)

target_link_libraries(gameplay
        PUBLIC core)

set_target_properties(gameplay PROPERTIES
        OUTPUT_NAME nggameplay)