#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 color;

uniform mat4 fullTransformMatrix;

out vec3 myColor;

void main(){
	vec4 v = vec4(position, 1.0);
	vec4 newPosition = fullTransformMatrix * v;
	gl_Position = newPosition;
	myColor = color;
}