#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Вершины треугольника (с координатами и цветами)
GLfloat vertices[] = {
    // Координаты      // Цвета
    -0.6f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f, // Левый нижний (красный)
     0.0f,  0.6f, 0.0f, 0.0f, 1.0f, 0.0f, // Верхний (зеленый)
     0.6f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f  // Правый нижний (синий)
};

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    out vec3 vertexColor;
    void main() {
        gl_Position = vec4(aPos, 1.0);
        vertexColor = aColor;
    })";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 vertexColor;
    out vec4 FragColor;
    void main() {
        FragColor = vec4(vertexColor, 1.0f);
    })";

// Функция для компиляции шейдера
GLuint compileShader(GLenum shaderType, const char* source) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader Compilation Error: " << infoLog << std::endl;
    }
    return shader;
}

// Функция для создания программы с шейдерами
GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Program Linking Error: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main() {
    // Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Создание окна
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Triangle", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Создание контекста OpenGL
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
        glViewport(0, 0, width, height);
    });

    // Инициализация GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Создание и компиляция шейдерной программы
    GLuint shaderProgram = createShaderProgram();

    // Создание VAO и VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Привязка и заполнение VBO, VAO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Основной цикл отрисовки
    while (!glfwWindowShouldClose(window)) {
        // Очистка экрана
        glClear(GL_COLOR_BUFFER_BIT);

        // Использование шейдерной программы
        glUseProgram(shaderProgram);

        // Отрисовка треугольника
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // Обновление экрана
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Освобождение ресурсов
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
