add_library(console)
add_library(console::console ALIAS console)

target_set_output_dir(console)

target_sources(console PRIVATE
    "include/console/basic_io.h"
    "include/console/utility.h"
    "src/basic_io.cpp"
    "src/utility.cpp")
target_set_options(console)

target_include_directories(console
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if (CONSOLE_INSTALL)
    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/consoleConfigVersion.cmake"
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(TARGETS console
        EXPORT consoleTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include
    )

    include(CMakePackageConfigHelpers)
    configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/consoleConfig.cmake.in"
        "${PROJECT_BINARY_DIR}/consoleConfig.cmake"
        INSTALL_DESTINATION lib/cmake/console
    )

    export(TARGETS console NAMESPACE console:: FILE consoleConfig.cmake)
    install(EXPORT consoleTargets NAMESPACE console:: DESTINATION lib/cmake/console)
    install(FILES "${PROJECT_BINARY_DIR}/consoleConfigVersion.cmake"
                  "${PROJECT_BINARY_DIR}/consoleConfig.cmake"
            DESTINATION lib/cmake/console)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/lib/include/ DESTINATION include)
endif ()
