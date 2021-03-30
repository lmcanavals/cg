#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * Shaders source code using multiline strings part of the <string> header
 **/
const char* vertexShaderSrc = R"glsl(#version 330 core
layout (location = 0) in vec3 vertPos;
void main() {
    gl_Position = vec4(vertPos, 1.0f);
})glsl";

const char* fragmentShaderSrc = R"glsl(#version 330 core
out vec4 color;
void main() {
    color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
})glsl";

int ok;             // check for error status
char infoLog[512];  // get error messages

/**
 * callback to resize viewport on window resize
 **/
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

/**
 * keyboard input processing
 **/
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLuint mkShader(const char* src, GLenum type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error compiling shader\n"
            << infoLog << std::endl;
    }
    return shader;
}

int main() {
    if (!glfwInit()) {
        return -1;
    }
    // let glfw know our desire to use version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Un rectangulito",
                                          nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Can't create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Can't load GLAD\n";
        return -1;
    }

    GLuint vertexShader = mkShader(vertexShaderSrc, GL_VERTEX_SHADER);
    GLuint fragmentShader = mkShader(fragmentShaderSrc, GL_FRAGMENT_SHADER);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Can't link program\n"
            << infoLog << std::endl;
    }

    float vertices[] = {  0.5f,  0.5f,  0.0f,
                          0.5f, -0.5f,  0.0f,
                         -0.5f, -0.5f,  0.0f,
                         -0.5f,  0.5f,  0.0f };
    unsigned int indices[] = { 0, 1, 3,
                               1, 2, 3 };

    unsigned int vbo, vao, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(program);

    glfwTerminate();

    return 0;
}
