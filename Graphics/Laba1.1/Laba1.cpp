#include <GL/glut.h>
#include <math.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // фон
    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
    
    // основа дома
    glColor3f(1.0f, 0.8f, 0.6f);
    glBegin(GL_QUADS);
        glVertex2f(-0.3f, -0.3f);
        glVertex2f(0.3f, -0.3f);
        glVertex2f(0.3f, 0.2f);
        glVertex2f(-0.3f, 0.2f);
    glEnd();

    // крыша
    glColor3f(0.7f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.4f, 0.2f);
        glVertex2f(0.4f, 0.2f);
        glVertex2f(0.0f, 0.5f);
    glEnd();

    // дверь
    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.1f, -0.3f);
        glVertex2f(0.1f, -0.3f);
        glVertex2f(0.1f, 0.0f);
        glVertex2f(-0.1f, 0.0f);
    glEnd();

    // труба
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(0.15f, 0.3f);
        glVertex2f(0.25f, 0.3f);
        glVertex2f(0.25f, 0.45f);
        glVertex2f(0.15f, 0.45f);
    glEnd();

    // овалы
    glColor3f(1.0f, 1.0f, 1.0f); 
    int segments = 20; //кол-во сегментов

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.20f, 0.50f); // центр первого овала
    for(int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.14159f * i / segments;
        glVertex2f(0.20f + 0.04f * cos(angle), 
                  0.50f + 0.02f * sin(angle));
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.25f, 0.55f); // центр второго овала
    for(int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.14159f * i / segments;
        glVertex2f(0.25f + 0.03f * cos(angle), 
                  0.55f + 0.015f * sin(angle));
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.30f, 0.60f); // центр третьего овала
    for(int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.14159f * i / segments;
        glVertex2f(0.30f + 0.035f * cos(angle), 
                  0.60f + 0.02f * sin(angle));
    }
    glEnd();

    // облако из четырёх овалов
    glColor3f(1.0f, 1.0f, 1.0f);

    // Первый овал облака
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.15f, 0.75f); // центр первого овала
    for(int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.14159f * i / segments;
        glVertex2f(0.15f + 0.1f * cos(angle),  // ширина овала
                  0.75f + 0.05f * sin(angle));  // высота овала
    }
    glEnd();

    // второй овал облака
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.25f, 0.80f); // центр второго овала
    for(int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.14159f * i / segments;
        glVertex2f(0.25f + 0.12f * cos(angle), 
                  0.80f + 0.06f * sin(angle));
    }
    glEnd();

    // третий овал облака
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.35f, 0.75f); // центр третьего овала
    for(int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.14159f * i / segments;
        glVertex2f(0.35f + 0.1f * cos(angle), 
                  0.75f + 0.05f * sin(angle));
    }
    glEnd();

    // четвёртый овал облака
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.25f, 0.70f); // центр четвёртого овала
    for(int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.14159f * i / segments;
        glVertex2f(0.25f + 0.11f * cos(angle), 
                  0.70f + 0.055f * sin(angle));
    }
    glEnd();

    glFlush();
}

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Простой домик с трубой и облаками");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
