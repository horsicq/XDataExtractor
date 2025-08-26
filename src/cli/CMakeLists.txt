cmake_minimum_required(VERSION 3.16)

project(xdec VERSION ${X_PROJECT_VERSION} LANGUAGES CXX)

include(../../dep/build_tools/cmake/cpp_standart_setup.cmake)

add_definitions(-DUSE_DEX)
add_definitions(-DUSE_PDF)
add_definitions(-DUSE_ARCHIVE)

if(WIN32)
    add_definitions(-DNOMINMAX)
endif()

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

target_link_libraries(${PROJECT_NAME} PRIVATE bzip2)
target_link_libraries(${PROJECT_NAME} PRIVATE lzma)
target_link_libraries(${PROJECT_NAME} PRIVATE zlib)
target_link_libraries(${PROJECT_NAME} PRIVATE capstone)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Concurrent)

if(${QT_VERSION_MAJOR} EQUAL 5)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Script)
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
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
