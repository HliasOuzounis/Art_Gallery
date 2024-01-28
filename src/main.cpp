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

using namespace std;
using namespace glm;

// Function prototypes
void initialize();
void createContext();
void mainLoop();
void depth_pass();
void light_pass(mat4, mat4, vec3);
void free();

// Global variables
GLuint shaderProgram, modelMatrixLocation, viewMatrixLocation, projectionMatrixLocation,
    materialLocation[4], useTextureLocation, viewPosLocation;

GLuint sceneFBO, textureColorbuffer, rbo;

GLFWwindow *window;
Camera *camera;

GLuint depthProgram;
GLuint shadowViewProjectionLocation, shadowModelLocation;
GLuint depthMapFBO, depthCubeMap;

void displayScene(GLuint texture);
Drawable *quad;
GLuint postProcessingProgram[PAINTINGS + 1], quadTextureSamplerLocation[PAINTINGS + 1];

void createPaintingTextures();
GLuint paintingTexturesFBO;
GLuint paintingTextures[PAINTINGS];
GLuint paintingsDepthMap[PAINTINGS];

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
void updatePaintingModelMatrix();

void createSceneBuffer()
{
    glGenFramebuffers(1, &sceneFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);

    // create a color attachment texture
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, W_WIDTH, W_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    // use a single renderbuffer object for both a depth AND stencil buffer.
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, W_WIDTH, W_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // now actually attach it
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << std::endl;
        std::cerr << "Error creating scene buffer" << std::endl;
    }

    // now that we have created the framebuffer and added all attachments we want to check if it is actually complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        glfwTerminate();
        throw runtime_error("Frame buffer not initialized correctly");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void createDepthBuffer()
{
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

    glGenTextures(1, &depthCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                     SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << std::endl;
        std::cerr << "Error creating depth buffer" << std::endl;
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        glfwTerminate();
        throw runtime_error("Depth buffer not initialized correctly");
    }
}

void createFinalScene()
{
    GLuint defaultShader = loadShaders("src/shaders/image_processing/vertex.glsl",
                                       "src/shaders/image_processing/default.frag.glsl");

    postProcessingProgram[MAINROOM] = defaultShader;

    postProcessingProgram[ROOM1] = defaultShader;

    postProcessingProgram[ROOM2] = defaultShader;

    postProcessingProgram[ROOM3] = loadShaders("src/shaders/image_processing/vertex.glsl",
                                               "src/shaders/image_processing/fish_eye.frag.glsl");
    postProcessingProgram[ROOM4] = loadShaders("src/shaders/image_processing/vertex.glsl",
                                               "src/shaders/image_processing/toon.frag.glsl");
    postProcessingProgram[ROOM5] = loadShaders("src/shaders/image_processing/vertex.glsl",
                                               "src/shaders/image_processing/chromatic_aberration.frag.glsl");

    postProcessingProgram[ROOM6] = defaultShader;

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
    shaderProgram = loadShaders("src/shaders/render_vertex.glsl", "src/shaders/render_fragment.glsl");

    depthProgram = loadShaders("src/shaders/depth_pass/depth_vertex.glsl",
                               "src/shaders/depth_pass/depth_fragment.glsl",
                               "src/shaders/depth_pass/depth_geometry.glsl");

    // Find uniforms
    modelMatrixLocation = glGetUniformLocation(shaderProgram, "M");
    viewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
    projectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");
    materialLocation[0] = glGetUniformLocation(shaderProgram, "material.Ka");
    materialLocation[1] = glGetUniformLocation(shaderProgram, "material.Kd");
    materialLocation[2] = glGetUniformLocation(shaderProgram, "material.Ks");
    materialLocation[3] = glGetUniformLocation(shaderProgram, "material.shininess");
    useTextureLocation = glGetUniformLocation(shaderProgram, "useTexture");
    viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos");

    // --- depthProgram ---
    shadowModelLocation = glGetUniformLocation(depthProgram, "M");

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
    createSceneBuffer();

    // --- depthProgram ---
    createDepthBuffer();

    // --- postProcessingProgram ---
    createFinalScene();

    // --- painting textures ---
    // createPaintingTextures();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void createPaintingTextures()
{
    for (int i = 0; i < PAINTINGS; i++)
    {
        glGenTextures(1, &paintingTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, W_WIDTH, W_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, paintingTextures[i]);
        gameState = GameState(i + 1);
        change_room();

        camera->position = player->position + vec3(0, player->height, 0);
        camera->update();

        depth_pass();
        light_pass(camera->viewMatrix, camera->projectionMatrix, camera->position);

        // copy textureColorbuffer to paintingTextures[i]
        glBindTexture(GL_TEXTURE_2D, paintingTextures[i]);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, W_WIDTH, W_HEIGHT, 0);

        glBindTexture(GL_TEXTURE_2D, 0);

        paintings[i]->texture.diffuse = paintingTextures[i];
        paintings[i]->useTexture = true;
    }
    gameState = MAINROOM;
    change_room();
}

void free()
{
    glDeleteProgram(shaderProgram);
    glDeleteProgram(depthProgram);
    glDeleteFramebuffers(1, &sceneFBO);
    glDeleteTextures(1, &textureColorbuffer);
    glDeleteFramebuffers(1, &depthMapFBO);
    glDeleteTextures(1, &depthCubeMap);
    glDeleteProgram(postProcessingProgram[gameState]);
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

        depth_pass();
        light_pass(camera->viewMatrix, camera->projectionMatrix, camera->position);

        displayScene(textureColorbuffer);

        change_state();
        glfwSwapBuffers(window);
        glfwPollEvents();

        lastTime = currentTime;

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}

void light_pass(mat4 viewMatrix, mat4 projectionMatrix, vec3 viewPos)
{
    glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
    glViewport(0, 0, W_WIDTH, W_HEIGHT);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glUseProgram(shaderProgram);

    glActiveTexture(DEPTH_TEXTURE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
    glUniform3fv(viewPosLocation, 1, &viewPos[0]);

    glUniform1i(glGetUniformLocation(shaderProgram, "diffuseColorSampler"), DIFFUSE_TEXTURE_LOCATION);
    glUniform1i(glGetUniformLocation(shaderProgram, "specularColorSampler"), SPECULAR_TEXTURE_LOCATION);
    glUniform1i(glGetUniformLocation(shaderProgram, "depthMap"), DEPTH_TEXTURE_LOCATION);

    // Draw currentRoom
    currentRoom->render(shaderProgram, modelMatrixLocation, materialLocation, useTextureLocation);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << std::endl;
        std::cerr << "Error in light pass" << std::endl;
    }
}

void depth_pass()
{
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(depthProgram);

    // ---- render the scene ---- //
    currentRoom->render(depthProgram, shadowModelLocation);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << std::endl;
        std::cerr << "Error in depth pass" << std::endl;
    }
}

void displayScene(GLuint texture)
{
    glUseProgram(postProcessingProgram[gameState]);

    glActiveTexture(DIFFUSE_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, texture);

    GLuint timeLocation;
    const int colors = 2;
    switch (gameState)
    {
    case ROOM1:
        FloydSteinbergDithering::applyDithering(colors);
        break;
    case ROOM2:
        Painterly::applyPainterly();
        break;
    case ROOM5:
        timeLocation = glGetUniformLocation(postProcessingProgram[gameState], "time");
        glUniform1f(timeLocation, (float)glfwGetTime());
        break;
    default:
        break;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, W_WIDTH, W_HEIGHT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

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
        player->position = vec3(0, 0, 0.0);
        camera->horizontalAngle = 0.0f;
        camera->verticalAngle = 0.0f;
        for (auto &painting : paintings)
        {
            painting->modelMatrix = painting->mainRoomModelMatrix;
            painting->update_frame_model_matrix();
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
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        gameState = ROOM6;
        change_room();
    }
}

void change_room()
{
    player->position = vec3(0, 0, -rooms[gameState]->depth / 2 + 1);
    camera->horizontalAngle = -3.14f;
    camera->verticalAngle = 0.0f;
    paintings[gameState - 1]->modelMatrix = paintings[gameState - 1]->secondaryRoomModelMatrix *
                                            translate(mat4(1.0f), vec3(0, 0, -rooms[gameState]->depth / 2 + 0.1));
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