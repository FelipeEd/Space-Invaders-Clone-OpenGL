#include <jellyfish/Jellyfish.hpp>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, bool &mouseLeftIsPressed, bool &mouseRightIsPressed, bool &mouseMiddleIsPressed, bool &rIsPressed);

// Global
// Tamanho da janela
unsigned int WIDTH = 1920;  //1280;
unsigned int HEIGHT = 1080; //720;

unsigned int vao;
unsigned int vbo;
Shader shaderProgram;

// Colors
float black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float white[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float blue[4] = {0.0f, 1.0f, 1.0f, 1.0f};
float red[4] = {1.0f, 0.0f, 0.0f, 1.0f};
float magenta[4] = {1.0f, 1.0f, 0.0f, 1.0f};
float green[4] = {0.0f, 1.0f, 0.0f, 1.0f};

// Texturas
unsigned int texturePlayer;
unsigned int textureBullet1;
unsigned int textureBullet2;
unsigned int textureBullet3;
unsigned int textureBullet4;
unsigned int textureBullet5;

unsigned int textureHitBox;

int Score = 0;
int Record = 0;

char laser1[30000];

int main()
{
    // inicialização e configuração de contexto
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // criação da janela e verificação de erros
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Hello triangle", NULL, NULL);
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
    glfwSwapInterval(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //-------------------------------------------------
    // Buffers e texturas

    createBuffers(vao, vbo, shaderProgram);

    texturePlayer = createTexture("bin/assets/Nave2SpriteSheet.png");

    textureBullet1 = createTexture("bin/assets/Bullet1.png");
    textureBullet2 = createTexture("bin/assets/Bullet2.png");
    textureBullet3 = createTexture("bin/assets/Bullet3.png");
    textureBullet4 = createTexture("bin/assets/Bullet4.png");
    textureBullet5 = createTexture("bin/assets/Bullet5.png");

    textureHitBox = createTexture("bin/assets/Hitbox.png");

    unsigned int textureAlien1 = createTexture("bin/assets/Alien1.png");
    unsigned int textureAlien2 = createTexture("bin/assets/Alien2.png");
    unsigned int textureAlien3 = createTexture("bin/assets/Alien3.png");
    unsigned int textureAlien4 = createTexture("bin/assets/Alien4.png");
    unsigned int textureBoss = createTexture("bin/assets/Boss.png");

    unsigned int textureBackground = createTexture("bin/assets/Background.png");
    unsigned int textureLose = createTexture("bin/assets/lose.png");
    unsigned int textureWin = createTexture("bin/assets/win.png");
    unsigned int textureTitle = createTexture("bin/assets/title.png");
    unsigned int textureNumbers = createTexture("bin/assets/numbers.png");

    // Audio
    FILE *f1;
    f1 = fopen("bin\\assets\\laser1.wav", "r");
    fread(&laser1, sizeof(char), 30000, f1);
    fclose(f1);

    mciSendString("open \"bin\\assets\\SpaceTheme.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
    mciSendString("play mp3 repeat", NULL, 0, NULL);

    Sprite numbers(0.05f, 0.05f, textureNumbers, 10);
    Sprite title(2.0f * WIDTH / HEIGHT, 2.0f, textureTitle, 1);

    while (!glfwWindowShouldClose(window))
    {

        Player player = Player();

        //fase 1
        Alien alien3_1(-1.0f, 0.80f, textureAlien3, 1, 3);
        Alien alien4_1(-1.0f, 0.35f, textureAlien4, 1, 4);

        AlienSquad wave5(alien3_1, 0.0f, 0.0f, 3, 8, 30);
        AlienSquad wave6(alien4_1, 0.0f, 0.0f, 1, 8, 20);

        // fase 2
        Alien alien1_2(-0.7f, 0.8f, textureAlien1, 1, 1);
        Alien alien2_2(-1.0f, 0.65f, textureAlien2, 1, 2);
        Alien alien3_2(-1.0f, 0.50f, textureAlien3, 1, 3);
        Alien alien4_2(-1.0f, 0.05f, textureAlien4, 1, 4);

        AlienSquad wave1(alien1_2, 0.0f, 0.0f, 1, 4, 150);
        AlienSquad wave2(alien2_2, 0.0f, 0.0f, 1, 8, 60);
        AlienSquad wave3(alien3_2, 0.0f, 0.0f, 3, 8, 30);
        AlienSquad wave4(alien4_2, 0.0f, 0.0f, 1, 8, 20);

        // fase 3
        Alien alienBoss(-1.0f, 0.60f, textureBoss, 1, 5);
        AlienSquad boss(alienBoss, 0.0f, 0.0f, 1, 1, 20);

        Entity background(0, 0, 2.0f * WIDTH / HEIGHT, 2.0f, textureBackground, 1);
        //Entity test(0.5f, 0, texturePlayer, 3);

        double prevTime = 0.0;
        double crntTime = 0.0;
        double timeDiff;
        unsigned int counter = 0;

        bool gameIsPaused = false;
        bool gameInTitle = true;
        bool mouseRightIsPressed = false;
        bool mouseMiddleIsPressed = false;
        bool mouseLeftIsPressed = false;
        bool rIsPressed = false;

        bool Reset = false;
        bool Won = false;
        int pauseCooldown = 60;
        Score = 0;
        Record = ReadRecord();
        Point start = {-0.95f, 0.95f};
        int fase = 1;

        //Title screen
        while (gameInTitle && !glfwWindowShouldClose(window))
        {
            processInput(window, mouseLeftIsPressed, mouseRightIsPressed, mouseMiddleIsPressed, rIsPressed);

            title.draw({0.0f, 0.0f});
            numbers.setColor(red);
            DisplayScore(numbers, Record, start);
            glfwSwapBuffers(window);

            if (pauseCooldown == 0 && mouseLeftIsPressed)
            {
                gameInTitle = false;
                pauseCooldown = 20;
            }
            if (pauseCooldown > 0)
                pauseCooldown--;
            glfwPollEvents();
        }

        // Loop da aplicação
        while ((!Reset && player.ship.active) && !glfwWindowShouldClose(window))
        {
            Reset = false;
            // Input handling
            processInput(window, mouseLeftIsPressed, mouseRightIsPressed, mouseMiddleIsPressed, rIsPressed);

            crntTime = glfwGetTime();
            timeDiff = crntTime - prevTime;
            counter++;

            // Coisas dependentes de tempo roda uma vez por frame
            if (timeDiff >= 1.0 / 120.0)
            {

                std::string FPS = std::to_string((1.0 / timeDiff) * counter);
                std::string ms = std::to_string((timeDiff / counter) * 1000);
                std::string newTitle = "Space Invaders - " + FPS + "FPS /" + ms + "ms";
                glfwSetWindowTitle(window, newTitle.c_str());
                prevTime = crntTime;
                counter = 0;

                if (mouseRightIsPressed && pauseCooldown == 0)
                {
                    gameIsPaused = !gameIsPaused;
                    pauseCooldown = 20;
                }

                if (rIsPressed && pauseCooldown == 0)
                {
                    Reset = true;
                    pauseCooldown = 20;
                }

                if (pauseCooldown > 0)
                    pauseCooldown--;

                mouseRightIsPressed = false;

                player.keyUpdate(window);
            }

            // Real loop ------------------------------------------------------------------------------------------

            // Define a cor de fundo da janela
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            // Limpa algum buffer específico
            glClear(GL_COLOR_BUFFER_BIT);

            background.draw();
            if (fase == 3)
            {
                boss.update(player);

                player.gun[0].interact(boss);
                player.gun[1].interact(boss);
                player.gun[2].interact(boss);

                player.update();

                if (!boss.Alive)
                {
                    Won = true;
                    break;
                }

                boss.draw();
            }
            if (fase == 2)
            {
                wave1.update(player);
                wave2.update(player);
                wave3.update(player);
                wave4.update(player);

                player.gun[0].interact(wave1);
                player.gun[1].interact(wave1);
                player.gun[2].interact(wave1);

                player.gun[0].interact(wave2);
                player.gun[1].interact(wave2);
                player.gun[2].interact(wave2);

                player.gun[0].interact(wave3);
                player.gun[1].interact(wave3);
                player.gun[2].interact(wave3);

                player.gun[0].interact(wave4);
                player.gun[1].interact(wave4);
                player.gun[2].interact(wave4);

                player.update();

                if (!wave1.Alive && !wave2.Alive && !wave3.Alive && !wave4.Alive)
                {
                    fase = 3;
                }

                wave1.draw();
                wave2.draw();
                wave3.draw();
                wave4.draw();
            }

            if (fase == 1)
            {
                wave5.update(player);
                wave6.update(player);

                player.gun[0].interact(wave5);
                player.gun[1].interact(wave5);
                player.gun[2].interact(wave5);

                player.gun[0].interact(wave6);
                player.gun[1].interact(wave6);
                player.gun[2].interact(wave6);

                player.update();

                if (!wave5.Alive && !wave6.Alive)
                {
                    fase = 2;
                }

                wave5.draw();
                wave6.draw();
            }

            player.draw();
            numbers.setColor(white);
            DisplayScore(numbers, Score, start);
            numbers.setColor(magenta);
            DisplayScore(numbers, player.currentGun, {0.85, 0.95});

            // Main do jogo Pausado
            if (gameIsPaused)
            {
                player.ship.hitbox.draw();

                player.gun[0].drawBulletsHitBoxes();
                player.gun[1].drawBulletsHitBoxes();
                player.gun[2].drawBulletsHitBoxes();

                if (fase == 3)
                {
                    boss.drawHitbox();
                }
                if (fase == 2)
                {
                    wave1.drawHitbox();
                    wave2.drawHitbox();
                    wave3.drawHitbox();
                    wave4.drawHitbox();
                }
                if (fase == 1)
                {
                    wave5.drawHitbox();
                    wave6.drawHitbox();
                }
            }
            // Faz a troca do framebuffer antigo para o novo (double buffer)
            glfwSwapBuffers(window);

            // Render ---------------------------------------------------------------------------------------------

            while (gameIsPaused && !glfwWindowShouldClose(window))
            {
                mouseMiddleIsPressed = false;
                processInput(window, mouseLeftIsPressed, mouseRightIsPressed, mouseMiddleIsPressed, rIsPressed);

                crntTime = glfwGetTime();
                timeDiff = crntTime - prevTime;
                counter++;
                if (timeDiff >= 1.0 / 120.0)
                {
                    std::string FPS = std::to_string((1.0 / timeDiff) * counter);
                    std::string ms = std::to_string((timeDiff / counter) * 1000);
                    std::string newTitle = "Space Invaders - " + FPS + "FPS /" + ms + "ms";
                    glfwSetWindowTitle(window, newTitle.c_str());
                    prevTime = crntTime;
                    counter = 0;

                    if (mouseRightIsPressed && pauseCooldown == 0)
                    {
                        gameIsPaused = !gameIsPaused;
                        pauseCooldown = 20;
                    }
                    if (mouseMiddleIsPressed && pauseCooldown == 0)
                    {
                        player.debug();
                        if (fase == 3)
                        {
                            boss.debug();
                        }
                        if (fase == 2)
                        {
                            wave1.debug();
                            wave2.debug();
                            wave3.debug();
                            wave4.debug();
                        }
                        if (fase == 1)
                        {
                            wave5.debug();
                            wave6.debug();
                        }
                        gameIsPaused = false;
                        pauseCooldown = 20;
                    }
                    if (pauseCooldown > 0)
                        pauseCooldown--;

                    mouseRightIsPressed = false;

                    glfwPollEvents();
                }
            }
            if (mouseMiddleIsPressed)
                gameIsPaused = true;

            //------------------------------------------------------------------------------

            // Captura eventos de IO (movimento de mouse, teclas pressionadas, etc)
            glfwPollEvents();
        }

        if (Won)
        {

            Sprite win(2.0f * WIDTH / HEIGHT, 2.0f, textureWin, 1);
            win.draw({0.0f, 0.0f});
            glfwSwapBuffers(window);

            print("Voce Ganhou !!! ");
            std::cout << "Score " << Score << std::endl;
            sleep_until(system_clock::now() + seconds(4));
        }
        else if (!Won && !glfwWindowShouldClose(window))
        {

            Sprite lose(2.0f * WIDTH / HEIGHT, 2.0f, textureLose, 1);
            lose.draw({0.0f, 0.0f});
            glfwSwapBuffers(window);

            print("Voce Perdeu :( ");
            std::cout << "Score " << Score << std::endl;
            sleep_until(system_clock::now() + seconds(4));
        }

        SaveScore();
    }
    // Desalocar memória dos objetos instanciados
    glfwTerminate(); // Faz a limpeza dos recursos utilizados pelo GLFW
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window, bool &mouseLeftIsPressed, bool &mouseRightIsPressed, bool &mouseMiddleIsPressed, bool &rIsPressed)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        mouseRightIsPressed = true;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    {
        mouseMiddleIsPressed = true;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        mouseLeftIsPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        print("R pressed");
        rIsPressed = true;
    }
}

// Função executada toda vez que há um redimensionamento de janela
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{

    glViewport(0, 0, width, height);
}
