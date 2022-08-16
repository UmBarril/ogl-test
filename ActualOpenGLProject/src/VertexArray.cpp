#include "VertexArray.hpp"
#include "Renderer.hpp"

VertexArray::VertexArray()
{
	GLErrorSafeCall(glGenVertexArrays(1, &m_VAObjectID));
	Bind();
}

VertexArray::~VertexArray()
{
	GLErrorSafeCall(glDeleteVertexArrays(1, &m_VAObjectID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	vb.Bind();

	const auto& elements = layout.GetElements();
	unsigned long long offset = 0;
	for (int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];

		// enable VAO on position i, so we can store stuff on theses places
		GLErrorSafeCall(glEnableVertexAttribArray(i));

		// Linking the current selected buffer to our VAO on the position i.
		GLErrorSafeCall(glVertexAttribPointer(i, element.count, element.type,
			 element.isNormalized, layout.GetStride(), (const void*)offset));

		offset += static_cast<unsigned long long>(element.count) * static_cast<unsigned long long>(VertexBufferElement::GetSizeOfType(element.type));
	}
}

void VertexArray::Bind() const
{
	GLErrorSafeCall(glBindVertexArray(m_VAObjectID));
}

void VertexArray::Unbind() const
{
	GLErrorSafeCall(glBindVertexArray(0)); // unbounding (0 means no buffer at all)
}
