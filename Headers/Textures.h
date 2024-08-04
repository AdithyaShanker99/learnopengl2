#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad.h>
#include<stb_image.h>

#include "shaderClass.h"

class Texture
{
public:
	// Reference ID of the Shader Program
	GLuint ID;
    const char* type;
	GLuint unit;

	Texture(const char* filepath, const char* textType, GLenum slot, GLenum format, GLenum pixelType);
	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};


#endif