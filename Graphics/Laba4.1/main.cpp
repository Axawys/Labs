#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <ctime>

// Parameters
float funnel_radius = 1.0f;
float funnel_height = 1.5f;
float hole_radius = 0.1f;
float gravity = -9.8f;
float restitution = 0.5f;
float piston_y = -funnel_height + hole_radius * funnel_height / funnel_radius;
float piston_velocity = 10.0f; // Upward velocity when piston activates
bool piston_active = false;

// Coin structure
struct Coin {
    glm::vec3 position;
    glm::vec3 velocity;
    float angle;
    float angularSpeed;
    bool active;
};

// Global variables
std::vector<Coin> coins;
float cameraAngleX = 0.0f;
float cameraAngleY = 30.0f;
float cameraDistance = 3.0f;
double lastMouseX = 0.0, lastMouseY = 0.0;
bool mousePressed = false;
bool spacePressed = false;

bool initOpenGL() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return false;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    return true;
}

void setupLighting() {
    GLfloat lightPos[] = { 2.0f, 2.0f, 2.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

void drawFunnel() {
    const int segments = 64;
    float hole_height = -funnel_height + hole_radius * funnel_height / funnel_radius;

    // Outer surface (visible from outside)
    glCullFace(GL_BACK);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float theta = (float)i / segments * 2.0f * M_PI;
        float x_top = funnel_radius * cos(theta);
        float z_top = funnel_radius * sin(theta);
        float x_bottom = hole_radius * cos(theta);
        float z_bottom = hole_radius * sin(theta);
        glNormal3f(-cos(theta), 0.5f, -sin(theta));
        glColor3f(0.5f, 0.5f, 0.6f);
        glVertex3f(x_top, 0.0f, z_top);
        glVertex3f(x_bottom, hole_height, z_bottom);
    }
    glEnd();

    // Inner surface (visible from inside)
    glCullFace(GL_FRONT);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float theta = (float)i / segments * 2.0f * M_PI;
        float x_top = funnel_radius * cos(theta);
        float z_top = funnel_radius * sin(theta);
        float x_bottom = hole_radius * cos(theta);
        float z_bottom = hole_radius * sin(theta);
        glNormal3f(cos(theta), -0.5f, sin(theta));
        glColor3f(0.4f, 0.4f, 0.5f);
        glVertex3f(x_top, 0.0f, z_top);
        glVertex3f(x_bottom, hole_height, z_bottom);
    }
    glEnd();
}

void drawPiston() {
    const int segments = 64;
    float radius = hole_radius * 1.1f; // Slightly larger for visibility

    glPushMatrix();
    glTranslatef(0.0f, piston_y + 0.01f, 0.0f); // Slight offset to avoid z-fighting
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.7f, 0.7f, 0.7f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i <= segments; ++i) {
        float theta = (float)i / segments * 2.0f * M_PI;
        glVertex3f(radius * cos(theta), 0.0f, radius * sin(theta));
    }
    glEnd();
    glPopMatrix();
}

void drawCoin(const Coin& coin) {
    const int segments = 32;
    float radius = 0.06f;

    glPushMatrix();
    glTranslatef(coin.position.x, coin.position.y, coin.position.z);
    glRotatef(coin.angle * 180.0f / M_PI, 0.0f, 1.0f, 0.0f);

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 0.85f, 0.1f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i <= segments; ++i) {
        float theta = (float)i / segments * 2.0f * M_PI;
        glVertex3f(radius * cos(theta), 0.0f, radius * sin(theta));
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = segments; i >= 0; --i) {
        float theta = (float)i / segments * 2.0f * M_PI;
        glVertex3f(radius * cos(theta), 0.0f, radius * sin(theta));
    }
    glEnd();

    glPopMatrix();
}

void updateCoins(float deltaTime) {
    float hole_height = -funnel_height + hole_radius * funnel_height / funnel_radius;
    float coin_radius = 0.06f;

    for (auto& coin : coins) {
        if (!coin.active) continue;

        if (coin.position.y > hole_height) {
            float r_y = funnel_radius + (hole_radius - funnel_radius) * (coin.position.y / hole_height);
            float r = r_y - coin_radius * 1.5f; // Increased offset to keep coins inside
            if (r < 0.0f) r = 0.0f;

            coin.angle += coin.angularSpeed * deltaTime;
            coin.position.x = r * cos(coin.angle);
            coin.position.z = r * sin(coin.angle);
            coin.velocity.y += gravity * deltaTime;
            coin.position.y += coin.velocity.y * deltaTime;
        } else {
            coin.velocity.y += gravity * deltaTime;
            coin.position += coin.velocity * deltaTime;
            coin.angle += coin.angularSpeed * deltaTime;

            // Piston interaction
            if (piston_active && coin.position.y <= piston_y + coin_radius) {
                coin.velocity.y = piston_velocity * restitution;
            }

            // Boundary check (prevent coins from falling too far)
            if (coin.position.y < piston_y - 2.0f) {
                coin.active = false;
            }
        }
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        mousePressed = (action == GLFW_PRESS);
        if (mousePressed) glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (mousePressed) {
        cameraAngleX += (xpos - lastMouseX) * 0.2f;
        cameraAngleY += (ypos - lastMouseY) * 0.2f;
        cameraAngleY = glm::clamp(cameraAngleY, -89.0f, 89.0f);
        lastMouseX = xpos;
        lastMouseY = ypos;
    }
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "Coin Funnel", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!initOpenGL()) {
        glfwTerminate();
        return -1;
    }

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, mouseCallback);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    std::srand((unsigned int)std::time(nullptr));
    float lastTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Handle Space key for coin spawn and piston activation
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spacePressed) {
            Coin newCoin;
            newCoin.position = glm::vec3(0.0f, 0.0f, 0.0f);
            newCoin.velocity = glm::vec3(0.0f, -0.5f, 0.0f);
            newCoin.angle = static_cast<float>(std::rand()) / RAND_MAX * 2.0f * M_PI;
            newCoin.angularSpeed = 4.0f + static_cast<float>(std::rand()) / RAND_MAX * 2.0f;
            newCoin.active = true;
            coins.push_back(newCoin);
            piston_active = true;
            spacePressed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
            spacePressed = false;
            piston_active = false;
        }

        // Camera controls
        float rotationSpeed = 100.0f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) cameraAngleX -= rotationSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) cameraAngleX += rotationSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) cameraAngleY = glm::clamp(cameraAngleY + rotationSpeed * deltaTime, -89.0f, 89.0f);
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) cameraAngleY = glm::clamp(cameraAngleY - rotationSpeed * deltaTime, -89.0f, 89.0f);

        updateCoins(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        float camX = cameraDistance * cos(glm::radians(cameraAngleY)) * sin(glm::radians(cameraAngleX));
        float camY = cameraDistance * sin(glm::radians(cameraAngleY));
        float camZ = cameraDistance * cos(glm::radians(cameraAngleY)) * cos(glm::radians(cameraAngleX));
        gluLookAt(camX, camY, camZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

        setupLighting();
        drawFunnel();
        drawPiston();
        for (const auto& coin : coins) {
            if (coin.active) drawCoin(coin);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}