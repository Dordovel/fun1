cmake_minimum_required (VERSION 3.1.0)
project(main)

set(CMAKE_CXX_FLAGS "-Wall -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(MYSQL_CONNECTOR_DIR ${CMAKE_SOURCE_DIR}/libs/mysql-connector)

file(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

include_directories(${MYSQL_CONNECTOR_DIR}/include)
include_directories(${MYSQL_CONNECTOR_DIR}/include/jdbc/cppconn)
link_directories(${MYSQL_CONNECTOR_DIR}/lib64)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

pkg_check_modules(ATKMM atkmm-1.6)
link_directories(${ATKMM_LIBRARY_DIRS})
include_directories(${ATKMM_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCE_FILES} main.cpp)

find_package(Threads)
if (THREADS_FOUND)
    target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
endif()

target_link_libraries(${PROJECT_NAME} curl mysqlcppconn ${GTKMM_LIBRARIES})
