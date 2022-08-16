#include "VertexBufferLayout.hpp"
#include "Renderer.hpp"


unsigned int VertexBufferElement::GetSizeOfType(GLenum type) {
	switch (type) {
	case GL_FLOAT: return sizeof(GLfloat); //4
	case GL_UNSIGNED_INT: return sizeof(GLuint); //4
	case GL_UNSIGNED_BYTE: return sizeof(GLubyte); //1
	}
	ASSERT(false);
	return 0;
}

template <typename T>
void VertexBufferLayout::Push(unsigned int count) {
	static_assert(false);
}

template<>
void VertexBufferLayout::Push<vertex>(unsigned int count) {
	for (int i = 0; i < count; i++) {
		this->Push<vec2>(1); // position
		this->Push<float>(3); // color (create vec3 Push later)
	}
}

template<>
void VertexBufferLayout::Push<vec2>(unsigned int count)
{
	this->Push<float>(2 * count);
}

template<>
void VertexBufferLayout::Push<float>(unsigned int count)
{
	m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
	m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}

template<>
void VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
	m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
	m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
}

template<>
void VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
	m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_FALSE });
	m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}
