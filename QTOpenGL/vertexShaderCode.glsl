#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 color;

out vec3 myColor;

void main(){
	gl_Position = vec4(position, 1.0);
	myColor = color;
}