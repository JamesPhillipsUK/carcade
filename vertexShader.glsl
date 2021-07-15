#version 460 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
uniform mat4 MVP;

void main()
{
//  gl_Position.xyz = vertexPosition_modelspace;
//  gl_Position.w = 1.0;
  gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
}