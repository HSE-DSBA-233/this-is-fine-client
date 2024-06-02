cmake_minimum_required(VERSION 3.5)

project(This_is_Fine VERSION 0.1 LANGUAGES CXX)

# Basic settings
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "$ENV{CONDA_PREFIX}/lib/cmake/Qt5")

# Find Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

# Find CPR package
find_package(cpr REQUIRED)

# Source files
set(TS_FILES This_is_Fine_en_US.ts)
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    chatcreatewindow.cpp
    chatcreatewindow.h
    chatcreatewindow.ui
    chatsitemwidget.cpp
    chatsitemwidget.h
    chatsitemwidget.ui
    chatclient.h
    chatclient.cpp
    chat-api.h
    json.hpp
    resources.qrc
    ${TS_FILES}
)

# Main executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(This_is_Fine
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(This_is_Fine SHARED ${PROJECT_SOURCES})
    else()
        add_executable(This_is_Fine ${PROJECT_SOURCES})
    endif()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# Add include directories
target_include_directories(This_is_Fine PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(This_is_Fine PRIVATE Qt${QT_VERSION_MAJOR}::Widgets cpr::cpr)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.This_is_Fine)
endif()
set_target_properties(This_is_Fine PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS This_is_Fine
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(This_is_Fine)
endif()

# Google Test setup
enable_testing()

add_subdirectory(extern/googletest)
include_directories(extern/googletest/googletest/include)

include_directories(${CMAKE_SOURCE_DIR})

# Ensure all necessary source files are included in the test executable
add_executable(runTests
    tests/test_mainwindow.cpp
    tests/test_chatcreate.cpp
    tests/test_chatsitemwidget.cpp
    mainwindow.cpp
    chatcreatewindow.cpp
    chatsitemwidget.cpp
    chatclient.cpp  # Include chatclient.cpp
)

target_include_directories(runTests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(runTests gtest gtest_main Qt${QT_VERSION_MAJOR}::Widgets cpr::cpr)

# Register the test
add_test(NAME runTests COMMAND runTests)

# Include spdlog
include_directories(${CMAKE_SOURCE_DIR}/logger/include)
