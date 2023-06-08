cmake_minimum_required(VERSION 3.10)
set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

project(projectGenerate VERSION 1.0)

# Disable CRT_SECURE_WARNINGS of MSVC
if(WIN32)
    if(NOT MINGW)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    endif()
endif()

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE "include/")
