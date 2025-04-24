#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <random>

// Вершинный шейдер для звезды (с деформацией)
const char* starVertShader = R"(
#version 330 core
layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
out vec3 FragPos;
out vec3 Normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

// Функции шума
float hash(float n) { return fract(sin(n)*43758.5453); }
float noise(vec3 x) {
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f*f*(3.0-2.0*f);
    float n = p.x + p.y*57.0 + 113.0*p.z;
    return mix(mix(mix(hash(n+0.0), hash(n+1.0),f.x),
               mix(hash(n+57.0), hash(n+58.0),f.x),f.y),
               mix(mix(hash(n+113.0), hash(n+114.0),f.x),
               mix(hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
}
float fbm(vec3 p) {
    float f = 0.0;
    p *= 3.0;
    f += 0.5000*noise(p); p *= 2.01;
    f += 0.2500*noise(p); p *= 2.02;
    f += 0.1250*noise(p); p *= 2.03;
    f += 0.0625*noise(p);
    return f;
}

void main() {
    // Деформация поверхности с эффектом "волос"
    vec3 p = pos;
    float displacement = fbm(p * 5.0 + time * 0.3) * 0.2; // Базовый шум
    float spike = pow(fbm(p * 2.0 + time * 0.1 + 10.0), 3.0) * 0.5; // Протуберанцы
    p += normal * (displacement + spike); // Смещение по нормали

    FragPos = vec3(model * vec4(p, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal;
    gl_Position = projection * view * model * vec4(p, 1.0);
}
)";

// Простой вершинный шейдер для черной дыры и диска (без деформации)
const char* simpleVertShader = R"(
#version 330 core
layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
out vec3 FragPos;
out vec3 Normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    FragPos = vec3(model * vec4(pos, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal;
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
)";

const char* starFragShader = R"(
#version 330 core
in vec3 FragPos;
in vec3 Normal;
out vec4 color;
uniform float time;
uniform vec3 viewPos;

// Функции шума
float hash(float n) { return fract(sin(n)*43758.5453); }
float noise(vec3 x) {
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f*f*(3.0-2.0*f);
    float n = p.x + p.y*57.0 + 113.0*p.z;
    return mix(mix(mix(hash(n+0.0), hash(n+1.0),f.x),
               mix(hash(n+57.0), hash(n+58.0),f.x),f.y),
               mix(mix(hash(n+113.0), hash(n+114.0),f.x),
               mix(hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
}
float fbm(vec3 p) {
    float f = 0.0;
    p *= 3.0;
    f += 0.5000*noise(p); p *= 2.01;
    f += 0.2500*noise(p); p *= 2.02;
    f += 0.1250*noise(p); p *= 2.03;
    f += 0.0625*noise(p);
    return f;
}

// Псевдослучайная точка на сфере
vec3 randomSpherePoint(float seed) {
    float u = hash(seed)*2.0-1.0;
    float theta = hash(seed+1.0)*2.0*3.14159;
    float r = sqrt(1.0 - u*u);
    return vec3(r*cos(theta), r*sin(theta), u);
}

// Эффект вспышки плазмы
float plasmaFlare(vec3 fragPos, vec3 flareCenter, float time, float seed) {
    float lifeTime = mod(time + seed*10.0, 4.0);
    float growthTime = 0.5;
    float decayTime = 2.0;
    float size = 0.0;
    if (lifeTime < growthTime) {
        size = lifeTime / growthTime;
    } else if (lifeTime < growthTime + decayTime) {
        size = 1.0 - (lifeTime - growthTime) / decayTime;
    }
    float dist = length(fragPos - flareCenter * 2.0);
    float intensity = size * exp(-dist*2.0) * 2.0;
    float turbulence = fbm(fragPos*5.0 + time*2.0 + seed) * 0.5;
    float wave = sin(dist*20.0 - time*15.0 + seed*10.0) * 0.2;
    return clamp(intensity * (1.0 + turbulence + wave), 0.0, 1.0);
}

// Эффект коронального выброса
float coronalEjection(vec3 fragPos, vec3 normal, float time, float seed) {
    vec3 dir = normalize(fragPos);
    float angle = dot(normalize(normal), dir);
    float activeRegion = smoothstep(0.7, 0.9, noise(fragPos*3.0 + seed));
    float lifePhase = mod(time*0.5 + seed*5.0, 3.0);
    float lifeIntensity = exp(-lifePhase*2.0) * 2.0;
    float shape = pow(1.0 - abs(angle), 2.0);
    return activeRegion * lifeIntensity * shape * 0.7;
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 baseColor = vec3(0.3, 0.5, 1.0);
    float coreDist = length(FragPos);
    float coreIntensity = 1.0 - smoothstep(0.0, 1.5, coreDist);
    vec3 coreColor = mix(baseColor, vec3(1.0, 0.9, 0.8), coreIntensity * 0.9);
    float granulation = fbm(FragPos * 10.0 + time * 0.3) * 0.4;
    float rim = 1.0 - dot(viewDir, norm);
    rim = pow(rim, 2.0) * 0.8;
    
    // Вспышки плазмы
    vec3 flareColor = vec3(0.0);
    for (int i = 0; i < 6; i++) {
        float seed = float(i) + floor(time / 4.0) * 100.0;
        vec3 flareCenter = randomSpherePoint(seed);
        float flareIntensity = plasmaFlare(FragPos, flareCenter, time, seed);
        vec3 flareCol = mix(vec3(1.0, 0.9, 0.8), vec3(0.5, 0.7, 1.0), flareIntensity*0.7);
        flareColor += flareCol * flareIntensity;
    }
    
    // Корональные выбросы
    float corona = 0.0;
    for (int i = 0; i < 3; i++) {
        float seed = float(i) + floor(time / 3.0) * 50.0;
        corona += coronalEjection(FragPos, norm, time, seed);
    }
    
    // Финальный цвет
    vec3 finalColor = coreColor * (1.0 + rim + granulation) + flareColor + corona*vec3(0.8, 0.9, 1.0);
    float pulse = 0.8 + 0.2 * sin(time * 1.3);
    finalColor = mix(finalColor, finalColor*vec3(1.0, 0.9, 0.7), pulse*0.3);
    color = vec4(finalColor * pulse, 1.0);
}
)";

const char* simpleFragShader = R"(
#version 330 core
out vec4 color;
uniform vec3 objColor;
void main() { color = vec4(objColor,1); }
)";

const char* diskFragShader = R"(
#version 330 core
out vec4 color;
uniform float time;
void main() {
    float i = 0.8 + 0.2*sin(time*5);
    color = vec4(i,i,i,0.8);
}
)";

// Камера
glm::vec3 camPos(0,3,15), camFront(0,-0.2,-1), camUp(0,1,0);
float yaw=-90, pitch=-10;

void processInput(GLFWwindow* window) {
    float speed = 0.1f;
    if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS) camPos += speed*camFront;
    if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS) camPos -= speed*camFront;
    if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS) camPos -= speed*glm::normalize(glm::cross(camFront,camUp));
    if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS) camPos += speed*glm::normalize(glm::cross(camFront,camUp));
    if(glfwGetKey(window,GLFW_KEY_UP)==GLFW_PRESS) { pitch+=1; if(pitch>89)pitch=89; }
    if(glfwGetKey(window,GLFW_KEY_DOWN)==GLFW_PRESS) { pitch-=1; if(pitch<-89)pitch=-89; }
    if(glfwGetKey(window,GLFW_KEY_LEFT)==GLFW_PRESS) yaw-=1;
    if(glfwGetKey(window,GLFW_KEY_RIGHT)==GLFW_PRESS) yaw+=1;
    camFront = glm::normalize(glm::vec3(
        cos(glm::radians(yaw))*cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw))*cos(glm::radians(pitch))));
}

std::vector<float> makeSphere(float r, int seg) {
    std::vector<float> v;
    for(int i=0;i<=seg;i++) {
        float lat = M_PI*(-0.5f + float(i)/seg);
        for(int j=0;j<=seg;j++) {
            float lng = 2*M_PI*float(j)/seg;
            float x = r*cos(lng)*cos(lat);
            float y = r*sin(lat);
            float z = r*sin(lng)*cos(lat);
            v.push_back(x); v.push_back(y); v.push_back(z);
            v.push_back(x/r); v.push_back(y/r); v.push_back(z/r); // Нормали
        }
    }
    return v;
}

std::vector<unsigned> makeSphereIndices(int seg) {
    std::vector<unsigned> idx;
    for(int i=0;i<seg;i++) {
        for(int j=0;j<seg;j++) {
            int k = i*(seg+1)+j;
            idx.push_back(k); idx.push_back(k+seg+1); idx.push_back(k+1);
            idx.push_back(k+1); idx.push_back(k+seg+1); idx.push_back(k+seg+2);
        }
    }
    return idx;
}

std::vector<float> makeDisk(float r1, float r2, int seg) {
    std::vector<float> v;
    for(int i=0;i<=seg;i++) {
        float a = 2*M_PI*i/seg;
        v.push_back(r1*cos(a)); v.push_back(0); v.push_back(r1*sin(a));
        v.push_back(r2*cos(a)); v.push_back(0); v.push_back(r2*sin(a));
    }
    return v;
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Star with Plasma Flares", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    // Шейдеры для звезды
    unsigned starProg = glCreateProgram();
    unsigned starVsh = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(starVsh,1,&starVertShader,NULL);
    glCompileShader(starVsh);
    glAttachShader(starProg,starVsh);
    unsigned starFsh = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(starFsh,1,&starFragShader,NULL);
    glCompileShader(starFsh);
    glAttachShader(starProg,starFsh);
    glLinkProgram(starProg);

    // Шейдеры для черной дыры
    unsigned simpleProg = glCreateProgram();
    unsigned simpleVsh = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(simpleVsh,1,&simpleVertShader,NULL);
    glCompileShader(simpleVsh);
    glAttachShader(simpleProg,simpleVsh);
    unsigned simpleFsh = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(simpleFsh,1,&simpleFragShader,NULL);
    glCompileShader(simpleFsh);
    glAttachShader(simpleProg,simpleFsh);
    glLinkProgram(simpleProg);

    // Шейдеры для диска
    unsigned diskProg = glCreateProgram();
    unsigned diskVsh = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(diskVsh,1,&simpleVertShader,NULL);
    glCompileShader(diskVsh);
    glAttachShader(diskProg,diskVsh);
    unsigned diskFsh = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(diskFsh,1,&diskFragShader,NULL);
    glCompileShader(diskFsh);
    glAttachShader(diskProg,diskFsh);
    glLinkProgram(diskProg);

    // Объекты
    auto sph = makeSphere(2,48);
    auto sphIdx = makeSphereIndices(48);
    auto disk = makeDisk(0.7,2.5,64);

    unsigned VAOs[3], VBOs[3], EBO;
    glGenVertexArrays(3,VAOs);
    glGenBuffers(3,VBOs);
    glGenBuffers(1,&EBO);

    // Звезда
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER,sph.size()*sizeof(float),sph.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sphIdx.size()*sizeof(unsigned),sphIdx.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Черная дыра
    auto bhSph = makeSphere(0.5,24);
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER,bhSph.size()*sizeof(float),bhSph.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Диск
    glBindVertexArray(VAOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER,disk.size()*sizeof(float),disk.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glm::mat4 proj = glm::perspective(glm::radians(45.0f),1920.0f/1080.0f,0.1f,100.0f);
        glm::mat4 view = glm::lookAt(camPos,camPos+camFront,camUp);
        float time = glfwGetTime();
        float angle = time*0.2f;
        glm::mat4 rot = glm::rotate(glm::mat4(1),angle,glm::vec3(0,1,0));

        // Звезда
        glm::mat4 starModel = rot * glm::translate(glm::mat4(1),glm::vec3(-8,0,0));
        glUseProgram(starProg);
        glUniformMatrix4fv(glGetUniformLocation(starProg,"model"),1,GL_FALSE,&starModel[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(starProg,"view"),1,GL_FALSE,&view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(starProg,"projection"),1,GL_FALSE,&proj[0][0]);
        glUniform1f(glGetUniformLocation(starProg,"time"),time);
        glUniform3fv(glGetUniformLocation(starProg,"viewPos"),1,&camPos[0]);
        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_TRIANGLES,sphIdx.size(),GL_UNSIGNED_INT,0);

        // Черная дыра
        glm::mat4 bhModel = rot * glm::translate(glm::mat4(1),glm::vec3(8,0,0));
        glUseProgram(simpleProg);
        glUniformMatrix4fv(glGetUniformLocation(simpleProg,"model"),1,GL_FALSE,&bhModel[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(simpleProg,"view"),1,GL_FALSE,&view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(simpleProg,"projection"),1,GL_FALSE,&proj[0][0]);
        glUniform3f(glGetUniformLocation(simpleProg,"objColor"),0,0,0); // Исправлено: 0,0,merzen -> 0,0,0
        glBindVertexArray(VAOs[1]);
        glDrawElements(GL_TRIANGLES,sphIdx.size(),GL_UNSIGNED_INT,0);

        // Диск
        glm::mat4 diskModel = bhModel * glm::rotate(glm::mat4(1),glm::radians(45.0f),glm::vec3(1,0,0));
        glUseProgram(diskProg);
        glUniformMatrix4fv(glGetUniformLocation(diskProg,"model"),1,GL_FALSE,&diskModel[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(diskProg,"view"),1,GL_FALSE,&view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(diskProg,"projection"),1,GL_FALSE,&proj[0][0]);
        glUniform1f(glGetUniformLocation(diskProg,"time"),time);
        glBindVertexArray(VAOs[2]);
        glDrawArrays(GL_TRIANGLE_STRIP,0,disk.size()/3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(3,VAOs);
    glDeleteBuffers(3,VBOs);
    glDeleteBuffers(1,&EBO);
    glDeleteProgram(starProg);
    glDeleteProgram(simpleProg);
    glDeleteProgram(diskProg);
    glfwTerminate();
    return 0;
}