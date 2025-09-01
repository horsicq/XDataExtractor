cmake_minimum_required(VERSION 3.16)

# Export compile commands for IDE integration (VS Code, clangd, etc.)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Read project version from release_version.txt and make it available to subprojects
file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/release_version.txt" X_PROJECT_VERSION)
if(NOT X_PROJECT_VERSION)
	set(X_PROJECT_VERSION "0.0.0")
endif()
set(X_PROJECT_VERSION "${X_PROJECT_VERSION}" CACHE STRING "Project version" FORCE)

project(XDE VERSION ${X_PROJECT_VERSION} LANGUAGES CXX)

add_subdirectory(src)

