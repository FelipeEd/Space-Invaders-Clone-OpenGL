#include <jellyfish/Jellyfish.hpp>

// Retorna o int da textura que foi alocada
unsigned int createTexture(const char *textureName)
{
    unsigned int texture;
    // Textura-----------------------------------------------------------------
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    int tex_width, tex_height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(textureName, &tex_width, &tex_height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
    //-------------------------------------------------------------------------
}

// Cria o VAO e VBO Unicos
void createBuffers(unsigned int &vao, unsigned int &vbo)
{

    // Centrado na origem
    float vertexData[] = {
        //   Posições           //  texture coorinates
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // left bottom
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // right bottom
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // top right

        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,  // top right
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // top left
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f // left bottom
    };

    /*
    // Centrado no canto esquerdo superior
    float vertexData[] = {
    //   Posições           //  texture coorinates
         0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // left bottom 
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,// right bottom
         1.0f,  0.0f, 0.0f, 1.0f, 1.0f,// top right

         1.0f,  0.0f, 0.0f, 1.0f, 1.0f,// top right
         0.0f,  0.0f, 0.0f, 0.0f, 1.0f,// left top
         0.0f, -1.0f, 0.0f, 0.0f, 0.0f// left bottom
    };
    */

    // Buffers-----------------------------------------------------------------
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    // 1- bind the Vertex Array Object
    // 2- bind and set vertex buffer(s)
    // 3- configure vertex attributes(s)
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // (location, size, GL_datatype, normalize, stride, pointer_to_start)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //-------------------------------------------------
}

// Transforma uma coordenadas em pixel para coordenada no NDC
float pixelXToNDC(int x)
{
    //de [0,width] para [-1,1]
    // e de [0,height] para [1,-1]
    return (2.0f * x - (float)WIDTH) / (float)WIDTH;
}

float NDCXToPixel(float x)
{
    return (x + 1) / 2.0 * WIDTH;
}

float pixelYToNDC(int y)
{
    return ((float)HEIGHT - 2.0f * y) / (float)HEIGHT;
}

void debug(Player player)
{
    player.debug();
}