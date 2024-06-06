cmake_minimum_required(VERSION 3.28)
project(pertroc C)

set(CMAKE_C_STANDARD 11)

add_executable(pertroc main.c
        tache.h
        tache.c
        projet.c
)
