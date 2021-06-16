add_library(gpu
        public/ng/gpu/gpu_context.hpp
        private/gpu_context.cpp
        public/ng/gpu/gpu_texture2d.hpp
        private/gpu_texture2d.cpp
        public/ng/gpu/gpu_texture_filtering.hpp
        public/ng/gpu/gpu_texture_wrapping.hpp
        private/gpu_buffer.cpp
        public/ng/gpu/gpu_buffer.hpp
        private/gpu_shader.cpp
        public/ng/gpu/gpu_shader.hpp
        private/gpu_program.cpp
        public/ng/gpu/gpu_program.hpp
        private/gpu_vertex_layout.cpp
        public/ng/gpu/gpu_vertex_layout.hpp
        private/opengl_helpers.cpp
        private/opengl_helpers.hpp
        public/ng/gpu/gpu_vertex_attribute.hpp
        public/ng/gpu/gpu_pixel_formats.hpp
        )

target_include_directories(gpu
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
               $<INSTALL_INTERFACE:include>
        PRIVATE private
        PRIVATE public/ng/gpu)

target_link_libraries(gpu
        PRIVATE glad)

set_target_properties(gpu PROPERTIES
        OUTPUT_NAME nggpu)