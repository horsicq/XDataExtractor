cmake_minimum_required(VERSION 3.16)

project(xdec VERSION ${X_PROJECT_VERSION} LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/../../dep/build_tools/cmake/cpp_standart_setup.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../dep/XExtractor/xextractor.cmake)

set(PROJECT_SOURCES
    ${XEXTRACTOR_SOURCES}
    main_console.cpp
)

if(WIN32)
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${QM_FILES}
        ../../res/resource.rc
    )
else()
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${QM_FILES}
    )
endif()

# Apply compile definitions per-target instead of globally
set(XDEC_DEFINES USE_DEX USE_PDF USE_ARCHIVE)
if(WIN32)
    list(APPEND XDEC_DEFINES NOMINMAX)
endif()
target_compile_definitions(${PROJECT_NAME} PRIVATE ${XDEC_DEFINES})

target_link_libraries(${PROJECT_NAME} PRIVATE bzip2)
target_link_libraries(${PROJECT_NAME} PRIVATE lzma)
target_link_libraries(${PROJECT_NAME} PRIVATE zlib)
target_link_libraries(${PROJECT_NAME} PRIVATE capstone)

if(DEFINED QT_VERSION_MAJOR)
    ## Ensure Qt imported targets are available in this directory so AUTOMOC/AUTOUIC work
    if(DEFINED QT_VERSION_MAJOR AND QT_VERSION_MAJOR EQUAL 5)
        if(NOT TARGET Qt5::Widgets)
            find_package(Qt5 REQUIRED COMPONENTS Widgets Concurrent Network Script ScriptTools PrintSupport OpenGL Svg Sql LinguistTools)
        endif()
        target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core)
        target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Concurrent)
    elseif(DEFINED QT_VERSION_MAJOR AND QT_VERSION_MAJOR GREATER_EQUAL 6)
        if(NOT TARGET Qt${QT_VERSION_MAJOR}::Widgets)
            find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Concurrent Network PrintSupport OpenGL Svg Qml LinguistTools)
        endif()
        target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core)
        target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Concurrent)
    else()
        # Fallback: try Qt5 if nothing defined
        if(NOT TARGET Qt5::Widgets)
            find_package(Qt5 COMPONENTS Widgets Concurrent Network Script ScriptTools PrintSupport OpenGL Svg Sql QUIET)
        endif()
        if(TARGET Qt5::Widgets)
            target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Concurrent)
        endif()
    endif()
endif()

if(DEFINED QT_VERSION_MAJOR AND QT_VERSION_MAJOR EQUAL 5)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Script)
endif()

if(DEFINED QT_VERSION_MAJOR AND QT_VERSION_MAJOR GREATER_EQUAL 6)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Qml)
endif()

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE Wintrust)
    target_link_libraries(${PROJECT_NAME} PRIVATE Crypt32)
endif()

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

# On Windows create a console subsystem explicitly for clarity
if(WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:CONSOLE)
    if(MSVC)
        # Avoid duplicate manifest: our resource.rc already includes windows.manifest.xml
        target_link_options(${PROJECT_NAME} PRIVATE /MANIFEST:NO)
    endif()
endif()

if(WIN32)
    install (TARGETS ${PROJECT_NAME} DESTINATION "./")
    include(../../dep/build_tools/cmake/deploy_qt_windows.cmake)
    include(../../dep/build_tools/cmake/deploy_msvc.cmake)
elseif(APPLE)
    # no console version for macOS app bundle
else()
    install (TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
