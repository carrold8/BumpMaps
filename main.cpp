// Created in part by following tutorials in the LearnOpenGL online textbook https://learnopengl.com/About
// Dear ImGui library used to create User Interface


#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.
#include<list>
#include<algorithm>

// Assimp file loader

#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"


#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/quaternion.hpp>
#include <glm/glm/gtx/quaternion.hpp>
#include <glm/glm/gtx/euler_angles.hpp>
#include <glm/glm/gtx/norm.hpp>




/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
#define DEMO "Models/rabbit.dae"
//#define TEAPOT "plane1.obj"
#define LIGHT_BOX "light_box.dae"

// Define the Scene models
#define FENCES "Scene_Models/fences.dae"
#define CUBE "Scene_Models/cube.dae"
#define CYLINDER "Scene_Models/cylinder.dae"
#define PATH_PLANE "Scene_Models/floor.dae"
#define GRASS_PLANE "Scene_Models/grass.dae"
#define TORUS "Scene_Models/cone_flat.dae"
#define WALLS "Scene_Models/walls.dae"
#define BALL "Scene_Models/ball.dae"


vec3 light_pos = vec3(1.2f, 20.0f, 10.0f);
vec3 light_pos2 = vec3(-1.0f, 25.0f, 2.5f);
vec3 blue = vec3(0.0f, 0.0f, 1.0f);
vec3 red = vec3(1.0f, 0.0f, 0.0f);
vec3 white = vec3(1.0f, 1.0f, 1.0f);


vec3 cameraPos = vec3(0.0f, 1.0f, 5.0f);
vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
vec3 cameraDirec = normalise(cameraPos - cameraTarget);
vec3 up = vec3(0.0f, 1.0f, 0.0f);
vec3 camRight = normalise(cross(up, cameraDirec));
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;


vec3 lightPositions[] = {
    vec3(0.0f,  20.0f, 10.0f),
    vec3(20.0f,  20.0f, 10.0f),
    vec3(0.0f, 0.0f, 10.0f),
    vec3(20.0f, 0.0f, 10.0f),
};

vec3 lightColours[] = {
    vec3(300.0f, 300.0f, 300.0f),
    vec3(300.0f, 300.0f, 300.0f),
    vec3(300.0f, 300.0f, 300.0f),
    vec3(300.0f, 300.0f, 300.0f),
};


// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


// mouse state
bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;


float camX = 0.0f;


#pragma region SimpleTypes
typedef struct ModelData
{
    size_t mPointCount = 0;
    std::vector<vec3> mVertices;
    std::vector<vec3> mNormals;
    std::vector<vec2> mTextureCoords;
    std::vector<vec3> mTangents;
    std::vector<vec3> mBitangents;
} ModelData;
#pragma endregion SimpleTypes

using namespace std;


GLuint shaderLight;
GLuint shaderProgramP;
GLuint shaderTexture;
GLuint shaderNormal;

GLuint teapot_texture;

unsigned int skyboxShader;



// Textures
unsigned int skyboxTexture, brickTextureMap, brickNormalMap, grassTextureMap, grassNormalMap, stoneTextureMap, stoneNormalMap, rockTextureMap, rockNormalMap;
unsigned int metalTextureMap, metalNormalMap, paintTextureMap, paintNormalMap, groundTextureMap, groundNormalMap;

ModelData light_box, demo;

// Scene model data
ModelData path_plane, fences_model, grass_plane, walls_model, torus_model, ball_model, cube_model, cylinder_model;
GLuint path_vao, fences_vao, grass_vao, walls_vao, torus_vao, ball_vao, cube_vao, cylinder_vao;

GLuint  skybox_vao, skybox_vbo, demo_vao, demo_vao_tex, light_box_vao;
float skybox;

vector<std::string> facesSkyBox;

vector<std::string> faces1
{
    ("skybox/right.jpg"),
    ("skybox/left.jpg"),
    ("skybox/top.jpg"),
    ("skybox/bottom.jpg"),
    ("skybox/front.jpg"),
    ("skybox/back.jpg")
};

vector<std::string> faces2
{
    ("skybox2/field_right.jpg"),
    ("skybox2/field_left.jpg"),
    ("skybox2/field_top.jpg"),
    ("skybox2/field_bottom.jpg"),
    ("skybox2/field_front.jpg"),
    ("skybox2/field_back.jpg")
};

vector<std::string> faces3
{
    ("skyboxCar/right.jpg"),
    ("skyboxCar/left.jpg"),
    ("skyboxCar/top.jpg"),
    ("skyboxCar/bottom.jpg"),
    ("skyboxCar/front.jpg"),
    ("skyboxCar/back.jpg")
};

//ModelData mesh_data;
unsigned int mesh_vao = 0;
int width = 800;
int height = 600;

GLuint loc1, loc2, loc3, loc4, loc5;

GLfloat rotate_y = 0.0f;
GLfloat rotate_light = 0.0f;

vec3 targetPos;



// Function declarations


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//Load Mesh Function 
ModelData load_mesh(const char* file_name);

// Shader Functions
char* readShaderSource(const char* shaderFile);
static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
GLuint CompileShaders(const char* vertex, const char* fragment);

// VBO functions
GLuint generateObjectBufferMesh(ModelData mesh_data, GLuint shader);

// Degreees and Radian Conversion
float degrees(float radians);
double radians(double degree);


// Texture Function
unsigned int loadTexture(char const* path);
unsigned int loadCubemap(vector<std::string> faces); 

// Display Functions
void displayNormalMap();
void displayScene();
void display();

// Init and Update Function
void init();
void updateScene();

// Multiply Vector by a float
vec3 vecXfloat(float f, vec3 v1);

// Mouse Callback and Key input
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);




const unsigned int screen_width = 1300;
const unsigned int screen_height = 975;




int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "Normal Mapping", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"


    init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    bool draw = true;

    float f = 2.5f;

    while (!glfwWindowShouldClose(window))
    {
       
        
        glEnable(GL_DEPTH_TEST); // enable depth-testing
        glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        
        glfwSetCursorPosCallback(window, mouse_callback);
        processInput(window);


        displayNormalMap();
        //displayScene();
        
        updateScene();


        ImGui::Begin("ImGui Window");
        ImGui::Text("ImGui text line");
        ImGui::SliderFloat("Rotate", &rotate_y, -90.0f, 90.0f);
        ImGui::SliderFloat3("Light Position", light_pos.v, -30.0f, 30.0f);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      
        glfwSwapBuffers(window);
        glfwPollEvents();


    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}




void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    float cameraSpeed = 10.5f * deltaTime;

    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += vecXfloat(cameraSpeed, cameraFront);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= vecXfloat(cameraSpeed, cameraFront);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= normalise(cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += normalise(cross(cameraFront, cameraUp)) * cameraSpeed;
    }
        

}

ModelData load_mesh(const char* file_name) {

    ModelData modelData;

    /* Use assimp to read the model file, forcing it to be read as    */
    /* triangles. The second flag (aiProcess_PreTransformVertices) is */
    /* relevant if there are multiple meshes in the model file that   */
    /* are offset from the origin. This is pre-transform them so      */
    /* they're in the right position.                                 */
    const aiScene* scene = aiImportFile(
        file_name,
        aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_CalcTangentSpace
    );

    if (!scene) {
        fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
        return modelData;
    }

    printf("  %i materials\n", scene->mNumMaterials);
    printf("  %i meshes\n", scene->mNumMeshes);
    printf("  %i textures\n", scene->mNumTextures);

    for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
        const aiMesh* mesh = scene->mMeshes[m_i];
        printf("    %i vertices in mesh\n", mesh->mNumVertices);
        modelData.mPointCount += mesh->mNumVertices;
        for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
            if (mesh->HasPositions()) {
                const aiVector3D* vp = &(mesh->mVertices[v_i]);
                modelData.mVertices.push_back(vec3(vp->x, vp->y, vp->z));
            }
            if (mesh->HasNormals()) {
                const aiVector3D* vn = &(mesh->mNormals[v_i]);
                modelData.mNormals.push_back(vec3(vn->x, vn->y, vn->z));
            }
            if (mesh->HasTextureCoords(0)) {
                const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
                modelData.mTextureCoords.push_back(vec2(vt->x, vt->y));
                //	cout << "Texture coordinates : " << modelData.mTextureCoords[v_i].v[0] << ", " << modelData.mTextureCoords[v_i].v[1] <<  endl;

            }
            if (mesh->HasTangentsAndBitangents()) {
                /* You can extract tangents and bitangents here              */
                /* Note that you might need to make Assimp generate this     */
                /* data for you. Take a look at the flags that aiImportFile  */
                /* can take.                                                 */
            //	cout << "Checking tangents and stuff\n";
                const aiVector3D* vtan = &(mesh->mTangents[v_i]);
                modelData.mTangents.push_back(vec3(vtan->x, vtan->y, vtan->z));

                const aiVector3D* vbt = &(mesh->mBitangents[v_i]);
                modelData.mBitangents.push_back(vec3(vbt->x, vbt->y, vbt->z));
                //	cout << "Tangent coordinates : " << modelData.mTangents[v_i].v[0] << ", " << modelData.mTangents[v_i].v[1] << ", " << modelData.mTangents[v_i].v[2] << endl;
                //	cout << "BiTangent coordinates : " << modelData.mBiTangents[v_i].v[0] << ", " << modelData.mBiTangents[v_i].v[1] << ", " << modelData.mBiTangents[v_i].v[2] << endl;
            }
        }
    }

    aiReleaseImport(scene);
    return modelData;
}




char* readShaderSource(const char* shaderFile) {
    FILE* fp;
    fopen_s(&fp, shaderFile, "rb");

    if (fp == NULL) { return NULL; }

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);
    char* buf = new char[size + 1];
    fread(buf, 1, size, fp);
    buf[size] = '\0';

    fclose(fp);

    return buf;
}


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    // create a shader object
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        std::cerr << "Error creating shader..." << std::endl;
        std::cerr << "Press enter/return to exit..." << std::endl;
        std::cin.get();
        exit(1);
    }
    const char* pShaderSource = readShaderSource(pShaderText);


    // Bind the source code to the shader, this happens before compilation
    glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
    // compile the shader and check for errors


    glCompileShader(ShaderObj);
    GLint success;
    // check for shader related errors using glGetShaderiv

    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024] = { '\0' };
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        std::cerr << "Error compiling "
            << (ShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader program: " << InfoLog << std::endl;
        std::cerr << "Press enter/return to exit..." << std::endl;
        std::cin.get();
        exit(1);
    }
    // Attach the compiled shader object to the program object
    glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders(const char* vertex, const char* fragment)
{
    //Start the process of setting up our shaders by creating a program ID
    //Note: we will link all the shaders together into this ID
    GLuint shaderProgram = glCreateProgram();
    if (shaderProgram == 0) {
        std::cerr << "Error creating shader program..." << std::endl;
        std::cerr << "Press enter/return to exit..." << std::endl;
        std::cin.get();
        exit(1);
    }


    // Create two shader objects, one for the vertex, and one for the fragment shader
    AddShader(shaderProgram, vertex, GL_VERTEX_SHADER);
    AddShader(shaderProgram, fragment, GL_FRAGMENT_SHADER);


    GLint Success = 0;
    GLchar ErrorLog[1024] = { '\0' };
    // After compiling all shader objects and attaching them to the program, we can finally link it
    glLinkProgram(shaderProgram);
    // check for program related errors using glGetProgramiv
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
        std::cerr << "Press enter/return to exit..." << std::endl;
        std::cin.get();
        exit(1);
    }

    // program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
    glValidateProgram(shaderProgram);
    // check for program related errors using glGetProgramiv
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
        std::cerr << "Press enter/return to exit..." << std::endl;
        std::cin.get();
        exit(1);
    }
    // Finally, use the linked shader program
    // Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
    glUseProgram(shaderProgram);
    return shaderProgram;
}


GLuint generateObjectBufferMesh(ModelData mesh_data, GLuint shader) {
    /*----------------------------------------------------------------------------
    LOAD MESH HERE AND COPY INTO BUFFERS
    ----------------------------------------------------------------------------*/

    //Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
    //Might be an idea to do a check for that before generating and binding the buffer.



    unsigned int vp_vbo = 0;
    loc1 = glGetAttribLocation(shader, "vertex_position");
    loc2 = glGetAttribLocation(shader, "vertex_normal");
    loc3 = glGetAttribLocation(shader, "vertex_texture");
    loc4 = glGetAttribLocation(shader, "vertex_tangent");
    loc5 = glGetAttribLocation(shader, "vertex_bitangent");

    glGenBuffers(1, &vp_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);
    unsigned int vn_vbo = 0;
    glGenBuffers(1, &vn_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);

    unsigned int vt_vbo = 0;
    glGenBuffers(1, &vt_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vt_vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec2), &mesh_data.mTextureCoords[0], GL_STATIC_DRAW);

    unsigned int vtan_vbo = 0;
    glGenBuffers(1, &vtan_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vtan_vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mTangents[0], GL_STATIC_DRAW);

    unsigned int vbt_vbo = 0;
    glGenBuffers(1, &vbt_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbt_vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mBitangents[0], GL_STATIC_DRAW);


    unsigned int vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(loc1);
    glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
    glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(loc2);
    glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
    glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(loc3);
    glBindBuffer(GL_ARRAY_BUFFER, vt_vbo);
    glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(loc4);
    glBindBuffer(GL_ARRAY_BUFFER, vtan_vbo);
    glVertexAttribPointer(loc4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(loc5);
    glBindBuffer(GL_ARRAY_BUFFER, vbt_vbo);
    glVertexAttribPointer(loc5, 3, GL_FLOAT, GL_FALSE, 0, NULL);


    return vao;

}

float degrees(float radians) {
    float pi = 3.14159265359;
    return (radians * (180 / pi));
}

double radians(double degree) {
    double pi = 3.14159265359;
    return (degree * (pi / 180));
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);


    cout << "Lenght of faces array: " << faces.size() << endl;

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


void displayNormalMap() {




    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glUseProgram(shaderLight);

    mat4 view = identity_mat4();
    mat4 persp_proj = perspective(fov, (float)screen_width / (float)screen_height, 0.1f, 100.0f);
    view = look_at(cameraPos, cameraPos + cameraFront, cameraUp);


    int matrix_location2 = glGetUniformLocation(shaderLight, "model");
    int view_mat_location2 = glGetUniformLocation(shaderLight, "view");
    int proj_mat_location2 = glGetUniformLocation(shaderLight, "proj");

    mat4 lightBox = identity_mat4();
    lightBox = translate(lightBox, light_pos);
//    lightBox = rotate_y_deg(lightBox, rotate_light);

    glUniformMatrix4fv(proj_mat_location2, 1, GL_FALSE, persp_proj.m);
    glUniformMatrix4fv(view_mat_location2, 1, GL_FALSE, view.m);
    glUniformMatrix4fv(matrix_location2, 1, GL_FALSE, lightBox.m);

    glBindVertexArray(light_box_vao);
    glDrawArrays(GL_TRIANGLES, 0, light_box.mPointCount);






    glUseProgram(shaderNormal);


    //Declare your uniform variables that will be used in your shader
    // Vertex Shader Uniforms
    int matrix_location = glGetUniformLocation(shaderNormal, "model");
    int view_mat_location = glGetUniformLocation(shaderNormal, "view");
    int proj_mat_location = glGetUniformLocation(shaderNormal, "proj");
    int camPos_shader = glGetUniformLocation(shaderNormal, "viewPos");
    int sampler_diffuse = glGetUniformLocation(shaderNormal, "diffuseMap");
    int sampler_normal = glGetUniformLocation(shaderNormal, "normalMap");
    int light_Shader = glGetUniformLocation(shaderNormal, "lightPos");


    // Root of the Hierarchy
    
    mat4 model = identity_mat4();
    model = rotate_y_deg(model, rotate_y);
    model = translate(model, vec3(-1, 0, 0));



    // update uniforms & draw
    glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
    glUniform3fv(camPos_shader, 1, cameraPos.v);
    glUniform3fv(light_Shader, 1, light_pos.v);
    glUniform1i(sampler_diffuse, 0);
    glUniform1i(sampler_normal, 1);


    glBindVertexArray(demo_vao_tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, stoneTextureMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, stoneNormalMap);
    glBindVertexArray(demo_vao_tex);
    glDrawArrays(GL_TRIANGLES, 0, demo.mPointCount);




    // Use Shader without normal mapping
    glUseProgram(shaderTexture);

    //Declare your uniform variables that will be used in your shader
    // Vertex Shader Uniforms
    int matrix_location1 = glGetUniformLocation(shaderTexture, "model");
    int view_mat_location1 = glGetUniformLocation(shaderTexture, "view");
    int proj_mat_location1 = glGetUniformLocation(shaderTexture, "proj");
    int sampler_texture1 = glGetUniformLocation(shaderTexture, "texture1");


    // Root of the Hierarchy
    mat4 model_2 = identity_mat4();

    view = look_at(cameraPos, cameraPos + cameraFront, cameraUp);
    model_2 = rotate_y_deg(model_2, rotate_y);
    model_2 = translate(model_2, vec3(1, 0, 0));



    // update uniforms & draw
    glUniformMatrix4fv(proj_mat_location1, 1, GL_FALSE, persp_proj.m);
    glUniformMatrix4fv(view_mat_location1, 1, GL_FALSE, view.m);
    glUniformMatrix4fv(matrix_location1, 1, GL_FALSE, model_2.m);
    glUniform1i(sampler_texture1, 0);


    glBindVertexArray(demo_vao_tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, stoneTextureMap);
    glDrawArrays(GL_TRIANGLES, 0, demo.mPointCount);



    


}



void displayScene() {




    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glUseProgram(shaderLight);

    mat4 view = identity_mat4();
    mat4 persp_proj = perspective(fov, (float)screen_width / (float)screen_height, 0.1f, 100.0f);
    view = look_at(cameraPos, cameraPos + cameraFront, cameraUp);


    int matrix_location2 = glGetUniformLocation(shaderLight, "model");
    int view_mat_location2 = glGetUniformLocation(shaderLight, "view");
    int proj_mat_location2 = glGetUniformLocation(shaderLight, "proj");

    mat4 lightBox = identity_mat4();
    lightBox = translate(lightBox, light_pos2);
    //    lightBox = rotate_y_deg(lightBox, rotate_light);

    glUniformMatrix4fv(proj_mat_location2, 1, GL_FALSE, persp_proj.m);
    glUniformMatrix4fv(view_mat_location2, 1, GL_FALSE, view.m);
    glUniformMatrix4fv(matrix_location2, 1, GL_FALSE, lightBox.m);

    glBindVertexArray(light_box_vao);
    glDrawArrays(GL_TRIANGLES, 0, light_box.mPointCount);






    glUseProgram(shaderNormal);


    //Declare your uniform variables that will be used in your shader
    // Vertex Shader Uniforms
    int matrix_location = glGetUniformLocation(shaderNormal, "model");
    int view_mat_location = glGetUniformLocation(shaderNormal, "view");
    int proj_mat_location = glGetUniformLocation(shaderNormal, "proj");
    int camPos_shader = glGetUniformLocation(shaderNormal, "viewPos");
    int sampler_diffuse = glGetUniformLocation(shaderNormal, "diffuseMap");
    int sampler_normal = glGetUniformLocation(shaderNormal, "normalMap");
    int light_Shader = glGetUniformLocation(shaderNormal, "lightPos");


   
    // Create Path 
    mat4 path = identity_mat4();

    // update uniforms & draw
    glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, path.m);
    glUniform3fv(camPos_shader, 1, cameraPos.v);
    glUniform3fv(light_Shader, 1, light_pos2.v);
    glUniform1i(sampler_diffuse, 0);
    glUniform1i(sampler_normal, 1);


    glBindVertexArray(path_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, groundTextureMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, groundNormalMap);
    glBindVertexArray(path_vao);
    glDrawArrays(GL_TRIANGLES, 0, path_plane.mPointCount);


    // Create Fences Model
    mat4 fences = identity_mat4();

    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, fences.m);
    glBindVertexArray(fences_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, stoneTextureMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, stoneNormalMap);
    glBindVertexArray(fences_vao);
    glDrawArrays(GL_TRIANGLES, 0, fences_model.mPointCount);

    // Create Cube
    mat4 cube = identity_mat4();
    cube = rotate_y_deg(cube, rotate_y);
    cube = translate(cube, vec3(8.5f, 4.0f, 8.5f));

    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, cube.m);
    glBindVertexArray(cube_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, brickTextureMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, brickNormalMap);
    glBindVertexArray(cube_vao);
    glDrawArrays(GL_TRIANGLES, 0, cube_model.mPointCount);


    // Create Ball
    mat4 ball = identity_mat4();
    ball = rotate_y_deg(ball, rotate_y);
    ball = translate(ball, vec3(-8.5f, 4.0f, -8.5f));

    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, ball.m);
    glBindVertexArray(ball_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, metalTextureMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, metalNormalMap);
    glBindVertexArray(ball_vao);
    glDrawArrays(GL_TRIANGLES, 0, ball_model.mPointCount);


    // Create Torus
    mat4 torus = identity_mat4();
    torus = rotate_z_deg(torus, rotate_y);
    torus = rotate_x_deg(torus, rotate_y);
    torus = translate(torus, vec3(-8.5f, 4.0f, 8.5f));

    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, torus.m);
    glBindVertexArray(cylinder_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, paintTextureMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, paintNormalMap);
    glBindVertexArray(cylinder_vao);
    glDrawArrays(GL_TRIANGLES, 0, cylinder_model.mPointCount);


    // Create Cylinder
    mat4 cylind = identity_mat4();
    cylind = rotate_y_deg(cylind, rotate_y);
    cylind = translate(cylind, vec3(8.5f, 4.0f, -8.5f));

    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, cylind.m);
    glBindVertexArray(cylinder_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rockTextureMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, rockNormalMap);
    glBindVertexArray(cylinder_vao);
    glDrawArrays(GL_TRIANGLES, 0, cylinder_model.mPointCount);



    // Draw Skybox

    glDepthFunc(GL_LEQUAL);
    glUseProgram(skyboxShader);

    glm::vec3 camPosGLM = glm::vec3(cameraPos.v[0], cameraPos.v[1], cameraPos.v[2]);
    glm::vec3 camFrontGLM = glm::vec3(cameraFront.v[0], cameraFront.v[1], cameraFront.v[2]);
    glm::vec3 camUpGLM = glm::vec3(cameraUp.v[0], cameraUp.v[1], cameraUp.v[2]);

    glm::mat4 viewSky = glm::mat4(1.0f);
    mat4 modelSky = identity_mat4();
    viewSky = glm::mat4(glm::mat3(glm::lookAt(camPosGLM, camPosGLM + camFrontGLM, camUpGLM)));

    int matrix_location_sky = glGetUniformLocation(skyboxShader, "model");
    int view_mat_location_sky = glGetUniformLocation(skyboxShader, "view");
    int proj_mat_location_sky = glGetUniformLocation(skyboxShader, "projection");
    int sampler_cube = glGetUniformLocation(skyboxShader, "skybox");

    glUniformMatrix4fv(proj_mat_location_sky, 1, GL_FALSE, persp_proj.m);
    glUniformMatrix4fv(view_mat_location_sky, 1, GL_FALSE, &viewSky[0][0]);
    glUniformMatrix4fv(matrix_location_sky, 1, GL_FALSE, modelSky.m);
    glUniform1f(sampler_cube, 0);

    glBindVertexArray(skybox_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);




}



void display() {




    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgramP);



    //Declare your uniform variables that will be used in your shader
    // Vertex Shader Uniforms
    int matrix_location = glGetUniformLocation(shaderProgramP, "model");
    int view_mat_location = glGetUniformLocation(shaderProgramP, "view");
    int proj_mat_location = glGetUniformLocation(shaderProgramP, "proj");


    // Root of the Hierarchy
    mat4 view = identity_mat4();
    mat4 persp_proj = perspective(fov, (float)width / (float)height, 0.1f, 1000.0f);
    mat4 model = identity_mat4();

    view = look_at(cameraPos, cameraPos + cameraFront, cameraUp);
    //model = rotate_y_deg(model, rotate_y);



            // update uniforms & draw
    glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);


    glBindVertexArray(demo_vao);
    glDrawArrays(GL_TRIANGLES, 0, demo.mPointCount);



}


void updateScene() {

    static double last_time = 0;
    double curr_time = glfwGetTime();
    if (last_time == 0)
        last_time = curr_time;
    float delta = (curr_time - last_time);
    last_time = curr_time;

    float currentFrame = glfwGetTime();


    deltaTime = curr_time - lastFrame;
    lastFrame = currentFrame;

    // Rotate the model slowly around the y axis at 20 degrees per second
 //   rotate_y += 20.5f * deltaTime;
 //   rotate_y = fmodf(rotate_y, 360.0f);

    rotate_light += 50.0f * deltaTime;
    rotate_light = fmodf(rotate_light, 360.0f);


 //   cout << "delta" << delta << endl;

}




void init()
{
    // Set up the shaders
    shaderTexture = CompileShaders("Shader_Files/Tex_Vertex.txt", "Shader_Files/Tex_Fragment.txt");
    shaderNormal = CompileShaders("Shader_Files/Normal_Vertex.txt", "Shader_Files/Normal_Fragment.txt");
    shaderLight = CompileShaders("Shader_Files/Light_Vertex.txt", "Shader_Files/Light_Fragment.txt");


    // Load Demo object
    demo = load_mesh(DEMO);
    demo_vao = generateObjectBufferMesh(demo, shaderProgramP);


    // Demo vao for texture and normal mapping
    demo_vao_tex = generateObjectBufferMesh(demo, shaderNormal);

    light_box = load_mesh(LIGHT_BOX);
    light_box_vao = generateObjectBufferMesh(light_box, shaderLight);

    // Load all scene 
    path_plane = load_mesh(PATH_PLANE);
    path_vao = generateObjectBufferMesh(path_plane, shaderNormal);

    grass_plane = load_mesh(GRASS_PLANE);
    grass_vao = generateObjectBufferMesh(grass_plane, shaderNormal);

    fences_model = load_mesh(FENCES);
    fences_vao = generateObjectBufferMesh(fences_model, shaderNormal);

    walls_model = load_mesh(WALLS);
    walls_vao = generateObjectBufferMesh(walls_model, shaderNormal);

    ball_model = load_mesh(BALL);
    ball_vao = generateObjectBufferMesh(ball_model, shaderNormal);

    torus_model = load_mesh(TORUS);
    torus_vao = generateObjectBufferMesh(torus_model, shaderNormal);

    cube_model = load_mesh(CUBE);
    cube_vao = generateObjectBufferMesh(cube_model, shaderNormal);

    cylinder_model = load_mesh(CYLINDER);
    cylinder_vao = generateObjectBufferMesh(cylinder_model, shaderNormal);

    


    // Load Textures with normal maps
    brickTextureMap = loadTexture("Textures/redbrick.jpg");
    brickNormalMap = loadTexture("Textures/redbrick_normal.jpg");


    grassTextureMap = loadTexture("Textures/grass.jpg");
    grassNormalMap = loadTexture("Textures/grass_normal.jpg");

    stoneTextureMap = loadTexture("Textures/chequered.jpg");
    stoneNormalMap = loadTexture("Textures/rabbit_normal.jpg");

    rockTextureMap = loadTexture("Textures/rocks.jpg");
    rockNormalMap = loadTexture("Textures/rocks_normal.jpg");

    metalTextureMap = loadTexture("Textures/orange.jpg");
    metalNormalMap = loadTexture("Textures/orange_normal.jpg");


    paintTextureMap = loadTexture("Textures/pink_paint.jpg");
    paintNormalMap = loadTexture("Textures/pink_paint_normal.jpg");

    groundTextureMap = loadTexture("Textures/ground.jpg");
    groundNormalMap = loadTexture("Textures/ground_normal.jpg");






        // Skybox 
    skyboxShader = CompileShaders("Shader_Files/Sky_Vertex.txt", "Shader_Files/Sky_Fragment.txt");

    float skybox[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };


    unsigned int location = glGetAttribLocation(skyboxShader, "vertex_position");

    glGenVertexArrays(1, &skybox_vao);
    glGenBuffers(1, &skybox_vbo);
    glBindVertexArray(skybox_vao);
    glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox), &skybox, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    skyboxTexture = loadCubemap(faces1);


}




vec3 vecXfloat(float f, vec3 v1) {

    vec3 result = vec3((v1.v[0] * f), (v1.v[1] * f), (v1.v[2] * f));
    return result;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {


    float x = static_cast<float>(xposIn);
    float y = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    float xoffset = x - lastX;
    float yoffset = lastY - y;
    lastX = x;
    lastY = y;

    float sensitivity = 0.5f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    vec3 front;
    front.v[0] = cos(radians(yaw)) * cos(radians(pitch));
    front.v[1] = sin(radians(pitch));
    front.v[2] = sin(radians(yaw)) * cos(radians(pitch));
    cameraFront = normalise(front);


}