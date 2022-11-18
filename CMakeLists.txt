cmake_minimum_required(VERSION 3.5)
project(Consumos2022)
include_directories(include)
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-g)

#add_executable(main src/main.c src/ADS1263.c src/DEV_Config.c)
#target_link_libraries(main bcm2835 pthread)

add_executable(udp_server src/udp-server.c)


add_executable(udp_client src/udp-client.c src/mcc_file) 

add_executable(scan src/continuous_scan.c src/mcc_file.c src/mcc_comm.c src/gpio.c src/util.c src/mcc128.c src/cJSON.c) 
target_link_libraries(scan pthread bcm_host)
