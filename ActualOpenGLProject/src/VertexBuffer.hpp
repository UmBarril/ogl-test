#pragma once
#include "IBindable.hpp"

class VertexBuffer : public IBindable {
private:
	// Buffer - Array of memory allocated on the GPU
	unsigned int m_RendererID; // m_ -> member variable
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};
