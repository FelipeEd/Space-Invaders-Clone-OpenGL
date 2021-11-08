#include "Rectangle.hpp"



Rectangle::Rectangle(float posx, float posy, float h, float w, float c[4], unsigned int intexture)
{

    Shader shaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");

    this->color = c;
    this->position[0] = posx;
    this->position[1] = posy;
    this->height = h;
    this->width = w;
    this->shader = shaderProgram;
    this->texture = intexture;
}

void Rectangle::draw()
{
    // Desenhar o triangulo
    // 1- ativar shader
    // 2- vincular VAO (pode ser feito uma vez fora do loop, caso exista apenas 1 VAO)
    // 3- usar glDrawArrays para desenhar conteudo do VAO
    this->shader.use();
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glBindVertexArray(vao);

    this->shader.setVec2("uPosition", this->position);
    //this->shader.setVec2("uScale", &width, &height);
    this->shader.setVec4("uColor", this->color);
    this->shader.setFloat("uAspectRatio", (float)HEIGHT/WIDTH);

    glDrawArrays(GL_TRIANGLES, 0, 6); // (primitiva, location, numero de vertices)

}
