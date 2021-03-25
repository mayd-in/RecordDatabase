cmake_minimum_required(VERSION 3.5)

project(RecordDatabaseQuick LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Sql LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Sql LinguistTools REQUIRED)

set(INCLUDE_FILES
    src/recordmanager.h
    src/documenthandler.h
)

set(SOURCE_FILES
    src/main.cpp
    src/recordmanager.cpp
    src/documenthandler.cpp
)

set(PROJECT_SOURCES
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
    qml.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RecordDatabaseQuick
        ${PROJECT_SOURCES}
    )
else()
    add_executable(RecordDatabaseQuick
      ${PROJECT_SOURCES}
    )
endif()

target_include_directories(RecordDatabaseQuick PRIVATE
    include
)

target_compile_definitions(RecordDatabaseQuick
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(RecordDatabaseQuick
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Sql)
