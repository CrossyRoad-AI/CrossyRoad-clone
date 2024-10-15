# Crossy road clone

## File tree

*Engine* contains all OpenGL and rendering related code.
This should, as of now, be enough to create a full clone of Crossy road.

*Game* contain all code related to the actual game clone: game logic, input listeners ...

*Assets* folder contain all data that is not compiled. The content of the folder is copied to *dist* at build time.

*Lib* contain all .h file required to imports used libs. Currently used libs are: glew, glfw3, freetype and glm

## Execution

No prior setup or software install required

Clone the repo with:
```sh
git clone https://github.com/CrossyRoad-AI/CrossyRoad-clone
```

Then simply run make to compile and run.
Compilation output is stored into *dist*

## Acronyms

GLFW: GL Frame Work
GLM: OpenGL Mathematics

## License

**TODO**: add license file