cmake_minimum_required(VERSION 3.5)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Core QuickTest REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core QuickTest REQUIRED)

set(INCLUDE_FILES
)

set(SOURCE_FILES
    test_dialogs.cpp
)

set(PROJECT_SOURCES
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
)

enable_testing()

# no need to copy around qml test files for shadow builds - just set the respective define
add_definitions(-DQUICK_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(TestQuick ${PROJECT_SOURCES})
add_test(NAME TestQuick COMMAND TestQuick)

target_link_libraries(TestQuick PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::QuickTest)
