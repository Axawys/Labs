#include <GL/glut.h>
#include <iostream>
#include <vector>

int numHouses = 1;

struct HousePosition {
    float xOffset;
    float yOffset;
    float scale;
};

std::vector<HousePosition> housePositions;

void drawHouse(float xOffset, float yOffset, float scale) {
    // Основной прямоугольник дома
    glColor3f(1.0f, 0.8f, 0.6f);
    glBegin(GL_QUADS);
        glVertex2f((-0.15f * scale) + xOffset, (-0.3f * scale) + yOffset);
        glVertex2f((0.15f * scale) + xOffset, (-0.3f * scale) + yOffset);
        glVertex2f((0.15f * scale) + xOffset, (0.2f * scale) + yOffset);
        glVertex2f((-0.15f * scale) + xOffset, (0.2f * scale) + yOffset);
    glEnd();

    // Крыша
    glColor3f(0.7f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLES);
        glVertex2f((-0.2f * scale) + xOffset, (0.2f * scale) + yOffset);
        glVertex2f((0.2f * scale) + xOffset, (0.2f * scale) + yOffset);
        glVertex2f((0.0f * scale) + xOffset, (0.5f * scale) + yOffset);
    glEnd();

    // Дверь
    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f((-0.05f * scale) + xOffset, (-0.3f * scale) + yOffset);
        glVertex2f((0.05f * scale) + xOffset, (-0.3f * scale) + yOffset);
        glVertex2f((0.05f * scale) + xOffset, (0.0f * scale) + yOffset);
        glVertex2f((-0.05f * scale) + xOffset, (0.0f * scale) + yOffset);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

    for (int i = 0; i < numHouses; i++) {
        drawHouse(housePositions[i].xOffset, housePositions[i].yOffset, housePositions[i].scale);
    }

    glFlush();
}

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    std::cout << "Введите количество домиков: ";
    std::cin >> numHouses;

    if (numHouses < 1) numHouses = 1;

    // Рассчитываем масштаб и позиции
    float baseScale = 1.0f / (1.0f + (numHouses - 1) * 0.15f); // Масштаб уменьшается плавно
    if (baseScale < 0.05f) baseScale = 0.05f; // Минимальный масштаб

    float houseWidth = 0.4f * baseScale; // Ширина дома с крышей
    float houseHeight = 0.8f * baseScale; // Высота дома с крышей
    float totalLength = 1.8f - houseWidth; // Учитываем ширину последнего дома
    float totalHeight = 1.8f - houseHeight; // Учитываем высоту последнего дома
    float diagonalLength = sqrt(totalLength * totalLength + totalHeight * totalHeight); // Длина диагонали
    float spacing = (diagonalLength - (numHouses * houseWidth)) / (numHouses + 1); // Отступы
    if (spacing < 0) spacing = 0;

    for (int i = 0; i < numHouses; i++) {
        float t = (i + 1) * (houseWidth + spacing); // Позиция вдоль диагонали
        float xOffset = -0.9f + (t / diagonalLength) * totalLength;
        float yOffset = -0.9f + (t / diagonalLength) * totalHeight;
        housePositions.push_back({xOffset, yOffset, baseScale});
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Домиков по диагонали без выхода за границы");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
