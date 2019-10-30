cmake_minimum_required(VERSION 3.3)
project(libipd
        VERSION 2019.0.1
        DESCRIPTION "A little library for IPD"
        LANGUAGES C)

###
### MAIN LIBRARY SETUP
###

add_library(libipd
        src/eprintf.c
        src/read_line.c
        src/test_rt.c)

set_target_properties(libipd PROPERTIES
        C_STANDARD            11
        C_STANDARD_REQUIRED   On
        C_EXTENSIONS          Off
        VERSION               ${PROJECT_VERSION})

target_include_directories(libipd SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_include_directories(libipd PRIVATE
        include)

###
### LIBRARY INSTALLATION
###

include(GNUInstallDirs)
install(TARGETS libipd EXPORT LibIPDConfig
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT LibIPDConfig DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/LibIPD)
export(TARGETS libipd FILE LibIPDConfig.cmake)
