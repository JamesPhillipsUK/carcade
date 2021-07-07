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

bool gameLoop(GLFWwindow **window)
{
  while (!glfwWindowShouldClose(*window))
  {
    glClear( GL_COLOR_BUFFER_BIT ); // Clear the screen.
    // Draw

    
    glfwSwapBuffers(*window);// Swap buffers
    glfwPollEvents();
  }
  return true;
}

int main(void)
{
  GLFWwindow *window; // Create our window.

  if (!initGLFW())
    return EXIT_FAILURE;
  if (!buildWindow(&window))
    return EXIT_SUCCESS;
  if (!gameLoop(&window))
    return EXIT_FAILURE;
  glfwDestroyWindow(window);
  glfwTerminate();
  return EXIT_SUCCESS;
}