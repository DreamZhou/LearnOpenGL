#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
//out vec3 outColor;
//uniform float xOffset;
out vec3 ourPosition;
void main()
{
   gl_Position = vec4(aPos,1.0);
   ourPosition = aPos;
}