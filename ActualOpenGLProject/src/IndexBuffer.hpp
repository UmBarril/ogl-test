#pragma once
#include "IBindable.hpp"

class IndexBuffer : public IBindable {
private:
	unsigned int m_RendererID; // m_ -> member variable
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* indices, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};
