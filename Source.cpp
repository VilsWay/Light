#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "iostream"
#include "WindowManager.h"
#include "Triangle.h"
#include "CubeData.h"
#include "Shader.h"
#include "TextureHandler.h"
#include "MatrixOperation.h"
#include "ModelPos.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"



Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 0;
float lastY = 0;
bool firstMouse;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

using namespace std;
int wWidth =1024, wHeight=920;
glm::vec3 lightPos(-1.2f, 0.0f, 20.0f);


int main()
{
  cout << "Hello There!!" << endl;
 
  //1. Create the Window , make it current context
  WindowManager* wmInst = WindowManager::_getInstance("SampleWindow", wWidth, wHeight);
  glfwMakeContextCurrent(wmInst->_getWindow());
  
  // 2. Intiliaze GLADs
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glViewport(0, 0, wWidth, wHeight);
  glEnable(GL_DEPTH_TEST);
  // 3. Initializing
  CubeData cubeDataObj;
  //unsigned int cubeVAO = cubeDataObj._getTexturedCubeVAO();
  unsigned int lightVAO = cubeDataObj._getSimpleCubeVAO();
  unsigned int cubeNormalVAO = cubeDataObj._getCubeNormalVAO();

  // 4. Shader Init
  Shader shaderProgram_Light("VertexShaderLightSource.txt", "FragmentShaderLightSource.txt");
  Shader shaderProgram_Object("VertexShaderLightDestObj.txt", "FragmentShaderLightDestObj.txt");

  glm::mat4 modelLight = glm::mat4(1.0f);
  glm::mat4 modelObject = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  projection = glm::perspective(glm::radians(45.f), (float)wWidth / (float)wHeight, 0.1f, 800.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -300.0f));
  translate(modelLight, lightPos);
  scale(modelLight, 0.25f, 0.25f, 0.25f);
  rotate(modelLight, 1200.8, 0.2, 0.8f, 0.4f);

  shaderProgram_Light.use();
  shaderProgram_Light.setMat4("projection", projection);
  shaderProgram_Light.setMat4("view", view);
  shaderProgram_Light.setMat4("model", modelLight);

  modelObject = glm::mat4(1.0f);
  rotate(modelObject, 1200.8, 0.2, 0.8f, 0.4f);
  shaderProgram_Object.use();
  shaderProgram_Object.setMat4("projection", projection);
  shaderProgram_Object.setMat4("view", view);
  shaderProgram_Object.setMat4("model", modelObject);

  

  shaderProgram_Object.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
  shaderProgram_Object.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
 // shaderProgram_Object.setVec3("viewPos", 0.0f, 0.0f, 0.0f);
  //shaderProgram_Object.setVec3("lightPos", lightPos);

  
  // 5. Render Loop

  float lightX = 2.0f * sin(glfwGetTime());
  float lightY = -0.3f;
  float lightZ = 1.5f * cos(glfwGetTime());
  lightPos = glm::vec3(lightX, lightY, lightZ);
  float x = -1.0;
  while (!glfwWindowShouldClose(wmInst->_getWindow()))
  {
    
    modelLight = glm::mat4(1.0f);
    scale(modelLight, 0.25f, 0.25f, 0.25f);
    rotate(modelLight, 1200.8, 0.2, 0.8f, 0.4f);

    //1. Input Processing
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    wmInst->ProcessInput();
    
    // 2. Set Frame Color, Reset the color and other buffer
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Draw Light source, Do not Touch
    shaderProgram_Light.use();
    
    
    lightX = 6.0f * sin(glfwGetTime());
    lightY = -0.3f;
    lightZ = 3.0f * cos(glfwGetTime());
    lightPos = glm::vec3(lightX, lightY, lightZ);
    
    shaderProgram_Object.setVec3("viewPos", 0.0f, 0.0f, 0.0f);
    x = x + -1.0f;
    translate(modelLight, lightPos);

    shaderProgram_Light.setMat4("model", modelLight);

    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Object being drawn , Change as per required...
    shaderProgram_Object.use();
    glBindVertexArray(cubeNormalVAO);
    shaderProgram_Object.setVec3("lightPos", lightPos);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //4. Frame ready swap the buffer.
    glfwSwapBuffers(wmInst->_getWindow());
    glfwPollEvents();
    
  }
  glfwTerminate();
  return 0;
}
