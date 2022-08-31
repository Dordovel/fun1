cmake_minimum_required (VERSION 3.1.0)
project(main)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -g -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(MYSQL_CONNECTOR_DIR ${CMAKE_SOURCE_DIR}/libs/mariadb-connector)

file(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

include_directories(${MYSQL_CONNECTOR_DIR}/include)
include_directories(${MYSQL_CONNECTOR_DIR}/include/mariadb/cppconn)
link_directories(${MYSQL_CONNECTOR_DIR}/lib/mariadb)

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

target_link_libraries(${PROJECT_NAME} curl mariadbcpp ${GTKMM_LIBRARIES} config++)
