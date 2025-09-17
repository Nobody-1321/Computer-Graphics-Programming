///////////////////////////////////////////////////////////////////////
//  Program 3                                                        //
//  This program demonstrates basic vector transformations in OpenGL //
//  using GLFW for window management and GLAD for loading OpenGL     //
//  functions. It renders a square that moves, scales, and rotates   //
//  over time using transformation matrices.                         //
//                                                                   //      
//  For more details about how this program works, visit my blog.    //
//                                                                   //      
//  Francisco Zavala                                                 //
///////////////////////////////////////////////////////////////////////

#include <glad/glad.h>  // Library for handling the loading of OpenGL functions
#include <GLFW/glfw3.h> // Library for handling window and user input
#include <iostream>     
#include <Utils.hpp>     // Utility functions for shader compilation and other tasks

#define numVAOs 1 // Number of Vertex Array Objects
#define numVBOs 1 // Number of Vertex Buffer Objects

GLuint renderingProgram;    
GLuint vao[numVAOs]; 
GLuint vbo[numVBOs];
// Transformation matrix
glm::mat4 mv_matrix = glm::mat4(1.0);
float scaleAmount = 1.0f;
bool vander1 = true;
bool vander2 = true;
float stran = 1.0;

// Sets up vertex data for a square (two triangles)
void setupvertices(){
    // Vertex array for a square (two triangles)
    float vertexPositions[] = {
        // First triangle
        -0.1f, -0.1f, 0.0f, // bottom left corner
        0.1f, -0.1f, 0.0f,  // bottom right corner
        0.1f,  0.1f, 0.0f,  // top right corner

        // Second triangle
        -0.1f, -0.1f, 0.0f, // bottom left corner
        0.1f,  0.1f, 0.0f,  // top right corner
        -0.1f,  0.1f, 0.0f  // top left corner
    };

    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(numVBOs, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

// Initialization function
void init(GLFWwindow* window){
    renderingProgram = Utils::createShaderProgram("./shaders/Vshader_03.glsl",
    "./shaders/Fshader_03.glsl");
    setupvertices(); // Setup the vertex data for rendering
}

// Function to update the display
void display(GLFWwindow* window, double currenTime){
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);

    // Get the location of the uniform mv_matrix
    GLuint mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    
    mv_matrix = glm::mat4(1.0); // Reset to identity matrix

    // Translate the object
    if (stran >= 0.5f){
        vander1 = false;
    }
    else if (stran <= -0.5f){
        vander1= true;
    }

    if(vander1 == false){
        stran -= 0.01f;
    }
    else{
        stran += 0.01f;
    }

    mv_matrix = glm::translate(mv_matrix, glm::vec3(stran, 0.0f, 0.0f));
    
    // Scale the object
    const float epsilon = 0.0001f;

    if (fabs(scaleAmount - 3.5f) < epsilon) {
        vander2 = false;
    } else if (fabs(scaleAmount - 0.5f) < epsilon) {
        vander2= true;
    }

    if (vander2) {
        scaleAmount += 0.01f; // Increase scale value
    } else {
        scaleAmount -= 0.01f; // Decrease scale value
    }
    
    mv_matrix = glm::scale(mv_matrix, glm::vec3(scaleAmount, scaleAmount, scaleAmount));

    // Rotate the object
    float angle = (float)currenTime; // Angle in radians based on current time
    mv_matrix = glm::rotate(mv_matrix, angle, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotation around Z axis
    
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv_matrix));

    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

int main() {
    
    // Initialize GLFW, terminate program if failed
    if (!glfwInit()) {exit(EXIT_FAILURE);}

    // Set window properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);                  
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            

    // Get monitor properties
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, " 03_mathematics_for_graphics ", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Set the OpenGL context for the window
    glfwMakeContextCurrent(window);

    // Initialize GLAD to load OpenGL functions
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "failed to initialize GLAD " << std::endl;
        return -1;
    }
    
    // Initialize the application
    init(window);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());          
        glfwPollEvents();        
        glfwSwapBuffers(window); 
    }

    // Terminate GLFW when program ends
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
