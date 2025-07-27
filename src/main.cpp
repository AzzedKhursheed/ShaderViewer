/*
*What is the following:
*   GLAD? 
*       Loads OpenGL functions into your app. Think: “How do I call OpenGL?”
*   GLFW?
*       Creates the window and OpenGL context. Think: “Where do I draw?”
*   glm?
*       OpenGL Mathematics Library
*   OpenGL?
*       Handles GPU state, shaders, vertex buffers, etc. Think: “Do the actual drawing.”
*   
*
*    
*
*
*
*
*    
*
*
*
*
*    
*
*
*
*
*/

// OpenGL loader (GLAD) and context/window management (GLFW)
#define GLFW_INCLUDE_NONE // Tell GLFW not to include OpenGL headers
#include <GLFW/glfw3.h>
#include <glad/gl.h>

// Standard libraries for I/O and math
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Project headers
#include "Camera.h"   // Provides view and projection matrices
#include "ObjModel.h" // Loads and draws a 3D .obj model
#include "Shader.h"   // Handles GLSL shader program compilation & usage

bool reloadRequested = false;

// Callback to adjust OpenGL viewport when the window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (key == GLFW_KEY_R && action == GLFW_PRESS)
    reloadRequested = true;
}

int main() {

  /////////////////////////////////////////////////INITIALIZATION
  /// PHASE////////////////////////////////

  // Step 1: Initialize GLFW (window + context management)
  if (!glfwInit()) {
    std::cout << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  // Step 2: Create a windowed mode window and OpenGL context
  GLFWwindow *window =
      glfwCreateWindow(800, 600, "ShaderViewer Test", NULL, NULL);
  if (!window) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  // Step 3: Make the window's OpenGL context current
  glfwMakeContextCurrent(window);

  // Set a callback for window resize events
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Step 4: Load OpenGL function pointers via GLAD
  // Must be called AFTER you have a valid OpenGL context (i.e., after
  // glfwMakeContextCurrent).
  if (!gladLoaderLoadGL()) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }

  // still need to understand this !!!!!
  glEnable(GL_DEPTH_TEST);
  //////////////////////////////////////////////////////////////////////////////////////////////////

  // Log the active OpenGL version
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

  // Step 5: Load and set up core objects
  // Local for now
  Shader shader("shaders/default.vert",
                "shaders/default.frag"); // Loads and compiles shaders
  ObjModel model("assets/suzanne.obj");  // Loads a 3D model from .obj file
  Camera camera; // Camera providing view/projection matrices

  // subscribe to user input for keys
  glfwSetKeyCallback(window, key_callback);

  // Step 6: Main rendering loop
  while (!glfwWindowShouldClose(window)) {

    // Check if Shader reload was requested
    if (reloadRequested) {
      shader.reload();
      std::cout << "Shaders reloaded!" << std::endl;
      reloadRequested = false;
    }

    // Clear the screen with a dark gray color
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // still need to understand this !!!!!
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use shader program and set matrices
    shader.use();

    // Identity model matrix (no transformations yet)
    glm::mat4 modelMat = glm::mat4(1.0f);

    //still need to understand this !!!!!
    glEnable(GL_DEPTH_TEST);
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Log the active OpenGL version
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Step 5: Load and set up core objects
    // Local for now
    Shader shader("shaders/default.vert", "shaders/default.frag"); // Loads and compiles shaders
    ObjModel model("assets/suzanne.obj");                          // Loads a 3D model from .obj file
    Camera camera;                                                 // Camera providing view/projection matrices

    //subscribe to user input for keys
    glfwSetKeyCallback(window, key_callback);

    // Step 6: Main rendering loop
    while (!glfwWindowShouldClose(window)) {

        //Check if Shader reload was requested
        if (reloadRequested) {
        shader.reload();
        std::cout << "Shaders reloaded!" << std::endl;
        reloadRequested = false;
        }

        // Clear the screen with a dark gray color
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        
        //still need to understand this !!!!!
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader program and set matrices
        shader.use();

        // Identity model matrix (no transformations yet)
        glm::mat4 modelMat = glm::mat4(1.0f);

        // Get view matrix from camera (defines camera position/direction)
        glm::mat4 view = camera.getViewMatrix();

        // Calculate projection matrix based on window aspect ratio
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glm::mat4 projection = camera.getProjectionMatrix(width / (float)height);

        // Send matrices to the shader
        shader.setMat4("model", glm::value_ptr(modelMat));
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));

        //Add time uniform for animation
        float timeVal = static_cast<float>(glfwGetTime());
        shader.setFloat("time", timeVal);
        
        // Draw the 3D model
        model.draw();

        // Swap front and back buffers (double-buffered rendering)
        glfwSwapBuffers(window);

        // Poll for window events (input, resize, etc.)
        glfwPollEvents();
    }

    // Calculate projection matrix based on window aspect ratio
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glm::mat4 projection = camera.getProjectionMatrix(width / (float)height);

    // Send matrices to the shader
    shader.setMat4("model", glm::value_ptr(modelMat));
    shader.setMat4("view", glm::value_ptr(view));
    shader.setMat4("projection", glm::value_ptr(projection));

    // Add time uniform for animation
    float timeValue = static_cast<float>(glfwGetTime());
    shader.setFloat("time", timeValue);

    // Draw the 3D model
    model.draw();

    // Swap front and back buffers (double-buffered rendering)
    glfwSwapBuffers(window);

    // Poll for window events (input, resize, etc.)
    glfwPollEvents();
  }

  // Cleanup and exit
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
