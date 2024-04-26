cmake_minimum_required(VERSION 3.14)

project(dbmanager LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql)

include_directories(inc/)
set(PROJECT_INC
    inc/dbmanager.h
    inc/dbtypes.h
)

set(PROJECT_SRC
    src/dbmanager.cpp
)

add_library(dbmanager STATIC
    ${PROJECT_INC}
    ${PROJECT_SRC}
)

target_link_libraries(dbmanager
    PRIVATE Qt${QT_VERSION_MAJOR}::Core
    PRIVATE Qt${QT_VERSION_MAJOR}::Sql
)

target_compile_definitions(dbmanager PRIVATE DBMANAGER_LIBRARY)
