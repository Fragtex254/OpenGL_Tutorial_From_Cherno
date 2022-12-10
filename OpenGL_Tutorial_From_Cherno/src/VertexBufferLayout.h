#pragma once
#include <vector>
#include <GL/glew.h>
#include "Renderer.h"
#include <stdexcept>
#include <iostream>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char nomalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:					return 4;
		case GL_UNSIGNED_INT:			return 4;
		case GL_UNSIGNED_BYTE:			return 1;
		}
		ASSERT(false);
		return 0;
	}
};


class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		:m_Stride(0){};

	#pragma region Push_Template

	template<typename T>
	void Push(unsigned int count) {
		//std::cout << "This is a template Push function be called!" << std::endl;
		std::runtime_error(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		//std::cout << "This is a specialization <float> Push function be called!" << std::endl;
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		//std::cout << "This is a specialization <unsigned int> Push function be called!" << std::endl;
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		//std::cout << "This is a specialization <unsigned char> Push function be called!" << std::endl;
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	/*Some Note Here
	* ABOUT static_assert: This assert will be work in the compile time in VS2022
	* and cherno's mean is that we can add a assert here and will use template will not hit that assert
	* but in VS2022 static_assert will be check in compile time
	* so we can use another assert to deal with it. 
	* with #include <stdexcept> we can replace static_assert() with std::runtime_error()
	*/

	#pragma endregion


	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }

	inline unsigned int GetStride() const { return m_Stride; }
};

