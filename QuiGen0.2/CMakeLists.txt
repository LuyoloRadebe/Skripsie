cmake_minimum_required(VERSION 3.12)
project(QuiGen0_2 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(QuiGen0_2
        BitmapGen.c
        DielectricBound.c
        DielectricStepGen.c
        Global.h
        GlobalStructures.h
        ListGen.c
        main.c
        QuiGen.c
        StepGen.c)

target_link_libraries(QuiGen0_2 m)