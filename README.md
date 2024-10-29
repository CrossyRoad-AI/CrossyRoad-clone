# Crossy road clone

Clone of the famous mobiler game [Crossy Road](https://www.crossyroad.com/), coded in C++ from scratch.

## Game

The current version only feature random terrain generation, hot game restart, player movement and collisions. <br />
Only two terrain types are available at the moment, but new types can be added easily later on.

## Rendering engine

The rendering engine is using OpenGL and some low level libraries to have a 3D rendering of the game. The engine currently support **.obj** file as models source files with data about vertices positions, colors and normals.

The engine currently implement a very basic lighting system, with single source point light. <br />
Basic text rendering is available, **.ttf** font file are processed using the **freetype** OpenGL library.

Finally, keyboard and mouse input are fully supported with event callback registration for multiple type of events.

## File tree

**Engine** contains all OpenGL and rendering related code. This should, as of now, be enough to create a basic, but clomplete, clone of Crossy road. <br />
**Game** contain all code related to the actual game clone: game logic, input listeners ... <br />
**Assets** folder contain all data that is not compiled. The content of the folder is copied to **dist** at build time. <br />
**Lib** contain all .h file required to imports used libs. Currently used libs are: **glew**, **glfw3**, **freetype** and **glm**

## Execution

Running the game should not require that much setup as no particular software of dependency is required. <br />
However, the compiler must be configured properly to compile the gane.

**Note:** you still need to have **gcc** and **make** installed on your machine !

### Compiling

The compilation target platform is Windows (it will currently not work on any other platform) with x86-64 architecture. <br />
That means that the compiler, which is g++ (or gcc), need to be configured with 64 bits compile mode (on windows gcc use mingw under the hood to compile C code so it must be configured for 64 bits compilation).

### Downloading source code

Clone the repo with:
```sh
git clone https://github.com/CrossyRoad-AI/CrossyRoad-clone
```

**Note:** to run the latest working version of the game, head to the main branch and follow the **Running** steps.

### Running the game

Head to the root folder of the project and simply run make. <br />
The makefile will handle everything (compilation, copying files ...) and automaticly run the generated **.exe** file.

File compilation output are stored in the **dist** folder. Note that it is safe to empty this folder if nedeed as all required .dll dependencies and other sources files needed to run the game have a copy stored into the **assets** folder.

**Note:** On windows, DO NOT use **ctrl-c** to close the program as the terminal will crash. Prefer using the proper exit method mentionned in **Using the game**

## Using the game

When running the **.exe** file the game window will show up with the game running.

Use the usual game keys W, A, S, D to move around. <br />
The **space** will enable the wireframe mode. It's not needed to play but is a usefull debugging feature.

To quit the game you can either:
- use the **ESC** key
- use the close icon on the window
- use the close button on Windows task bar
- or kill the process with the task manager

## Acronyms

GLFW: GL Frame Work
GLM: OpenGL Mathematics

## License

**TODO**: add license file