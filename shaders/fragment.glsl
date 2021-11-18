#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

// uniforms são variáveis alteradas no código OpenGL
uniform vec4 uColor;
uniform sampler2D ourTexture;
uniform float uNFrames;
uniform float uCurrentFrame;

void main()
{
    float deltaFrame = (1.0 / uNFrames);
    vec2 frameTexCoord = vec2(TexCoord.x * deltaFrame + uCurrentFrame * deltaFrame, TexCoord.y);

    //if (texture(ourTexture, frameTexCoord).a <=0.5){
    //    discard;
    //}
    FragColor = texture(ourTexture, frameTexCoord) * uColor; //RGBA
} 