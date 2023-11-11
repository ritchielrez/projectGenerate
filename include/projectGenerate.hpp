#ifndef PROJECT_GENERATE_H
#define PROJECT_GENERATE_H

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

#ifdef WIN32
#include <direct.h>
#include <windows.h>
#define GetCurrentDir _getcwd
#else
#include <signal.h>
#include <unistd.h>
#define getCurrentDir getcwd
#endif

#ifdef PROJ_GEN_WINDOWS
#include <direct.h>
#include <windows.h>
#define getCurrentDir _getcwd

#else
#ifdef PROJ_GEN_UNIX
#include <signal.h>
#include <unistd.h>
#define getCurrentDir getcwd
#endif

#endif

#define STRING_SIZE_NAME_MAX 256

namespace projectGenerate
{
inline int appendPath(char *dest, const char *src)
{
    std::size_t destlen = strlen(dest) + strlen(src) + 1;

    if (destlen <= STRING_SIZE_NAME_MAX)
    {
#ifdef WIN32
        strcat(dest, "\\");
#else
        strcat(dest, "/");
#endif
        strcat(dest, src);
        return 0;
    }
    return 1;
}

inline int makeDir(const char *path)
{
    char cmd[STRING_SIZE_NAME_MAX] = "mkdir ";
    strcat(cmd, path);

    if (system(cmd) == -1)
        return 1;
    return 0;
}

inline int strConcat(char *dst, const char *src)
{
    if (strlen(dst) + strlen(src) > STRING_SIZE_NAME_MAX)
    {
        return 1;
    }
    strcat(dst, src);
    return 0;
}

inline void printHelpMsg()
{
    std::cout << "A simple cpp project generator written in C++\n\n";
    std::cout << "Usage: projectGenerate [OPTIONS] [PROJECT_NAME]\n\n";
    std::cout << "Arguments:\n";
    std::cout << "\t[PROJECT_NAME]... [default: none]\n\n";
    std::cout << "Options:\n";
    std::cout << "\tcreate    \t\t\t Create a new project\n";
    std::cout << "\t-h, --help\t\t\t Print this help message\n";
    std::cout << "\n";
}

inline void createProject(char *projectDirName)
{
    char currentPath[STRING_SIZE_NAME_MAX];
    char projectPath[STRING_SIZE_NAME_MAX];
    char includePath[STRING_SIZE_NAME_MAX];
    char srcPath[STRING_SIZE_NAME_MAX];
    char srcFileName[STRING_SIZE_NAME_MAX];
    char includeFileName[STRING_SIZE_NAME_MAX];
    char cmakeFileName[STRING_SIZE_NAME_MAX];

    // Generate a clang-format file based on Microsoft formatting style in the project directory
    char clangFormatCmd[STRING_SIZE_NAME_MAX] = "clang-format -style=microsoft -dump-config > ";
    if (strConcat(clangFormatCmd, projectDirName))
    {
        std::cerr << "Concatenated string exceeds " << STRING_SIZE_NAME_MAX << " characters\n";
    }
#ifdef WIN32
    if (strConcat(clangFormatCmd, "\\.clang-format"))
    {
        std::cerr << "Concatenated string exceeds " << STRING_SIZE_NAME_MAX << " characters\n";
    }
#else
    if (strConcat(clangFormatCmd, "/.clang-format"))
    {
        std::cerr << "Concatenated string exceeds " << STRING_SIZE_NAME_MAX << " characters\n";
    }
#endif

    std::string srcFileBuffer =
        "#define HEADER_FILE_IMPLEMENTATION\n#include <iostream>\n\nint main()\n{\n\tstd::cerr << "
        "\"Helllo World\\n\";\n\treturn 0;\n}\n";
    std::string includeFileBuffer =
        "#ifndef HEADER_FILE_INCLUDED\n#define HEADER_FILE_INCLUDED\n#endif // HEADER_FILE_INCLUDED\n#ifdef "
        "HEADER_FILE_IMPLEMENTATION\n#endif // HEADER_FILE_IMPLEMENTATION\n";
    std::string cmakeFileBuffer =
        "cmake_minimum_required(VERSION 3.10)\nset(CXX_STANDARD 20)\nset(CXX_STANDARD_REQUIRED ON)\n\nproject(";
    cmakeFileBuffer += projectDirName;
    cmakeFileBuffer +=
        " VERSION 1.0)\n\nadd_executable(main src/main.cpp)\n\ntarget_include_directories(main PRIVATE \"include/\")\n";

    if (!getCurrentDir(currentPath, STRING_SIZE_NAME_MAX))
        exit(errno);

    // Make the project root directory
    // First, copy the current path from where the programing is running, to the `projectPath` string
    // `projectPath` string now contains the current working directory path, make a folder
    // named after the 1st argument passed by the user to the program
    strncpy(projectPath, currentPath, STRING_SIZE_NAME_MAX);
    if (appendPath(projectPath, projectDirName))
    {
        std::cerr << "Full path of the project directory exceeds 256 characters\n";
        exit(1);
    }
    if (makeDir(projectPath))
    {
        std::cerr << "Error creating project directory\n";
        exit(1);
    }

    // Now let's make the include directory inside the project root directory
    // We copy the path to the project root directory, to the `includePath` string
    strncpy(includePath, projectPath, STRING_SIZE_NAME_MAX);
    if (appendPath(includePath, "include"))
    {
        std::cerr << "Full path of the include directory inside the project directory exceeds 256 characters\n";
        exit(1);
    }
    if (makeDir(includePath))
    {
        std::cerr << "Error creating include directory\n";
        exit(1);
    }

    // Do the same to create a src directory inside the project root directory
    strncpy(srcPath, projectPath, STRING_SIZE_NAME_MAX);
    if (appendPath(srcPath, "src"))
    {
        std::cerr << "Full path of the src directory inside the project directory exceeds 256 characters\n";
        exit(1);
    }
    if (makeDir(srcPath))
    {
        std::cerr << "Error creating src directory\n";
        exit(1);
    }

    // Now let's create main.cpp file in the src directory
    // First, copy the src directory path to `srcFileName` string, then create the src file
    strncpy(srcFileName, srcPath, STRING_SIZE_NAME_MAX);
    if (appendPath(srcFileName, "main.cpp"))
    {
        std::cerr << "Full path of the src file inside the src directory exceeds 256 characters\n";
        exit(1);
    }

    std::ofstream srcFile(srcFileName);
    srcFile << srcFileBuffer;
    srcFile.close();

    // In the same way we create the header file
    strncpy(includeFileName, includePath, STRING_SIZE_NAME_MAX);
    if (appendPath(includeFileName, projectDirName))
    {
        std::cerr << "Full path of the header file inside the include directory exceeds 256 characters\n";
        exit(1);
    }
    if (strConcat(includeFileName, ".hpp"))
    {
        std::cerr << "Concatenated string exceeds " << STRING_SIZE_NAME_MAX << " characters\n";
    }

    std::ofstream includeFile(includeFileName);
    includeFile << includeFileBuffer;
    includeFile.close();

    // Now we will the create the CMakeLists.txt file
    strncpy(cmakeFileName, projectDirName, STRING_SIZE_NAME_MAX);
    if (appendPath(cmakeFileName, "CMakeLists.txt"))
    {
        std::cerr << "Full path of the CMakeLists.txt file inside the project directory exceeds 256 characters\n";
        exit(1);
    }

    std::ofstream cmakeFile(cmakeFileName);
    cmakeFile << cmakeFileBuffer;
    cmakeFile.close();

    // Now generate clang-format config based on Microsoft style
    system(clangFormatCmd);
}

} // namespace projectGenerate

#endif // PROJECT_GENERATE_H
