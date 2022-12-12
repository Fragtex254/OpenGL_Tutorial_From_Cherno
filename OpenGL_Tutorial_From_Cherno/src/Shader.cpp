#include "Shader.h"
#include<iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"





Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath),m_RendererID(0)
{
	ShaderProgramSources source = ParseShader(m_FilePath);
	m_RendererID = CreataShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));

}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));

}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}


void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name)!=m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }


    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location==-1)
        std::cout << "Warning Uniform!" << std::endl;
    m_UniformLocationCache[name] = location;
    
    return location;
}


#pragma region ShaderFunction

ShaderProgramSources Shader::ParseShader(const std::string& filepath)
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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
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

unsigned int  Shader::CreataShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

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