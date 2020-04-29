/**
* Main file that renders the window to the screen and runs the game
*/
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void checkVertexShaderCompileError(unsigned int vertexShader);
void checkFragmentShaderCompileError(unsigned int fragmentShader);
void checkShaderProgramCompileError(unsigned int shaderProgram);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Source code for the vertex shader in the OpenGL Shading Language, written as a C string
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// Source code for the fragment shader in the OpenGL Shading Language, written as a C string
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

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

    // Build and compile our shader programs
    // -------------------------------------
    
    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Attach that C string of vertex shader source code
    glCompileShader(vertexShader);  // Compile that source code
    checkVertexShaderCompileError(vertexShader); // Check for vertex shader compile time errors

    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkFragmentShaderCompileError(fragmentShader); // Check for fragment shader compile time errors

    // Link the shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);    // Attach the shaders
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);   // Link the shaders
    checkShaderProgramCompileError(shaderProgram);  // Check for shader program compile time errors

    // After we've linked the vertex and fragment shaders to the shader program, we no longer need them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // -------------------------------------
    // End shader section



    // Set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    unsigned int VAO;   // Vertex Array Object
    glGenVertexArrays(1, &VAO);

    unsigned int VBO;   // Vertex Buffer Object (array pointer)
    glGenBuffers(1, &VBO);  // Generates a Buffer Object

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and the configure vertex attribute(s)
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);     // Bind the newly created Vertex Buffer to the GL_ARRAY_BUFFER target
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // Copy the previously defined vertex data into the buffer's memory

    // Tell OpenGL how it should interpret the vertex data (per vertex attribute)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


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

        // Draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);     // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
* GLFW: Whenever the window size is changed (by OS or user resize), this callback function executes
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

/**
* Check for Vertex Shader errors on compilation
*/
void checkVertexShaderCompileError(unsigned int vertexShader)
{
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    else if (success)
    {
        std::cout << "Grant is a straight god for debugging this garbage tutorial (vertex shader)\n" << std::endl;
    }
}

/**
* Check for Fragment Shader errors on compilation
*/
void checkFragmentShaderCompileError(unsigned int fragmentShader)
{
    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    else if (success)
    {
        std::cout << "Grant is a straight god for debugging this garbage tutorial (fragment shader)\n" << std::endl;
    }
}

/**
* Check for Shader Program errors on compilation
*/
void checkShaderProgramCompileError(unsigned int shaderProgram)
{
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    else if (success)
    {
        std::cout << "Grant is a straight god for debugging this garbage tutorial (shader program)\n" << std::endl;
    }
}