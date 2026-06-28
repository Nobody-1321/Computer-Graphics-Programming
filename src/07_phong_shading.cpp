#define GLFW_INCLUDE_NONE
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>
//
// NOTA: Asegúrate de que este include apunte correctamente a tu clase de utilidades
#include <Utils.hpp>
#include <Sphere.hpp> 

using namespace std;

// ==========================================
// CONFIGURACIÓN GLOBAL DE CONFIGURACIONES Y OPENGL
// ==========================================
#define numVAOs 1
#define numVBOs 3 // 3 buffers: Posiciones, Normales y Elementos (EBO)

GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// Inicializamos la Esfera global con un radio de 1.0f, 48 sectores y 24 stacks
Sphere miEsfera(1.0f, 48, 24);

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::mat4 normalMatrix;

float aspectRatio;

glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp;

float deltaTime = 0.0f; 
float lastFrame = 0.0f; 

float Zoom = 45.0f;
float yaw; 
float pitch;   
float lastX, lastY; 
bool firstMouse;
float sensitivity; 

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void setUpVertices();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void calculateDeltaTime() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

// Posiciones para distribuir las 5 esferas en el espacio
glm::vec3 spherePositions[] = {
    glm::vec3(0.0f,  0.0f, 0.0f),
    glm::vec3(2.0f,  0.0f, 0.0f), 
    glm::vec3(4.0f,  0.0f, 0.0f),
    glm::vec3(-2.0f, 0.0f, 0.0f),
    glm::vec3(3.0f,  2.0f, 3.0f), // Esta actuará como la fuente de luz visible
};

glm::vec4 colorValue[] ={
    glm::vec4(0.0f, 0.4f, 0.9f, 1.0f),
    glm::vec4(0.0f, 0.4f, 0.9f, 1.0f),
    glm::vec4(0.0f, 0.4f, 0.9f, 1.0f),
    glm::vec4(0.0f, 0.4f, 0.9f, 1.0f),
    glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), // Color blanco para la esfera que emite luz
};

GLuint renderingPrograms[5];

void init() {

    renderingPrograms[4] = Utils::createShaderProgram("./shaders/vertex_shader7.glsl", "./shaders/fragment_shader7.glsl");
    renderingPrograms[3] = Utils::createShaderProgram("./shaders/vertex_shader7.glsl", "./shaders/fragment_shader7.glsl");
    renderingPrograms[2] = Utils::createShaderProgram("./shaders/vertex_shader7_SL.glsl", "./shaders/fragment_shader7_SL.glsl");
    renderingPrograms[1] = Utils::createShaderProgram("./shaders/vertex_shader7_DL.glsl", "./shaders/fragment_shader7_DL.glsl");
    renderingPrograms[0] = Utils::createShaderProgram("./shaders/vertex_shader7_AL.glsl", "./shaders/fragment_shader7_AL.glsl");

    cameraPos   = glm::vec3(0.0f, 0.0f, 6.0f); // Alejar un poco para ver la escena completa
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); 
    cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

    yaw = -90.0f;             
    pitch = 0.0f;             
    firstMouse = true;
    sensitivity = 0.1f; 

    setUpVertices();
}

void display(GLFWwindow *window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    for (int i = 0; i < 5; i++)
    {
        glUseProgram(renderingPrograms[i]);

        model = glm::mat4(1.0f);
        model = glm::translate(model, spherePositions[i]);

        float angle = glm::radians(20.0f * (float)glfwGetTime()); 
        model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // Ajustar escala de la esfera
        normalMatrix = glm::transpose(glm::inverse(model));

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(Zoom), aspectRatio, 0.1f, 100.0f);

        unsigned int modelLoc       = glGetUniformLocation(renderingPrograms[i], "model");
        unsigned int normalMatrixLoc = glGetUniformLocation(renderingPrograms[i], "normalMatrix");
        unsigned int alightColor    = glGetUniformLocation(renderingPrograms[i], "lightColor");
        unsigned int objectColorVec = glGetUniformLocation(renderingPrograms[i], "objectColor");
        unsigned int viewLoc        = glGetUniformLocation(renderingPrograms[i], "view");
        unsigned int projectionLoc  = glGetUniformLocation(renderingPrograms[i], "projection");
        unsigned int lightPosLoc    = glGetUniformLocation(renderingPrograms[i], "lightPos");
        unsigned int viewPosLoc     = glGetUniformLocation(renderingPrograms[i], "viewPos");
        
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(cameraPos));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        glUniform4fv(alightColor, 1, glm::value_ptr(colorValue[4]));
        glUniform4fv(objectColorVec, 1, glm::value_ptr(colorValue[i]));
        
        // La posición de la luz física será la de la 5ta esfera (índice 4)
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(spherePositions[4]));

        glBindVertexArray(vao[0]); 
        
        // Dibujado usando Elementos Indexados (Esencial para la clase Sphere)
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(miEsfera.getNumIndices()), GL_UNSIGNED_INT, 0);
    }
}

int main()
{
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, "Muestra de Iluminacion con Esferas", nullptr, nullptr);
    
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMaximizeWindow(window);
    aspectRatio = (float)mode->width / (float)mode->height;
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    glfwSetCursorPosCallback(window, mouse_callback); 
    glfwSetScrollCallback(window, scroll_callback); 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    init();
    calculateDeltaTime(); // Primera inicialización del tiempo antes del ciclo

    while (!glfwWindowShouldClose(window))
    {
        calculateDeltaTime();
        display(window);
        processInput(window);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glDeleteVertexArrays(1, vao);
    glDeleteBuffers(numVBOs, vbo);
    glfwTerminate();

    return 0;
}

void setUpVertices(void)
{
    // Recuperamos referencias constantes de los datos geométricos generados
    const std::vector<glm::vec3>& vertices = miEsfera.getVertices();
    const std::vector<glm::vec3>& normales = miEsfera.getNormals();
    const std::vector<unsigned int>& indices = miEsfera.getIndices();

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(numVBOs, vbo); 

    // Buffer 1: Posiciones de los Vértices
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    // Buffer 2: Vectores Normales (Crucial para que tus Shaders de Iluminación calculen los reflejos)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, normales.size() * sizeof(glm::vec3), normales.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(1);

    // Buffer 3: EBO (Element Buffer Object) que indexa los triángulos de la esfera
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(0); // Desenlazamos el VAO de manera segura
}

void processInput(GLFWwindow *window)
{
    float cameraSpeed = 2.5f * deltaTime; 

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    aspectRatio = (float)width / (float)height;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f) Zoom = 1.0f;
    if (Zoom > 45.0f) Zoom = 45.0f;
}