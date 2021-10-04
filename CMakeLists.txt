cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(MariadbTestRaspi)

add_executable(Test test.c)
include_directories(/usr/include/mariadb /usr/include/mariadb/mysql /usr/local/include)
target_link_libraries(Test PJ_RPI)
target_link_libraries(Test mariadb)
