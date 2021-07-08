#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glew.h>

#include "cShader.h"

GLuint LoadShaders(const char *vertex_file_path,const char *fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	FILE *filePointer = fopen(vertex_file_path, "r");/* Read the file. */
  char *vertexFileContents;
	int len;
  char c = '\0';
  if (filePointer == NULL)/* If it can't access, or find the file: */
  {
    printf("Error opening file!\n");
    return 0;
  }

	fseek( filePointer , 0L , SEEK_END);
	len = ftell(filePointer);
	rewind(filePointer);
	vertexFileContents = calloc(1, len + 1);
	int curPos = 0;

  while (c != EOF)/* Read the file to the textFile. */
  {
    c = fgetc(filePointer);/* Get a char from the file. */
    vertexFileContents[curPos] = c;/* Add the char to our textFile variable. */
		curPos++;
  }
	vertexFileContents[curPos - 1] = '\0';
  fclose(filePointer);/* Save the computer, close the file. */

	// Read the Fragment Shader code from the file
	GLint Result = GL_FALSE;
	int InfoLogLength;

	filePointer = fopen(fragment_file_path, "r");/* Read the file. */
  char *fragmentFileContents;
  c = '\0';
  if (filePointer == NULL)/* If it can't access, or find the file: */
  {
    printf("Error opening file!\n");
    return 0;
  }

	fseek( filePointer , 0L , SEEK_END);
	len = ftell(filePointer);
	rewind(filePointer);
	fragmentFileContents = calloc(1, len + 1);
	curPos = 0;

  while (c != EOF)/* Read the file to the textFile. */
  {
    c = fgetc(filePointer);/* Get a char from the file. */
    fragmentFileContents[curPos] = c;/* Add the char to our textFile variable. */
		curPos++;
  }
	fragmentFileContents[curPos - 1] = '\0';
  fclose(filePointer);/* Save the computer, close the file. */

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	//char const * VertexSourcePointer = VertexShaderCode.c_str();
	char const * VertexSourcePointer = vertexFileContents;
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		GLchar *VertexShaderErrorMessage;
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	//char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	char const * FragmentSourcePointer = fragmentFileContents;
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		GLchar *FragmentShaderErrorMessage;
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		GLchar *ProgramErrorMessage;
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}