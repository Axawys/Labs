#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <ctime>

// Параметры конуса
float funnel_radius = 1.0f;
float funnel_height = 1.5f;
float hole_radius = 0.1f;

// Параметры цилиндра
const float cylinder_radius = 0.3f;
const float cylinder_height = 0.2f;
const float cylinder_base_y = -funnel_height + hole_radius * funnel_height / funnel_radius - cylinder_height;

// Физические параметры
const float gravity = -9.8f;
const float restitution = 0.5f;
const float friction = 0.8f;

struct Coin {
    glm::vec3 position;
    glm::vec3 velocity;
    float angle;
    float angularSpeed;
    bool active;
    float alpha;
    float fadeTime;
    bool stopped;
    float tilt_x;
    float tilt_z;
};

std::vector<Coin> coins;
float accumulated_height = cylinder_base_y; // Глобальная переменная
bool keyPressed = false;
bool oKeyPressed = false;
bool disintegrate = false;
bool disintegrationTriggered = false;
float disintegrateTime = 0.0f;
const float disintegrateDuration = 1.0f;
const float coin_fade_duration = 0.5f;
int spacePressCount = 0;

// Переменные камеры
float cameraAngleX = 0.0f;
float cameraAngleY = 30.0f;
float cameraDistance = 3.0f;
float cameraOffsetY = 0.0f;
double lastMouseX = 0.0;
double lastMouseY = 0.0;
bool mousePressed = false;

bool initOpenGL() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW init failed" << std::endl;
        return false;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}

void setupLighting() {
    GLfloat lightPosition[] = { 2.0f, 2.0f, 2.0f, 1.0f };
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat materialShininess[] = { 50.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, materialShininess);
}

void drawFunnel(float deltaTime) {
    const int segments = 128;
    const int quadrant_segments = segments / 4;
    float hole_height = -funnel_height + hole_radius * funnel_height / funnel_radius;

    if (disintegrate) {
        disintegrateTime += deltaTime;
        if (disintegrateTime >= disintegrateDuration) {
            return;
        }
    }

    float progress = disintegrate ? disintegrateTime / disintegrateDuration : 0.0f;
    float offset = progress * 2.0f;
    float alpha = 1.0f - progress;

    for (int q = 0; q < 4; ++q) {
        float start_angle = q * M_PI / 2.0f;
        float end_angle = (q + 1) * M_PI / 2.0f;
        float center_angle = (start_angle + end_angle) / 2.0f;
        float offset_x = offset * cos(center_angle);
        float offset_z = offset * sin(center_angle);

        glPushMatrix();
        glTranslatef(offset_x, 0.0f, offset_z);

        glCullFace(GL_BACK);
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i <= quadrant_segments; ++i) {
            float t = (float)i / quadrant_segments;
            float theta = start_angle + t * (end_angle - start_angle);
            float x_top = funnel_radius * cos(theta);
            float z_top = funnel_radius * sin(theta);
            float x_bottom = hole_radius * cos(theta);
            float z_bottom = hole_radius * sin(theta);
            glNormal3f(-cos(theta), 0.5f, -sin(theta));
            glColor4f(0.5f, 0.5f, 0.6f, alpha);
            glVertex3f(x_top, 0.0f, z_top);
            glVertex3f(x_bottom, hole_height, z_bottom);
        }
        glEnd();

        glCullFace(GL_FRONT);
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i <= quadrant_segments; ++i) {
            float t = (float)i / quadrant_segments;
            float theta = start_angle + t * (end_angle - start_angle);
            float x_top = funnel_radius * cos(theta);
            float z_top = funnel_radius * sin(theta);
            float x_bottom = hole_radius * cos(theta);
            float z_bottom = hole_radius * sin(theta);
            glNormal3f(cos(theta), -0.5f, sin(theta));
            glColor4f(0.4f, 0.4f, 0.5f, alpha);
            glVertex3f(x_top, 0.0f, z_top);
            glVertex3f(x_bottom, hole_height, z_bottom);
        }
        glEnd();

        glPopMatrix();
    }
}

void drawCylinder() {
    const int segments = 128;
    float top_y = cylinder_base_y + cylinder_height;

    // Внешняя поверхность цилиндра
    glCullFace(GL_BACK);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float theta = (float)i / segments * 2.0f * M_PI;
        float x = cylinder_radius * cos(theta);
        float z = cylinder_radius * sin(theta);
        glNormal3f(cos(theta), 0.0f, sin(theta));
        glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        glVertex3f(x, top_y, z);
        glVertex3f(x, cylinder_base_y, z);
    }
    glEnd();

    // Внутренняя поверхность цилиндра
    glCullFace(GL_FRONT);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float theta = (float)i / segments * 2.0f * M_PI;
        float x = cylinder_radius * cos(theta);
        float z = cylinder_radius * sin(theta);
        glNormal3f(-cos(theta), 0.0f, -sin(theta));
        glColor4f(0.4f, 0.4f, 0.4f, 1.0f);
        glVertex3f(x, top_y, z);
        glVertex3f(x, cylinder_base_y, z);
    }
    glEnd();

    // Нижняя грань (видимая сверху)
    glCullFace(GL_BACK);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
    glVertex3f(0.0f, cylinder_base_y, 0.0f);
    for (int i = segments; i >= 0; --i) {
        float theta = (float)i / segments * 2.0f * M_PI;
        float x = cylinder_radius * cos(theta);
        float z = cylinder_radius * sin(theta);
        glVertex3f(x, cylinder_base_y, z);
    }
    glEnd();

    // Нижняя грань (видимая снизу)
    glCullFace(GL_FRONT);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glColor4f(0.6f, 0.6f, 0.6f, 1.0f);
    glVertex3f(0.0f, cylinder_base_y, 0.0f);
    for (int i = 0; i <= segments; ++i) {
        float theta = (float)i / segments * 2.0f * M_PI;
        float x = cylinder_radius * cos(theta);
        float z = cylinder_radius * sin(theta);
        glVertex3f(x, cylinder_base_y, z);
    }
    glEnd();
}

void drawCoinShadow(const Coin& coin, float accumulated_height) {
    const int segments = 32;
    float radius = 0.06f;
    float shadow_y = coin.stopped ? coin.position.y : accumulated_height;

    glPushMatrix();
    glTranslatef(coin.position.x, shadow_y + 0.001f, coin.position.z);
    glScalef(1.0f + 0.2f * fabs(coin.tilt_x), 1.0f, 1.0f + 0.2f * fabs(coin.tilt_z));

    glDisable(GL_LIGHTING);
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(0.0f, 0.0f, 0.0f, 0.3f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i <= segments; ++i) {
        float theta = (float)i / segments * 2.0f * M_PI;
        float x = radius * cos(theta);
        float z = radius * sin(theta);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void drawCoin(const Coin& coin) {
    const int segments = 32;
    float radius = 0.06f;
    glPushMatrix();
    glTranslatef(coin.position.x, coin.position.y, coin.position.z);
    if (!coin.stopped) {
        glRotatef(coin.angle * 180.0f / M_PI, 0.0f, 1.0f, 0.0f);
    } else {
        glRotatef(coin.tilt_x * 180.0f / M_PI, 1.0f, 0.0f, 0.0f);
        glRotatef(coin.tilt_z * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
    }

    GLfloat emission[] = { 0.5f * coin.alpha, 0.4f * coin.alpha, 0.0f, coin.alpha };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glColor4f(1.0f, 0.85f, 0.1f, coin.alpha);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i <= segments; ++i) {
        float theta = (float)i / segments * 2.0f * M_PI;
        float x = radius * cos(theta);
        float z = radius * sin(theta);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = segments; i >= 0; --i) {
        float theta = (float)i / segments * 2.0f * M_PI;
        float x = radius * cos(theta);
        float z = radius * sin(theta);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float theta = (float)i / segments * 2.0f * M_PI;
        float x = radius * cos(theta);
        float z = radius * sin(theta);
        glNormal3f(x/radius, 0.0f, z/radius);
        glVertex3f(x, 0.0f, z);
        glVertex3f(x, 0.01f, z);
    }
    glEnd();

    GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmission);
    
    glPopMatrix();
}

void updateCoins(float deltaTime) {
    float hole_height = -funnel_height + hole_radius * funnel_height / funnel_radius;
    const float coin_radius = 0.06f;
    const float coin_thickness = 0.01f;

    for (auto& coin : coins) {
        if (!coin.active || coin.stopped) continue;

        if (coin.position.y > hole_height) {
            float r_y = funnel_radius + (hole_radius - funnel_radius) * (coin.position.y / hole_height);
            float r = r_y - coin_radius - 0.01f;
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

            float r = sqrt(coin.position.x * coin.position.x + coin.position.z * coin.position.z);
            if (r > cylinder_radius - coin_radius) {
                glm::vec3 normal(coin.position.x / r, 0.0f, coin.position.z / r);
                float dot = glm::dot(coin.velocity, normal);
                coin.velocity -= 2.0f * dot * normal * restitution; // Исправлено
                coin.velocity *= friction;
                float scale = (cylinder_radius - coin_radius - 0.01f) / r;
                coin.position.x *= scale;
                coin.position.z *= scale;
            }

            for (auto& other : coins) {
                if (&other != &coin && other.stopped) {
                    float distance = glm::length(coin.position - other.position);
                    if (distance < 2.0f * coin_radius && distance > 0.0f) {
                        glm::vec3 direction = (coin.position - other.position) / distance;
                        float dot = glm::dot(coin.velocity, direction);
                        coin.velocity -= 2.0f * dot * direction * restitution;
                        coin.velocity *= friction;
                        float overlap = 2.0f * coin_radius - distance;
                        coin.position += direction * overlap * 0.5f;
                    }
                }
            }

            if (coin.position.y <= accumulated_height + coin_thickness) {
                coin.stopped = true;
                coin.position.y = accumulated_height;
                coin.velocity = glm::vec3(0.0f);
                coin.alpha = 1.0f;

                coin.tilt_x = (static_cast<float>(std::rand()) / RAND_MAX - 0.5f) * 0.3f;
                coin.tilt_z = (static_cast<float>(std::rand()) / RAND_MAX - 0.5f) * 0.3f;

                bool collision;
                const float min_distance = 2.0f * coin_radius;
                int max_attempts = 10;
                int attempt = 0;
                do {
                    collision = false;
                    for (const auto& other : coins) {
                        if (&other != &coin && other.stopped) {
                            float distance = glm::length(coin.position - other.position);
                            if (distance < min_distance && distance > 0.0f) {
                                collision = true;
                                float offset_r = (cylinder_radius - coin_radius - 0.01f) * (static_cast<float>(std::rand()) / RAND_MAX);
                                float offset_angle = static_cast<float>(std::rand()) / RAND_MAX * 2.0f * M_PI;
                                coin.position = glm::vec3(offset_r * cos(offset_angle), accumulated_height, offset_r * sin(offset_angle));
                                break;
                            }
                        }
                    }
                    attempt++;
                    if (attempt >= max_attempts) {
                        coin.position = glm::vec3(0.0f, accumulated_height, 0.0f);
                        break;
                    }
                } while (collision);

                accumulated_height += coin_thickness;
                if (accumulated_height >= cylinder_base_y + cylinder_height - coin_radius && !disintegrationTriggered) {
                    disintegrate = true;
                    disintegrationTriggered = true;
                    disintegrateTime = 0.0f;
                    accumulated_height = cylinder_base_y + cylinder_height - coin_radius;
                }
            }
        }
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        mousePressed = (action == GLFW_PRESS);
        if (mousePressed) {
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        }
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (mousePressed) {
        double deltaX = xpos - lastMouseX;
        double deltaY = ypos - lastMouseY;
        cameraAngleX += deltaX * 0.2f;
        cameraAngleY += deltaY * 0.2f;
        cameraAngleY = glm::clamp(cameraAngleY, -89.0f, 89.0f);
        lastMouseX = xpos;
        lastMouseY = ypos;
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW init failed" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Coin Funnel", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
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

        int spaceState = glfwGetKey(window, GLFW_KEY_SPACE);
        if (spaceState == GLFW_PRESS && !keyPressed && !disintegrate) {
            Coin newCoin;
            newCoin.position = glm::vec3(0.0f, 0.0f, 0.0f);
            newCoin.velocity = glm::vec3(0.0f, -0.5f, 0.0f);
            newCoin.angle = static_cast<float>(std::rand()) / RAND_MAX * 2.0f * M_PI;
            newCoin.angularSpeed = 4.0f + static_cast<float>(std::rand()) / RAND_MAX * 2.0f;
            newCoin.active = true;
            newCoin.alpha = 1.0f;
            newCoin.fadeTime = 0.0f;
            newCoin.stopped = false;
            newCoin.tilt_x = 0.0f;
            newCoin.tilt_z = 0.0f;
            coins.push_back(newCoin);
            keyPressed = true;
            spacePressCount++;
            std::cout << "SPACE pressed " << spacePressCount << " time(s)\n";
        }
        if (spaceState == GLFW_RELEASE) {
            keyPressed = false;
        }

        int oState = glfwGetKey(window, GLFW_KEY_O);
        if (oState == GLFW_PRESS && !oKeyPressed && !disintegrationTriggered) {
            disintegrate = true;
            disintegrationTriggered = true;
            disintegrateTime = 0.0f;
            oKeyPressed = true;
        }
        if (oState == GLFW_RELEASE) {
            oKeyPressed = false;
        }

        float rotationSpeed = 100.0f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            cameraAngleX -= rotationSpeed * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            cameraAngleX += rotationSpeed * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            cameraAngleY += rotationSpeed * deltaTime;
            if (cameraAngleY > 89.0f) cameraAngleY = 89.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            cameraAngleY -= rotationSpeed * deltaTime;
            if (cameraAngleY < -89.0f) cameraAngleY = -89.0f;
        }

        float moveSpeed = 2.0f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cameraOffsetY += moveSpeed * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cameraOffsetY -= moveSpeed * deltaTime;
        }
        const float minOffsetY = -5.0f;
        const float maxOffsetY = 5.0f;
        if (cameraOffsetY < minOffsetY) cameraOffsetY = minOffsetY;
        if (cameraOffsetY > maxOffsetY) cameraOffsetY = maxOffsetY;

        updateCoins(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        float camX = cameraDistance * cos(glm::radians(cameraAngleY)) * sin(glm::radians(cameraAngleX));
        float camY = cameraDistance * sin(glm::radians(cameraAngleY)) + cameraOffsetY;
        float camZ = cameraDistance * cos(glm::radians(cameraAngleY)) * cos(glm::radians(cameraAngleX));
        gluLookAt(camX, camY, camZ, 0.0f, cameraOffsetY, 0.0f, 0.0f, 1.0f, 0.0f);

        setupLighting();
        
        glCullFace(GL_BACK);
        for (const auto& coin : coins) {
            if (coin.active) {
                drawCoinShadow(coin, accumulated_height);
            }
        }
        for (const auto& coin : coins) {
            if (coin.active) {
                drawCoin(coin);
            }
        }
        
        drawFunnel(deltaTime);
        drawCylinder();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}