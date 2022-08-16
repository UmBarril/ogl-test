#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader.hpp"
#include "Renderer.hpp"

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
	std::ifstream stream(filepath);

	enum class  ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) { // npos - notfound
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int shaderID = glCreateShader(type);

	// c_str() returns the char pointer inside std::string source 
	// notice that your source SHOULD be alive all the time, since this ponter is bound to it. 
	// YOU ARE NOT MAKING A COPY
	const char* src = source.c_str();

	// shaderID,
	// amount of shaders we are defining,
	// a pointer to the pointer of your source,
	glShaderSource(shaderID, 1, &src, nullptr);
	glCompileShader(shaderID);

	// error handling
	int result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		// something went wrong....

		// getting the size of the error message
		int errMessageLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &errMessageLength);

		// allocating space on the stack for receiveing the message
		// useful for small things/bad for big stuff
		char* errMessage = (char*)_malloca(errMessageLength * sizeof(char));
		// char message[length] // for some reason this doesn't work, so we use the solution above
		
		glGetShaderInfoLog(shaderID, errMessageLength, &errMessageLength, errMessage);
		
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader :(" << '\n';
		std::cout << errMessage << '\n';
		return 0;
	}

	return shaderID;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
	/* our main program */
	/*Glenum*/ unsigned int programID = glCreateProgram();

	/* vertex shader - will run on every vertex */ 
	/*Glenum*/ unsigned int vertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexShader);
	/* fragment shader - will run on every fragment (pixels) */
	/*Glenum*/ unsigned int fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	
	// attaching our shaders to our main program
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	
	// READ DOCUMENTATION !!! www.docs.gl
	glLinkProgram(programID);
	glValidateProgram(programID);

	// after attaching, we should delete them 
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}

Shader::Shader(const std::string& filepath) 
	: m_FilePath(filepath)
{
	ShaderProgramSource shaders = ParseShader(filepath);
	m_ShaderProgramID = CreateShader(shaders.vertex, shaders.fragment);
	Bind();
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderProgramID);
}

void Shader::Bind() const
{
	GLErrorSafeCall(glUseProgram(m_ShaderProgramID));
}

void Shader::Unbind() const
{
	GLErrorSafeCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float x, float y, float z, float alpha)
{
	GLErrorSafeCall(glUniform4f(GetUniformLocation(name), x, y, z, alpha));
}

void Shader::SetUniform2f(const std::string& name, float x, float y)
{
	GLErrorSafeCall(glUniform2f(GetUniformLocation(name), x, y));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}
	GLErrorSafeCall(int uniformLocation = glGetUniformLocation(m_ShaderProgramID, name.c_str()));
	if (uniformLocation != -1) {
		std::cout << "Warning: unifom '" << name << "' doesn't exist!" << '\n';
	}
	m_UniformLocationCache[name] = uniformLocation;
	return uniformLocation;
}

