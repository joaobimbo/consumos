cmake_minimum_required(VERSION 3.5)
project(Consumos2022)
include_directories(include)

add_executable(main src/main.c src/ADS1263.c src/DEV_Config.c)
target_link_libraries(main bcm2835)

add_executable(udp_test src/udp-server.c)
target_link_libraries(main pthread)


add_executable(udp_test2 src/udp-client.c)
