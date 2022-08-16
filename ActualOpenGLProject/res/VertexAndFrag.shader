#shader vertex
#version 460 core

// https://gamedev.stackexchange.com/questions/29672/in-out-keywords-in-glsl
// https://stackoverflow.com/questions/17537879/in-webgl-what-are-the-differences-between-an-attribute-a-uniform-and-a-varying

// # Variable Qualifiers
// ## Qualifiers give a special meaning to the variable. The following qualifiers are available:
//    const – The declaration is of a compile time constant.

//    attribute (in) – Global variables that may change per vertex, that are passed from the OpenGL application to vertex shaders. 
//		This qualifier can only be used in vertex shaders. For the shader this is a read-only variable. See Attribute section.

//    uniform – Global variables that may change per primitive [...], that are passed from the OpenGL application to the shaders.
//		This qualifier can be used in both vertex and fragment shaders. For the shaders this is a read-only variable. See Uniform section.

//	  varying (out) – used for interpolated data between a vertex shader and a fragment shader.
//		Available for writing in the vertex shader, and read-only in a fragment shader. See Varying section.

layout(location = 0) in vec2 position; // get the attibute on postion 0 from the buffer and put it on a variable called position
layout(location = 1) in vec3 vertexColor;

out vec3 v_Color;

void main() 
{
	// vec4 gl_Position 
	gl_Position = vec4(position, 0.0, 1.0);
	v_Color = vertexColor;
}

#shader fragment // 
#version 460 core

out vec4 color; // same as gl_FragColor (depricated)

in vec3 v_Color; // varying from the vertex shader

uniform vec4 u_Color; // anything that starts with u is an Uniform
uniform vec2 screenResolution;

void main() 
{
	//color = vec4((gl_FragCoord.x * 2) / screenResolution.x, (gl_FragCoord.y * 2) / screenResolution.y,1.0,1.0); //RGBA
	//color = u_Color;
	color = vec4(v_Color, 1.0);
}
