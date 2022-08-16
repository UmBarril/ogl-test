#include "VertexBuffer.hpp"
#include "Renderer.hpp"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	// more than 1 means that it will return an array of buffer id's
	const int amountOfBuffersToGen = 1; 
	// generates new buffers and store their names in an array
	GLErrorSafeCall(glGenBuffers(amountOfBuffersToGen, &m_RendererID)); 
	// selecting our buffer
	this->Bind();
	// define the values inside the buffer
	GLErrorSafeCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLErrorSafeCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const // select this buffer
{
	GLErrorSafeCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); 
}

void VertexBuffer::Unbind() const // unselect any buffer
{
	GLErrorSafeCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); 
}
