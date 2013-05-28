#include <GL/glew.h>
#include <GL/glfw.h>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics.hpp"

extern const GLubyte  *gluErrorString(GLenum error);

glm::vec3 Graphics::cameraPosition = glm::vec3(-2.7f, 1.0f, -2.7f);
float Graphics::cameraRotation = 0.0f;

Graphics::Graphics() {

}

void Graphics::drawCube(float shade, float alpha, glm::vec3 center, float size) {
  glm::mat4 model = glm::translate(glm::mat4(1.0f), center) * glm::scale(glm::mat4(1.0f), glm::vec3(size));

  GLuint MID = glGetUniformLocation(this->shaderprogram, "M");
  glUniformMatrix4fv(MID, 1, GL_FALSE, &model[0][0]);

  GLuint VID = glGetUniformLocation(this->shaderprogram, "V");
  glUniformMatrix4fv(VID, 1, GL_FALSE, &this->view[0][0]);

  GLuint PID = glGetUniformLocation(this->shaderprogram, "P");
  glUniformMatrix4fv(PID, 1, GL_FALSE, &this->projection[0][0]);

  GLuint shadeID = glGetUniformLocation(this->shaderprogram, "in_Color");
  glUniform1f(shadeID, shade);

  GLuint alphaID = glGetUniformLocation(this->shaderprogram, "in_Alpha");
  glUniform1f(alphaID, alpha);

  GLuint cameraID = glGetUniformLocation(this->shaderprogram, "cameraPosition");
  glUniform3f(cameraID, Graphics::cameraPosition.x, Graphics::cameraPosition.y, Graphics::cameraPosition.z);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, this->cubeBuffer);
  glVertexAttribPointer(
     0,                  // attribute 0
     3,                  // size
     GL_FLOAT,           // type
     GL_FALSE,           // normalized?
     0,                  // stride
     (void*)0            // array buffer offset
  );

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, this->cubeNormals);
  glVertexAttribPointer(
      1,                                // attribute
      3,                                // size
      GL_FLOAT,                         // type
      GL_FALSE,                         // normalized?
      0,                                // stride
      (void*)0                          // array buffer offset
  );

  glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
   
  glDisableVertexAttribArray(0);
}

void Graphics::prepareContext() {
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  glm::vec4 cameraPosition = glm::rotate(glm::mat4(1.0f), Graphics::cameraRotation, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(Graphics::cameraPosition, 0.0f);
  Graphics::cameraRotation += 0.03;
  if(Graphics::cameraRotation >= 360)
    Graphics::cameraRotation -= 360;

  this->view = glm::lookAt(
    glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z), // camera position
    glm::vec3(0,0,0), // look at
    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
  );

}

void Graphics::init(int width, int height, bool fullscreen) {
  this->width = width;
  this->height = height;

  this->initGlfw(fullscreen);
  this->initGL();
  this->prepareContext();
  this->initShaders("res/screen.vert", "res/screen.frag");
  this->initBuffers();
}

void Graphics::clearScreen() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::initBuffers() {
  static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f,  // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,  // triangle 1 : end
    1.0f, 1.0f,-1.0f,   // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,  // triangle 2 : end
    1.0f,-1.0f, 1.0f, 
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.01f
  };

  static GLfloat g_vertex_normal_data[3 * 12 * 3];

  for(int i = 0; i < 12 * 3; i += 3) {
    glm::vec3 normal = glm::cross(
      glm::vec3(g_vertex_buffer_data[i * 3 + 3], g_vertex_buffer_data[i * 3 + 4], g_vertex_buffer_data[i * 3 + 5]) - glm::vec3(g_vertex_buffer_data[i * 3], g_vertex_buffer_data[i * 3 + 1], g_vertex_buffer_data[i * 3 + 2]),
      glm::vec3(g_vertex_buffer_data[i * 3 + 6], g_vertex_buffer_data[i * 3 + 7], g_vertex_buffer_data[i * 3 + 8]) - glm::vec3(g_vertex_buffer_data[i * 3], g_vertex_buffer_data[i * 3 + 1], g_vertex_buffer_data[i * 3 + 2]));

    g_vertex_normal_data[i * 3] = normal.x;
    g_vertex_normal_data[i * 3 + 1] = normal.y;
    g_vertex_normal_data[i * 3 + 2] = normal.z;

    g_vertex_normal_data[i * 3 + 3] = normal.x;
    g_vertex_normal_data[i * 3 + 4] = normal.y;
    g_vertex_normal_data[i * 3 + 5] = normal.z;
    
    g_vertex_normal_data[i * 3 + 6] = normal.x;
    g_vertex_normal_data[i * 3 + 7] = normal.y;
    g_vertex_normal_data[i * 3 + 8] = normal.z;
  }

  // Generate 1 buffer, put the resulting identifier in vertexbuffer
  glGenBuffers(1, &this->cubeBuffer);
   
  // The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, this->cubeBuffer);
   
  // Give our vertices to OpenGL.
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  glGenBuffers(1, &this->cubeNormals);
  glBindBuffer(GL_ARRAY_BUFFER, this->cubeNormals);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_normal_data), g_vertex_normal_data, GL_STATIC_DRAW);
}

void Graphics::initShaders(const char *vertex_file_path, const char *fragment_file_path){
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
 
    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
 
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
 
    GLint Result = GL_FALSE;
    int InfoLogLength;
 
    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);
 
    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
 
    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const *FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
 
    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
 
    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
 
    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
 
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
 
    this->shaderprogram = ProgramID;


    glUseProgram(this->shaderprogram);
}

void Graphics::initGL() {
  GLenum err = glewInit();
  if (GLEW_OK != err)
    std::cerr << "Error: " << glewGetErrorString(err) << std::endl;

  glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
  glViewport(0, 0, this->width, this->height);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_MULTISAMPLE);
  glDisable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);

  // Projection matrix : 45° Field of View, correct aspect ratio, display range : 0.1 unit <-> 100 units
  this->projection = glm::perspective(45.0f, (float)this->width / (float)this->height, 0.1f, 100.0f);
}

void Graphics::initGlfw(bool fullscreen) {
  //initialize glfw
  if (!glfwInit())
    throw std::runtime_error("Failed to initialize GLFW.");

  //use opengl 3.3
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);
  //user can't resize window
  glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
  glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x antialiasing

  //open glfw window
  if (!glfwOpenWindow(this->width, this->height, 8, 8, 8, 8, 8, 8, fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW))
    throw std::runtime_error("Failed to open GLFW window.");

  //center window onscreen
  GLFWvidmode vm;
  glfwGetDesktopMode(&vm);
  glfwSetWindowPos(vm.Width / 2 - width / 2, vm.Height / 2 - height / 2);

  //set title
  glfwSetWindowTitle("4DPong");
  //vsync i think: 0 = no, 1 = yes
  glfwSwapInterval(1);

  std::cout << glGetString(GL_VERSION) << std::endl;
}
