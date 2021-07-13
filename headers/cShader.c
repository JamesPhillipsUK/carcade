/**
 * cShader.c
 * Load, compile, and check GLSL shaders, written in pure C.  No complex maths or C++ dependencies needed.
 * Inspired by: https://github.com/opengl-tutorials/ogl/blob/master/common/shader.cpp
 * 
 * @author Jesse Phillips <james@jamesphillipsuk.com>
 **/
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "cShader.h"

/**
 * loadShaders, loads a set of GLSL shaders, given their filepaths.
 * 
 * @param vertexFile - the filepath of the vertex shader.
 * @param fragmentFile - the filepath of the fragment shader.
 * @return GLuint the program ID these shaders are compiled and linked to.
 **/
GLuint loadShaders(const char *vertexFile,const char *fragmentFile)
{
  /* Create the shaders and store their IDs. */
  GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  /* STEP 1: READ THE SHADERS */
  /* READ IN THE VERTEX SHADER */
  FILE *filePointer = fopen(vertexFile, "r"); /* Read the file. */
  char *vertexFileContents; /* This will store the file contents */
  int len; /* This holds the file length, needed to calloc() the heap memory for vertexFileContents. */
  char c = '\0';/* The current character.  Initialised to null. */
  if (filePointer == NULL) /* If it can't access, or find the file: */
  {
    printf("Error opening file!\n");
    return 0;
  }
  /* fseek() through the file to find its length. */
  fseek( filePointer , 0L , SEEK_END);
  len = ftell(filePointer);
  rewind(filePointer); /* Return to the start to begin reading it. */
  vertexFileContents = calloc(1, len + 1); /* Pwease can I have some heap memory? uwu */
  int curPos = 0;

  while (c != EOF)
  {
    c = fgetc(filePointer); /* Get a char from the file. */
    vertexFileContents[curPos] = c; /* Add the char. */
    curPos++;
  }
  vertexFileContents[curPos - 1] = '\0';
  fclose(filePointer); /* Save the computer, close the file. */

  /* READ IN THE FRAGMENT SHADER */
  filePointer = fopen(fragmentFile, "r"); /* Reset filePointer for the new file.  Don't let a good variable go to waste. */
  char *fragmentFileContents; /* This will store the file contents */
  c = '\0'; /* Reset c */
  if (filePointer == NULL) /* If it can't access, or find the file: */
  {
    printf("Error opening file!\n");
    return 0;
  }
  /* fseek() through the file to find its length. */
  fseek( filePointer , 0L , SEEK_END);
  len = ftell(filePointer);
  rewind(filePointer); /* Return to the start to begin reading it. */
  fragmentFileContents = calloc(1, len + 1); /* calloc() the memory needed for the file. */
  curPos = 0;

  while (c != EOF)
  {
    c = fgetc(filePointer);/* Get a char from the file. */
    fragmentFileContents[curPos] = c;/* Add the char. */
    curPos++;
  }
  fragmentFileContents[curPos - 1] = '\0';
  fclose(filePointer);/* Save the computer, close the file. */

  /* STEP 2: COMPILE THE SHADERS */
  /* COMPILE THE VERTEX SHADER */
  /*printf("Compiling vertex shader : %s\n", vertexFile);*/ /* DEBUG INFO */
  char const *vertexSourcePointer = vertexFileContents; /* glShaderSource() only wants char const *s, not char*s.  Don't ask why. */
  glShaderSource(vertexShaderID, 1, &vertexSourcePointer , NULL); /* Give GLEW the shaders. */
  glCompileShader(vertexShaderID); /* ANd compile them. */

  /* COMPILE THE FRAGMENT SHADER */
  /*printf("Compiling fragment shader : %s\n", fragmentFile);*/ /* DEBUG INFO */
  char const *fragmentSourcePointer = fragmentFileContents; /* As above, but with fragments, not vertices. */
  glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer , NULL);
  glCompileShader(fragmentShaderID);

  /* STEP 3: CHECK THE SHADER VALIDITY */
  GLint result = GL_FALSE;
  int infoLogLength;
  /* CHECK THE VERTEX SHADER */
  glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result); /* What's the compilation status?  Did it work? */
  glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength); /* Is there an error message? */
  if ( infoLogLength > 0 )
  {
    GLchar *vertexShaderErrorMessage;
    glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
    printf("%s\n", &vertexShaderErrorMessage[0]); /* If there's an error, print it to console. */
  }

  /* CHECK THE FRAGMENT SHADER */
  glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result); /* As above, but with fragments, not vertices. */
  glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
  if ( infoLogLength > 0 )
  {
    GLchar *fragmentShaderErrorMessage;
    glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
    printf("%s\n", &fragmentShaderErrorMessage[0]);
  }

  /* STEP 4: LINK THE PROGRAM TO THE SHADERS */
  /* LINK */
  GLuint programID = glCreateProgram(); /* Make the program and save its ID. */
  glAttachShader(programID, vertexShaderID); /* Attach the shaders, and link the two together. */
  glAttachShader(programID, fragmentShaderID);
  glLinkProgram(programID);

  /* CHECK */
  glGetProgramiv(programID, GL_LINK_STATUS, &result); /* What's the linking status?  Did it work? */
  glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength); /* Is there an error message? */
  if ( infoLogLength > 0 )
  {
    GLchar *programErrorMessage;
    glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
    printf("%s\n", &programErrorMessage[0]); /* If there's an error, print it to console. */
  }
  /* STEP 5: CLEANUP */
  glDetachShader(programID, vertexShaderID);
  glDetachShader(programID, fragmentShaderID);
  glDeleteShader(vertexShaderID);
  glDeleteShader(fragmentShaderID);
  free(vertexFileContents);
  free(fragmentFileContents);
  return programID;
}