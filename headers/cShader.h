/**
 * cShader.h
 * Load, compile, and check GLSL shaders, written in pure C.  No complex maths or C++ dependencies needed.
 * Inspired by: https://github.com/opengl-tutorials/ogl/blob/master/common/shader.hpp
 * 
 * @author Jesse Phillips <james@jamesphillipsuk.com>
 **/
#ifndef SHADER_H
#define SHADER_H
/**
 * loadShaders, loads a set of GLSL shaders, given their filepaths.
 * 
 * @param vertexFile - the filepath of the vertex shader.
 * @param fragmentFile - the filepath of the fragment shader.
 * @return GLuint the program ID these shaders are compiled and linked to.
 **/
GLuint loadShaders(const char *vertexFile, const char *fragmentFile);

#endif