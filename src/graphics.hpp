#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/gl.h>
#include <glm/glm.hpp>

#include "vector.hpp"

class Graphics {
  public:
    glm::vec3 cameraPosition;

    Graphics();

    void drawCube(float, glm::vec3, float);
    void prepareContext();
    void init(int, int, bool);
    void clearScreen();
  private:
    GLuint shaderprogram;
    GLuint cubeBuffer;
    GLuint cubeNormals;
    glm::mat4 projection;
    glm::mat4 view;
    int width;
    int height;

    void    initShaders(const char * vertex_file_path, const char * fragment_file_path);
    void    initGL();
    void    initGlfw(bool fullscreen);
    void    initBuffers();
};

#endif