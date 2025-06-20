//////////////////////////////////////////////////////////////////////
//  Program 2                                                       //
//  This program is a basic OpenGl application using GLFW           //
//  for window management and GLAD for loading OpenGL functions.    //
//  The program creates a window and clears the window with a       //
//  red color.                                                      //
//                                                                  //      
//  you can find more information about how                         //
//  this program works in my blog.                                  //
//                                                                  //      
//  Francisco Zavala                                                //
//////////////////////////////////////////////////////////////////////

#include <glad/glad.h>  // Library for handling the loading of OpenGL functions
#include <GLFW/glfw3.h> // Library for handling window and user input
#include <iostream>     

#define numVAOs 1 // Number of Vertex Array Objects
#define numVBOs 1 // Number of Vertex Buffer Objects

GLuint renderingProgram;    
GLuint vao[numVAOs]; 
GLuint vbo[numVBOs];


/*
    const char *vshaderSource =
        "#version 450\n"
        "layout(location=0) in vec3 position;\n"
        "void main(void) {\n"
        "    gl_Position = vec4(position, 1.0);\n"
        "}\n";

    const char *fshaderSource =
        "#version 450\n"
        "out vec4 color;\n"
        "void main(void) {\n"
        "    color = vec4(1.0, 0.0, 0.0, 1.0);\n" // Rojo
        "}\n";

*/

GLuint createShaderProgram() {
    const char *vshaderSource =
        "#version 450\n"
        "layout(location=0) in vec3 position;\n"
        "out vec4 color_;\n"
        "void main(void) {\n"
        "    gl_Position = vec4(position, 1.0);\n"
        "    if(gl_VertexID == 0 || gl_VertexID == 3) {\n"
        "        color_ = vec4(1.0, 0.0, 0.0, 1.0); // Rojo para los vértices 0 y 3\n"
        "    } else if(gl_VertexID == 1 || gl_VertexID == 5) {\n"
        "        color_ = vec4(0.0, 0.0, 1.0, 1.0); // Verde para los vértices 1 y 4\n"
        "    } else {\n"
        "        color_ = vec4(0.0, 1.0, 0.0, 1.0); // Azul para los vértices 2 y 5\n"
        "    }\n"
        "}\n";

    const char *fshaderSource =
        "#version 450\n"
        "out vec4 color;\n"
        "in vec4 color_;\n"
        "void main(void) {\n"
        "    color =  color_;\n" // Usar el color interpolado
        "}\n";

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint vfprogram = glCreateProgram();

    glShaderSource(vShader, 1, &vshaderSource, NULL);
    glShaderSource(fShader, 1, &fshaderSource, NULL);
    glCompileShader(vShader);
    glCompileShader(fShader);

    glAttachShader(vfprogram, vShader);
    glAttachShader(vfprogram, fShader);
    glLinkProgram(vfprogram);

    return vfprogram;
}


void setupvertices(){
    // Array de vértices para un cuadrado (dos triángulos)
    float vertexPositions[] = {
        // Primer triángulo
        -0.5f, -0.5f, 0.0f, // esquina inferior izquierda
        0.5f, -0.5f, 0.0f, // esquina inferior derecha
        0.5f,  0.5f, 0.0f, // esquina superior derecha

        // Segundo triángulo
        -0.5f, -0.5f, 0.0f, // esquina inferior izquierda
        0.5f,  0.5f, 0.0f, // esquina superior derecha
        -0.5f,  0.5f, 0.0f  // esquina superior izquierda
    };

    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(numVBOs, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

// Initialization function (currently empty)
void init(GLFWwindow* window){
    renderingProgram = createShaderProgram(); // Create the shader program
    setupvertices(); // Setup the vertex data for rendering
}

// Function to update the display
void display(GLFWwindow* window){
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);
    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //glDrawArrays(GL_LINE_LOOP, 0, 6); // Draw the square as a line loop

}

int main() {
    
    // Initialize GLFW, terminate program if failed
    if (!glfwInit()) {exit(EXIT_FAILURE);}

    // Set window properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);                  
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            

    //monitor properties
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, " 02_triangles ", nullptr, nullptr);    

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
        display(window);          
        glfwPollEvents();        
        glfwSwapBuffers(window); 
    }

    // Terminate GLFW when program ends
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}