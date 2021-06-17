add_library(app
        private/application.cpp
        public/ng/app/application.hpp
        public/ng/app/mouse.hpp
        public/ng/app/keyboard.hpp
        private/keyboard.cpp)

target_include_directories(app
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
               $<INSTALL_INTERFACE:include>
        PRIVATE private
        PRIVATE public/ng/app)

target_link_libraries(app
        PRIVATE gpu
        PUBLIC core)

set_target_properties(app PROPERTIES
        OUTPUT_NAME ngapp)