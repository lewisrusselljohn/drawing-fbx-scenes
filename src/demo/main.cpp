#include <glew.h>
#include <glfw3.h>
#include <stdio.h>

#include "Cage.h"
#include "FbxLoader.h"
#include "VaoContainer.h"
#include "TextureShader.h"
#include "ShaderSources.h"

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void Setup(GLFWwindow*);
void Cleanup(GLFWwindow*);

double oldmousex = 0.0;
double oldmousey = 0.0;
double yaw_angle = 0.0;
double pitch_angle = 0.0;

bool forward = false;

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "FPS", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    const char* errorPointer = NULL;
    glfwGetError(&errorPointer);

    if (glfwGetError(&errorPointer) != GLFW_NO_ERROR)
    {
        std::cout << " GLFW Error: " << std::endl;
    }

    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetKeyCallback(window, KeyCallback);

    // Initialize GLEW - OpenGL extension functions - after making GL context current
    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        printf_s("GLEW Error: %s\n", glewGetErrorString(error));
        return -1;
    }
    // clean error state
    glGetError();

    Setup(window);

    Cage* cage = new Cage(20.0, 20);
    FbxLoader* loader = new FbxLoader();

    VaoContainer* container = new VaoContainer();
    TextureShader* texture_shader = new TextureShader(vs_source, fs_source);
    Camera* camera = new Camera();

    /*
        Load the contents of your fbx file into the Vao Container
        Make mesh a triangle mesh when exporting
    */
    loader->Load("cube.fbx", container);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Update the camera transforms with user input
        camera->UpdateCamera(yaw_angle, pitch_angle, forward);

        glEnable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Render here */
        texture_shader->UseProgram(camera->GetProjectionMatrix(),
            camera->GetModelViewMatrix());

        // Pass program id to draw function so it can set the world transform uniform
        cage->DrawLines(texture_shader->GetProgramId());
        container->DrawVaos(texture_shader->GetProgramId());
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    Cleanup(window);

    glfwTerminate();

	return 0;
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    double cx = xpos - oldmousex;
    double cy = ypos - oldmousey;

    double magnitude = sqrt(cx * cx + cy * cy);
    cx /= magnitude;
    cy /= magnitude;
    cx *= 0.05;
    cy *= 0.05;

    pitch_angle += cy * -1.0f;
    yaw_angle += cx * -1.0f;

    oldmousex = xpos;
    oldmousey = ypos;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    switch (key)
    {
    case GLFW_KEY_W:
    {
        if (action == GLFW_PRESS)
            forward = true;
        else if (action == GLFW_RELEASE)
            forward = false;
        break;
    }
    }
}

void Setup(GLFWwindow* window)
{
    if (glfwRawMouseMotionSupported()) {
        // Mouse lock feature is supported
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else {
        // Mouse lock feature is not supported
    } 
}

void Cleanup(GLFWwindow* window)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
