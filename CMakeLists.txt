cmake_minimum_required(VERSION 3.0.0)
project(trabFinal)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(
    log
    src/log/log.c/src/log.c
    src/log/log.c/src/log.h
)

add_library(
    data_gen
    src/data_gen/db.c
    src/data_gen/db.h
)

add_library(
    abp
    src/models/ABP/abp.c
    src/models/ABP/abp.h
)

add_library(
    lse
    src/models/LSE/lse.c
    src/models/LSE/lse.h
)

add_library(
    random
    src/data_gen/mtwister/mtwister.c
    src/data_gen/mtwister/mtwister.c
)

add_executable(trabFinal src/main.c)

target_link_libraries(lse 
    PRIVATE log
)

target_link_libraries(abp 
    PRIVATE log
)

target_link_libraries(data_gen 
    PRIVATE random
)

target_link_libraries(trabFinal 
    PRIVATE data_gen
    PRIVATE log
    PRIVATE lse
    PRIVATE abp
)
