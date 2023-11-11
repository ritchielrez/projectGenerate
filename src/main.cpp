#include "projectGenerate.hpp"

#include <cstring>

enum Options
{
    noOpt,
    createOpt,
    helpOpt
};

int main(int argc, char *argv[])
{
    enum Options current_option = noOpt;
    if (argc < 2)
    {
        projectGenerate::printHelpMsg();
        return 0;
    }

    if (strcmp(argv[1], "create") == 0)
    {
        current_option = createOpt;
    }
    else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        current_option = helpOpt;
    }
    else
    {
        return 1;
    }

    switch(current_option) {
        case createOpt:
            if(argc < 3)
            {
                std::cerr << "Provide a project name to create one\n";
                return 1;
            }
            projectGenerate::createProject(argv[2]);
        break;
        case helpOpt:
            projectGenerate::printHelpMsg();
        break;
        case noOpt:
        break;
    }

    return 0;
}
