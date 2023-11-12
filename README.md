# Ritchiel's C++ Project Generator
## This is a project generator to make generating new cmake based projects much easier
### Description
Creating a new project from scratch in **C++** is easy, when you are just probably going to create a CLI project.
The problem is though you have to go through same old processes, to create the directory structure of the project and
a `CMakeLists.txt` to make sure it compiles. To make this repetitive process easier for myself, I have started this 
project, any feedbacks to improve it would be much appreciated.

### How to run the project
It is faily a simple proccess to compile this project. Just clone this repository to a directory and change into it.
Then run these commands in the **Terminal/Command Prompt** from the project directory:
```
cmake -Bbuild/ -S.
cmake --build build
```

Then run the executable with the project name to generate a new project:
```
./build/projectGenerate create my-cpp-project
```

#### And that's it, now you have your project generated, it is that simple.
