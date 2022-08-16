#pragma once
#include <GL\glew.h>
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

// only works on visual studio compiler
#define ASSERT(x) if(!(x)) __debugbreak(); 

#define GLErrorSafeCall(x) GLClearErrors();\
	x;\
	ASSERT(GLReadErrors(#x, __FILE__, __LINE__));

void GLClearErrors(); 
bool GLReadErrors(const char* function, const char* file, int line); 

class Renderer : public IBindable {
private:
	unsigned int m_RendererID;
public:
	void Bind() const;
	void Unbind() const;

	void Draw(const VertexArray& vb, const IndexBuffer& ib, const Shader& shader);
	void Clear();
};
