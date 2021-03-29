cmake_minimum_required(VERSION 3.5)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Widgets Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Test REQUIRED)

set(INCLUDE_FILES
    ../include/dialogs.h
)

set(SOURCE_FILES
    test_dialogs.cpp
    ../lib/dialogs.cpp
)

set(PROJECT_SOURCES
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
)

enable_testing()

add_executable(TestNative ${PROJECT_SOURCES})
add_test(NAME TestNative COMMAND TestNative)

target_link_libraries(TestNative PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)
