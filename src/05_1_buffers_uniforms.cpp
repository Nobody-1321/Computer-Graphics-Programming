#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <glm/glm.hpp>
#include <Utils.hpp>

using namespace std;

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint vLoc, projLoc;
int width, height;
float aspect, timeFactor;
GLuint tfLoc;
glm::mat4 pMat, vMat, mMat, mvMat;
bool colorsActive = true;
double startTime;

void setupVertices(void) {
    // 12 triangles * 3 vertices * 3 values (x, y, z)
    float CubeVertexPositions[108] = {
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
    };

    float CubeVertexColors[108] = {
        // Cara frontal (rojo → rojo claro)
        1.0f, 0.0f, 0.0f,   1.0f, 0.4f, 0.4f,   1.0f, 0.0f, 0.0f,
        1.0f, 0.4f, 0.4f,   1.0f, 0.0f, 0.0f,   1.0f, 0.4f, 0.4f,
        // Cara trasera (verde → verde claro)
        0.0f, 1.0f, 0.0f,   0.4f, 1.0f, 0.4f,   0.0f, 1.0f, 0.0f,
        0.4f, 1.0f, 0.4f,   0.0f, 1.0f, 0.0f,   0.4f, 1.0f, 0.4f,
        // Cara izquierda (azul → azul claro)
        0.0f, 0.0f, 1.0f,   0.4f, 0.4f, 1.0f,   0.0f, 0.0f, 1.0f,
        0.4f, 0.4f, 1.0f,   0.0f, 0.0f, 1.0f,   0.4f, 0.4f, 1.0f,
        // Cara derecha (amarillo → amarillo claro)
        1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.4f,   1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.4f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.4f,
        // Cara superior (magenta → magenta claro)
        1.0f, 0.0f, 1.0f,   1.0f, 0.4f, 1.0f,   1.0f, 0.0f, 1.0f,
        1.0f, 0.4f, 1.0f,   1.0f, 0.0f, 1.0f,   1.0f, 0.4f, 1.0f,
        // Cara inferior (cian → cian claro)
        0.0f, 1.0f, 1.0f,   0.4f, 1.0f, 1.0f,   0.0f, 1.0f, 1.0f,
        0.4f, 1.0f, 1.0f,   0.0f, 1.0f, 1.0f,   0.4f, 1.0f, 1.0f
    };


    glGenVertexArrays(1, vao);  // creates VAO and returns the integer ID
    glBindVertexArray(vao[0]);
    
    glGenBuffers(numVBOs, vbo);  // creates VBO and returns the integer ID
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    
    // loads the cube vertices into the 0th VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertexPositions), CubeVertexPositions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);  // associates 0th attribute with buffer
    glEnableVertexAttribArray(0);  // enable the 0th vertex attribute

    // Cargar colores en el segundo VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertexColors), CubeVertexColors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);  // Asociar el atributo 1 con los colores
    glEnableVertexAttribArray(1);  // Habilitar el atributo 1
    
}

void init (GLFWwindow *window){
    renderingProgram = Utils::createShaderProgram(
        "./shaders/Vshader_051.glsl",
        "./shaders/Fshader_05.glsl");

    if (Utils::checkOpenGLError()){
        std::cout << "ERROR: Could not create the shader program" << std::endl;
    }
    
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);  // 1.0472 radians = 60 degrees
    

    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
    cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = -1.0f;
    
    setupVertices();
}

void display (GLFWwindow *window, double currentTime){

    glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);
    
    vLoc = glGetUniformLocation(renderingProgram, "v_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

    vMat = glm::lookAt(
    glm::vec3(cameraX, cameraY, cameraZ), // eye position
    glm::vec3(0.0f, 0.0f, 0.0f),         // center: where the eye is looking at
    glm::vec3(0.0f, 1.0f, 0.0f)          // up: the upward direction
    );

    glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
     
    timeFactor = ((float)currentTime);
    tfLoc = glGetUniformLocation(renderingProgram, "tf");
    glUniform1f(tfLoc, (float)timeFactor);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glBindVertexArray(vao[0]);    
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100000);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "Error al renderizar: " << err << std::endl;
    }
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight);

int main(void) {
    if (!glfwInit()) {exit(EXIT_FAILURE);}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, " program 04 ", nullptr, nullptr);    
    glfwSetWindowSizeCallback(window, window_size_callback);
    
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
   
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    init(window);
    // Inicializar el tiempo de inicio
    startTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
    aspect = (float)width / (float)height;
    glViewport(0, 0, width, height);
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}