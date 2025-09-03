cmake_minimum_required(VERSION 3.16)

# Detect Qt major version early; we only need Core to query QT_VERSION_MAJOR
find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)

# Guard QT_VERSION_MAJOR being undefined to avoid tokenization errors when
# find_package didn't set it. Use DEFINED check before comparing numeric value.
if(DEFINED QT_VERSION_MAJOR AND QT_VERSION_MAJOR EQUAL 5)
    if(${QT_VERSION} VERSION_GREATER_EQUAL 5.6.0)
        find_package(Qt5 REQUIRED COMPONENTS Widgets Concurrent Network Script ScriptTools PrintSupport OpenGL Svg Sql LinguistTools)
    else()
        find_package(Qt5 REQUIRED COMPONENTS Widgets Concurrent Network Script ScriptTools PrintSupport OpenGL Svg Sql)
    endif()
endif()

if(DEFINED QT_VERSION_MAJOR AND QT_VERSION_MAJOR GREATER_EQUAL 6)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Concurrent Network PrintSupport OpenGL Svg Qml LinguistTools)
endif()

# Ensure imported Qt targets exist for AUTOMOC/AUTOUIC. In some CMake
# environments the generic find_package(QT ...) may not populate the
# Qt5::/Qt6:: targets for this directory, so explicitly find the
# concrete Qt package if needed.
if(DEFINED QT_VERSION_MAJOR AND QT_VERSION_MAJOR EQUAL 5)
    if(NOT TARGET Qt5::Widgets)
        find_package(Qt5 REQUIRED COMPONENTS Widgets Concurrent Network Script ScriptTools PrintSupport OpenGL Svg Sql LinguistTools)
    endif()
elseif(DEFINED QT_VERSION_MAJOR AND QT_VERSION_MAJOR GREATER_EQUAL 6)
    if(NOT TARGET Qt${QT_VERSION_MAJOR}::Widgets)
        find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Concurrent Network PrintSupport OpenGL Svg Qml LinguistTools)
    endif()
endif()

# Read version if not provided by the root CMakeLists
if(NOT DEFINED X_PROJECT_VERSION)
    file(STRINGS ${CMAKE_CURRENT_LIST_DIR}/../release_version.txt X_PROJECT_VERSION)
endif()

# Main project for the source subtree
project(XDataExtractor VERSION ${X_PROJECT_VERSION} LANGUAGES CXX)

# Enable Qt automoc/uic/rcc globally for subdirectories
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(X_COMPANYNAME "ntinfo")
set(X_PROJECTNAME "XDataExtractor")
set(X_MAINTAINER "horsicq@gmail.com")
set(X_DESCRIPTION "XDataExtractor is an embedded data extractor for various file formats.")
set(X_HOMEPAGE "https://horsicq.github.io")
set(X_ORIGINAL_FILENAME "xde")

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.xde)
endif()

include(../dep/build_tools/cmake/deploy_init.cmake)

message(STATUS X_PROJECT_ARCH: ${X_PROJECT_ARCH})

add_subdirectory(../dep/XCppfilt XCppfilt)
add_subdirectory(../dep/XCapstone XCapstone)
add_subdirectory(../dep/XArchive XArchive)
add_subdirectory(../dep/XYara XYara)
add_subdirectory(gui xde)
add_subdirectory(cli xdec)
