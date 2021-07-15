add_library(asset
        private/asset_registry.cpp
        public/ng/asset/asset_registry.hpp
        public/ng/asset/asset.hpp
        private/asset.cpp
        private/asset_package.cpp
        public/ng/asset/asset_package.hpp
        private/asset_package_stream.cpp
        public/ng/asset/asset_package_stream.hpp
        public/ng/asset/asset_factory.hpp
        private/asset_factory.cpp
        )

target_include_directories(asset
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
               $<INSTALL_INTERFACE:include>
        PRIVATE private
        PRIVATE public/ng/asset)

target_link_libraries(asset
        PUBLIC core
        PUBLIC deser)

set_target_properties(asset PROPERTIES
        OUTPUT_NAME ngasset)