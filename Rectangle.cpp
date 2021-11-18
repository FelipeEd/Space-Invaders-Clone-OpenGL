#include "Rectangle.hpp"


Rectangle::Rectangle(){}


Rectangle::Rectangle(float h, float w, unsigned int intexture, int nframes)
{

    Shader shaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");

    this->scale[0] = h*1.0f/4.0f;
    this->scale[1] = w*1.0f/4.0f;

    this->nFrames = nframes;
    this->shader = shaderProgram;
    this->texture = intexture;

}


void Rectangle::draw(float position[2])
{
    // Desenhar o triangulo
    // 1- ativar shader
    // 2- vincular VAO (pode ser feito uma vez fora do loop, caso exista apenas 1 VAO)
    // 3- usar glDrawArrays para desenhar conteudo do VAO
    this->shader.use();
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glBindVertexArray(vao);

    this->shader.setVec2("uTranslate", position);
    this->shader.setVec2("uScale", this->scale);
    this->shader.setVec4("uColor", this->color);
    this->shader.setFloat("uAspectRatio", (float)HEIGHT/WIDTH);

    this->shader.setFloat("uNFrames", this->nFrames);
    this->shader.setFloat("uCurrentFrame", this->currentFrame);

    glDrawArrays(GL_TRIANGLES, 0, 6); // (primitiva, location, numero de vertices)

}


void Rectangle::setCurrentFrame(int n)
{
    this->currentFrame = n;
}


void Rectangle::setColor(float newcolor[4])
{
    this->color[0] = newcolor[0];
    this->color[1] = newcolor[1];
    this->color[2] = newcolor[2];
    this->color[3] = newcolor[3];
}

