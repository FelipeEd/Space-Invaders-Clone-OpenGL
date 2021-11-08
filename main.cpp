#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// Global
// Tamanho da janela
unsigned int WIDTH = 1280;
unsigned int HEIGHT = 720;

unsigned int vao;
unsigned int vbo;
// Colors
float cyan[4] = {0.0f,1.0f,1.0f,1.0f};
float red[4] = {1.0f,0.0f,0.0f,1.0f};
float magenta[4] = {1.0f,1.0f,0.0f,1.0f};

#include "BuffersAndGlobals.h"
#include "Shader.hpp"
#include "Rectangle.hpp"


int main()
{
    // inicialização e configuração de contexto
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);

    // criação da janela e verificação de erros
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello triangle", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Falha na criacao da janela" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD: Carregamento dos ponteiros de funções para utilizar OpenGL moderno
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Falha na inicializacao do GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    //-------------------------------------------------
    // Buffers e texturas
    
    createBuffers(vao,vbo);
    unsigned int texture1 = createTexture("teste1.jpg");
    unsigned int texture2 = createTexture("teste2.jpg");

    Rectangle rect(0.0f, 0.0f, 1.0f, 1.0f, red, texture1);
    Rectangle rect2(-1.0f, 1.0f, 1.0f, 1.0f, cyan, texture2);
    Rectangle rect3(1.0f, 1.0f, 1.0f, 1.0f, magenta, texture1);

    // Loop da aplicação
    while (!glfwWindowShouldClose(window))
    {
        // Input handling
        processInput(window);

        // Render ----------------------------------------------------------------------
        // Define a cor de fundo da janela
        glClearColor(1.0f, 0.5f, 1.0f, 1.0f);
        // Limpa algum buffer específico
        glClear(GL_COLOR_BUFFER_BIT); // A limpeza do framebuffer de cor será feita com a cor definida em glClearColor
    
        rect.draw();
        rect2.draw();
        rect3.draw();
     
        // Faz a troca do framebuffer antigo para o novo (double buffer)
        glfwSwapBuffers(window);

        //------------------------------------------------------------------------------

        // Captura eventos de IO (movimento de mouse, teclas pressionadas, etc)
        glfwPollEvents();
    }

    // Desalocar memória dos objetos instanciados
    glfwTerminate(); // Faz a limpeza dos recursos utilizados pelo GLFW
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

}

// Função executada toda vez que há um redimensionamento de janela
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   
    glViewport(0, 0, width, height);
    
}

