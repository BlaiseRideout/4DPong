#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <string>

#include "vector.hpp"

class Graphics {
  public:
    glm::vec3 cameraPosition;
    float cameraRotation;

    Graphics();

    void    drawText(glm::vec2 pos, std::string);
    void    drawCube(float, float, glm::vec3, float);
    void    prepareContext();
    void    init(int, int, bool);
    void    clearScreen();
    GLuint  loadBitmap(const char*);
  private:
    GLuint      shaderprogram;
    GLuint      textProgram;
    GLuint      cubeBuffer;
    GLuint      cubeNormals;
    GLuint      textBuffer;
    GLuint      textUVs;
    GLuint      textTex;
    glm::mat4   projection;
    glm::mat4   view;
    int         width;
    int         height;

    void    initShaders();
    GLuint  loadShader(const char * vertex_file_path, const char * fragment_file_path);
    void    initGL();
    void    initGlfw(bool fullscreen);
    void    initFont();
    void    initBuffers();
};

#endif