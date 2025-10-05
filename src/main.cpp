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
*   ImGui? : Immediate Mode Graphical User Interface
*       Gives UI elements that update every fram (dafuq? thats instense).
*       This is what we'll use to get a window for live editing Shaders
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
#include <glad/gl.h>
#include <GLFW/glfw3.h>

// Standard libraries for I/O and math
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream> // For file operations
#include <vector> // For std::vector

// Project headers
#include "Shader.h"     // Handles GLSL shader program compilation & usage
#include "ObjModel.h"   // Loads and draws a 3D .obj model
#include "Camera.h"     // Provides view and projection matrices

// ImGui headers
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


bool reloadRequested = false;   //Updates to true if R key pressed for reload
float modelX = -1.0f; // Position of the first model (modifiable with arrow keys)
float modelY = -1.0f; // Position of the first model (modifiable with arrow keys)
//look at key_callback for more


// ============================Call Back methods for subscriptions==================================================================

// Callback to adjust OpenGL viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    //tells OpenGL what part of the window you want to render to.
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

      if (key == GLFW_KEY_R && action == GLFW_PRESS)
            reloadRequested = true;

    // Move model left/right
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        modelX -= 0.1f;
    }
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        modelX += 0.1f;
    }

    // Move model Up/Down
    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        modelY += 0.1f;
    }
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        modelY -= 0.1f;
    }

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // Get the camera pointer from the window user pointer
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera) {
        camera->processScroll(static_cast<float>(yoffset));
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (camera) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            camera->startDrag(xpos, ypos);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            camera->endDrag();
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera && camera->dragging)
        camera->updateDrag(xpos, ypos);
}


//===============================================================================================================================

int main() {

    //===============================================INITIALIZATION PHASE =======================================================

    // Step 1: Initialize GLFW (window + context management)
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Step 2: Create a windowed mode window and OpenGL context
    GLFWwindow* window = glfwCreateWindow(1200, 800, "ShaderViewer Test", NULL, NULL);
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
    // Must be called AFTER you have a valid OpenGL context (i.e., after glfwMakeContextCurrent).
    if (!gladLoaderLoadGL()) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    //This enables depth perception
    //Specifically helps gl figure out overlaps on geometry
    //so it will make the object face in front will be rendered while face in the back will not be unless object is flipped
    glEnable(GL_DEPTH_TEST);
    
    //=========================================================================================================================    
    
    //================================================ Object initialization ==================================================    
    
    // Log the active OpenGL version
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Step 5: Load and set up core objects
    // Local for now
    Shader shader("shaders/default.vert", "shaders/default.frag"); // Loads and compiles shaders
    ObjModel model("assets/suzanne.obj");                          // Loads a 3D model from .obj file
    Camera camera;                                                 // Camera providing view/projection matrices


    //=============================================== Event Subscription =====================================================

    //Creates a custom pointer to the Camera object which allows us to get it later when scroll_callback is called
    glfwSetWindowUserPointer(window, &camera);

    //subscribe to user input for scroll wheels
    glfwSetScrollCallback(window, scroll_callback);

    //subscribe to user input for keys
    glfwSetKeyCallback(window, key_callback);

    //subscribe to user input for button press
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    glfwSetCursorPosCallback(window, cursor_position_callback);

    //=========================================================================================================================

    //================================================ ImGui initialization ==================================================    
    /*
    * I tried moving imgui init up to initialization phase but then could no longer control move window with mouse
    * But If I do that but remove glfwSetWindowUserPointer(window, &camera); it starts working again???
    *
    * Chatgpt says:
    * ImGui installs input callbacks that internally rely on glfwGetWindowUserPointer.
    * If init set before, ImGui and chained callbacks will receive a null pointer.
    * Commenting glfwGetWindowUserPointer out entirely "works" only because ImGui handles its own input,
    * but custom input (e.g., camera drag/scroll) will break without this.
    * 
    * */

    // ImGui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();                         //All ImGui state (UI style, IO config, fonts, windows, etc.) is stored in this context. You must call this before doing anything else with ImGui.              
    ImGui::StyleColorsDark();                       //Theme, can try StyleColorsLight() or StyleColorsClassic()
    ImGui_ImplGlfw_InitForOpenGL(window, true);     //This connects ImGui input to GLFW, so it can receive keyboard, mouse, and gamepad input.
    ImGui_ImplOpenGL3_Init("#version 330");         //This allows ImGui to render with OpenGL 3.3 and ensures its shaders compile properly with your current OpenGL context.
    ImGui::GetIO().IniFilename = "build/imgui.ini"; // Set ImGui ini filename

    // Load fragment shader source
    std::string fragShaderPath = "shaders/default.frag";
    std::vector<char> fragSourceVec;
    {
        std::ifstream fragFile(fragShaderPath);
        fragSourceVec = std::vector<char>((std::istreambuf_iterator<char>(fragFile)), std::istreambuf_iterator<char>());
        fragSourceVec.push_back('\0'); // Ensure null-terminated
    }
    bool fragChanged = false;

    //=========================================================================================================================

    //=============================================== Render Loop ============================================================

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
        // Model matrix for first model (position controlled by Keys) (See modelX and modelY)
        glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(modelX, modelY, 0.0f));
        glm::mat4 modelMat2 = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // Get view matrix from camera (defines camera position/direction)
        glm::mat4 view = camera.getViewMatrix();

        // Calculate projection matrix based on window aspect ratio
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glm::mat4 projection = camera.getProjectionMatrix(width / (float)height);

        // Send matrices to the shader and draw
        shader.setMat4("model", glm::value_ptr(modelMat));
        model.draw();

        // Send matrices to the shader and draw
        shader.setMat4("model", glm::value_ptr(modelMat2));
        model.draw();

        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));

        //Add time uniform for animation
        float timeVal = static_cast<float>(glfwGetTime());
        shader.setFloat("time", timeVal);

        // Monitor camera coordinates in the window title
        char title[128];
        snprintf(title, sizeof(title), "ShaderViewer - Camera: (%.2f, %.2f, %.2f)", camera.position.x, camera.position.y, camera.position.z);
        glfwSetWindowTitle(window, title);



        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Force position and size for the editor window // if you remove this you can move it around and stuff
        ImGui::SetNextWindowPos(ImVec2(width * 0.6f, 0), ImGuiCond_Always);   // Snap to right side
        ImGui::SetNextWindowSize(ImVec2(width * 0.4f, height), ImGuiCond_Always); // Take up 40% width

        // ImGui window for fragment shader editing
        ImGui::Begin("Fragment Shader Editor");     //Title
        if (ImGui::InputTextMultiline("##frag", fragSourceVec.data(), fragSourceVec.size(), ImVec2(-FLT_MIN, 400), ImGuiInputTextFlags_AllowTabInput)) 
        {
            fragChanged = true;
        }

        
        if (ImGui::Button("Save & Reload (R)")) {
            std::ofstream fragFile(fragShaderPath);
            fragFile << fragSourceVec.data();
            fragFile.close();
            reloadRequested = true;
            fragChanged = false;
        }
        ImGui::End();

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap front and back buffers (double-buffered rendering)
        glfwSwapBuffers(window);

        // Poll for window events (input, resize, etc.)
        glfwPollEvents();
    }

    // =========================================================================================================================

    // ===================================================Clean Up======================================================================

    // ImGui cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Cleanup and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
    
    // =========================================================================================================================
}
