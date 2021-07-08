/**
 * cArcade.
 * Project description goes here.
 * 
 * Copyright (C) 2021 Jesse Phillips, Released under the GNU GPL v3.0 or later.
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the 
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the License, 
 * or (at your option) any later version.  This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.  
 * You should have received a copy of the GNU General Public License along with this program.  
 * If not, see <http://www.gnu.org/licenses/>.
**/
/**
 * cArcade.c
 * 
 * @author Jesse Phillips <james@jamesphillipsuk.com>
 **/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "headers/cShader.h"/* Custom shader headers. */

bool initGLFW()
{
  glewExperimental = true; // This is GLEW.  Even though "Experimental" seems weird, it's needed.
  if(!glfwInit())
    return false;

  glfwWindowHint(GLFW_SAMPLES, 4); // 4x "MSAA" antialiasing.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Set the OpenGL version.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  return true;
}

bool buildWindow (GLFWwindow **window)
{
  *window = glfwCreateWindow( 1024, 768, "cArcade", NULL, NULL);
  if (*window == NULL)
  {
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(*window); // Initialize GLEW
  if (glewInit() != GLEW_OK)
    return false;

  glfwSetInputMode(*window, GLFW_STICKY_KEYS, GL_TRUE); // Capture keyboard input.

  return true;
}

bool gameLoop(GLFWwindow **window, GLuint *vertexbuffer, GLuint *programID)
{
  while (!glfwWindowShouldClose(*window))
  {
    glClear( GL_COLOR_BUFFER_BIT ); // Clear the screen.
    glUseProgram(*programID);
    // Draw

    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);
    glVertexAttribPointer(
      0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
    );
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);
    
    glfwSwapBuffers(*window);// Swap buffers
    glfwPollEvents();
  }
  return true;
}

int main(void)
{
  GLFWwindow *window; // Create our window.
  GLuint VertexArrayID; // Vertex Array Object.

  if (!initGLFW())
    return EXIT_FAILURE;
  if (!buildWindow(&window))
    return EXIT_FAILURE;

  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
  GLuint programID = LoadShaders( "vertexShader.glsl", "fragmentShader.glsl" ); // Create and compile GLSL Shaders

  // An array of 3 vectors / vertices: x, y, z
  static const GLfloat g_vertex_buffer_data[] = 
  {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
  };
  GLuint vertexbuffer;// This will identify our vertex buffer
  glGenBuffers(1, &vertexbuffer);// Generate 1 buffer, put the resulting identifier in vertexbuffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);// The following commands will talk about our 'vertexbuffer' buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);// Give our vertices to OpenGL.

  if (!gameLoop(&window, &vertexbuffer, &programID))
    return EXIT_FAILURE;

  glfwDestroyWindow(window);
  glfwTerminate();
  return EXIT_SUCCESS;
}