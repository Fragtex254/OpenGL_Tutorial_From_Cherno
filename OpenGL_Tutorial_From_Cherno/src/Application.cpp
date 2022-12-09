#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include <fstream>
#include <string>
#include <sstream>

/*
* all the learning resources link here:
* https://docs.gl/
* https://learnopengl-cn.github.io/
* https://glew.sourceforge.net/
*/

#pragma region Shader Function
struct ShaderProgramSources
{
    std::string VertexSource;
    std::string FragmentSource;

};

static ShaderProgramSources ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    //This enum class just like a classifier
    enum class ShaderType 
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };


    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    /*    
    *Read the all lines in the file and find the flag "#shader" and then start to pick line up
    *Set the classifier's state and put the line into two shader string stream
    *And use the constructor to return a "ShaderProgramSources" struct
    */

    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(),ss[1].str() };
}

unsigned int compileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    //check the state of shader if sth wrong will print log in the console
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compiled " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);//don't forget to delete shader in this case
        return 0;
    }

    return id;
}

static unsigned int  CreataShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //link the shader with the program
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    //delete the shader
    glDeleteShader(vs);
    glDeleteShader(fs);
    //and return the "V&Fshadered" program id
    return program;
}
#pragma endregion


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

    ShaderProgramSources source = ParseShader("res/shaders/Basic.shader");
    std::cout << "Vertex Shader" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "Fragment Shader" << std::endl;
    std::cout << source.FragmentSource << std::endl;

    unsigned int shader = CreataShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);


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
    glDeleteProgram(shader);
    glfwTerminate();
    #pragma endregion

    return 0;
}