#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include <fstream>
#include <string>
#include <sstream>

#pragma region Resource Link
/*
* all the learning resources link here:
* https://docs.gl/
* https://learnopengl-cn.github.io/
* https://glew.sourceforge.net/
*/
#pragma endregion

#pragma region Debug define
#define ASSERT(x) if(!(x)) __debugbreak();
//use this back-slant to keep the code stiil in one #define
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))
//"#x" is the return the function name as a const char* array
//"__FILE__" is the current file where the breakpoint be hitted
//"__LINE__" is the current line where the breakpoint be hitted

/*
* after we have this define 
* we can wrap our function into the GLCALL(our_Function) to check whether we have made sth wrong
*/

#pragma endregion

#pragma region Debug function
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << " )" << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}
#pragma endregion

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

    glfwSwapInterval(1);

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
         0.5f, -0.5f,
         0.5f,  0.5f,
         -0.5f,  0.5f,
    };

    unsigned int indices[]
    {
        0,1,2,
        2,3,0
    };

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),nullptr);

    unsigned int ibo;//ebo
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);


    ShaderProgramSources source = ParseShader("res/shaders/Basic.shader");

    unsigned int shader = CreataShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    int location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);
    glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f);

    float r = 0.0f;
    float increment = 0.05f;

    #pragma endregion


    #pragma region Render Loop
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform4f(location, r, 0.3f, 0.8f, 1.0f);

        if (r>1.0f)
        {
            increment = -0.05f;
        }
        else if (r<0.0f)
        {
            increment = 0.05f;
        }

        r += increment;

        //it is important to emphasize the parameter of type in the Function::glDrawElements(type),must be the GL_UNSIGNED_INT!!!
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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