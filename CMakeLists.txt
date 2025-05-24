cmake_minimum_required(VERSION 3.30)
project(nouveau_image_processing_dufour_juret_rafin C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(nouveau_image_processing_dufour_juret_rafin
        bmp24.c
        bmp24.h
        bmp8.c
        bmp8.h
        main.c
        color_conversion.c)