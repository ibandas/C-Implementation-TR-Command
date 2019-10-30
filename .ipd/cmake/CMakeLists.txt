# Turn on CTest
enable_testing()

include_directories(src)

add_subdirectory(.ipd/lib/libipd EXCLUDE_FROM_ALL)

find_package(Catch2 CONFIG QUIET)
if (NOT Catch2_FOUND)
    add_subdirectory(.ipd/lib/catch EXCLUDE_FROM_ALL)
endif (NOT Catch2_FOUND)

# Sets us up to know if we're on Windows.
function (windows_setup name)
    if (WIN32)
        target_compile_definitions(${name} PRIVATE IPD_WINDOWS)
    else (WIN32)
        target_compile_definitions(${name} PRIVATE IPD_UNIX)
    endif (WIN32)
endfunction(windows_setup)

# Adds a program with the given name and source files, and sets the
# language to C++ 14
function (add_c_program name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} libipd)
    set_property(TARGET ${name} PROPERTY C_STANDARD           11)
    set_property(TARGET ${name} PROPERTY C_STANDARD_REQUIRED  On)
    set_property(TARGET ${name} PROPERTY C_EXTENSIONS         Off)
    windows_setup(${name})
endfunction (add_c_program)

# Adds a program with the given name and source files, and sets the
# language to C++ 14
function (add_cxx_program name)
    add_executable(${name} ${ARGN})
    set_property(TARGET ${name} PROPERTY CXX_STANDARD           14)
    set_property(TARGET ${name} PROPERTY CXX_STANDARD_REQUIRED  On)
    set_property(TARGET ${name} PROPERTY CXX_EXTENSIONS         Off)
    windows_setup(${name})
endfunction (add_cxx_program)

# Adds a test program with the given name and source files
function (add_c_test name)
    add_c_program(${name} ${ARGN})
    add_test(NAME Test_${name} COMMAND ${name})
endfunction (add_c_test)

# Adds a test program with the given name and source files
function (add_cxx_test name)
    add_cxx_program(${name} ${ARGN})
    target_link_libraries(${name} catch)
    add_test(NAME Test_${name} COMMAND ${name})
endfunction (add_cxx_test)
