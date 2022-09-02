#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cerrno>
#include <array>

#include "GL.hpp"
#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"

// most of this code was made following the "The Cherno" OpenGl Series
// link to that is here: https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2

std::array<vertex, 4> bufferData{ {
	// positions depend on resoltion beacause they are normalized
	// x = -1 left
	// x = 1 right
	// y = -1 top
	// y = 1 down
	/*   positions  |      colors      */
	{ {-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f} },
	{ { 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f} },
	{ { 0.5f,  0.5f}, {1.0f, 1.0f, 1.0f} },
	{ {-0.5f,  0.5f}, {0.0f, 1.5f, 1.0f} },
}};
std::array<unsigned int, 6> indices { // counter-clockwise
	0, 1, 2, // trA
	2, 3, 0  // trB
};
float red = 0.0f;
float increment = 0.01f;

void draw(Renderer& renderer, Shader& shader) {
	shader.SetUniform4f("u_Color", red, 0.3f, 0.4f, 1.0f);
	red += increment;
	if (red > 1.0f) {
		increment = -0.01f;
	}
	else if (red < 0.0f) {
		increment = 0.01f;
	}
}

void keyPressedEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_W:
		if(action == GLFW_PRESS) {
			/* TODO: do something here */
			std::cout << "it worked!" << '\n';
		}
		break;
	}
}

GLFWwindow* initializeContext(const int window_width, const int window_height, const char* windowname)
{
	// initialize glfw
	if (!glfwInit()) {
		std::cout << "Error when intializing glfw" << '\n';
		return nullptr;
	}

	// forcing the use of the OpenGL Core Profile 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window_width, window_height, windowname, nullptr, nullptr);
	glfwSetKeyCallback(window, keyPressedEvent); // make glfw send key events to my new function

	// specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
	glfwMakeContextCurrent(window);
	glViewport( 0.0f, 0.0f, window_width, window_height ); 
	// projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glMatrixMode( GL_PROJECTION ); 
	// replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glLoadIdentity( ); 
	// essentially set coordinate system
    glOrtho( 0, window_width, 0, window_height, 0, 1 ); 
	// (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glMatrixMode( GL_MODELVIEW ); 
	// same as above comment
    glLoadIdentity( ); 

	// initialize glew only after OpenGl context has become current
	//glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "OpenGL Error: " << glGetError() << '\n';
		glfwTerminate();
		return nullptr;
	}
	return window;
}

class TextureBuffer {
private:
	GLuint textureBufferID;
public:
	unsigned int height;
	unsigned int width;
	uint32_t* data;
};

void textureTest() {
	GLuint textureBufferID;
	const unsigned int width = 200;
	const unsigned int height = 200;
	// uint32_t* imageData = new uint32_t[width * height];   // default
	uint32_t* imageData = new uint32_t[width * height](); // initialize with zero

	GLErrorSafeCall(glGenTextures(1, &textureBufferID));
	GLErrorSafeCall(glBindTexture(GL_TEXTURE_2D, textureBufferID));
	const int mipmapLevel = 0; // zero means no mipmap reduction
	GLErrorSafeCall(glTexImage2D(GL_TEXTURE_2D, mipmapLevel, GL_RGB8, width, height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, imageData));

	/* have no idea of what these next 4 lines do */
	GLErrorSafeCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLErrorSafeCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLErrorSafeCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLErrorSafeCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	delete[] imageData;
}

// remember, OPENGL IS A STATE MACHINE
int main(int argc, char** argv) {
	vec2 resolution{ 640,480 };
	GLFWwindow* window = initializeContext(resolution.x, resolution.y, "Game");
	if (window == nullptr) {
		return -1;
	}
	{ // scope, so all stack alocations are deleted in the end
		VertexArray vao;
		Renderer renderer;

		VertexBuffer vb(bufferData.data(), sizeof(bufferData));

		VertexBufferLayout layout;
		layout.Push<vertex>(1);

		vao.AddBuffer(vb, layout);

		IndexBuffer ib(indices.data(), indices.size());

		Shader shader("res\\VertexAndFrag.shader");
		shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.4f, 1.0f);
		shader.SetUniform2f("screenResolution", resolution.x, resolution.y);

		std::cout << glGetString(GL_VERSION) << '\n';
		while (!glfwWindowShouldClose(window)) {
			/* clearing so we can change the scene right after */
			// (it does nothing now because we literally just have a rainbow square being shown)
			renderer.Clear();

			/* d r a w */
			draw(renderer, shader);

			//renderer.Bind();
			renderer.Draw(vao, ib, shader);

			// sending our screen buffer to the front so it can be drawn
			GLErrorSafeCall(glfwSwapBuffers(window));
			/* process events */
			GLErrorSafeCall(glfwPollEvents());
		}
	}
	glfwTerminate();
	return 0;
}
