#pragma once
#include <string>
#include <unordered_map>

#include "IBindable.hpp"

struct ShaderProgramSource {
	std::string vertex;
	std::string fragment;
};

class Shader : public IBindable
{
private:
	unsigned int m_ShaderProgramID;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, float x, float y, float z, float alpha);
	void SetUniform2f(const std::string& name, float x, float y);
private:
	int GetUniformLocation(const std::string& name);

	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};

