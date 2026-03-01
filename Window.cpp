#define GLFW_INCLUDE_NONE
#include <filesystem>
#include <fstream>
#include <ios>
#include <ostream>
#include <string>
#include <cstring>
#include <GLFW/glfw3.h>
#include "glad/glad/glad.h"
#include <iostream>

std::string readFile(std::filesystem::path path){
    std::ifstream in(path, std::ios::binary);
    const auto sz = std::filesystem::file_size(path);
    std::string result(sz, '\0');
    in.read(result.data(), sz);

    return result;
}

unsigned int compileShader(unsigned int shaderType, std::string& src){
    unsigned id = glCreateShader(shaderType);
    const char* raw = src.c_str();
    glShaderSource(id, 1, &raw, nullptr);
    glCompileShader(id);

    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(id, 512, nullptr, log);
        std::cerr << "Shader error: " << log << std::endl;
    }
    return id;
}

unsigned int createShader(std::string& vertex, std::string& fragment){
    unsigned vs = compileShader(GL_VERTEX_SHADER, vertex);
    unsigned fs = compileShader(GL_FRAGMENT_SHADER, fragment);

    unsigned program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void){
    GLFWwindow* window;

    if (!glfwInit())
    {
        return -1;
    }

    window = glfwCreateWindow(800, 600, "a", NULL, NULL);
    if (!window){
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "OpenGL load fail" << std::endl;
        return -1;
    }

    float vertices[] = {
        -1.0, -1.0,
        1.0, -1.0,
        1.0, 1.0,

        1.0, 1.0,
        -1.0, 1.0,
        -1.0, -1.0

    };
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), vertices,  GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    std::string vertex = readFile("/home/user/projects/CPPprojects/OpenGlTest/vertex.shader");
    std::string fragment = readFile("/home/user/projects/CPPprojects/OpenGlTest/fragment.shader");

    unsigned int shader = createShader(vertex, fragment);

    glUseProgram(shader);

    glUniform2f(glGetUniformLocation(shader, "resolution"), 800.0, 600.0);

    while (!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        glUniform1f(glGetUniformLocation(shader, "time"), (float)glfwGetTime());
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}