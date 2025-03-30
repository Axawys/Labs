#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

float droneX = 0.0f;
float droneY = 0.0f;
float droneSpeed = 0.0f;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        droneY += droneSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        droneY -= droneSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        droneX -= droneSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        droneX += droneSpeed;

    if (droneX > 1.0f) droneX = 1.0f;
    if (droneX < -1.0f) droneX = -1.0f;
    if (droneY > 1.0f) droneY = 1.0f;
    if (droneY < -1.0f) droneY = -1.0f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 800, 600, GLFW_DONT_CARE);
}

void renderDrone() {
    // Вытянутый корпус дрона
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.5f, 0.5f); // Серый цвет корпуса
    glVertex2f(droneX - 0.2f, droneY - 0.05f);  // Удлиненный влево
    glVertex2f(droneX + 0.2f, droneY - 0.05f);  // Удлиненный вправо
    glVertex2f(droneX + 0.2f, droneY + 0.05f);
    glVertex2f(droneX - 0.2f, droneY + 0.05f);
    glEnd();

    // Стойки для пропеллеров (4 штуки)
    glBegin(GL_LINES);
    glColor3f(0.3f, 0.3f, 0.3f); // Темно-серый цвет стоек
    
    // Левая верхняя стойка
    glVertex2f(droneX - 0.15f, droneY + 0.05f);
    glVertex2f(droneX - 0.25f, droneY + 0.15f);
    
    // Правая верхняя стойка
    glVertex2f(droneX + 0.15f, droneY + 0.05f);
    glVertex2f(droneX + 0.25f, droneY + 0.15f);
    
    // Левая нижняя стойка
    glVertex2f(droneX - 0.15f, droneY - 0.05f);
    glVertex2f(droneX - 0.25f, droneY - 0.15f);
    
    // Правая нижняя стойка
    glVertex2f(droneX + 0.15f, droneY - 0.05f);
    glVertex2f(droneX + 0.25f, droneY - 0.15f);
    glEnd();

    // Пропеллеры (круги) - используем GL_TRIANGLE_FAN для аппроксимации круга
    const int numSegments = 20;
    const float radius = 0.05f;
    glColor3f(1.0f, 0.0f, 0.0f); // Красный цвет пропеллеров

    // Левый верхний пропеллер
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(droneX - 0.25f, droneY + 0.15f); // Центр
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * M_PI * i / numSegments;
        glVertex2f(droneX - 0.25f + radius * cos(angle), 
                  droneY + 0.15f + radius * sin(angle));
    }
    glEnd();

    // Правый верхний пропеллер
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(droneX + 0.25f, droneY + 0.15f);
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * M_PI * i / numSegments;
        glVertex2f(droneX + 0.25f + radius * cos(angle), 
                  droneY + 0.15f + radius * sin(angle));
    }
    glEnd();

    // Левый нижний пропеллер
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(droneX - 0.25f, droneY - 0.15f);
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * M_PI * i / numSegments;
        glVertex2f(droneX - 0.25f + radius * cos(angle), 
                  droneY - 0.15f + radius * sin(angle));
    }
    glEnd();

    // Правый нижний пропеллер
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(droneX + 0.25f, droneY - 0.15f);
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * M_PI * i / numSegments;
        glVertex2f(droneX + 0.25f + radius * cos(angle), 
                  droneY - 0.15f + radius * sin(angle));
    }
    glEnd();
}

int main() {
    std::cout << "Введите скорость движения дрона (например, 0.05): ";
    std::cin >> droneSpeed;

    if (droneSpeed <= 0.0f || droneSpeed > 1.0f) {
        std::cout << "Скорость должна быть больше 0 и меньше 1. Установлено значение по умолчанию: 0.05" << std::endl;
        droneSpeed = 0.05f;
    }

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Drone Control", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowPos(window, (mode->width - 800) / 2, (mode->height - 600) / 2);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        renderDrone();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}