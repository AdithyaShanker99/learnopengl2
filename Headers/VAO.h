#ifndef VAO_Class_H
#define VAO_Class_H

#include <glad.h>
#include "VBO.h"

class VAO
{
    public:
         GLuint ID;
         VAO();

         void LinkAttrib(VBO &VBO, GLuint layout, GLuint numcomponents, GLenum type, GLsizeiptr stride, void* offset);

         void Bind();
         void Unbind();
         void Delete();

};
#endif