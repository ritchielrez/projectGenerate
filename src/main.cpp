#include "projectGenerate.hpp"

#include <cassert>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>

#ifdef WINDOWS
#include <direct.h>
#include <windows.h>
#define GetCurrentDir _getcwd
#else
#include <signal.h>
#include <unistd.h>
#define getCurrentDir getcwd
#endif

int main(int argc, char *argv[])
{
    assert(("Invalid number of arguments", argc == 2));

    char currentPath[STRING_SIZE_NAME_MAX];
    char projectPath[STRING_SIZE_NAME_MAX];
    char includePath[STRING_SIZE_NAME_MAX];
    char srcPath[STRING_SIZE_NAME_MAX];
    char srcFileName[STRING_SIZE_NAME_MAX];
    char includeFileName[STRING_SIZE_NAME_MAX];
    char cmakeFileName[STRING_SIZE_NAME_MAX];
    char *projectDirName = argv[1];

    char clangFormatCmd[STRING_SIZE_NAME_MAX] = "clang-format -style=microsoft -dump-config > ";
    strcat(clangFormatCmd, projectDirName);
    strcat(clangFormatCmd, "/.clang-format");

    std::string srcFileBuffer = "#define HEADER_FILE_DEFINITION\n#include <iostream>\n\nint main(){\n\tstd::cout << "
                                "\"Helllo World\\n\";\n\treturn 0;\n}\n";
    std::string includeFileBuffer =
        "#ifndef HEADER_FILE_INCLUDED\n#define HEADER_FILE_INCLUDED\n#endif // HEADER_FILE_INCLUDED\n#ifdef "
        "HEADER_FILE_DEFINITION\n#endif // HEADER_FILE_DEFINITION\n";
    std::string cmakeFileBuffer =
        "cmake_minimum_required(VERSION 3.10)\nset(CXX_STANDARD 20)\nset(CXX_STANDARD_REQUIRED ON)\n\nproject(";
    cmakeFileBuffer += projectDirName;
    cmakeFileBuffer +=
        " VERSION 1.0)\n\nadd_executable(main src/main.cpp)\n\ntarget_include_directories(main PRIVATE \"include/\")\n";

    if (!getCurrentDir(currentPath, STRING_SIZE_NAME_MAX))
        return errno;

    // Make the project root directory
    // First, copy the current path from where the programing is running, to the `projectPath` string
    // `projectPath` string now contains the current working directory path, make a folder
    // named after the 1st argument passed by the user to the program

    strncpy(projectPath, currentPath, STRING_SIZE_NAME_MAX);
    if (projectGenerate::appendPath(projectPath, projectDirName))
    {
        std::cout << "Full path of the project directory exceeds 256 characters\n";
        return 1;
    }
    if (projectGenerate::makeDir(projectPath))
    {
        std::cout << "Error creating project directory\n";
        return 1;
    }

    // Now let's make the include directory inside the project root directory
    // We copy the path to the project root directory, to the `includePath` string
    // Now we the same thing as above to create your desired include directory

    strncpy(includePath, projectPath, STRING_SIZE_NAME_MAX);
    if (projectGenerate::appendPath(includePath, "include"))
    {
        std::cout << "Full path of the include directory inside the project directory exceeds 256 characters\n";
        return 1;
    }
    if (projectGenerate::makeDir(includePath))
    {
        std::cout << "Error creating include directory\n";
        return 1;
    }

    // Do the same to create a src directory inside the project root directory

    strncpy(srcPath, projectPath, STRING_SIZE_NAME_MAX);
    if (projectGenerate::appendPath(srcPath, "src"))
    {
        std::cout << "Full path of the src directory inside the project directory exceeds 256 characters\n";
        return 1;
    }
    if (projectGenerate::makeDir(srcPath))
    {
        std::cout << "Error creating src directory\n";
        return 1;
    }

    // Now let's create main.cpp file in the src directory
    // First, copy the src directory path to `srcFileName` string, then create the src file

    strncpy(srcFileName, srcPath, STRING_SIZE_NAME_MAX);
    if (projectGenerate::appendPath(srcFileName, "main.cpp"))
    {
        std::cout << "Full path of the src file inside the src directory exceeds 256 characters\n";
        return 1;
    }

    std::ofstream srcFile(srcFileName);
    srcFile << srcFileBuffer;
    srcFile.close();

    // In the same way we create the header file
    strncpy(includeFileName, includePath, STRING_SIZE_NAME_MAX);
    if (projectGenerate::appendPath(includeFileName, projectDirName))
    {
        std::cout << "Full path of the header file inside the include directory exceeds 256 characters\n";
    }
    strcat(includeFileName, ".hpp");

    std::ofstream includeFile(includeFileName);
    includeFile << includeFileBuffer;
    includeFile.close();

    // Now we will the create the CMakeLists.txt file
    strncpy(cmakeFileName, projectDirName, STRING_SIZE_NAME_MAX);
    if (projectGenerate::appendPath(cmakeFileName, "CMakeLists.txt"))
    {
        std::cout << "Full path of the CMakeLists.txt file inside the project directory exceeds 256 characters\n";
    }

    std::ofstream cmakeFile(cmakeFileName);
    cmakeFile << cmakeFileBuffer;
    cmakeFile.close();

    // Now generate clang-format config based on Microsoft style
    system(clangFormatCmd);

    return 0;
}
