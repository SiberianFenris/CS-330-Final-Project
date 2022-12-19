#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library

#define STB_IMAGE_IMPLEMENTATION // Texture Libraries
#include "stb_image.h"      // Image loading Utility functions

#include <vector>

//GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnOpengl/camera.h>

using namespace std; // Standard namespace

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif


// Unnamed namespace
namespace
{
    const char* const WINDOW_TITLE = "3D Scene - Ron Causey"; // Macro for window title

    // Variables for window width and height
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    // Stores the GL data relative to a given mesh
    struct GLMesh
    {
        GLuint vao;
        GLuint vbos[4];     // Handles for the vertex buffer objects
        GLuint nIndices;    // Number of indices of the mesh
    };

    // Main GLFW window
    GLFWwindow* gWindow = nullptr;

    bool isPerspective = false;

    // Camera
    GLMesh gCameraMesh;

    // Triangle mesh data
    GLMesh gPolygonMesh;
    GLMesh gPlaneMesh;
    GLMesh gTableMesh;
    GLMesh gCubeMesh;
    GLMesh gTabletMesh;
    GLMesh gCylinderMesh;
    GLMesh gSphereMesh;


    glm::vec2 gUVScale(0.7f, 0.7f);
    GLint gTexWrapMode = GL_REPEAT;
 
    // Textures
    GLuint gTexturePolygonId;
    GLuint gTexturePlaneId;
    GLuint gTextureTableId;
    GLuint gTextureCubeId;
    GLuint gTextureTabletId;
    GLuint gTextureCylinderId;
    GLuint gTextureSphereId;


    // Shaders
    GLuint programId;
    GLuint gCameraId;
    GLuint gPolygonId;
    GLuint gPlaneId;
    GLuint gTableId;
    GLuint gCubeId;
    GLuint gTabletId;
    GLuint gCylinderId;
    GLuint gSphereId;


    Camera gCamera(glm::vec3(0.0f, 0.0f, 4.0f));

    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    glm::vec3 gCameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 gCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 gCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


    float gDeltaTime = 0.0f; //time between current frame and last frame
    float gLastFrame = 0.0f;

    // Cube and light color
    glm::vec3 gObjectColor(0.6f, 0.5f, 0.75f);
    //glm::vec3 gObjectColor(1.0f, 1.0f, 2.0f);
    glm::vec3 gLightColor(1.1f, 1.1f, 1.1f);

    // Light position and scale
    glm::vec3 gLightPosition(-4.5f, 1.5f, 1.0f);
    glm::vec3 gLightScale(2.5f);
}


bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UCreatePolygonMesh(GLMesh& mesh);
void UCreatePlaneMesh(GLMesh& mesh);
void UCreateTableMesh(GLMesh& mesh);
void UCreateCubeMesh(GLMesh& mesh);
void UCreateTabletMesh(GLMesh& mesh);
void UCreateCylinderMesh();
void UCreateSphereMesh();

void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

void UDestroyMesh(GLMesh& mesh);
bool UCreateTexture(const char* filename, GLuint& textureId);
void UDestroyTexture(GLuint textureId);

void URender();
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);

/* Vertex Shader Source Code 1*/
const GLchar* vertexShaderSource1 = GLSL(440,
    layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
layout(location = 0) in vec2 textureCoordinate;
out vec2 vertexTextureCoordinate;

out vec4 fragmentColor;

uniform sampler2D uTextureBase;
uniform sampler2D uTextureExtra;
uniform bool multipleTextures;

//Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
    vertexTextureCoordinate = textureCoordinate;
}
);


/* Fragment Shader Source Code 1*/
const GLchar* fragmentShaderSource1 = GLSL(440,
    in vec2 vertexTextureCoordinate;

out vec4 fragmentColor;

uniform sampler2D uTexture;

void main()
{
    fragmentColor = texture(uTexture, vertexTextureCoordinate); // Sends texture to the GPU for rendering
}
);

/* Vertex Shader Source Code 2*/
const GLchar* vertexShaderSource2 = GLSL(440,
    layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
layout(location = 0) in vec2 textureCoordinate;
out vec2 vertexTextureCoordinate;

out vec4 fragmentColor;

uniform sampler2D uTextureBase;
uniform sampler2D uTextureExtra;
uniform bool multipleTextures;



//Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
    vertexTextureCoordinate = textureCoordinate;

}
);


/* Fragment Shader Source Code 2*/
const GLchar* fragmentShaderSource2 = GLSL(440,
    in vec2 vertexTextureCoordinate;

out vec4 fragmentColor;

uniform sampler2D uTexture;

void main()
{
    fragmentColor = texture(uTexture, vertexTextureCoordinate); // Sends texture to the GPU for rendering

}
);

/* Vertex Shader Source Code 3*/
const GLchar* vertexShaderSource3 = GLSL(440,
    layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
layout(location = 0) in vec2 textureCoordinate;
out vec2 vertexTextureCoordinate;

out vec4 fragmentColor;

uniform sampler2D uTextureBase;
uniform sampler2D uTextureExtra;
uniform bool multipleTextures;



//Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
    vertexTextureCoordinate = textureCoordinate;

}
);


/* Fragment Shader Source Code 3*/
const GLchar* fragmentShaderSource3 = GLSL(440,
    in vec2 vertexTextureCoordinate;

out vec4 fragmentColor;

uniform sampler2D uTexture;

void main()
{
    fragmentColor = texture(uTexture, vertexTextureCoordinate); // Sends texture to the GPU for rendering

}
);


/* Vertex Shader Source Code 4*/
const GLchar* vertexShaderSource4 = GLSL(440,
    layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
layout(location = 0) in vec2 textureCoordinate;
out vec2 vertexTextureCoordinate;

out vec4 fragmentColor;

uniform sampler2D uTextureBase;
uniform sampler2D uTextureExtra;
uniform bool multipleTextures;



//Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
    vertexTextureCoordinate = textureCoordinate;

}
);


/* Fragment Shader Source Code 4*/
const GLchar* fragmentShaderSource4 = GLSL(440,
    in vec2 vertexTextureCoordinate;

out vec4 fragmentColor;

uniform sampler2D uTexture;

void main()
{
    fragmentColor = texture(uTexture, vertexTextureCoordinate); // Sends texture to the GPU for rendering

}
);

/* Vertex Shader Source Code 5*/
const GLchar* vertexShaderSource5 = GLSL(440,
    layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
layout(location = 0) in vec2 textureCoordinate;
out vec2 vertexTextureCoordinate;

out vec4 fragmentColor;

uniform sampler2D uTextureBase;
uniform sampler2D uTextureExtra;
uniform bool multipleTextures;



//Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
    vertexTextureCoordinate = textureCoordinate;

}
);


/* Fragment Shader Source Code 5*/
const GLchar* fragmentShaderSource5 = GLSL(440,
    in vec2 vertexTextureCoordinate;

out vec4 fragmentColor;

uniform sampler2D uTexture;

void main()
{
    fragmentColor = texture(uTexture, vertexTextureCoordinate); // Sends texture to the GPU for rendering

}
);

/* Vertex Shader Source Code 6*/
const GLchar* vertexShaderSource6 = GLSL(440,
    layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
layout(location = 0) in vec2 textureCoordinate;
out vec2 vertexTextureCoordinate;

out vec4 fragmentColor;

uniform sampler2D uTextureBase;
uniform sampler2D uTextureExtra;
uniform bool multipleTextures;



//Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
    vertexTextureCoordinate = textureCoordinate;

}
);


/* Fragment Shader Source Code 6*/
const GLchar* fragmentShaderSource6 = GLSL(440,
    in vec2 vertexTextureCoordinate;

out vec4 fragmentColor;

uniform sampler2D uTexture;

void main()
{
    fragmentColor = texture(uTexture, vertexTextureCoordinate); // Sends texture to the GPU for rendering

}
);

/* Fragment Shader Source Code 7*/
const GLchar* fragmentShaderSource7 = GLSL(440,
    in vec2 vertexTextureCoordinate;

out vec4 fragmentColor;

uniform sampler2D uTexture;

void main()
{
    fragmentColor = vec4(0.0f, 0.0f, 0.0f, 1.0f); // Sends texture to the GPU for rendering

}
);

/* Cube Vertex Shader Source Code*/
const GLchar* VertexShaderSource = GLSL(440,

    layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
layout(location = 1) in vec3 normal; // VAP position 1 for normals
layout(location = 2) in vec2 textureCoordinate;

out vec3 vertexNormal; // For outgoing normals to fragment shader
out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader
out vec2 vertexTextureCoordinate;

//Uniform / Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates

    vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

    vertexNormal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
    vertexTextureCoordinate = textureCoordinate;
}
);


/* Fragment Shader Source Code*/
const GLchar* fragmentShaderSource = GLSL(440,
    in vec2 vertexTextureCoordinate;

out vec4 fragmentColor;

uniform sampler2D uTexture;

void main()
{
    fragmentColor = vec4(1.0f, 1.0f, 1.0f, 1.0f); // Sends texture to the GPU for rendering

}
);

/* Tablet Vertex Shader Source Code*/
const GLchar* VertexShaderSource7 = GLSL(440,

    layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
layout(location = 1) in vec3 normal; // VAP position 1 for normals
layout(location = 2) in vec2 textureCoordinate;

out vec3 vertexNormal; // For outgoing normals to fragment shader
out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader
out vec2 vertexTextureCoordinate;

//Uniform / Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates

    vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

    vertexNormal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
    vertexTextureCoordinate = textureCoordinate;
}
);


/* Cube Fragment Shader Source Code*/
const GLchar* FragmentShaderSource = GLSL(440,

    in vec3 vertexNormal; // For incoming normals
in vec3 vertexFragmentPos; // For incoming fragment position
in vec2 vertexTextureCoordinate;

out vec4 fragmentColor; // For outgoing cube color to the GPU

// Uniform / Global variables for object color, light color, light position, and camera/view position
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPosition;
uniform sampler2D uTexture; // Useful when working with multiple textures
uniform vec2 uvScale;

void main()
{
    /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

    //Calculate Ambient lighting*/
    float ambientStrength = 0.5f; // Set ambient or global lighting strength
    vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

    //Calculate Diffuse lighting*/
    vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
    vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
    float impact = max(dot(norm, lightDirection), 0.2);// Calculate diffuse impact by generating dot product of normal and light
    vec3 diffuse = impact * lightColor; // Generate diffuse light color

    //Calculate Specular lighting*/
    float specularIntensity = 5.5f; // Set specular light strength
    float highlightSize = 0.4f; // Set specular highlight size
    vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
    vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
    //Calculate specular component
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.2), highlightSize);
    vec3 specular = specularIntensity * specularComponent * lightColor;

    // Texture holds the color to be used for all three components
    vec4 textureColor = texture(uTexture, vertexTextureCoordinate * uvScale);

    // Calculate phong result
    vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;

    fragmentColor = vec4(phong, 1.0); // Send lighting results to GPU
}
);

// Images are loaded with Y axis going down, but OpenGL's Y axis goes up, so let's flip it
void flipImageVertically(unsigned char* image, int width, int height, int channels)
{
    for (int j = 0; j < height / 2; ++j)
    {
        int index1 = j * width * channels;
        int index2 = (height - 1 - j) * width * channels;

        for (int i = width * channels; i > 0; --i)
        {
            unsigned char tmp = image[index1];
            image[index1] = image[index2];
            image[index2] = tmp;
            ++index1;
            ++index2;
        }
    }
}

int main(int argc, char* argv[])
{
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Create the mesh
    UCreatePolygonMesh(gPolygonMesh); // Calls the function to create the Vertex Buffer Object
    UCreatePlaneMesh(gPlaneMesh);
    UCreateTableMesh(gTableMesh);
    UCreateCubeMesh(gCubeMesh);
    UCreateCubeMesh(gTabletMesh);
    UCreateCylinderMesh();
    UCreateSphereMesh();

    if (!UCreateShaderProgram(vertexShaderSource4, fragmentShaderSource4, gPolygonId))
        return EXIT_FAILURE;
    if (!UCreateShaderProgram(vertexShaderSource6, fragmentShaderSource6, gPlaneId))
        return EXIT_FAILURE;
    if (!UCreateShaderProgram(vertexShaderSource2, fragmentShaderSource2, gTableId))
        return EXIT_FAILURE;
    if (!UCreateShaderProgram(VertexShaderSource, fragmentShaderSource, gCubeId))
        return EXIT_FAILURE;
    if (!UCreateShaderProgram(VertexShaderSource7, fragmentShaderSource7, gTabletId))
        return EXIT_FAILURE;
    if (!UCreateShaderProgram(vertexShaderSource5, fragmentShaderSource5, gCylinderId))
        return EXIT_FAILURE;
    if (!UCreateShaderProgram(vertexShaderSource3, fragmentShaderSource3, gSphereId))
        return EXIT_FAILURE;

    // Load Texture 1 ----- Light One
    //const char* texFilename1 = "../../resources/textures/white.jpg";
    //if (!UCreateTexture(texFilename1, gTextureCubeId))
    //{
    //    cout << "Failed to load texture " << texFilename1 << endl;
    //    return EXIT_FAILURE;
    //}
    //// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    //glUseProgram(gCubeId);
    //// We set the texture as texture unit 0
    //glUniform1i(glGetUniformLocation(gCubeId, "uTexture"), 3);
    //// We set the texture as texture unit 1
    ////glUniform1i(glGetUniformLocation(gCubeId, "uTextureExtra"), 1);


    // Load Texture 2 ----- Table
    const char* texFilename2 = "../../resources/textures/Grey.jpg";
    if (!UCreateTexture(texFilename2, gTextureTableId))
    {
        cout << "Failed to load texture " << texFilename2 << endl;
        return EXIT_FAILURE;
    }
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(gTableId);
    // We set the texture as texture unit 0
    glUniform1i(glGetUniformLocation(gTableId, "uTexture"), 2);
    // We set the texture as texture unit 1
    //glUniform1i(glGetUniformLocation(gTableId, "uTextureExtra"), 1);


    // Load Texture 3 ------ Watermelon Ball
    const char* texFilename3 = "../../resources/textures/ball.jpg";
    if (!UCreateTexture(texFilename3, gTextureSphereId))
    {
        cout << "Failed to load texture " << texFilename3 << endl;
        return EXIT_FAILURE;
    }
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(gSphereId);
    // We set the texture as texture unit 0
    glUniform1i(glGetUniformLocation(gSphereId, "uTexture"), 5);
    // We set the texture as texture unit 1
    //glUniform1i(glGetUniformLocation(gSphereId, "uTextureExtra"), 1);

    // Load Texture 4 ------ Hershey Kiss
    const char* texFilename4 = "../../resources/textures/Hershey.jpg";
    if (!UCreateTexture(texFilename4, gTexturePolygonId))
    {
        cout << "Failed to load texture " << texFilename4 << endl;
        return EXIT_FAILURE;
    }
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(gPolygonId);
    // We set the texture as texture unit 0
    glUniform1i(glGetUniformLocation(gPolygonId, "uTexture"), 0);
    // We set the texture as texture unit 1
    //glUniform1i(glGetUniformLocation(gPolygonId, "uTextureExtra"), 0);

    // Load Texture 5 -------- Monster Can
    const char* texFilename5 = "../../resources/textures/monster.jpg";
    if (!UCreateTexture(texFilename5, gTextureCylinderId))
    {
        cout << "Failed to load texture " << texFilename5 << endl;
        return EXIT_FAILURE;
    }
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(gCylinderId);
    // We set the texture as texture unit 0
    glUniform1i(glGetUniformLocation(gCylinderId, "uTexture"), 4);
    // We set the texture as texture unit 1
    //glUniform1i(glGetUniformLocation(gCylinderId, "uTextureExtra"), 0);

    // Load Texture 6 ------- Kiss Logo Ribbon
    const char* texFilename6 = "../../resources/textures/Ribbon.jpg";
    if (!UCreateTexture(texFilename6, gTexturePlaneId))
    {
        cout << "Failed to load texture " << texFilename6 << endl;
        return EXIT_FAILURE;
    }
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(gPlaneId);
    // We set the texture as texture unit 0
    glUniform1i(glGetUniformLocation(gPlaneId, "uTexture"), 1);
    // We set the texture as texture unit 1
    //glUniform1i(glGetUniformLocation(gPlaneId, "uTextureExtra"), 0);

        // Load Texture 7 ----- Tablet
    //const char* texFilename7 = "../../resources/textures/phone.png";
    //if (!UCreateTexture(texFilename1, gTextureTabletId))
    //{
    //    cout << "Failed to load texture " << texFilename1 << endl;
    //    return EXIT_FAILURE;
    //}
    //// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    //glUseProgram(gTabletId);
    //// We set the texture as texture unit 0
    //glUniform1i(glGetUniformLocation(gTabletId, "uTexture"), 7);
    //// We set the texture as texture unit 1
    ////glUniform1i(glGetUniformLocation(gTabletId, "uTextureExtra"), 1);

    while (!glfwWindowShouldClose(gWindow))
    {

        // per-frame timing
        // --------------------
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;

        UProcessInput(gWindow);

        URender();

        glfwPollEvents();
    }

    // Release mesh data
    UDestroyMesh(gPolygonMesh);
    UDestroyMesh(gPlaneMesh);
    UDestroyMesh(gTableMesh);
    UDestroyMesh(gCubeMesh);
    UDestroyMesh(gTabletMesh);
    UDestroyMesh(gCylinderMesh);
    UDestroyMesh(gSphereMesh);

    // Release texture
    UDestroyTexture(gCubeId);
    UDestroyTexture(gTextureCubeId);

    UDestroyTexture(gTabletId);
    UDestroyTexture(gTextureTabletId);

    UDestroyTexture(gTableId);
    UDestroyTexture(gTextureTableId);

    UDestroyTexture(gSphereId);
    UDestroyTexture(gTextureSphereId);

    UDestroyTexture(gPolygonId);
    UDestroyTexture(gTexturePolygonId);

    UDestroyTexture(gCylinderId);
    UDestroyTexture(gTextureCylinderId);

    UDestroyTexture(gPlaneId);
    UDestroyTexture(gTexturePlaneId);


    // Release Shader program
    UDestroyShaderProgram(gPolygonId);
    UDestroyShaderProgram(gPlaneId);
    UDestroyShaderProgram(gTableId);
    UDestroyShaderProgram(gCubeId);
    UDestroyShaderProgram(gTabletId);
    UDestroyShaderProgram(gCylinderId);
    UDestroyShaderProgram(gSphereId);

    exit(EXIT_SUCCESS); // Terminates the program successfully
}

// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    // GLFW: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);
    glfwSetCursorPosCallback(*window, UMousePositionCallback);
    glfwSetScrollCallback(*window, UMouseScrollCallback);
    glfwSetMouseButtonCallback(*window, UMouseButtonCallback);


    // tell GLFW to capture our mouse
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{
    static const float cameraSpeed = 2.5f;


    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        gCamera.ProcessKeyboard(LEFT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)

        gCamera.ProcessKeyboard(UP, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        gCamera.ProcessKeyboard(DOWN, gDeltaTime);

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        isPerspective = false;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        isPerspective = true;

    bool keypress = false;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cout << "You pressed W! ";
        keypress = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cout << "You pressed S! ";
        keypress = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cout << "You pressed A! ";
        keypress = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cout << "You pressed D! ";
        keypress = true;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        cout << "You pressed Q! ";
        keypress = true;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        cout << "You pressed E! ";
        keypress = true;
    }



    if (keypress)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        cout << "Cursor at position (" << x << ", " << y << ")" << endl;
    }

}

void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (gFirstMouse)
    {
        gLastX = xpos;
        gLastY = ypos;
        gFirstMouse = false;
    }

    float xoffset = xpos - gLastX;
    float yoffset = gLastY - ypos; // reversed since y-coordinates go from bottom to top

    gLastX = xpos;
    gLastY = ypos;

    gCamera.ProcessMouseMovement(xoffset, yoffset);
}
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    gCamera.ProcessMouseScroll(yoffset);
}

void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
    {
        if (action == GLFW_PRESS)
            cout << "Left mouse button pressed" << endl;
        else
            cout << "Left mouse button released" << endl;
    }
    break;

    case GLFW_MOUSE_BUTTON_MIDDLE:
    {
        if (action == GLFW_PRESS)
            cout << "Middle mouse button pressed" << endl;
        else
            cout << "Middle mouse button released" << endl;
    }
    break;

    case GLFW_MOUSE_BUTTON_RIGHT:
    {
        if (action == GLFW_PRESS)
            cout << "Right mouse button pressed" << endl;
        else
            cout << "Right mouse button released" << endl;
    }
    break;

    default:
        cout << "Unhandled mouse button event" << endl;
        break;
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

}

// Functioned called to render a frame
void URender()
{

    // Enable z-depth

    glEnable(GL_DEPTH_TEST);

    // Clear the frame and z buffers
    glClearColor(0.68f, 0.65f, 0.64f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection;

    if (isPerspective == false) {
        projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }
    else if (isPerspective == true) {
        projection = glm::ortho(-15.0f, 15.0f, -8.0f, 15.0f, 0.5f, 100.0f);

        isPerspective == false;

    }

    glm::mat4 view = gCamera.GetViewMatrix();


    glUseProgram(gCameraId);

    GLint modelLoc = glGetUniformLocation(gCameraId, "model");
    GLint viewLoc = glGetUniformLocation(gCameraId, "view");
    GLint projLoc = glGetUniformLocation(gCameraId, "projection");


    glBindVertexArray(gCameraMesh.vao);


    //glDrawArrays(GL_TRIANGLES, 0, gCameraMesh.nIndices);
    glDrawElements(GL_TRIANGLES, gCameraMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle

    // ------------------ Hershey Kiss Start --------------------///

    // 1. Scales the object by 2
    glm::mat4 scale = glm::scale(glm::vec3(0.5f, 0.4f, 0.3f));

    // 2. Rotates shape by 3.14f * 0.5f pi radians around the Z axis
    glm::mat4 rotation = glm::rotate(3.14f * 0.50f, glm::vec3(-13.0f, 0.0f, 0.1f));

    // 3. Place object at the origin
                                            // Left/Right   Up/Down    Back/Forward  
    glm::mat4 translation = glm::translate(glm::vec3(0.46f, -2.2f, -13.5f));

    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // Transforms the camera: move the camera back (z axis)
    glm::mat4 modeledShape = glm::translate(glm::vec3(2.5f, -2.89f, -15.1f));

    // Rotates the model view around the X axis
    glm::mat4 viewRotation = glm::rotate(-3.14f * 0.55f, glm::vec3(0.0f, 0.0f, 0.0f));

    // Set the shader to be used
    glUseProgram(gPolygonId);

    // Retrieves and passes transform matrices to the Shader program
    modelLoc = glGetUniformLocation(gPolygonId, "model");
    viewLoc = glGetUniformLocation(gPolygonId, "view");
    projLoc = glGetUniformLocation(gPolygonId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Reference matrix uniforms from the Cube Shader program for the cub color, light color, light position, and camera position
    GLint objectColorLoc = glGetUniformLocation(gPolygonId, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(gPolygonId, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(gPolygonId, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(gPolygonId, "viewPosition");

    // Pass color, light, and camera data to the Cube Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc = glGetUniformLocation(gPolygonId, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gPolygonMesh.vao);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTexturePolygonId);

    // Draws the triangles
    glDrawElements(GL_TRIANGLE_FAN, gPolygonMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle

    // Deactivate the Vertex Array Object
    glBindVertexArray(0);

    //--------------- Hershey Kiss End-----------------//


    // ------------------ Hershey Kiss Logo Start -----------------------//

    // 1. Scales the object by 2
    scale = glm::scale(glm::vec3(-0.1f, 0.5f, 0.1f));

    // 2. Rotates shape 2 by 3.141592f * 0.6f pi radians around the Z axis
    rotation = glm::rotate(3.14f * 0.50f, glm::vec3(0.4f, -1.5f, 0.0f));

    // 3. Place object 2 at the origin Left/Right
                                // Left/Right   Up/Down    Back/Forward  
    translation = glm::translate(glm::vec3(0.4f, -0.9f, -13.4f));

    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    // Transforms the camera: move the camera back (z axis)
    modeledShape = glm::translate(glm::vec3(-2.5f, -1.4f, -16.1f));


    // Rotates the Pyramid model view around the X axis
    viewRotation = glm::rotate(-3.14f * 0.45f, glm::vec3(1.0f, 0.5f, 5.8f));

    glUseProgram(gPlaneId);

    modelLoc = glGetUniformLocation(gPlaneId, "model");
    viewLoc = glGetUniformLocation(gPlaneId, "view");
    projLoc = glGetUniformLocation(gPlaneId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(gPlaneMesh.vao);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gTexturePlaneId);


    glDrawElements(GL_TRIANGLE_FAN, gPlaneMesh.nIndices, GL_UNSIGNED_SHORT, 0);
    // ---------------Hershey Kiss Logo End---------------//

    // ------------------ Table Start -----------------------//

    // 1. Scales the object by 2
    scale = glm::scale(glm::vec3(15.0f, 6.0f, 1.0f));

    // 2. Rotates shape 2 by 3.141592f * 0.6f pi radians around the Z axis
    rotation = glm::rotate(3.14f * 0.55f, glm::vec3(1.0f, 0.0f, 0.0f));

    // 3. Place object 2 at the origin Tilts
                                // Left/Right   Up/Down    Back/Forward  
    translation = glm::translate(glm::vec3(0.0f, -3.0f, -15.0f));

    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    // Transforms the camera: move the camera back (z axis)
    modeledShape = glm::translate(glm::vec3(0.0f, 5.5f, -17.0f));

    // Rotates the Pyramid model view around the X axis
    viewRotation = glm::rotate(-3.14f * 0.45f, glm::vec3(4.0f, 0.0f, 2.0f));

    glUseProgram(gTableId);

    modelLoc = glGetUniformLocation(gTableId, "model");
    viewLoc = glGetUniformLocation(gTableId, "view");
    projLoc = glGetUniformLocation(gTableId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(gTableMesh.vao);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gTextureTableId);


    glDrawElements(GL_TRIANGLE_FAN, gPlaneMesh.nIndices, GL_UNSIGNED_SHORT, 0);
    
    //----------------Table End-----------------//
    
    // ------------------ Tablet Start-----------------------//

    // 1. Scales the object by 2
    scale = glm::scale(glm::vec3(2.5f, 2.5f, 0.4f));

    // 2. Rotates shape 2 by 3.141592f * 0.6f pi radians around the Z axis
                            // Degree tilts object      Left/Right  Tilts 
    rotation = glm::rotate(3.14f * 0.55f, glm::vec3(0.5f, 0.0f, 0.0f));

    // Left/Right   Up/Down    Back/Forward      
    translation = glm::translate(glm::vec3(4.9f, -2.05f, -14.5f));

    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    // Transforms the camera: move the camera back (z axis)
    modeledShape = glm::translate(glm::vec3(7.0f, 3.6f, -7.5f));

    // Rotates the Pyramid model view around the X axis
    viewRotation = glm::rotate(-3.14f * 0.50f, glm::vec3(0.0f, 0.2f, 0.0f));


    glUseProgram(gTabletId);

    // Retrieves and passes transform matrices to the Shader program
    modelLoc = glGetUniformLocation(gTabletId, "model");
    viewLoc = glGetUniformLocation(gTabletId, "view");
    projLoc = glGetUniformLocation(gTabletId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Reference matrix uniforms from the Cube Shader program for the cub color, light color, light position, and camera position
    objectColorLoc = glGetUniformLocation(gTabletId, "objectColor");
    lightColorLoc = glGetUniformLocation(gTabletId, "lightColor");
    lightPositionLoc = glGetUniformLocation(gTabletId, "lightPos");
    viewPositionLoc = glGetUniformLocation(gTabletId, "viewPosition");

    // Pass color, light, and camera data to the Cube Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
    //glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    UVScaleLoc = glGetUniformLocation(gTabletId, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    glBindVertexArray(gTabletMesh.vao);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, gTextureTabletId);


    glDrawElements(GL_TRIANGLE_FAN, gTabletMesh.nIndices, GL_UNSIGNED_SHORT, 0);
    //---------------- Tablet End----------------//

    // --------------- Light One ---------------------//
        // 1. Scales the object by 2
    scale = glm::scale(glm::vec3(0.5f, 0.5f, 0.4f));

    // 2. Rotates shape 2 by 3.141592f * 0.6f pi radians around the Z axis
                            // Degree tilts object      Left/Right  Tilts 
    rotation = glm::rotate(3.14f * 0.55f, glm::vec3(0.5f, 0.9f, 0.0f));

    // Left/Right   Up/Down    Back/Forward      
    translation = glm::translate(glm::vec3(-2.5f, 6.05f, -10.5f));

    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    // Transforms the camera: move the camera back (z axis)
    modeledShape = glm::translate(glm::vec3(7.0f, 3.6f, -7.5f));

    // Rotates the Pyramid model view around the X axis
    viewRotation = glm::rotate(-3.14f * 0.50f, glm::vec3(0.0f, 0.2f, 0.0f));


    glUseProgram(gCubeId);

    modelLoc = glGetUniformLocation(gCubeId, "model");
    viewLoc = glGetUniformLocation(gCubeId, "view");
    projLoc = glGetUniformLocation(gCubeId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(gCubeMesh.vao);

    glDrawElements(GL_TRIANGLE_FAN, gCubeMesh.nIndices, GL_UNSIGNED_SHORT, 0);

    //------------ Light One End ------------------//

    // --------------- Light Two ---------------------//
        // 1. Scales the object by 2
    scale = glm::scale(glm::vec3(0.5f, 0.5f, 0.4f));

    // 2. Rotates shape 2 by 3.141592f * 0.6f pi radians around the Z axis
                            // Degree tilts object      Left/Right  Tilts 
    rotation = glm::rotate(3.14f * 0.55f, glm::vec3(0.5f, 0.9f, 0.0f));

    // Left/Right   Up/Down    Back/Forward      
    translation = glm::translate(glm::vec3(3.5f, 6.05f, -10.5f));

    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    // Transforms the camera: move the camera back (z axis)
    modeledShape = glm::translate(glm::vec3(7.0f, 3.6f, -7.5f));

    // Rotates the Pyramid model view around the X axis
    viewRotation = glm::rotate(-3.14f * 0.50f, glm::vec3(0.0f, 0.2f, 0.0f));


    glUseProgram(gCubeId);

    modelLoc = glGetUniformLocation(gCubeId, "model");
    viewLoc = glGetUniformLocation(gCubeId, "view");
    projLoc = glGetUniformLocation(gCubeId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(gCubeMesh.vao);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTexturePolygonId);

    glDrawElements(GL_TRIANGLE_FAN, gCubeMesh.nIndices, GL_UNSIGNED_SHORT, 0);

    //------------ Light Two End ------------------//


    //------------------------- Monster Can Start -------------------------------------//
    
    glBindVertexArray(gCylinderMesh.vao);

    // 1.                             Length  Thickness
    scale = glm::scale(glm::vec3(1.2f, 3.5f, 1.1f));
    // 2. Rotates shape 2 by 3.141592f * 0.6f pi radians around the Z axis
    rotation = glm::rotate(-3.14f * 0.65f, glm::vec3(0.0f, 0.5f, 0.0f));
    // Left/Right   Up/Down    Back/Forward
    translation = glm::translate(glm::vec3(-6.2f, -0.8f, -14.5f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    // Transforms the camera: move the camera back (z axis)
    modeledShape = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

    // Rotates the Pyramid model view around the X axis
    viewRotation = glm::rotate(-3.14f * 0.55f, glm::vec3(1.0f, 0.0f, 0.0f));


    glUseProgram(gCylinderId);

    modelLoc = glGetUniformLocation(gCylinderId, "model");
    viewLoc = glGetUniformLocation(gCylinderId, "view");
    projLoc = glGetUniformLocation(gCylinderId, "projection");


    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, gCylinderMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(gCylinderMesh.vao);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, gTextureCylinderId);

    glDrawElements(GL_TRIANGLE_FAN, gCylinderMesh.nIndices, GL_UNSIGNED_SHORT, 0);

    //--------Monster Can End------------//

        //------------------------- Watermelon Ball Start -------------------------------------//
    glBindVertexArray(gSphereMesh.vao);

    // 1.                             Length  Thickness
    scale = glm::scale(glm::vec3(2.5f, 2.5f, 2.1f));
    // 2. Rotates shape 2 by 3.141592f * 0.6f pi radians around the Z axis
    rotation = glm::rotate(-3.14f * 0.68f, glm::vec3(-3.2f, -0.5f, 0.8f));
    // Left/Right   Up/Down    Back/Forward
    translation = glm::translate(glm::vec3(-2.7f, -1.1f, -14.5f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    // Transforms the camera: move the camera back (z axis)
    modeledShape = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

    // Rotates the Pyramid model view around the X axis
    viewRotation = glm::rotate(-3.14f * 0.55f, glm::vec3(2.0f, 0.0f, 0.0f));

    glUseProgram(gSphereId);

    modelLoc = glGetUniformLocation(gSphereId, "model");
    viewLoc = glGetUniformLocation(gSphereId, "view");
    projLoc = glGetUniformLocation(gSphereId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, gSphereMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(gSphereMesh.vao);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, gTextureSphereId);

    glDrawElements(GL_TRIANGLE_FAN, gSphereMesh.nIndices, GL_UNSIGNED_SHORT, 0);

    //--------Watermelon Ball End------------//

    // Deactivate the Vertex Array Object
    glBindVertexArray(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
}

// Implements the UCreateMesh function
void UCreatePolygonMesh(GLMesh& mesh)
{

    const float REPEAT = 1;

    // Toggles wireframe on/off
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Position and Color data
    GLfloat verts[] = {
        // X Y Z        // Texture coordinates
        0.0f, 0.0f, 4.0f,        1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,        0.0f, 1.0f,
        0.0f, -1.0f, 0.0f,       1.0f, 0.0f,

       -0.67f, -0.74f, 0.0f,    0.0f, 1.0f,
       -1.0f, 0.0f, 0.0f,       1.0f, 0.0f,
       -0.7f, 0.72f, 0.0f,      0.0f, 1.0f,

        0.0f, 1.0f, 0.0f,      1.0f, 0.0f,
        0.75f, 0.66f, 0.0f,     0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,       0.0f, 0.0f,

        0.76f, -0.64f, 0.0f,    1.0f, 0.0f,
       -0.00f, -1.00f, 0.0f,   0.0f, 1.0f,


    };

    // Index data to share position data
    GLushort indices[] = {


        0,1,2,
        1,2,3,
        1,3,4,
        1,4,5,
        1,5,6,
        1,6,7,
        1,7,8,
        1,8,9,
        1,9,10,
        1,10,11,
        1,11,12,
        1,12,13,
        1,13,14,
        1,14,15,
    };


    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerUV = 2;

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);


    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(2, mesh.vbos);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    mesh.nIndices = sizeof(indices) / sizeof(indices[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);
}

void UCreatePlaneMesh(GLMesh& mesh)
{
    const float REPEAT = 1;


    // Position and Color data
    GLfloat verts[] = {
        // Vertex Positions    // Colors (r,g,b,a)
         0.5f,  1.5f, 0.0f,   1.0f, 0.0f, // Vertex 0 Top Right Cyan
         0.5f, -1.5f, 0.0f,   0.0f, 1.0f,// Vertex 1 Bottom Left Blue
        -0.5f, -1.5f, 0.0f,   1.0f, 0.0f, // Vertex 2 Bottom Right Yellow
        -0.5f,  1.5f, 0.0f,   1.0f, 1.0f, // Vertex 3 Bottom Center Pink
         0.0f,  1.0f, 1.0f,   0.0f, 1.0f, // Vertex 4 Top Center White
    };

    // Index data to share position data
    GLushort indices[] = {
        // 3D Pyramid all colored
        0, 1, 2, // Triangle 1
        0, 3, 2, // Triangle 2
        0, 1, 4, // Triangle 3
        1, 2, 4, // Triangle 4
        2, 3, 4, // Triangle 5
        3, 0, 4, // Triangle 6
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerUV = 2;

    unsigned int buffer;
    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(2, mesh.vbos);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    mesh.nIndices = sizeof(indices) / sizeof(indices[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);
}

// Implements the UCreateMesh function
void UCreateTableMesh(GLMesh& mesh)
{
    const float REPEAT = 1;

    // Toggles wireframe on/off
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Position and Color data
    GLfloat verts[] = {

        // Plane

        // Vertex Positions    // Colors (r,g,b,a)
        0.5f,  0.5f, -1.0f,          1.0f, 0.0f, // 0 Top Back Left - Red
        0.5f, -0.5f, -1.0f,          0.0f, 1.0f, // 1 Top Back Right - Green
       -0.5f, -0.5f, -1.0f,          0.0f, 1.0f,  // 2 Top Front Right - Blue
       -0.5f,  0.5f, -1.0f,          1.0f, 0.0f, // 3 Top Front Left - Pink                         
        0.5f, -0.5f, -0.9f,          1.0f, 0.0f, // 4 Back Bottom Right - Purple
        0.5f,  0.5f, -0.9f,          0.0f, 1.0f, // 5 Back Bottom Left - Yellow
       -0.5f,  0.5f, -0.9f,          0.0f, 0.0f, // 6 Front Bottom Left - Pink/Blue
       -0.5f, -0.5f, -0.9f,          1.0f, 0.0f,   // 7 Front Bottom Right - Pink

    };



    // Index data to share position data
    GLushort indices[] = {
        0, 1, 3,   // Triangle 1
        1, 2, 3,   // Triangle 2
        0, 1, 4,   // Triangle 3
        0, 4, 5,   // Triangle 4
        0, 5, 6,   // Triangle 5
        0, 3, 6,   // Triangle 6
        4, 5, 6,   // Triangle 7
        4, 6, 7,   // Triangle 8
        2, 3, 6,   // Triangle 9
        2, 6, 7,   // Triangle 10
        1, 4, 7,   // Triangle 11
        1, 2, 7,   // Triangle 12

    };


    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerUV = 2;

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);


    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(2, mesh.vbos);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    mesh.nIndices = sizeof(indices) / sizeof(indices[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);
}

// Implements the UCreateMesh function
void UCreateCubeMesh(GLMesh& mesh)
{
    const float REPEAT = 1;

    // Toggles wireframe on/off
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Position and Color data
    GLfloat verts[] = {



        // Vertex Positions    // Colors (r,g,b,a)
         0.5f,  0.5f, 0.0f,        1.0f, 1.0f, 0.0f,       1.0f, 0.5f, // 0 Top Back Left - Red
         0.5f, -0.5f, 0.0f,        1.0f, 1.0f, 0.0f,       0.5f, 1.0f, // 1 Top Back Right - Green
        -0.5f, -0.5f, 0.0f,        1.0f, 1.0f, 0.0f,       0.5f, 1.0f, // 2 Top Front Right - Blue
        -0.5f,  0.5f, 0.0f,        1.0f, 1.0f, 1.0f,       0.5f, 1.0f, // 3 Top Front Left - Pink                                            
         0.5f, -0.5f, -1.0f,       0.0f, 1.0f, 0.0f,       0.5f, 0.5f, // 4 Back Bottom Right - Purple
         0.5f,  0.5f, -1.0f,       1.0f, 0.0f, 0.0f,       1.0f, 0.5f, // 5 Back Bottom Left - Yellow
        -0.5f,  0.5f, -1.0f,       1.0f, 0.0f, 0.0f,       0.5f, 1.0f, // 6 Front Bottom Left - Pink/Blue
        -0.5f, -0.5f, -1.0f,       0.0f, 0.0f, 0.0f,       0.0f, 1.0f, // 7 Front Bottom Right - Pink


    };

    // Index data to share position data
    GLushort indices[] = {
        0, 1, 3,   // Triangle 1
        1, 2, 3,   // Triangle 2
        0, 1, 4,   // Triangle 3
        0, 4, 5,   // Triangle 4
        0, 5, 6,   // Triangle 5
        0, 3, 6,   // Triangle 6
        4, 5, 6,   // Triangle 7
        4, 6, 7,   // Triangle 8
        2, 3, 6,   // Triangle 9
        2, 6, 7,   // Triangle 10
        1, 4, 7,   // Triangle 11
        1, 2, 7,   // Triangle 12

    };


    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    unsigned int buffer;
    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(2, mesh.vbos);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    mesh.nIndices = sizeof(indices) / sizeof(indices[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

// Implements the UCreateMesh function
void UCreateTabletMesh(GLMesh& mesh)
{
    const float REPEAT = 1;

    // Toggles wireframe on/off
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Position and Color data
    GLfloat verts[] = {



        // Vertex Positions    // Colors (r,g,b,a)
         0.5f,  0.5f, 0.0f,        1.0f, 1.0f, 0.0f,       1.0f, 0.5f, // 0 Top Back Left - Red
         0.5f, -0.5f, 0.0f,        1.0f, 1.0f, 0.0f,       0.5f, 1.0f, // 1 Top Back Right - Green
        -0.5f, -0.5f, 0.0f,        1.0f, 1.0f, 0.0f,       0.5f, 1.0f, // 2 Top Front Right - Blue
        -0.5f,  0.5f, 0.0f,        1.0f, 1.0f, 1.0f,       0.5f, 1.0f, // 3 Top Front Left - Pink                                            
         0.5f, -0.5f, -1.0f,       0.0f, 1.0f, 0.0f,       0.5f, 0.5f, // 4 Back Bottom Right - Purple
         0.5f,  0.5f, -1.0f,       1.0f, 0.0f, 0.0f,       1.0f, 0.5f, // 5 Back Bottom Left - Yellow
        -0.5f,  0.5f, -1.0f,       1.0f, 0.0f, 0.0f,       0.5f, 1.0f, // 6 Front Bottom Left - Pink/Blue
        -0.5f, -0.5f, -1.0f,       0.0f, 0.0f, 0.0f,       0.0f, 1.0f, // 7 Front Bottom Right - Pink


    };

    // Index data to share position data
    GLushort indices[] = {
        0, 1, 3,   // Triangle 1
        1, 2, 3,   // Triangle 2
        0, 1, 4,   // Triangle 3
        0, 4, 5,   // Triangle 4
        0, 5, 6,   // Triangle 5
        0, 3, 6,   // Triangle 6
        4, 5, 6,   // Triangle 7
        4, 6, 7,   // Triangle 8
        2, 3, 6,   // Triangle 9
        2, 6, 7,   // Triangle 10
        1, 4, 7,   // Triangle 11
        1, 2, 7,   // Triangle 12

    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    unsigned int buffer;
    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(2, mesh.vbos);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    mesh.nIndices = sizeof(indices) / sizeof(indices[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

// Implements the UCreateMesh function
void UCreateCylinderMesh() {

    const float REPEAT = 1;

    std::vector<GLfloat> verticesData;

    std::vector<glm::vec4> rainbowColors = {
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
        glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
        glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
    };

    float x, z;
    int N = 36;
    int ind = 0;
    for (int i = 0; i < N; i++) {

        x = 0.5 * cos((2 * 3.1428 * i) / N);
        z = 0.5 * sin((2 * 3.1428 * i) / N);

        verticesData.push_back(x);
        verticesData.push_back(0.5);
        verticesData.push_back(z);

        verticesData.push_back(rainbowColors[ind % 6].r);
        verticesData.push_back(rainbowColors[ind % 6].g);
        verticesData.push_back(rainbowColors[ind % 6].b);
        verticesData.push_back(rainbowColors[ind % 6].a);

        //printf("%i->P(%i, %f) = %f %f %f \n", ind, i, 360.0 * i / N, x, 0.5, z);

        ind++;

        verticesData.push_back(x);
        verticesData.push_back(-0.5);
        verticesData.push_back(z);

        verticesData.push_back(rainbowColors[ind % 6].r);
        verticesData.push_back(rainbowColors[ind % 6].g);
        verticesData.push_back(rainbowColors[ind % 6].b);
        verticesData.push_back(rainbowColors[ind % 6].a);

        //printf("%i->P(%i, %f) = %f %f %f \n", ind, i, 360.0 * i / N, x, 0.5, z);

        ind++;
    }

    verticesData.push_back(0);
    verticesData.push_back(-0.5);
    verticesData.push_back(0);

    verticesData.push_back(rainbowColors[ind % 6].r);
    verticesData.push_back(rainbowColors[ind % 6].g);
    verticesData.push_back(rainbowColors[ind % 6].b);
    verticesData.push_back(rainbowColors[ind % 6].a);

    ind++;

    verticesData.push_back(0);
    verticesData.push_back(0.5);
    verticesData.push_back(0);

    verticesData.push_back(rainbowColors[ind % 6].r);
    verticesData.push_back(rainbowColors[ind % 6].g);
    verticesData.push_back(rainbowColors[ind % 6].b);
    verticesData.push_back(rainbowColors[ind % 6].a);

    std::vector<GLushort> verticesIndices;

    int a, b, c, d;
    for (int i = 0; i < (2 * N); i = i + 2) {
        a = i;
        b = i + 1;

        c = (i + 2) % (N * 2);
        d = (i + 3) % (N * 2);

        verticesIndices.push_back(a);
        verticesIndices.push_back(b);
        verticesIndices.push_back(c);

        //printf("%i %i %i \n", a, b, c);

        verticesIndices.push_back(b);
        verticesIndices.push_back(c);
        verticesIndices.push_back(d);

        //printf("%i %i %i \n\n", b, c, d);            

        verticesIndices.push_back(b);
        verticesIndices.push_back(d);
        verticesIndices.push_back(2 * N);

        verticesIndices.push_back(a);
        verticesIndices.push_back(c);
        verticesIndices.push_back(2 * N + 1);
    }


    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerUV = 4;


    glGenVertexArrays(2, &gCylinderMesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(gCylinderMesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(2, gCylinderMesh.vbos);
    glBindBuffer(GL_ARRAY_BUFFER, gCylinderMesh.vbos[0]); // Activates the buffer

    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<int>(verticesData.size() * sizeof(GLfloat)),
        &verticesData[0],
        GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    gCylinderMesh.nIndices = verticesIndices.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gCylinderMesh.vbos[1]);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<int>(verticesIndices.size() * sizeof(GLushort)),
        &verticesIndices[0],
        GL_STATIC_DRAW);

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);
}

// Implements the UCreateMesh function
void UCreateSphereMesh() {

    const float REPEAT = 1;

    std::vector<GLfloat> verticesData;

    std::vector<glm::vec4> rainbowColors = {
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
        glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
        glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
    };

    float x, y, z;
    int N = 108;
    int ind = 0;
    for (int i = 0; i <= N; i++) {
        for (int j = 0; j < N; j++) {

            x = 0.5 * cos((2 * 3.1428 * j) / N) * sin((3.1428 * i) / N);
            y = 0.5 * cos((3.1428 * i) / N);
            z = 0.5 * sin((2 * 3.1428 * j) / N) * sin((3.1428 * i) / N);

            verticesData.push_back(x);
            verticesData.push_back(y);
            verticesData.push_back(z);

            verticesData.push_back(rainbowColors[ind % 6].r);
            verticesData.push_back(rainbowColors[ind % 6].g);
            verticesData.push_back(rainbowColors[ind % 6].b);
            verticesData.push_back(rainbowColors[ind % 6].a);

            //printf("%i->P(%i,%i) = (%f, %f) = %f %f %f \n", ind, i, j, 360.0 * i / N, 360.0 * j / N, x, y, z);

            ind++;
        }
    }

    std::vector<GLushort> verticesIndices;

    int a, b, c, d;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            a = i * N + j;
            b = i * N + ((j + 1) % N);

            c = (i + 1) * N + j;
            d = (i + 1) * N + ((j + 1) % N);

            verticesIndices.push_back(a);
            verticesIndices.push_back(b);
            verticesIndices.push_back(c);

            //printf("%i %i %i \n", a, b, c);

            verticesIndices.push_back(b);
            verticesIndices.push_back(c);
            verticesIndices.push_back(d);

            //printf("%i %i %i \n\n", b, c, d);            
        }
    }


    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerUV = 4;


    glGenVertexArrays(2, &gSphereMesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(gSphereMesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(2, gSphereMesh.vbos);
    glBindBuffer(GL_ARRAY_BUFFER, gSphereMesh.vbos[0]); // Activates the buffer

    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<int>(verticesData.size() * sizeof(GLfloat)),
        &verticesData[0],
        GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    gSphereMesh.nIndices = verticesIndices.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gSphereMesh.vbos[1]);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<int>(verticesIndices.size() * sizeof(GLushort)),
        &verticesIndices[0],
        GL_STATIC_DRAW);

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);
}

void UDestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(2, mesh.vbos);
}

/*Generate and load the texture*/
bool UCreateTexture(const char* filename, GLuint& gTextureId)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image)
    {
        flipImageVertically(image, width, height, channels);

        glGenTextures(1, &gTextureId);
        glBindTexture(GL_TEXTURE_2D, gTextureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

        return true;
    }

    // Error loading the image
    return false;
}


void UDestroyTexture(GLuint gTextureId)
{
    glGenTextures(1, &gTextureId);
}

// Implements the UCreateShaders function
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
    // Compilation and linkage error reporting
    int success = 0;
    char infoLog[512];

    // Create a Shader program object.
    programId = glCreateProgram();

    // Create the vertex and fragment shader objects
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Retrive the shader source
    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    // Compile the vertex shader, and print compilation errors (if any)
    glCompileShader(vertexShaderId); // compile the vertex shader
    // check for shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glCompileShader(fragmentShaderId); // compile the fragment shader
    // check for shader compile errors
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    // Attached compiled shaders to the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);   // links the shader program
    // check for linking errors
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glUseProgram(programId);    // Uses the shader program

    return true;
}

void UDestroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}

