#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Координаты дрона и параметры движения
float droneX = 0.0f;
float droneY = 0.0f;
float angle = 0.0f;
float radius = 0.5f; // Начальный радиус
float speed = 0.02f; // скорость
bool isMoving = false; // летит или нет
bool moveTowardsCenter = true; // Направление полета (к центру или к внешнему радиусу)

void processInput(GLFWwindow* window) {
    // Проверяем нажатие клавиш
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); // Закрываем окно

    // Фуллскрин по F
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 800, 600, GLFW_DONT_CARE);

    // Запуск/остановка по пробелу
    static bool spacePressed = false;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!spacePressed) {
            isMoving = !isMoving;
            spacePressed = true;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        spacePressed = false;
    }

    // Ускорение
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        speed += 0.001f;
        if (speed > 0.1f) speed = 0.1f;
    }

    // Замедление
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        speed -= 0.001f;
        if (speed < 0.0f) speed = 0.0f;
    }

    // Смена направления по "r"
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        moveTowardsCenter = !moveTowardsCenter; // Меняем направление
    }
}

void updateDronePosition() {
    if (isMoving) {
        // Обновление позиции дрона с учетом спирального движения
        droneX = radius * cos(angle);
        droneY = radius * sin(angle);
        angle += speed; // Увеличиваем угол для спирального движения

        // Если движемся к центру, уменьшаем радиус
        // Если движемся наружу, увеличиваем радиус
        if (moveTowardsCenter) {
            radius -= speed * 0.01f;
            if (radius < 0.1f) radius = 0.1f; // Минимальный радиус
        } else {
            radius += speed * 0.01f;
            if (radius > 0.5f) radius = 0.5f; // Максимальный радиус
        }
    }
}

void renderDrone() {
    // Корпус
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex2f(droneX - 0.1f, droneY - 0.025f);
    glVertex2f(droneX + 0.1f, droneY - 0.025f);
    glVertex2f(droneX + 0.1f, droneY + 0.025f);
    glVertex2f(droneX - 0.1f, droneY + 0.025f);
    glEnd();

    // Пропеллеры 
    const int numSegments = 20;
    const float propRadius = 0.03f; // Меньший радиус пропеллеров
    glColor3f(1.0f, 0.0f, 0.0f);
    
    float propPositions[4][2] = {
        {-0.15f, 0.1f},
        {0.15f, 0.1f},
        {-0.15f, -0.1f},
        {0.15f, -0.1f}
    };
    
    for (auto& pos : propPositions) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(droneX + pos[0], droneY + pos[1]);
        for (int i = 0; i <= numSegments; i++) {
            float propAngle = 2.0f * M_PI * i / numSegments;
            glVertex2f(droneX + pos[0] + propRadius * cos(propAngle), 
                      droneY + pos[1] + propRadius * sin(propAngle));
        }
        glEnd();
    }
}

int main() {
    std::cout << "Введите радиус полета дрона (например, 0.5): ";
    std::cin >> radius;

    if (radius <= 0.0f || radius > 1.0f) {
        std::cout << "Радиус должен быть > 0 и < 1. Установлено значение 0.5" << std::endl;
        radius = 0.5f;
    }

    if (!glfwInit()) {
        std::cerr << "не запустился glfw" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Drone Control", NULL, NULL);
    if (!window) {
        std::cerr << "не создалось окно" << std::endl;
        glfwTerminate();
        return -1;
    }

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowPos(window, (mode->width - 800) / 2, (mode->height - 600) / 2);
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "не запустился glew" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        updateDronePosition(); // Двигаем

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
