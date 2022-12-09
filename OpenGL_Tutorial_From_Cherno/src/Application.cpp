#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include<iostream>

/*
* all the learning resources link here:
* https://docs.gl/
* https://learnopengl-cn.github.io/
* https://glew.sourceforge.net/
*/


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Init the GLEW and glewInit function have to behide the windowcontext*/
    if (glewInit()!=GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
    }

    //
    std::cout << glGetString(GL_VERSION) << std::endl;





    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}