
#include "pgr.h"   // includes all PGR libraries, like shader, glm, assimp ...

#include "sceneGraph/AxesNode.h"

namespace pgr {
namespace sg {

GLuint AxesNode::m_vertexArrayObject  = 0;
GLuint AxesNode::m_vertexBufferObject = 0;
GLuint AxesNode::m_program            = 0;

GLint  AxesNode::m_PVMmatrixLoc  = -1;
GLint  AxesNode::m_posLoc        = -1;
GLint  AxesNode::m_colLoc        = -1;

// For all tasks - multiple colors and matrix multiplication
static const std::string strVertexShader(
    "#version 140\n"
    "uniform mat4 PVMmatrix;\n"
    "in vec4 position;\n"
    "in vec4 color;\n"
    "smooth out vec4 theColor;\n"
    "void main()\n"
    "{\n"
    "	gl_Position = PVMmatrix * position;\n"
    "	theColor = color;\n"
    "}\n"
);

static const std::string strFragmentShader(
    "#version 140\n"
    "smooth in vec4 theColor;\n"
    "out vec4 outputColor;\n"
    "void main()\n"
    "{\n"
    "	outputColor = theColor;\n"
    "}\n"
);

static const float vertexData[] = {
  // vertices for the base of the pyramid
  // xyzw                        rgba
  0.0f,  0.0f,  0.0f,  1.0f,     1.0f, 0.0f, 0.0f, 1.0, //X
  1.0f,  0.0f,  0.0f,  1.0f,     1.0f, 0.0f, 0.0f, 1.0,

  0.0f,  0.0f,  0.0f,  1.0f,     0.0f, 1.0f, 0.0f, 1.0f, //Y
  0.0f,  1.0f,  0.0f,  1.0f,     0.0f, 1.0f, 0.0f, 1.0f,

  0.0f,  0.0f,  0.0f,  1.0f,     0.0f, 0.0f, 1.0f, 1.0f, //Z
  0.0f,  0.0f,  1.0f,  1.0f,     0.0f, 0.0f, 1.0f, 1.0f,
};


AxesNode::AxesNode(const std::string &name, SceneNode * parent):
  SceneNode(name, parent) {
  if(m_program == 0) {
    std::vector<GLuint> shaderList;

    // Push vertex shader and fragment shader
    shaderList.push_back(pgr::createShaderFromSource(GL_VERTEX_SHADER,    strVertexShader  ));
    shaderList.push_back(pgr::createShaderFromSource(GL_FRAGMENT_SHADER,  strFragmentShader));

    // Create the program with two shaders
    m_program       = pgr::createProgram(shaderList);
    m_PVMmatrixLoc  = glGetUniformLocation( m_program, "PVMmatrix");
    m_posLoc        = glGetAttribLocation(  m_program, "position");
    m_colLoc        = glGetAttribLocation(  m_program, "color");
  }

  if(m_vertexArrayObject == 0) {
    glGenBuffers(1, &m_vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &m_vertexArrayObject );
    glBindVertexArray( m_vertexArrayObject );
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    // vertices of triangles
    glEnableVertexAttribArray(m_posLoc);
    glVertexAttribPointer(m_posLoc, 4, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*) 0);
    // 8 = 4 + 4 floats per vertex - color
    glEnableVertexAttribArray(m_colLoc);
    glVertexAttribPointer(m_colLoc, 4, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(4*sizeof(float)));
    glBindVertexArray( 0 );
  }

}

AxesNode::~AxesNode(void) {
  if(m_program == 0)
    pgr::deleteProgramAndShaders( m_program );
  if(m_vertexArrayObject == 0) { // vertex array with one red point ;-(
    glDeleteBuffers     (1, &m_vertexBufferObject);
    glDeleteVertexArrays(1, &m_vertexArrayObject );
  }
}

void AxesNode::draw(const glm::mat4 & view_matrix, const glm::mat4 & projection_matrix) {
  // inherited draw - draws all children
  SceneNode::draw(view_matrix, projection_matrix);

  glm::mat4 matrix = projection_matrix * view_matrix * globalMatrix();

  glUseProgram(m_program);
  glUniformMatrix4fv(m_PVMmatrixLoc, 1, GL_FALSE, glm::value_ptr(matrix) );

  glBindVertexArray( m_vertexArrayObject );
  glDrawArrays(GL_LINES, 0, 6);
  glBindVertexArray( 0 );
}

} // end namespace sg
} // end namespace pgr

