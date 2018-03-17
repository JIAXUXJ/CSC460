cmake_minimum_required(VERSION 3.9)
project(pro2_bak_fri)

set(CMAKE_CXX_STANDARD 11)

add_executable(pro2_bak_fri
        active.c
        active.h
        avr_console.h
        command.sh
        cswitch.s
        main.c
        os.c
        os.h
        queue.h
        README.md)
