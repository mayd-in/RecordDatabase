cmake_minimum_required(VERSION 3.5)

project(RecordDatabaseNative LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Sql LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Sql LinguistTools REQUIRED)

set(INCLUDE_FILES
    include/mainwindow.h
    include/dialogs.h
    include/recordmanager.h
    include/texteditor.h
)

set(SOURCE_FILES
    src/main.cpp
    lib/mainwindow.cpp
    lib/dialogs.cpp
    lib/recordmanager.cpp
    lib/texteditor.cpp
)

set(PROJECT_SOURCES
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RecordDatabaseNative
        ${PROJECT_SOURCES}
    )
else()
    add_executable(RecordDatabaseNative
        ${PROJECT_SOURCES}
    )
endif()

#target_include_directories(RecordDatabaseNative PRIVATE
#    include
#)

include_directories(include)  # Include for tests too

target_link_libraries(RecordDatabaseNative PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

add_subdirectory(test)
