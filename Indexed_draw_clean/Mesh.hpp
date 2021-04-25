//
//  Mesh.hpp
//  Indexed_draw_clean
//
//  Created by Robert Kigobe on 21/04/2021.
//
#pragma once


#include <GL/glew.h>

class Mesh
{
public:
    Mesh();

    void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
    void RenderMesh();
    void ClearMesh();

    ~Mesh();

private:
    GLuint VAO, VBO, IBO;
    GLsizei indexCount;
};
