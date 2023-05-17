#ifndef PROJECT_GENERATE_H
#define PROJECT_GENERATE_H

#include <cstdlib>
#include <cstring>

#ifdef WINDOWS
#include <direct.h>
#include <windows.h>
#define GetCurrentDir _getcwd
#else
#include <signal.h>
#include <unistd.h>
#define getCurrentDir getcwd
#endif

#define STRING_SIZE_NAME_MAX 256

namespace projectGenerate
{
inline int appendPath(char *dest, const char *src)
{
    int destlen = strlen(dest) + strlen(src) + 1;

    if (destlen <= STRING_SIZE_NAME_MAX)
    {
        strcat(dest, "/");
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
    if(strlen(dst) + strlen(src) > STRING_SIZE_NAME_MAX)
    {
        return 1;
    }
    strcat(dst, src);
    return 0;
}

} // namespace projectGenerate

#endif // PROJECT_GENERATE_H
