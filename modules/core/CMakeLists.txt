add_library(core
        private/hash.cpp
        public/ng/core/hash.hpp
        public/ng/core/time.hpp
        public/ng/core/name.hpp)

target_include_directories(core
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
               $<INSTALL_INTERFACE:include>
        PRIVATE private
        PRIVATE public/ng/core)

set_target_properties(core PROPERTIES
        OUTPUT_NAME ngcore)