#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;
uniform mat4 uMVP;
out vec2 TexCoord;
void main() { 
    gl_Position = uMVP * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
}