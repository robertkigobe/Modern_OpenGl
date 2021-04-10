//
//  main.cpp
//  Indexed_Draws
//
//  Created by Robert Kigobe on 09/04/2021.
//

//Glew
#define GLEW_STATIC
//#include <GL/glew.h>
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600; //window dimensions
const float toRadians = 3.14159265f / 180.0f; //function to convert degrees to radians

GLuint VBO, IBO,VAO, shader; //IBO is the index buffer object
GLuint uniformModel; //used to control magnitude of movement

//control movement of directions
bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;
//rotation navriables
float curAngle = 0.0f;
//scaling variables
bool sizeDirection = true;
float maxSize = 0.8f;
float minSize = 0.1f;
float curSize = 0.4f;



// Vertex Shader code
static const char* vShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
layout (location = 0) in vec3 pos;                                            \n\
                                                                              \n\
out vec4 vertexColor ;                                                         \n\
                                                \n\
uniform mat4 model;                                                           \n\
void main()                                                                   \n\
{                                                                             \n\
    gl_Position = model * vec4(pos, 1.0);                                     \n\
    vertexColor = vec4(clamp(pos,0.0f,1.0f),1.0f);                           \n\
}";

// Fragment Shader
static const char* fShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
in vec4 vertexColor;                                                          \n\
out vec4 colour;                                                              \n\
                                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
    colour = vertexColor;                                                    \n\
}";

void CreateTriangle()
{
    unsigned int indicies[] = {
            0,3,1,
            1,3,2,
            2,3,0,
            0,1,2
    };
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f,-1.0f,1.0f, // this createts a vertex in the z axis
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //create buffer for IBO and bind the data
    glGenBuffers(1,&IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies,GL_STATIC_DRAW);
    
    
    //create and bind data for VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0); //unbind IBO buffer

    glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, 1024, NULL, eLog);
        fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
    shader = glCreateProgram();

    if (!shader)
    {
        printf("Failed to create shader\n");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }
    
    uniformModel = glGetUniformLocation(shader,"model"); //bind a value in the shader of the model to be translated

}

int main()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        return 1;
    }


    // Setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core Profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow Forward Compatbility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the window
    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }

    // Get Buffer Size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST); //allows what triagles to draw over the others
    
    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    CompileShaders();

    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow))
    {
        // Get + Handle user input events
        glfwPollEvents();

        if (direction)
        {
            triOffset += triIncrement;
        }
        else {
            triOffset -= triIncrement;
        }

        if (abs(triOffset) >= triMaxOffset)
        {
            direction = !direction;
        }
        
        //continuous rotation
        curAngle += 0.5f;
        
        if (curAngle >=360)
        {
            curAngle -= 360;
        }
        
        //continuous scaling
        if (sizeDirection)
        {
            curSize += 0.001f;
        } else
        {
            curSize -=0.001f;
        }

        if(curSize >= maxSize || curSize <= minSize)
        {
            sizeDirection =! sizeDirection;
        }
        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);
        
        glm::mat4 model; //creates an identity matrix
        model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f,1.0f, 0.0f));//rotation by the z axis
        model = glm::translate(model, glm::vec3(0.0f,triOffset,0.0f ));//translates the x value of the model matrix
        model = glm::scale(model,glm::vec3(0.4f,0.4f,1.0f)); //this allows us to scale the images pragmatically and not in the shader components always use the scale first before other transformations
        glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));


        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
        glDrawElements(GL_TRIANGLES,12,GL_UNSIGNED_INT,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}



