cmake_minimum_required(VERSION 3.10)
project(tr C)
include(.ipd/cmake/CMakeLists.txt)

add_c_test(test_translate
        test/test_translate.c
        src/translate.c)

add_c_program(tr
        src/tr.c
        src/translate.c)
