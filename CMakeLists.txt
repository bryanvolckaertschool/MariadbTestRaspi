cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(MariaDBTest)

add_executable(Test test.c)
include_directories(/usr/include/mariadb /usr/include/mariadb/mysql)

target_link_libraries(Test mariadb)
