//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "spotLight.h"
#include "pointLight.h"
#include "directionLight.h"
#include "cube.h"
#include "sphere.h"
#include "stb_image.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const * path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
void bed(Shader &lightingShader, glm::mat4 alTogether, Cube &cube);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

// camera
Camera camera(glm::vec3(0.0f, 1.1f, 5.2f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm:: vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// lights
DirectionLight dirlight(

    -0.2f, -1.0f, -0.3f,     // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.4f, 0.4f, 0.4f,        // diffuse
    0.5f, 0.5f, 0.5f         // specular
);
// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3( 1.50f,  1.50f,  0.0f),
    glm::vec3( 1.5f,  -1.5f,  0.0f),
    glm::vec3(-1.5f,  1.5f,  0.0f),
    glm::vec3( -1.5f,  -1.5f,  0.0f)
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);
SpotLight spotlight(
        
                    0.0f, 1.1f, 5.2f,  // position
                    0.0f, 0.0f, -1.0f, // direction
                    0.0f, 0.0f, 0.0f,  // ambient
                    1.0f, 1.0f, 1.0f,  // diffuse
                    1.0f, 1.0f, 1.0f,  // specular
                    1.0f,   //k_c
                    0.09f,  //k_l
                    0.032f, //k_q
                    12.5f,  //cutoff
                    15.0f   //outer cutoff
);

// light settings
bool dirLightOn = true;
bool pointLightOn = true;
bool spotLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("/Users/macbookpro/Desktop/test/test/vertexShaderForLighting.vs", "/Users/macbookpro/Desktop/test/test/fragmentShaderForLighting.fs");
    //Shader lightingShader("/Users/macbookpro/Desktop/test/test/vertexShaderForGouraudShading.vs", "/Users/macbookpro/Desktop/test/test/fragmentShaderForGouraudShading.fs");
    Shader lightingShaderWithTexture("/Users/macbookpro/Desktop/test/test/vertexShaderForPhongShadingWithTexture.vs", "/Users/macbookpro/Desktop/test/test/fragmentShaderForPhongShadingWithTexture.fs");
    Shader ourShader("/Users/macbookpro/Desktop/test/test/vertexShader.vs", "/Users/macbookpro/Desktop/test/test/fragmentShader.fs");
    
    string diffuseMapPath = "/Users/macbookpro/Desktop/test/test/container2.png";
    string specularMapPath = "/Users/macbookpro/Desktop/test/test/container2_specular.png";
    
    string diffuseMapPath_emoji = "/Users/macbookpro/Desktop/test/test/emoji.png";
    string specularMapPath_white_background = "/Users/macbookpro/Desktop/test/test/whiteBackground.png";
    
    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    
    unsigned int diffMap1 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap1 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube1 = Cube(diffMap1, specMap1, 32.0f, 0.0f, 0.0f, 2.0f, 2.0f);
    
    // GL_MIRRORED_REPEAT
    unsigned int diffMap2 = loadTexture(diffuseMapPath.c_str(), GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap2 = loadTexture(specularMapPath.c_str(), GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube2 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 2.0f, 2.0f);
    
    // GL_CLAMP_TO_EDGE
    unsigned int diffMap3 = loadTexture(diffuseMapPath.c_str(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap3 = loadTexture(specularMapPath.c_str(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube3 = Cube(diffMap3, specMap3, 32.0f, 0.0f, 0.0f, 2.0f, 2.0f);
    
    // GL_NEAREST
    unsigned int diffMap4 = loadTexture(diffuseMapPath_emoji.c_str(), GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
    unsigned int specMap4 = loadTexture(specularMapPath_white_background.c_str(), GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
    Cube cube4 = Cube(diffMap4, specMap4, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    
    // GL_LINEAR
    unsigned int diffMap5 = loadTexture(diffuseMapPath_emoji.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    unsigned int specMap5 = loadTexture(specularMapPath_white_background.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    Cube cube5 = Cube(diffMap5, specMap5, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    
    
    //Sphere sphere = Sphere();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        
        // directional light
        dirlight.setUpDirectionLight(lightingShader);
        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);
        // spotLight
        spotlight.position = camera.Position;
        spotlight.direction = camera.Front;
        spotlight.setUpSpotLight(lightingShader);
        
        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        
        //bed(lightingShader, model, cube);
        
        /*glm::mat4 modelForSphere = glm::mat4(1.0f);
        modelForSphere = glm::translate(model, glm::vec3(1.7f, 1.2f, 0.5f));
        sphere.drawSphere(lightingShader, modelForSphere);*/
        
        //container with texture
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        
        // directional light
        dirlight.setUpDirectionLight(lightingShaderWithTexture);
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);
        // spotLight
        spotlight.setUpSpotLight(lightingShaderWithTexture);
        
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);
        
        glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(model, glm::vec3(-1.5f, 1.2f, 0.5f));
        cube.drawCubeWithTexture(lightingShaderWithTexture,modelMatrixForContainer);
        
        glm::mat4 modelMatrixForContainer1 = glm::mat4(1.0f);
        modelMatrixForContainer1 = glm::translate(model, glm::vec3(-0.3f, 1.2f, 0.5f));
        cube1.drawCubeWithTexture(lightingShaderWithTexture,modelMatrixForContainer1);
        
        // GL_MIRRORED_REPEAT
        glm::mat4 modelMatrixForContainer2 = glm::mat4(1.0f);
        modelMatrixForContainer2 = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.5f));
        cube2.drawCubeWithTexture(lightingShaderWithTexture,modelMatrixForContainer2);
        
        // GL_CLAMP_TO_EDGE
        glm::mat4 modelMatrixForContainer3 = glm::mat4(1.0f);
        modelMatrixForContainer3 = glm::translate(model, glm::vec3(-0.3f, 0.0f, 0.5f));
        cube3.drawCubeWithTexture(lightingShaderWithTexture,modelMatrixForContainer3);
        
        // GL_NEAREST
        glm::mat4 modelMatrixForContainer4 = glm::mat4(1.0f);
        modelMatrixForContainer4 = glm::translate(model, glm::vec3(-1.5f, -1.2f, 0.5f));
        cube4.drawCubeWithTexture(lightingShaderWithTexture,modelMatrixForContainer4);
        
        // GL_LINEAR
        glm::mat4 modelMatrixForContainer5 = glm::mat4(1.0f);
        modelMatrixForContainer5 = glm::translate(model, glm::vec3(-0.3f, -1.2f, 0.5f));
        cube5.drawCubeWithTexture(lightingShaderWithTexture,modelMatrixForContainer5);
        
        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
            
        // we now draw as many light bulbs as we have point lights.
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            cube.drawCube(ourShader, model, 0.8f, 0.8f, 0.8f);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void bed(Shader &lightingShader, glm::mat4 alTogether, Cube &cube)
{
    float baseHeight = 0.3;
    float width = 1;
    float length = 2;
    float pillowWidth = 0.3;
    float pillowLength = 0.15;
    float blanketWidth = 0.8;
    float blanketLength = 0.7;
    float headHeight = 0.6;
    
    //base
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width,baseHeight,length));
    translate = glm::translate(model, glm::vec3(-0.5,0,-0.5));
    model = alTogether * scale * translate;
    cube.setMaterialisticProperty(glm::vec3(0.545,0.271,0.075),glm::vec3(0.545,0.271,0.075),glm::vec3(0.5,0.5,0.5),32.0);
    cube.drawCubeWithMaterialisticProperty(lightingShader, model);
    
    //foam
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0,baseHeight,0));
    scale = glm::scale(model, glm::vec3(width,0.06,length));
    translate = glm::translate(model, glm::vec3(-0.5,0,-0.5));
    model = alTogether * translate2 * scale * translate;
    cube.setMaterialisticProperty(glm::vec3(0.804,0.361,0.361),glm::vec3(0.804,0.361,0.361),glm::vec3(0.5,0.5,0.5),32.0);
    cube.drawCubeWithMaterialisticProperty(lightingShader, model);
    
    //pillow 1
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3((width/2)-(0.1+pillowWidth/2),baseHeight+1*0.06,(length/2)-(0.025+pillowWidth/2)));
    scale = glm::scale(model, glm::vec3(pillowWidth,0.04,pillowLength));
    translate = glm::translate(model, glm::vec3(-0.5,0,-0.5));
    model = alTogether * translate2 * scale * translate;
    cube.setMaterialisticProperty(glm::vec3(1.0,0.647,0.0),glm::vec3(1.0,0.647,0.0),glm::vec3(0.5,0.5,0.5),32.0);
    cube.drawCubeWithMaterialisticProperty(lightingShader, model);
    
    //pillow 2
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3((-width/2)+(0.1+pillowWidth/2),baseHeight+1*0.06,(length/2)-(0.025+pillowWidth/2)));
    scale = glm::scale(model, glm::vec3(pillowWidth,0.04,pillowLength));
    translate = glm::translate(model, glm::vec3(-0.5,0,-0.5));
    model = alTogether * translate2 * scale * translate;
    cube.setMaterialisticProperty(glm::vec3(1.0,0.647,0.0),glm::vec3(1.0,0.647,0.0),glm::vec3(0.5,0.5,0.5),32.0);
    cube.drawCubeWithMaterialisticProperty(lightingShader, model);
    
    //blanket
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0,baseHeight+1*0.06,-(length/2-0.025)+blanketLength/2));
    scale = glm::scale(model, glm::vec3(blanketWidth,0.015,blanketLength));
    translate = glm::translate(model, glm::vec3(-0.5,0,-0.5));
    model = alTogether * translate2 * scale * translate;
    cube.setMaterialisticProperty(glm::vec3(0.541,0.169,0.886),glm::vec3(0.541,0.169,0.886),glm::vec3(0.5,0.5,0.5),32.0);
    cube.drawCubeWithMaterialisticProperty(lightingShader, model);
    
    //head
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0,0,(length/2-0.02/2)+0.02));
    scale = glm::scale(model, glm::vec3(width,headHeight,0.02));
    translate = glm::translate(model, glm::vec3(-0.5,0,-0.5));
    model = alTogether * translate2 * scale * translate;
    cube.setMaterialisticProperty(glm::vec3(0.545,0.271,0.075),glm::vec3(0.545,0.271,0.075),glm::vec3(0.5,0.5,0.5),32.0);
    cube.drawCubeWithMaterialisticProperty(lightingShader, model);
    
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
   
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if(rotateAxis_X) rotateAngle_X -=1;
        else if(rotateAxis_Y) rotateAngle_Y -=1;
        else rotateAngle_Z -=1;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.01;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.01;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.01;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.01;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.01;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.01;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.01;
    
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }
    
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if(dirLightOn)
        {
            dirlight.turnOff();
            dirLightOn = !dirLightOn;
        }
        else
        {
            dirlight.turnOn();
            dirLightOn = !dirLightOn;
        }
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if(pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if(spotLightOn)
        {
            spotlight.turnOff();
            spotLightOn = !spotLightOn;
        }
        else
        {
            spotlight.turnOn();
            spotLightOn = !spotLightOn;
        }
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if(dirLightOn)
        {
            if(ambientToggle)
            {
                dirlight.turnAmbientOff();
                ambientToggle = !ambientToggle;
            }
            else
            {

                dirlight.turnAmbientOn();
                ambientToggle = !ambientToggle;
            }
        }
        if(pointLightOn)
        {
            if(ambientToggle)
            {
                pointlight1.turnAmbientOff();
                pointlight2.turnAmbientOff();
                pointlight3.turnAmbientOff();
                pointlight4.turnAmbientOff();
                ambientToggle = !ambientToggle;
            }
            else
            {
                pointlight1.turnAmbientOn();
                pointlight2.turnAmbientOn();
                pointlight3.turnAmbientOn();
                pointlight4.turnAmbientOn();
                ambientToggle = !ambientToggle;
            }
        }
        if(spotLightOn)
        {
            if(ambientToggle)
            {
                spotlight.turnAmbientOff();
                ambientToggle = !ambientToggle;
            }
            else
            {
                spotlight.turnAmbientOn();
                ambientToggle = !ambientToggle;
            }
        }
        
    }
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if(dirLightOn)
        {
            if(diffuseToggle)
            {
                dirlight.turnDiffuseOff();
                diffuseToggle = !diffuseToggle;
            }
            else
            {

                dirlight.turnDiffuseOn();
                diffuseToggle = !diffuseToggle;
            }
        }
        if(pointLightOn)
        {
            if(diffuseToggle)
            {
                pointlight1.turnDiffuseOff();
                pointlight2.turnDiffuseOff();
                pointlight3.turnDiffuseOff();
                pointlight4.turnDiffuseOff();
                diffuseToggle = !diffuseToggle;
            }
            else
            {
                pointlight1.turnDiffuseOn();
                pointlight2.turnDiffuseOn();
                pointlight3.turnDiffuseOn();
                pointlight4.turnDiffuseOn();
                diffuseToggle = !diffuseToggle;
            }
        }
        if(spotLightOn)
        {
            if(diffuseToggle)
            {
                spotlight.turnDiffuseOff();
                diffuseToggle = !diffuseToggle;
            }
            else
            {
                spotlight.turnDiffuseOn();
                diffuseToggle = !diffuseToggle;
            }
        }
        
    }
    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if(dirLightOn)
        {
            if(specularToggle)
            {
                dirlight.turnSpecularOff();
                specularToggle = !specularToggle;
            }
            else
            {

                dirlight.turnSpecularOn();
                specularToggle = !specularToggle;
            }
        }
        if(pointLightOn)
        {
            if(specularToggle)
            {
                pointlight1.turnSpecularOff();
                pointlight2.turnSpecularOff();
                pointlight3.turnSpecularOff();
                pointlight4.turnSpecularOff();
                specularToggle = !specularToggle;
            }
            else
            {
                pointlight1.turnSpecularOn();
                pointlight2.turnSpecularOn();
                pointlight3.turnSpecularOn();
                pointlight4.turnSpecularOn();
                specularToggle = !specularToggle;
            }
        }
        if(spotLightOn)
        {
            if(specularToggle)
            {
                spotlight.turnSpecularOff();
                specularToggle = !specularToggle;
            }
            else
            {
                spotlight.turnSpecularOn();
                specularToggle = !specularToggle;
            }
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const * path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
