#version 430

in vec3 myColor;
out vec4 theColor;

void main(){
	theColor = vec4(myColor, 1);
}
