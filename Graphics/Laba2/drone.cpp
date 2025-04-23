#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>

// Структура для хранения точек следа
struct TrailPoint {
    float x, y;
    float alpha; // Прозрачность для эффекта затухания
};

// Координаты дрона и параметры движения
float droneX = 0.0f;
float droneY = 0.0f;
float angle = 0.0f;
float radius = 0.5f;
float speed = 0.02f;
float minRadius = 0.1f;
float maxRadius = 0.5f;
bool isMoving = false;
bool isShrinking = true;
bool isClockwise = true;
bool rPressed = false;

// Вектор для хранения следа
std::vector<TrailPoint> trail;
const int maxTrailLength = 100; // Максимальная длина следа

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

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

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !rPressed) {
        isClockwise = !isClockwise;
        isShrinking = !isShrinking;
        rPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
        rPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        speed += 0.001f;
        if (speed > 0.1f) speed = 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        speed -= 0.001f;
        if (speed < 0.0f) speed = 0.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        radius -= 0.003f;
        if (radius < minRadius) radius = minRadius;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        radius += 0.003f;
        if (radius > maxRadius) radius = maxRadius;
    }
}

void updateDronePosition() {
    if (isMoving) {
        droneX = radius * cos(angle);
        droneY = radius * sin(angle);
        
        angle += isClockwise ? speed : -speed;

        if (isShrinking) {
            radius -= speed * 0.02f;
            if (radius <= minRadius) {
                radius = minRadius;
                isShrinking = false;
            }
        } else {
            radius += speed * 0.02f;
            if (radius >= maxRadius) {
                radius = maxRadius;
                isShrinking = true;
            }
        }

        // Добавляем новую точку в след
        trail.push_back({droneX, droneY, 1.0f});
        if (trail.size() > maxTrailLength) {
            trail.erase(trail.begin());
        }

        // Уменьшаем прозрачность старых точек
        for (auto& point : trail) {
            point.alpha -= 0.015f;
            if (point.alpha < 0.0f) point.alpha = 0.0f;
        }
    }
}

void renderTrail() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBegin(GL_LINE_STRIP);
    for (const auto& point : trail) {
        glColor4f(1.0f, 0.5f, 0.0f, point.alpha); // Оранжевый след с затуханием
        glVertex2f(point.x, point.y);
    }
    glEnd();
    
    glDisable(GL_BLEND);
}

void renderDrone() {
    // Отрисовываем след
    renderTrail();

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
    const float propRadius = 0.03f;
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
        updateDronePosition();

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