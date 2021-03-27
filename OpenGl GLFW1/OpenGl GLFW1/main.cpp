//
//  main.cpp
//  OpenGl GLFW1
//
//  Created by Robert Kigobe on 25/03/2021.
//

#include <iostream>
#include <stdio.h>
#include <string.h>

//Glew
#define GLEW_STATIC
#include <GL/glew.h>


//GLFW
#include <GLFW/glfw3.h>


const GLint WIDTH = 800, HEIGHT = 600; //creates windo dimensions

int main(){
    
    //initialize glfw
    if(!glfwInit())
    {
        printf("Initialization of GLFW failed");
        glfwTerminate();
        return 1;
    }
    
    //setup GLFW window properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //version of GLFW 3, a modern version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //set glfw to use 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Not backward compartible to old opengl code
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //necessary for mac and allows to work forward
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //necessary for mac
    
    //create the window
    GLFWwindow *window = glfwCreateWindow(WIDTH,HEIGHT, "Test Window",nullptr,nullptr);
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight); //necessary for mac retina display compatibity
    
    if (!window){
        
        //std::cout <<"Failed to create window" << std::endl;
        printf("Window creation failed");
        glfwTerminate();
        
        return -1;
    }
    
    //Get buffer size information
    int bufferWidth,bufferHeight; //necessary for mac retina display compatibility
    
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    
    //Set Context for Glew to use
    glfwMakeContextCurrent(window);
    
    //Allow modern extensions features
    glewExperimental = GL_TRUE;
    
    //Initialize Glew
    if (glewInit() != GLEW_OK){
        //std::cout << "Failed to initialize GLEW" <<std::endl; //check for errors
        printf("Failed to initialize GLEW");
        glfwDestroyWindow(window); //Kills window that was created
        glfwTerminate();
        return 1;
    }
    
    //Set up View Port Size
    glViewport(0,0,bufferWidth,bufferHeight);
    
    
    //Loop until window should close
    while(!glfwWindowShouldClose(window)){
        
        //Get and handle user inputs events
        glfwPollEvents();
        
      
        glClearColor(0.2f,0.3f,0.3f,1.0f);   //Clear window of any contents
        glClear(GL_COLOR_BUFFER_BIT); //Clears all colors in the window
        
        glfwSwapBuffers(window); //Allows the buffer you have just been drawinig to to be on top.
        
    }
    
    glfwTerminate();
    return 0;
    
}

