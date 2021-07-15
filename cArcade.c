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
#include <cglm/cglm.h>
#include "headers/cShader.h"/* Custom shader headers. */

#define WINDOWWIDTH 1024
#define WINDOWHEIGHT 768

/**
 * initGLFW initialises GLFW with the setup we need.
 * 
 * @return boolean - did it work?
 **/
bool initGLFW()
{
  glewExperimental = true; /* This is GLEW.  Even though "Experimental" seems weird, it's needed. */
  if(!glfwInit())
    return false;

  glfwWindowHint(GLFW_SAMPLES, 4); /* 4x "MSAA" antialiasing. */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); /* Set the OpenGL version. 4.6.0 here.*/
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  return true;
}

/**
 * buildWindow builds the window we've initialised.
 * 
 * @param window - a pointer to the window.
 * @return boolean - did it work?
 **/
bool buildWindow (GLFWwindow **window)
{
  *window = glfwCreateWindow(WINDOWWIDTH, WINDOWHEIGHT, "cArcade", NULL, NULL);
  if (*window == NULL)
  {
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(*window); /* Initialize GLEW */
  if (glewInit() != GLEW_OK)
    return false;

  glfwSetInputMode(*window, GLFW_STICKY_KEYS, GL_TRUE); /* Capture keyboard input. */
  return true;
}

/**
 * gameLoop - The runtime of the game.  This generates and pushes every frame.
 * 
 * @param window - the window the game is in.
 * @param vertexBuffer - the vertices of the shapes in the game.
 * @param programID - the program ID which links graphics and shaders to each other and this program.
 * @return boolean - did it work?
 **/
bool gameLoop(GLFWwindow **window, GLuint *vertexBuffer, GLuint *programID, mat4 *mvp, GLuint *matrixID)
{
  double startTime = glfwGetTime(); /* Start the clock for the FPS Counter */
  int framePerSecondCount = 0; /* Start the FPS counter at 0. */

  /* BEGIN THE LOOP */
  while (!glfwWindowShouldClose(*window))
  {
    glClear( GL_COLOR_BUFFER_BIT ); /* Clear the screen. */
    glUseProgram(*programID);

    glUniformMatrix4fv(*matrixID, 1, GL_FALSE, mvp[0][0]);

    /* THIS IS WHERE WE HANDLE THE GRAPHICS. */
    /* Task: draw a triangle */
    /* Step 1: create the data structure needed to define the triangle. */
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, *vertexBuffer);
    glVertexAttribPointer(
      0,                  /* Index of the first vertec to be modified. */
      3,                  /* Size - number of components per vertex attribute. */
      GL_FLOAT,           /* Data type of each component.  We'll use floats. */
      GL_FALSE,           /* Do we need to normalise the values, or treat them as fixed points? */
      0,                  /* Stride - the byte offset between attributes.  We're packing it in as tight as possible with 0. */
      (void*)0            /* Pointer - specifies the array buffer offset. */
    );
    /* Step 2: draw the data above. */
    glDrawArrays(GL_TRIANGLES, 0, 3); /*Draw triangles, starting at the first point (0), working to the third (3).  One complete triangle. */
    glDisableVertexAttribArray(0);
    
    /* Step 3: Prepare the next frame of the loop. */
    glfwSwapBuffers(*window);
    glfwPollEvents();

    /* FPS Counter. */
     double currentTime = glfwGetTime(); /* Get current time. */
     framePerSecondCount++;
     if ( currentTime - startTime >= 1.0 ) /* Has one second passed? */
     {
        printf("%f fps\n", (double)framePerSecondCount);/* Return current framerate, then reset the data. */
        framePerSecondCount = 0;
        startTime += 1.0;
     }
    /* END THE LOOP. */
  }
  return true;
}

/**
 * The main body of the program.  Calls all other functions.
 * 
 * @param void - any console arguments are cast into the void.  We're not using them at the moment.
 * @return EXIT_FAILURE or EXIT_SUCCESS (see stdlib.h).
 **/
int main(void)
{
  GLFWwindow *window; /* We can't work without a window. */
  GLuint vertexArrayID; /* This will store the ID of our vertex array (for our shape). */

  if (!initGLFW())
    return EXIT_FAILURE;
  if (!buildWindow(&window))
    return EXIT_FAILURE;

  glGenVertexArrays(1, &vertexArrayID); /* Generate the vertex array. */
  glBindVertexArray(vertexArrayID);
  GLuint programID = loadShaders( "vertexShader.glsl", "fragmentShader.glsl" ); /* Build our shaders from our GLSL files. */

  /* 45° Field of View, Projection matrix with a display range of 0.1 to 100 units */
  mat4 projection;
  glm_perspective(glm_rad(45.0f), WINDOWWIDTH / WINDOWHEIGHT, 0.1f, 100.0f, projection);

  /* Camera matrix */
  mat4 view;
  glm_mat4_identity(view);
  glm_lookat((vec3){4,3,3}, (vec3){0,0,0}, (vec3){0,1,0}, view);

  /* Model matrix */
  mat4 model;// glm_mat4(1.0f);
  glm_mat4_identity(model);

  /* Model-View-Projection */
  mat4 mvp;// = projection * view * model;
  glm_mat4_mul(projection, view, mvp);
  glm_mat4_mul(mvp, model, mvp);

  GLuint matrixID = glGetUniformLocation(programID, "MVP");

  /* This holds 3, 3D vertices: x, y, and z. */
  static const GLfloat vertexBufferData[] = 
  {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
  };
  GLuint vertexBuffer;
  glGenBuffers(1, &vertexBuffer); /* Generate a buffer for vertexBuffer. */
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW); /* Give our vertices to OpenGL. */

  if (!gameLoop(&window, &vertexBuffer, &programID, &mvp, &matrixID))
    return EXIT_FAILURE;

  glfwDestroyWindow(window);
  glfwTerminate();
  return EXIT_SUCCESS;
}