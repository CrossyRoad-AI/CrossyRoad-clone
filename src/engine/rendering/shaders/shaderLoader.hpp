#ifndef SHADERLOADER_HPP
#define SHADERLOADER_HPP

char* loaderShaderFile(const char* fileName);

unsigned int compileShader(const unsigned int type, const char* source);
unsigned int createShaderProgram(const char* vertexShaderFilename, const char* fragmentShadeFilename);

#endif