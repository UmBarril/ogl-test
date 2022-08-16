#pragma once
#include <vector>
#include "VertexBufferLayout.hpp"
#include "VertexBuffer.hpp"
#include "IBindable.hpp"

// vertex array object (where we actually save our stuff)
class VertexArray : public IBindable { // our VAO
private:
	unsigned int m_VAObjectID; // m_RendererID
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;
};
