#version 330 core

layout(location = 0) in vec3 aPos; // Vertex position
layout(location = 1) in vec3 aNormal; // Vertex normal
layout(location = 2) in vec3 aColor; // Vertex color

out vec3 crntPos;
out vec3 Normal;
out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    crntPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    color = aColor;
    
    gl_Position = projection * view * vec4(crntPos, 1.0);
}
