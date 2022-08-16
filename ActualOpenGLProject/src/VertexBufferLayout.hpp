#pragma once
#include <vector>
#include "GL.hpp"
#include "Math.hpp"

struct VertexBufferElement {
	GLenum type;
	unsigned int count;
	GLboolean isNormalized;

	static unsigned int GetSizeOfType(GLenum type);
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride; // in bytes
public:
	VertexBufferLayout()
		: m_Stride(0) { };

	template <typename T>
	void Push(unsigned int count);

	template<>
	void Push<vec2>(unsigned int count);

	template<>
	void Push<vertex>(unsigned int count);

	template<>
	void Push<float>(unsigned int count);

	template<>
	void Push<unsigned int>(unsigned int count);

	template<>
	void Push<unsigned char>(unsigned int count);

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }

	// Stride -> large step
	// - returns the size of this Vertex and therefore
	//	 how much you have to jump to get to the next postion in a Vertex Array
	inline unsigned int GetStride() const { return m_Stride; }
};


