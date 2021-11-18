#version 330 core

// Location define a posição do atributo no VAO
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;

uniform float uAspectRatio;
uniform vec2 uTranslate;
uniform vec2 uScale;


void main()
{
    // gl_Position é uma das variaveis presentes no vertex shader
    // definirá a posição final do vértice
    // Deve ser um vec4 (X, Y, Z, W)
    gl_Position =  vec4( uAspectRatio * uScale.x * aPos.x +  uTranslate.x , uScale.y * aPos.y  + uTranslate.y, aPos.z, 1.0);
    //gl_Position =  vec4(uScale.x * aPos.x +  uTranslate.x , uScale.y * aPos.y  + uTranslate.y, aPos.z, 1.0);
    TexCoord = aTexCoord;
}
