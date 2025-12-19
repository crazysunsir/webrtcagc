cmake_minimum_required(VERSION 3.9)
project(agc)

# 可执行文件
add_executable(agc main.c agc.c)
target_link_libraries(agc m)

# 共享库 (供 Go 调用)
add_library(agc_go SHARED agc.c agc_wrapper.c)
target_link_libraries(agc_go m)
set_target_properties(agc_go PROPERTIES
    OUTPUT_NAME "agc"
    VERSION 1.0.0
    SOVERSION 1
)

# macOS 特定设置
if(APPLE)
    set_target_properties(agc_go PROPERTIES
        INSTALL_NAME_DIR "@rpath"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
endif()

# Linux 特定设置
if(UNIX AND NOT APPLE)
    set_target_properties(agc_go PROPERTIES
        INSTALL_RPATH "$ORIGIN"
    )
endif()