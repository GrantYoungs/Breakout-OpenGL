/**
* Main file that renders the window to the screen and runs the game
*/
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // Initilize GLFW, GLAD, and OpenGL
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // Include this line if you're using a Mac

    // Creation of the glfw window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "BreakOut", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /*
    * Render loop - this will keep the window running until it has been explicitly told to close
    */
    while (!glfwWindowShouldClose(window))
    {
        // Process input commands
        processInput(window);

        // Render a color to the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // State-setting function
        glClear(GL_COLOR_BUFFER_BIT);           // State-using function

        // Check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // glfw: Terminate, clearing all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}

/**
* Resizes the window
* GLFW: Whenever the window size is changed (by OS or user resize), this callback functino executes
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/**
* Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
*/
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}