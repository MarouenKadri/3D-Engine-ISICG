#version 450  
layout (location = 0) in vec3 Position;  
layout (location=1) in vec3 VertexColor ;
out vec3 color;
uniform float TranslationX ;
uniform float uBrightness;


void main() 
{   

color=uBrightness*VertexColor;
gl_Position = vec4(Position.x+TranslationX, Position.y,Position.z,1.0);


}