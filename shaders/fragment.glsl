#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

// uniforms são variáveis alteradas no código OpenGL
uniform vec4 uColor;
uniform sampler2D ourTexture;

void main()
{
    
    FragColor = texture(ourTexture, TexCoord) * uColor; //RGBA
} 