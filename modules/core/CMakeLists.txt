add_library(core
        private/hash.cpp
        public/ng/core/hash.hpp
        public/ng/core/time.hpp
        public/ng/core/string_name.hpp
        private/string_name.cpp
        private/name_table.hpp
        private/name_table.cpp
        public/ng/core/transform2d.hpp
        private/transform2d.cpp
        public/ng/core/memory_pool.hpp
        public/ng/core/endian.hpp)

target_include_directories(core
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
               $<INSTALL_INTERFACE:include>
        PRIVATE private
        PRIVATE public/ng/core)

target_link_libraries(core
        PUBLIC glm::glm)

set_target_properties(core PROPERTIES
        OUTPUT_NAME ngcore)