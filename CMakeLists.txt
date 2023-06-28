cmake_minimum_required(VERSION 3.10)
set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(WARNINGS_AS_ERRORS TRUE)

project(projectGenerate VERSION 1.0)

include(cmake/CompilerWarnings.cmake)
include(cmake/StandardProjectSettings.cmake)
include(cmake/StaticAnalyzers.cmake)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE "include/")
