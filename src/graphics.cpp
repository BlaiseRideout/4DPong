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

Graphics::Graphics() {
  this->cameraPosition.x = -2.7f;
  this->cameraPosition.y = 1.0f;
  this->cameraPosition.z = -2.7f;
  this->cameraRotation = 0;
}

void Graphics::drawText(glm::vec2 pos, std::string text) {
  glm::mat4 mat = glm::ortho(0.0f, (float)this->width, 0.0f, (float)this->height, -1.0f, 1.0f);
  //mat = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)) * mat;

  std::vector<float> vertexBuf;
  std::vector<float> uvBuf;

  for(unsigned int i = 0; i < text.length(); ++i) {
    char a = text[i];
    if(a < '0' || a > '9')
      continue;

    float texX = (float)(a - '0') / 10.0f;

    uvBuf.push_back((texX + .1f) * (250 / 256));
    uvBuf.push_back(0.0f);

    uvBuf.push_back((texX + .1f) * (250 / 256));
    uvBuf.push_back(50.0f / 64.0f);

    uvBuf.push_back(texX * (250 / 256));
    uvBuf.push_back(0.0f);

    uvBuf.push_back((texX + .1f) * (250 / 256));
    uvBuf.push_back(50.0f / 64.0f);

    uvBuf.push_back(texX * (250 / 256));
    uvBuf.push_back(50.0f / 64.0f);

    uvBuf.push_back(texX * (250 / 256));
    uvBuf.push_back(0.0f);

    int x, y;
    if(vertexBuf.size() > 0) {
      x = vertexBuf[vertexBuf.size() - 2] + 1;
      y = vertexBuf[vertexBuf.size() - 1] + 1;
    }
    else {
      x = 0;
      y = 0;
    }

    vertexBuf.push_back(x + 25);
    vertexBuf.push_back(y);

    vertexBuf.push_back(x + 25);
    vertexBuf.push_back(y + 50);

    vertexBuf.push_back(x);
    vertexBuf.push_back(y);

    vertexBuf.push_back(x + 25);
    vertexBuf.push_back(y + 50);

    vertexBuf.push_back(x);
    vertexBuf.push_back(y + 50);

    vertexBuf.push_back(x);
    vertexBuf.push_back(y);
  }

  GLuint vertices;
  glGenBuffers(1, &vertices);
  glBindBuffer(GL_ARRAY_BUFFER, vertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexBuf.size(), &vertexBuf[0], GL_STATIC_DRAW);

  GLuint UVs;
  glGenBuffers(1, &UVs);
  glBindBuffer(GL_ARRAY_BUFFER, UVs);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uvBuf.size(), &uvBuf[0], GL_STATIC_DRAW);


  glUseProgram(this->textProgram);
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  GLuint posID = glGetUniformLocation(this->textProgram, "pos");
  glUniform2f(posID, pos.x, pos.y);

  GLuint matID = glGetUniformLocation(this->textProgram, "mat");
  glUniformMatrix4fv(matID, 1, GL_FALSE, &mat[0][0]);  

  GLuint texID = glGetUniformLocation(this->textProgram, "tex");
  glUniform1iARB(texID, this->textTex);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertices);
  glVertexAttribPointer(
     0,                  // attribute 0
     2,                  // size
     GL_FLOAT,           // type
     GL_FALSE,           // normalized?
     0,                  // stride
     (void*)0            // array buffer offset
  );
   
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, UVs);
  glVertexAttribPointer(
      1,                                // attribute
      2,                                // size
      GL_FLOAT,                         // type
      GL_FALSE,                         // normalized?
      0,                                // stride
      (void*)0                          // array buffer offset
  );

  glDrawArrays(GL_TRIANGLES, 0, vertexBuf.size() / 2);
   
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void Graphics::drawCube(float shade, float alpha, glm::vec3 center, float size) {
  this->prepareContext();

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
  glUniform3f(cameraID, cameraPosition.x, cameraPosition.y, cameraPosition.z);

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
  glDisableVertexAttribArray(1);
}

void Graphics::prepareContext() {
  glUseProgram(this->shaderprogram);
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  glm::vec4 cameraPosition = glm::rotate(glm::mat4(1.0f), this->cameraRotation, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(this->cameraPosition, 0.0f);

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
  this->initShaders();
  this->initBuffers();

  this->initFont();
}

void Graphics::clearScreen() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::initFont() {
  this->textTex = loadBitmap("res/font.bmp");
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

void Graphics::initShaders(){
    this->shaderprogram = loadShader("res/screen.vert", "res/screen.frag");
    this->textProgram = loadShader("res/text.vert", "res/text.frag");
}

GLuint Graphics::loadShader(const char *vertex_file_path, const char *fragment_file_path) {
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

    return ProgramID;
}

void Graphics::initGL() {
  GLenum err = glewInit();
  if (GLEW_OK != err)
    std::cerr << "Error: " << glewGetErrorString(err) << std::endl;

  glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
  glViewport(0, 0, this->width, this->height);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_MULTISAMPLE);
  glDisable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

GLuint Graphics::loadBitmap(const char * imagepath) {
  // Data read from the header of the BMP file
  unsigned char header[54]; // Each BMP file begins by a 54-bytes header
  unsigned int dataPos;     // Position in the file where the actual data begins
  unsigned int width, height;
  unsigned int imageSize;   // = width*height*3
  // Actual RGB data
  unsigned char * data;

  // Open the file
  FILE * file = fopen(imagepath,"rb");
  if (!file) {
    throw std::runtime_error("Couldn't load image");
  }

  if(fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
    throw std::runtime_error("File not a bmp file");
  }

  if ( header[0]!='B' || header[1]!='M' ) {
    throw std::runtime_error("File not a bmp file");
  }

  // Read ints from the byte array
  dataPos    = *(int*)&(header[0x0A]);
  imageSize  = *(int*)&(header[0x22]);
  width      = *(int*)&(header[0x12]);
  height     = *(int*)&(header[0x16]);

  // Some BMP files are misformatted, guess missing information
  if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
  if (dataPos==0)      dataPos=54; // The BMP header is done that way

  // Create a buffer
  data = new unsigned char [imageSize];
   
  // Read the actual data from the file into the buffer
  if(fread(data,1,imageSize,file) != imageSize)
    throw std::runtime_error("Corrupt bmp file");
   
  //Everything is in memory now, the file can be closed
  fclose(file);

  // Create one OpenGL texture
  GLuint textureID;
  glGenTextures(1, &textureID);
   
  // "Bind" the newly created texture : all future texture functions will modify this texture
  glBindTexture(GL_TEXTURE_2D, textureID);
   
  // Give the image to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
   
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  return textureID;
}