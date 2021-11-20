# GLFWCMakeTemplate

A simple template for a C application using GLFW with GLAD and CMake.

All libraries are fetched with cmake's FetchContent feature from their github repos.

The GLAD version being used loads the latest version of core opengl but not any extensions. It is fetched from a repo from my github account.

The `main.c` file contains some basic code which sets up glfw with some convenient input handlers.
