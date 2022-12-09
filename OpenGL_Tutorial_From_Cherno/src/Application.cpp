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
    #pragma region Initial part
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
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
    }

    /*Print the glew version*/
    std::cout << glGetString(GL_VERSION) << std::endl;
    #pragma endregion

    #pragma region Prepare Render Data

    float positions[]{
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),nullptr);



    #pragma endregion


    #pragma region Render Loop
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    #pragma endregion


    #pragma region terminal
    glfwTerminate();
    #pragma endregion

    return 0;
}