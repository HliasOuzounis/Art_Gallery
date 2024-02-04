#include <chrono>

// Include C++ headers
#include <iostream>
#include <string>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Shader loading utilities and other
#include <common/camera.h>
#include <common/model.h>
#include <common/shader.h>
#include <common/texture.h>
#include <common/util.h>

#include "constants.h"

// extra helper functions
#include "components/rooms/room.h"
#include "components/rooms/main_room.h"
#include "components/rooms/secondary_room.h"
#include "components/paintings/paintings.h"
#include "components/player/player.h"
#include "components/light/light.h"

#include "post_processing/floyd_steinberg.h"
#include "post_processing/painterly.h"

#include "FBOs/FBO.h"
#include "FBOs/sceneFBO/sceneFBO.h"
#include "FBOs/depthFBO/depthFBO.h"
#include "FBOs/paintingsFBO/paintingsFBO.h"
#include "FBOs/renderFBO/renderFBO.h"

#include "rendering/main_loop.h"

using namespace std;
using namespace glm;

// Function prototypes
void initialize();
void createContext();
void mainLoop();
void free();

// Global variables
GLFWwindow *window;
Camera *camera;

SceneFBO *sceneFBO;
DepthFBO *depthFBO;

void displayScene(FBO *fbo, GLuint texture);
Drawable *quad;
GLuint postProcessingProgram[PAINTINGS + 1], quadTextureSamplerLocation[PAINTINGS + 1];
RenderFBO *renderFBO;

void createPaintingTextures();
GLuint paintingTexturesFBO;

Room *currentRoom;
Room *rooms[PAINTINGS + 1];
MainRoom *mainRoom;
vector<Painting *> paintings;
Player *player = new Player();

GLuint testTexture;

enum GameState
{
    MAINROOM,
    ROOM1,
    ROOM2,
    ROOM3,
    ROOM4,
    ROOM5,
    ROOM6,
};
GameState gameState = MAINROOM;

void change_state();
void change_room();


void createFinalScene()
{
    GLuint defaultShader = loadShaders("src/shaders/image_processing/vertex.glsl",
                                       "src/shaders/image_processing/default.frag.glsl");

    postProcessingProgram[MAINROOM] = defaultShader;

    postProcessingProgram[ROOM1] = defaultShader;

    postProcessingProgram[ROOM2] = defaultShader;

    postProcessingProgram[ROOM3] = defaultShader;

    postProcessingProgram[ROOM4] = loadShaders("src/shaders/image_processing/vertex.glsl",
                                               "src/shaders/image_processing/fish_eye.frag.glsl");
    postProcessingProgram[ROOM5] = loadShaders("src/shaders/image_processing/vertex.glsl",
                                               "src/shaders/image_processing/toon.frag.glsl");
    postProcessingProgram[ROOM6] = loadShaders("src/shaders/image_processing/vertex.glsl",
                                               "src/shaders/image_processing/chromatic_aberration.frag.glsl");


    vector<vec3> quadVertices = {
        vec3(-1.0f, 1.0f, 0.0f),  // top left
        vec3(-1.0f, -1.0f, 0.0f), // bottom left
        vec3(1.0f, -1.0f, 0.0f),  // bottom right

        vec3(-1.0f, 1.0f, 0.0f), // top left
        vec3(1.0f, -1.0f, 0.0f), // bottom right
        vec3(1.0f, 1.0f, 0.0f)   // top right
    };
    vector<vec2> quadUVs = {
        vec2(0.0f, 1.0f), // top left
        vec2(0.0f, 0.0f), // bottom left
        vec2(1.0f, 0.0f), // bottom right

        vec2(0.0f, 1.0f), // top left
        vec2(1.0f, 0.0f), // bottom right
        vec2(1.0f, 1.0f)  // top right
    };
    vector<vec3> quadNormals = {
        vec3(0.0f, 0.0f, 1.0f), // top left
        vec3(0.0f, 0.0f, 1.0f), // bottom left
        vec3(0.0f, 0.0f, 1.0f), // bottom right

        vec3(0.0f, 0.0f, 1.0f), // top left
        vec3(0.0f, 0.0f, 1.0f), // bottom right
        vec3(0.0f, 0.0f, 1.0f)  // top right
    };
    quad = new Drawable(quadVertices, quadUVs, quadNormals);
}

void createContext()
{
    initializeMainRenderLoop();

    // --- postProcessingProgram ---

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << std::endl;
        std::cerr << "Error in shader creation" << std::endl;
    }

    int wallPoints = 50;
    float mainRoomRadius = 10.0f,
          mainRoomHeight = 8.5f;

    float SecondaryRoomWidth = 7.5f,
          SecondaryRoomHeight = 7.5f,
          SecondaryRoomDepth = 15.0f;

    rooms[0] = new MainRoom(mainRoomHeight, mainRoomRadius, wallPoints);
    for (int i = 1; i < PAINTINGS + 1; i++)
    {
        Room *secondaryRoom = new SecondaryRoom(SecondaryRoomHeight, SecondaryRoomWidth, SecondaryRoomDepth);
        rooms[i] = secondaryRoom;
    }
    currentRoom = rooms[0];

    float paintingHeight = 6.0f,
          paintingWidth = 4.5f,
          paintingYpos = 3.75f;

    float angleStep = 2 * M_PI / PAINTINGS;
    for (int i = 0; i < PAINTINGS; i++)
    {
        Painting *painting = new Painting(paintingHeight, paintingWidth, paintingYpos, mainRoomRadius, angleStep * i);
        paintings.push_back(painting);
        rooms[0]->subObjects.push_back(painting);
        rooms[i + 1]->subObjects.push_back(painting);
    }

    // Create frame buffers
    sceneFBO = new SceneFBO();

    // --- depthProgram ---
    depthFBO = new DepthFBO();

    // --- postProcessingProgram ---
    createFinalScene();
    renderFBO = new RenderFBO();

    // --- painting textures ---
    createPaintingTextures();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void createPaintingTextures()
{
    PaintingsFBO *paintingsFBO = new PaintingsFBO();
    for (int i = 0; i < PAINTINGS; i++)
    {   
        GLuint paintingTexture;
        paintingsFBO->addTexture(paintingTexture);

        gameState = GameState(i + 1);
        change_room();
        camera->position = player->position + vec3(0, player->height, 0);
        camera->update();


        depthPass(depthFBO, currentRoom);
        lightPass(sceneFBO, camera, currentRoom, depthFBO->depthCubeMap);

        displayScene(paintingsFBO, sceneFBO->colorTexture);

        paintings[i]->texture.diffuse = paintingTexture;
        paintings[i]->useTexture = true;
    }
    gameState = MAINROOM;
    change_room();
}

void free()
{
    glfwTerminate();
}

void mainLoop()
{
    do
    {
        static double lastTime = glfwGetTime();

        // Compute time difference between current and last frame
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);


        player->move(window, deltaTime, camera->horizontalAngle);
        //*/
        if (!currentRoom->isInside(player->position))
        {
            player->velocity = vec3(0, player->velocity.y, 0);
            player->position = player->prevPosition;
            player->updatePosition(camera->horizontalAngle, deltaTime);
        }
        //*/

        camera->position = player->position + vec3(0, player->height, 0);
        camera->update();

        depthPass(depthFBO, currentRoom);
        lightPass(sceneFBO, camera, currentRoom, depthFBO->depthCubeMap);

        displayScene(renderFBO, sceneFBO->colorTexture);

        change_state();
        glfwSwapBuffers(window);
        glfwPollEvents();

        lastTime = currentTime;

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}

void displayScene(FBO *fbo, GLuint texture)
{
    glUseProgram(postProcessingProgram[gameState]);

    glActiveTexture(DIFFUSE_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, texture);

    GLuint timeLocation;
    const int colors = 2;
    switch (gameState)
    {
    case ROOM2:
        FloydSteinbergDithering::applyDithering(colors);
        break;
    case ROOM3:
        Painterly::applyPainterly();
        break;
    case ROOM6:
        timeLocation = glGetUniformLocation(postProcessingProgram[gameState], "time");
        glUniform1f(timeLocation, (float)glfwGetTime());
        break;
    default:
        break;
    }

    fbo->bind();

    glUniform1i(quadTextureSamplerLocation[gameState], DIFFUSE_TEXTURE_LOCATION);
    quad->bind();
    quad->draw();

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        glewGetErrorString(error);
        std::cerr << "OpenGL error: " << glewGetErrorString(error) << std::endl;
        std::cerr << "Error in display scene pass" << std::endl;
    }
}

void change_state()
{
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    {
        gameState = MAINROOM;
        change_room();
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        gameState = ROOM1;
        change_room();
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        gameState = ROOM2;
        change_room();
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        gameState = ROOM3;
        change_room();
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        gameState = ROOM4;
        change_room();
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        gameState = ROOM5;
        change_room();
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        gameState = ROOM6;
        change_room();
    }
}

void change_room()
{
    currentRoom = rooms[gameState];
    if (gameState == MAINROOM){
        player->position = vec3(0, 0, 0.0);
        camera->horizontalAngle = 0.0f;
        camera->verticalAngle = 0.0f;
        for (auto &painting : paintings)
        {
            painting->modelMatrix = painting->mainRoomModelMatrix;
            painting->updateFrameModelMatrix();
        }
        return;
    }
    player->position = vec3(0, 0, -rooms[gameState]->depth / 2 + 1);
    camera->horizontalAngle = -3.14f;
    camera->verticalAngle = 0.1f;
    paintings[gameState - 1]->modelMatrix = paintings[gameState - 1]->secondaryRoomModelMatrix *
                                            translate(mat4(1.0f), vec3(0, 0, -rooms[gameState]->depth / 2 + 0.1));
    paintings[gameState - 1]->updateFrameModelMatrix();
}

void initialize()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        throw runtime_error("Failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        throw runtime_error(string(string("Failed to open GLFW window.") +
                                   " If you have an Intel GPU, they are not 3.3 compatible." +
                                   "Try the 2.1 version.\n"));
    }
    glfwMakeContextCurrent(window);

    // Start GLEW extension handler
    glewExperimental = GL_TRUE;

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw runtime_error("Failed to initialize GLEW\n");
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, W_WIDTH / 2, W_HEIGHT / 2);

    // Gray background color
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    // enable texturing and bind the depth texture
    glEnable(GL_TEXTURE_2D);

    // Log
    logGLParameters();
    // Create camera
    camera = new Camera(window);
}

int main(void)
{
    try
    {
        initialize();
        createContext();
        mainLoop();
        free();
    }
    catch (exception &ex)
    {
        cout << ex.what() << endl;
        getchar();
        free();
        return -1;
    }
    return 0;
}