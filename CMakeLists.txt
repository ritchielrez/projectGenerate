cmake_minimum_required(VERSION 3.10)
set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(WARNINGS_AS_ERRORS TRUE)

project(projectGenerate VERSION 1.0)

include(cmake/CompilerWarnings.cmake)
include(cmake/StandardProjectSettings.cmake)
include(cmake/StaticAnalyzers.cmake)

if(WIN32)
    add_compile_definitions(PROJ_GEN_WINDOWS)
endif(WIN32)
if (UNIX)
    add_compile_definitions(PROJ_GEN_UNIX)
endif (UNIX)

add_executable(${PROJECT_NAME} "src/main.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE "include/")
