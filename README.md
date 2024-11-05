# Crossy road clone

Clone of the famous mobile game [Crossy Road](https://www.crossyroad.com/), coded in C++ from scratch.

## Game

The current version only features random terrain generation, hot game restart, player movement and collisions. <br />
Only two terrain types are available at the moment, but new types can be added easily later on.

## Rendering engine

The rendering engine used is OpenGL and some low level libraries to have a 3D rendering of the game. The engine currently supports **.obj** files as the models' source files with data about vertex positions, colors and normals.

The engine currently implements a very basic lighting system, with single source point lighting. <br />
Basic text rendering is available, **.ttf** font files are processed using the **freetype** OpenGL library.

Finally, keyboard and mouse input is fully supported with event callback registration for multiple types of events.

## File tree

**Engine** contains all OpenGL and rendering related code. This should, as of now, be enough to create a basic, but clomplete, clone of Crossy road. <br />
**Game** contains all code related to the actual game clone: game logic, input listeners ... <br />
**Assets** folder contains all data that is not compiled. The content of the folder is copied to **dist** at build time. <br />
**Lib** contains all .h files required to import used libraries. Currently used libraries are: **glew**, **glfw3**, **freetype**, and **glm**

## Execution

Running the game should not require that much setup as no particular software of dependency is required. <br />
However, the compiler must be configured properly to compile the game.

**Note:** you still need to have **gcc** and **make** installed on your machine !

### Compiling

The compilation target platform is Windows (it will currently not work on any other platform) with x86-64 architecture. <br />
That means that the compiler, which is g++ (or gcc), needs to be configured with 64 bits compile mode (on windows gcc use mingw under the hood to compile C code so it must be configured for 64 bits compilation).

### Downloading source code

Clone the repo with:
```sh
git clone https://github.com/CrossyRoad-AI/CrossyRoad-clone
```

**Note:** to run the latest working version of the game, head to the main branch and follow the **Running** steps.

### Running the game

Head to the root folder of the project and simply run make. <br />
The makefile will handle everything (compilation, copying files ...) and automatically run the generated **.exe** file.

File compilation output is stored in the **dist** folder. Note that it is safe to empty this folder if nedeed as all required .dll dependencies and other sources files needed to run the game have a copy stored in the **assets** folder.

**Note:** On windows, DO NOT use **ctrl-c** to close the program as the terminal will crash. Use the proper exit method mentionned in **Using the game**.

## Using the game

When running the **.exe** file, the game window will pop up with the game running.

Use the usual game keys W, A, S, D to move around. <br />
The **space** will enable the wireframe mode. It's not needed to play but is a useful debugging feature.

To quit the game you can either:
- use the **ESC** key
- use the close icon on the window
- use the close button on Windows task bar
- or kill the process with the task manager

## Linking with python AI

Although the game is playable with the keyboard, it has been designed to be linked with another program. The communication with the other program is achieved with windows Shared Memory, which reserve a physical memory space, avaialbe for any program, to read and write to it.

A special communication protocol have been defined allowing for timed read and write to preserved data integrity. To simplify the integration of this protocol inside yor own app, you may use the python class called **sharedMemoryManager**, which is available inside the RL-model repo of the same organization.

For both programs, .exe and python, need to be executed as admin to have the rights to register and open a Shared memory space.

## Acronyms

GLFW: GL Frame Work <br />
GLM: OpenGL Mathematics

## License

**TODO**: add license file
