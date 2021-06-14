# Ngine
Ngine is yet another attempt to make a personal game engine that offer good tools to make my games

## Building
The engine uses CMake 3.14 as its build tool generator. Therefore, to build ngine, make sure you have cmake installed
and use it to generate a makefile for your favorite build tool/IDE and build the project from there.

## Project structure
Ngine uses a modular structure where each module is a library that must be linked to the final executable to be used.
Each module contains:
* A public folder, containing all public headers that can be included from other targets
* A private folder, containing all the source files that cannot be included from external targets.
* A CMakeLists.txt file, describing the module and linking it to it's dependencies

## License
[MIT](https://choosealicense.com/licenses/mit/)