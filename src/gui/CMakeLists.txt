cmake_minimum_required(VERSION 3.16)

project(XDataExtractor VERSION ${X_PROJECT_VERSION} LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/../../dep/build_tools/cmake/cpp_standart_setup.cmake)

message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
include(${CMAKE_CURRENT_LIST_DIR}/../../dep/build_tools/cmake/init_translation.cmake)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/../../dep/XExtractorWidget/xextractorwidget.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../dep/XAboutWidget/xaboutwidget.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../dep/XStyles/xstyles.cmake)

set(PROJECT_SOURCES
    ${XEXTRACTORWIDGET_SOURCES}
    ${XABOUTWIDGET_SOURCES}
    dialogabout.cpp
    dialogabout.h
    dialogabout.ui
    dialogoptions.cpp
    dialogoptions.h
    dialogoptions.ui
    guimainwindow.cpp
    guimainwindow.h
    guimainwindow.ui
    main_gui.cpp
    resources.qrc
)

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
include(${CMAKE_CURRENT_LIST_DIR}/../../dep/build_tools/cmake/create_translation.cmake)
endif()

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32
        ${PROJECT_SOURCES}
        ${QM_FILES}
        ../../res/resource_icon.rc
    )
elseif(APPLE)
    add_executable(${PROJECT_NAME}
        MACOSX_BUNDLE
        ${PROJECT_SOURCES}
        ${QM_FILES}
    )

    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/../../res/main.icns PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../res/main.icns)

else()
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${QM_FILES}
    )
endif()

set(XDE_DEFINES USE_DEX USE_PDF USE_ARCHIVE)
if(WIN32)
    list(APPEND XDE_DEFINES NOMINMAX)
endif()
target_compile_definitions(${PROJECT_NAME} PRIVATE ${XDE_DEFINES})

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    if(APPLE)
        include(${CMAKE_CURRENT_LIST_DIR}/../../dep/build_tools/cmake/macdeployqt.cmake)
    endif()
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE bzip2)
target_link_libraries(${PROJECT_NAME} PRIVATE lzma)
target_link_libraries(${PROJECT_NAME} PRIVATE zlib)
target_link_libraries(${PROJECT_NAME} PRIVATE capstone)
target_link_libraries(${PROJECT_NAME} PRIVATE cppfilt)
target_link_libraries(${PROJECT_NAME} PRIVATE yara)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Concurrent)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::PrintSupport)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::OpenGL)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Svg)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Sql)

if(${QT_VERSION_MAJOR} EQUAL 5)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Script)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::ScriptTools)
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

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_INSTALL_BINDIR: ${CMAKE_INSTALL_BINDIR}")
message(STATUS "CMAKE_INSTALL_LIBDIR: ${CMAKE_INSTALL_LIBDIR}")

if(WIN32 AND MSVC)
    # Avoid duplicate manifest: our resource file already embeds windows.manifest.xml
    target_link_options(${PROJECT_NAME} PRIVATE /MANIFEST:NO)
endif()

if(WIN32)
    install (TARGETS ${PROJECT_NAME} DESTINATION "./")
    include(${CMAKE_CURRENT_LIST_DIR}/../../dep/build_tools/cmake/deploy_qt_windows.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/../../dep/build_tools/cmake/deploy_msvc.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/../../dep/build_tools/cmake/deploy_openssl.cmake)
elseif(APPLE)
    install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
else()
    install (TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
