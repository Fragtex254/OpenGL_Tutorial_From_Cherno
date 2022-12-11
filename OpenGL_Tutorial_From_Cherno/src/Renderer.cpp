#include "Renderer.h"
#include <iostream>

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

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
	//it is important to emphasize the parameter of type in the Function::glDrawElements(type),must be the GL_UNSIGNED_INT!!!
	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}
