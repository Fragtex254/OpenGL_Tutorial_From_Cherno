#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

//glm math library
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

//Test
#include "Test/TestClearColor.h"



#pragma region Resource Link
/*
* all the learning resources link here:
* https://docs.gl/
* https://learnopengl-cn.github.io/
* https://www.glfw.org/
* https://glew.sourceforge.net/
*/
#pragma endregion


int main(void)
{
    #pragma region Initial part
    GLFWwindow* window;

    glfwSwapInterval(1);

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwInitHint(GLEW_VERSION_MAJOR, 3);
    glfwInitHint(GLFW_VERSION_MINOR, 3);
    glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /*
    *GLFW_OPENGL_COMPAT_PROFILE has a default vao for you but GLFW_OPENGL_CORE_PROFILE has not!
    */

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Init the GLEW and glewInit function have to behide the window context*/
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
    }

    /*Print the glew version*/
    std::cout << glGetString(GL_VERSION) << std::endl;
    #pragma endregion
    {
        #pragma region Prepare Render Data


        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));


        Renderer renderer;

        #pragma region ImGui_Init

		// Setup ImGui binding
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();		// Setup style

        #pragma endregion


        #pragma endregion

        Test::TestClearColor test;

        #pragma region Render Loop
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            test.OnUpdate(0.0f);
            test.OnRender();

			ImGui_ImplGlfwGL3_NewFrame();
            test.OnImGuiRender();
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        #pragma endregion


        #pragma region terminal
    }

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
    glfwTerminate();
    #pragma endregion

    return 0;
}