cmake_minimum_required(VERSION 3.5)
project(Consumos2022)

add_executable(main src/main.c src/ADS1263.c src/DEV_Config.c)
include_directories(include)
target_link_libraries(main bcm2835)