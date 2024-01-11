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

// extra helper functions
#include "extra/rooms/room.h"
#include "extra/paintings/paintings.h"
#include "extra/player/player.h"
#include "extra/light/light.h"

using namespace std;
using namespace glm;

// Function prototypes
void initialize();
void createContext();
void mainLoop();
void depth_pass(Light *light);
void light_pass(mat4, mat4);
void free();

#define W_WIDTH 1024
#define W_HEIGHT 768
#define TITLE "Art Gallery"

#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

// Global variables
GLuint shaderProgram, ModelMatrixLocation, ViewMatrixLocation, ProjectionMatrixLocation,
    materialLocation[4], useTextureLocation, shadowMapSamplerLocation;
GLuint renderFBO, renderedImage;
int imageBuffer[W_WIDTH * W_HEIGHT * 3];

GLFWwindow *window;
Camera *camera;

GLuint depthProgram;
GLuint shadowViewProjectionLocation, shadowModelLocation;
GLuint depthMapFBO, depthMap;

void createDepthMap();
void renderDepthMap(GLuint texture);
GLuint miniMapProgram, quadTextureSamplerLocation;
Drawable *quad;

Room *currentRoom;
Room *rooms[6];
MainRoom *mainRoom;
vector<Painting *> paintings;
Player *player = new Player();
Light *light;

enum GameState
{
    MAINROOM,
    ROOM1,
    ROOM2,
    ROOM3,
    ROOM4,
    ROOM5,
};

void change_state();
void change_room();
void set_paintings_visibility();

GameState gameState = MAINROOM;

void createRenderBuffer()
{
    glGenFramebuffers(1, &renderFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);

    glGenTextures(1, &renderedImage);
    glBindTexture(GL_TEXTURE_2D, renderedImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W_WIDTH, W_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedImage, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        glfwTerminate();
        throw runtime_error("Frame buffer not initialized correctly");
    }
}

void createDepthBuffer()
{
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        glfwTerminate();
        throw runtime_error("Depth buffer not initialized correctly");
    }
}

void createContext()
{
    shaderProgram = loadShaders("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");

    depthProgram = loadShaders("src/shaders/depth_pass/vertex.glsl", "src/shaders/depth_pass/fragment.glsl");

    // Find uniforms
    ModelMatrixLocation = glGetUniformLocation(shaderProgram, "M");
    ViewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
    ProjectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");
    materialLocation[0] = glGetUniformLocation(shaderProgram, "material.Ka");
    materialLocation[1] = glGetUniformLocation(shaderProgram, "material.Kd");
    materialLocation[2] = glGetUniformLocation(shaderProgram, "material.Ks");
    materialLocation[3] = glGetUniformLocation(shaderProgram, "material.shininess");
    useTextureLocation = glGetUniformLocation(shaderProgram, "useTexture");

    // --- depthProgram ---
    shadowViewProjectionLocation = glGetUniformLocation(depthProgram, "VP");
    shadowModelLocation = glGetUniformLocation(depthProgram, "M");

    int numPaintings = 5,
        wallPoints = 50;

    float mainRoomRadius = 10.0f,
          mainRoomHeight = 10.0f;

    float SecondaryRoomWidth = 7.5f,
          SecondaryRoomHeight = 7.5f,
          SecondaryRoomDepth = 15.0f;

    rooms[0] = new MainRoom(mainRoomHeight, mainRoomRadius, wallPoints);
    Room *secondaryRoom = new SecondaryRoom(SecondaryRoomHeight, SecondaryRoomWidth, SecondaryRoomDepth);
    for (int i = 1; i < numPaintings + 1; i++)
    {
        rooms[i] = secondaryRoom;
    }
    currentRoom = rooms[0];

    float paintingHeight = 6.0f,
          paintingWidth = 4.5f,
          paintingYpos = 3.75f;
    paintings = createPaintings(numPaintings, paintingHeight, paintingWidth, paintingYpos, mainRoomRadius);
    light = new Light(vec3(0, 10, 0), vec4(1, 1, 1, 1), 1.0f, 10.0f);

    // Create frame buffers
    createRenderBuffer();
    createDepthBuffer();

    createDepthMap();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void free()
{
    glDeleteProgram(shaderProgram);
    glDeleteProgram(depthProgram);
    glDeleteFramebuffers(1, &renderFBO);
    glDeleteTextures(1, &renderedImage);
    glDeleteFramebuffers(1, &depthMapFBO);
    glDeleteTextures(1, &depthMap);
    glDeleteProgram(miniMapProgram);
    glfwTerminate();
}

void mainLoop()
{
    do
    {
        static double lastTime = glfwGetTime();

        currentRoom = rooms[gameState];

        // Compute time difference between current and last frame
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);

        // camera
        player->move(window, deltaTime, camera->horizontalAngle);
        if (!currentRoom->isInside(player->position))
        {
            player->velocity = vec3(0, player->velocity.y, 0);
            player->position = player->prevPosition;
            player->updatePosition(camera->horizontalAngle, deltaTime);
        }

        camera->position = player->position + vec3(0, player->height, 0);
        camera->update();

        light->position.y = currentRoom->height + light->light_displacement;

        depth_pass(light);

        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
        {
            light_pass(light->viewMatrix, light->projectionMatrix);
        }
        else
        {
            light_pass(camera->viewMatrix, camera->projectionMatrix);
        }

        // renderDepthMap(depthMap);

        change_state();
        glfwSwapBuffers(window);
        glfwPollEvents();

        lastTime = currentTime;

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}

void light_pass(mat4 viewMatrix, mat4 projectionMatrix)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // glBindFramebuffer(GL_READ_FRAMEBUFFER, renderFBO);
    glViewport(0, 0, W_WIDTH, W_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(ProjectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    glUniform1i(glGetUniformLocation(shaderProgram, "diffuseColorSampler"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "specularColorSampler"), 1);
    glUniform1i(glGetUniformLocation(shaderProgram, "shadowMapSampler"), 2);

    light->upload_to_shaders(shaderProgram);

    light->draw(ModelMatrixLocation, materialLocation);

    // Draw currentRoom
    currentRoom->draw(ModelMatrixLocation, materialLocation, useTextureLocation);

    // Draw paintings
    for (auto &painting : paintings)
    {
        painting->draw(ModelMatrixLocation, materialLocation, useTextureLocation);

        if (painting->checkCollision(player))
        {
            cout << "Collision" << endl;
        }
    }
    // glReadPixels(0, 0, W_WIDTH, W_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);

    // FILE *out = fopen("test.tga", "w");
    // short TGAhead[] = {0, 2, 0, 0, 0, 0, W_WIDTH, W_HEIGHT, 24};
    // fwrite(&TGAhead, sizeof(TGAhead), 1, out);
    // fwrite(imageBuffer, 3 * W_WIDTH * W_HEIGHT, 1, out);
    // fclose(out);
}

void depth_pass(Light *light)
{
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(depthProgram);

    // sending the view and projection matrix to the shader
    mat4 view_projection = light->get_light_space_matrix();
    glUniformMatrix4fv(shadowViewProjectionLocation, 1, GL_FALSE, &light->get_light_space_matrix()[0][0]);

    // ---- rendering the scene ---- //
    currentRoom->draw(shadowModelLocation);

    for (auto &painting : paintings)
    {
        painting->draw(shadowModelLocation);
    }
}

void createDepthMap()
{
    miniMapProgram = loadShaders("src/shaders/quad/vertex.glsl", "src/shaders/quad/fragment.glsl");
    quadTextureSamplerLocation = glGetUniformLocation(miniMapProgram, "textureSampler");

    vector<vec3> quadVertices = {
        vec3(0.5, 0.5, -1.0),
        vec3(1.0, 0.5, -1.0),
        vec3(1.0, 1.0, -1.0),
        vec3(1.0, 1.0, -1.0),
        vec3(0.5, 1.0, -1.0),
        vec3(0.5, 0.5, -1.0)};

    vector<vec2> quadUVs = {
        vec2(0.0, 0.0),
        vec2(1.0, 0.0),
        vec2(1.0, 1.0),
        vec2(1.0, 1.0),
        vec2(0.0, 1.0),
        vec2(0.0, 0.0)};

    quad = new Drawable(quadVertices, quadUVs);
}

void renderDepthMap(GLuint texture)
{
    // using the correct shaders to visualize the depth texture on the quad
    glUseProgram(miniMapProgram);
    // enabling the texture - follow the aforementioned pipeline
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(quadTextureSamplerLocation, 0);

    // Drawing the quad
    quad->bind();
    quad->draw();
}

void change_state()
{
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    {
        gameState = MAINROOM;
        player->position = vec3(0, 0, 0.0);
        camera->horizontalAngle = 0.0f;
        camera->verticalAngle = 0.0f;

        for (auto &painting : paintings)
        {
            painting->modelMatrix = painting->mainRoomModelMatrix;
            painting->update_frame_model_matrix();
            painting->visible = true;
        }
        return;
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
}

void change_room()
{
    player->position = vec3(0, 0, -rooms[gameState]->depth / 2 + 1);
    camera->horizontalAngle = -3.14f;
    camera->verticalAngle = 0.0f;
    set_paintings_visibility();
}

void set_paintings_visibility()
{
    if (gameState == MAINROOM)
    {
        return;
    }
    for (auto &painting : paintings)
    {
        painting->visible = false;
    }
    paintings[gameState - 1]->visible = true;
    paintings[gameState - 1]->modelMatrix = translate(mat4(1.0f), vec3(0, rooms[gameState]->height / 2, -rooms[gameState]->depth / 2 + 0.1));
    paintings[gameState - 1]->update_frame_model_matrix();
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