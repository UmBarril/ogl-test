#include "IndexBuffer.hpp"
#include "Renderer.hpp"

IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count) : m_Count(count)
{
	GLErrorSafeCall(glGenBuffers(1, &m_RendererID));
	this->Bind();
	GLErrorSafeCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), indices, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLErrorSafeCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const // select this buffer
{
	GLErrorSafeCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); 
}

void IndexBuffer::Unbind() const // unselect any buffer
{
	GLErrorSafeCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); 
}
