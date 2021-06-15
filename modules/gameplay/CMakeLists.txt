add_library(gameplay
        private/game_instance.cpp
        public/ng/gameplay/game_instance.hpp
        )

target_include_directories(gameplay
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
               $<INSTALL_INTERFACE:include>
        PRIVATE private
        PRIVATE public/ng/gameplay)

set_target_properties(gameplay PROPERTIES
        OUTPUT_NAME nggameplay)