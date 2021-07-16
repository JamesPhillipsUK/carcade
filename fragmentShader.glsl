#version 460 core

in vec3 fragmentColor;

// Ouput data
out vec3 color;

void main()
{
  //color = vec3(1,0,0);

  color = fragmentColor;
}