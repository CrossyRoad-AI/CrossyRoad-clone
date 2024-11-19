#include <iostream>
#include <stdlib.h>

#include <glew.h>

char* loaderShaderFile(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if(file == NULL) std::cout << "Error when reading shader file" << std::endl;

    // Calculate file length (aka get size to malloc char*)
    fseek(file, 0, SEEK_END);
    const long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Alloc memory to store file
    char *string = (char *) malloc(sizeof(char) * fsize);
    for(int i = 0; i < fsize; i++) string[i] = 0;

    // Read entire file
    fread(string, 1, fsize, file);

    fclose(file);
    return string;
}

unsigned int compileShader(const unsigned int type, const char* source) {
    // Create shader
    const unsigned int shader = glCreateShader(type);

    // Load and compile shader
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Verify shader
    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    
    if(result == GL_FALSE) { // If validation fail, extract error code
        // Get error message length
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        // Lload message into string
        char* message = (char *) malloc(length * sizeof(char));
        glGetShaderInfoLog(shader, length, &length, message);

        // Print error message
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;

        free(message);
        exit(6);
    }

    return shader;
}

unsigned int createShaderProgram(const char* vertexShaderFilename, const char* fragmentShadeFilename) {
    // Load shader code as string from source files
    char* vertexShader = loaderShaderFile(vertexShaderFilename);
    char* fragmentShader = loaderShaderFile(fragmentShadeFilename);

    // Compile shaders
    const unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    const unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Generate program
    const unsigned int program = glCreateProgram();

    // Attach shaders to program
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    // Free up space by deleting compiled shaders shaders
    glDeleteShader(vs);
    glDeleteShader(fs);

    // Free shaders source code
    free(vertexShader);
    free(fragmentShader);

    return program;
}