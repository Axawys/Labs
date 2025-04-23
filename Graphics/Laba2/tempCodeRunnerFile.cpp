#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Координаты дрона и параметры движения
float droneX = 0.0f;
float droneY = 0.0f;
float angle = 0.0f;
float radius = 0.5f;  // Начальный радиус
float speed = 0.02f;  // Скорость движения
float minRadius = 0.1f; // Минимальный радиус
float maxRadius = 0.5f; // Максимальный радиус
bool isMoving = false; // Летит ли дрон
bool isShrinking = true; // Сжимаем ли радиус или расширяем
bool isClockwise = true; // Двигается ли по часовой стрелке
bool rPressed = false; // Для отслеживания первого нажатия клавиши R

void processInput(GLFWwindow* window) {
    // Проверяем нажатие клавиш
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); // Закрываем окно

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

    // Изменение направления по 'r' и переключение режима изменения радиуса
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !rPressed) {
        isClockwise = !isClockwise;  // Меняем направление
        isShrinking = !isShrinking;  // Переключаем режим радиуса (уменьшать/увеличивать)
        rPressed = true;  // Фиксируем, что клавиша была нажата
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
        rPressed = false;  // Разрешаем следующее нажатие клавиши R
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

    // Изменение радиуса влево и вправо
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        radius -= 0.003f;  // Увеличиваем скорость уменьшения радиуса
        if (radius < minRadius) radius = minRadius;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        radius += 0.003f;  // Увеличиваем скорость увеличения радиуса
        if (radius > maxRadius) radius = maxRadius;
    }
}

void updateDronePosition() {
    if (isMoving) {
        // Двигаемся по спирали
        droneX = radius * cos(angle);
        droneY = radius * sin(angle);
        
        // Меняем направление движения в зависимости от isClockwise
        angle += isClockwise ? speed : -speed;

        // Колебания радиуса
        if (isShrinking) {
            radius -= speed * 0.02f; // Увеличиваем скорость сжатия радиуса
            if (radius <= minRadius) {
                radius = minRadius;
                isShrinking = false; // Переключаем режим на расширение
            }
        } else {
            radius += speed * 0.02f; // Увеличиваем скорость расширения радиуса
            if (radius >= maxRadius) {
                radius = maxRadius;
                isShrinking = true; // Переключаем режим на сжатие
            }
        }
    }
}

void drawDashedSpiral() {
    glColor3f(1.0f, 1.0f, 1.0f);  // Белый цвет для спирали
    const int numSegments = 200;
    float angleStep = 2.0f * M_PI / numSegments;

    glBegin(GL_LINES);
    for (int i = 0; i < numSegments; i++) {
        float currentAngle = i * angleStep;
        float nextAngle = (i + 1) * angleStep;
        
        // Определяем текущие и следующие координаты для двух точек
        float x1 = radius * cos(currentAngle);
        float y1 = radius * sin(currentAngle);
        float x2 = radius * cos(nextAngle);
        float y2 = radius * sin(nextAngle);
        
        // Рисуем линию (пунктир, только в случае определённых промежутков)
        if (i % 2 == 0) {
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
        }
    }
    glEnd();
}

void renderDrone() {
    // Отрисовываем пунктирную спираль
    drawDashedSpiral();

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
