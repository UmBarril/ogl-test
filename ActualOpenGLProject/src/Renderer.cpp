#include "Renderer.hpp"
#include <iostream>

void GLClearErrors()
{
	while(glGetError() != GL_NO_ERROR) { }
}

bool GLReadErrors(const char* function, const char* file, int line)
{
	while(GLenum error = glGetError()) { 
		std::cout << "[OpenGL Error] (" << error << "): " << function << ' ' << file << ':' << line << '\n';
		return false;
	}
	return true;
}

void Renderer::Bind() const
{
}

void Renderer::Unbind() const
{
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
	// drawing a buffer without an index buffer
	//GLErrorSafeCall(glDrawArrays(GL_TRIANGLES, 0, amountOfVertices));
	
	va.Bind();
	ib.Bind();
	shader.Bind();

	// drawing with index buffers
	GLErrorSafeCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); // nullptr because we have already saved our indices in a buffer
}

void Renderer::Clear()
{
	// why https://stackoverflow.com/questions/19469194/why-do-we-have-to-clear-depth-buffer-in-opengl-during-rendering/19469291#19469291
	GLErrorSafeCall(glClear(GL_COLOR_BUFFER_BIT));
}
