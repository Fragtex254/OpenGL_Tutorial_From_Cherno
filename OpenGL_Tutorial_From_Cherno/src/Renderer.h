#pragma once
#include <GL\glew.h>

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

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

#pragma endregion