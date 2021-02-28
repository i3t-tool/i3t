#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "GameObject.h"
#include "World2.h"
#include "pgr.h"


void GameObject::draw(glm::mat4 parentTransform){
  if (this->draw_callback != NULL){this->draw_callback(this);}
  if (!this->isRender){return;}
  glUseProgram(this->shader);
  glUniform3f(this->shader_camera, World2::mainCamPos[0], World2::mainCamPos[1], World2::mainCamPos[2]);
  glUniformMatrix4fv(this->shader_Pmatrix, 1, GL_FALSE, glm::value_ptr(World2::perspective));
  glm::mat4 tmp = parentTransform * this->transformation;
  glUniformMatrix4fv(this->shader_VMmatrix, 1, GL_FALSE, glm::value_ptr(World2::mainCamera * tmp));
  glUniformMatrix4fv(this->shader_VNmatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(tmp))));
  glUniformMatrix4fv(this->shader_Mmatrix, 1, GL_FALSE, glm::value_ptr(tmp));

  for (int i = 0; i < this->textures.size() && i < this->samplers.size(); i++){
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, this->textures[i]);
  }

  glBindVertexArray(this->vao);
  glDrawElements(this->primitive, this->num_triangles * 3, GL_UNSIGNED_INT, (void*)0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
  //glUseProgram(0);
  CHECK_GL_ERROR();
}
void GameObject::transform(glm::vec3 trans, glm::vec3 scale, glm::vec3 rotAxis, float degrees){
  glm::mat4 pos = glm::translate(glm::mat4(1.0f), trans);
  glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), rotAxis);
  glm::mat4 scal = glm::scale(glm::mat4(1.0f), scale);

  // this->transform = rot*scal;
  this->transformation = rot * this->transformation;
  this->transformation = this->transformation * scal;

  this->transformation[3] = pos[3];
}
void GameObject::translate(glm::vec3 translate){
  glm::vec4 mov = glm::vec4(translate[0], translate[1], translate[2], 0.0f);
  this->transformation[3] += mov;
}
void GameObject::rotateCamera(glm::vec3 axis, float angleDegrees){
  // this->transform=this->transform*glm::rotate(glm::mat4(1.0f), glm::radians(angleDegrees), axis);
}
void GameObject::rotate(glm::vec3 axis, float angleDegrees){
  glm::vec4 tmp = this->transformation[3];
  this->transformation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  this->transformation = glm::rotate(glm::mat4(1.0f), glm::radians(angleDegrees), axis) * this->transformation;
  this->transformation[3] = tmp;
}
void GameObject::scale(glm::vec3 scale){
  glm::mat4 scal = glm::scale(glm::mat4(1.0f), scale);
  this->transformation = this->transformation * scal;
}
GameObject::GameObject(){
  this->isRender = false;
  this->draw_callback = NULL;
  this->transformation = glm::mat4(1.0f);
  this->num_triangles = 0;
  this->num_vertices = 0;
  this->num_attribs = 0;
  this->shader = 0;
  this->attr_pos = 0;
  this->attr_norm = 0;
  this->attr_uv = 0;
  this->shader_VMmatrix = 0;
}
GameObject::GameObject(pgr::MeshData mesh, GLuint shader, GLuint texture){
  this->isRender = true;
  this->draw_callback = NULL;
  this->num_vertices = mesh.nVertices;
  this->num_triangles = mesh.nTriangles;
  this->num_attribs = mesh.nAttribsPerVertex;
  this->shader = shader;
  this->attr_pos = glGetAttribLocation(this->shader, "position");
  this->attr_norm = glGetAttribLocation(this->shader, "norm"); // printf("%d", this->attr_norm);
  this->attr_uv = glGetAttribLocation(this->shader, "uv");
  this->shader_camera = glGetUniformLocation(this->shader, "camera");
  this->shader_Mmatrix = glGetUniformLocation(this->shader, "Mmatrix");
  this->shader_VMmatrix = glGetUniformLocation(this->shader, "VMmatrix");
  this->shader_VNmatrix = glGetUniformLocation(this->shader, "VNmatrix");
  this->shader_Pmatrix = glGetUniformLocation(this->shader, "Pmatrix");
  this->transformation = glm::mat4(1.0f);
  this->textures.push_back(texture);

  glUseProgram(shader);
  for (int i = 0; i < 32; i++){
    char name[10] = {0};
    sprintf(name, "tex%d", i);
    int pos = glGetUniformLocation(this->shader, name);
    if (pos < 0){break;}
    this->samplers.push_back(pos);
    glUniform1i(pos, i);
  }
  glUseProgram(0);
  // buffer for vertices
  glGenBuffers(1, &this->vbo_positions);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo_positions);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.nVertices * mesh.nAttribsPerVertex, mesh.verticesInterleaved,GL_STATIC_DRAW);

  // buffer for triangle indices - ELEMENT_ARRAY
  glGenBuffers(1, &this->vbo_indices);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_indices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * mesh.nTriangles * 3, mesh.triangles,GL_STATIC_DRAW); // 3-indices per triangle


  ///////////// VAO	////////////
  glGenVertexArrays(1, &this->vao);
  glBindVertexArray(this->vao);
  // vertex positions
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo_positions);
  glEnableVertexAttribArray(this->attr_pos);
  glVertexAttribPointer(this->attr_pos, 3, GL_FLOAT, GL_FALSE, mesh.nAttribsPerVertex * sizeof(float),(void*)(0)); // [xyz][nx,ny,nz][s,t]
  if (this->attr_norm >= 0){ // norm pos
    glEnableVertexAttribArray(this->attr_norm);
    glVertexAttribPointer(this->attr_norm, 3, GL_FLOAT, GL_FALSE, mesh.nAttribsPerVertex * sizeof(float),(void*)(4 * 3)); // [xyz][nx,ny,nz][s,t]
  }
  if (this->attr_uv >= 0){ // uv
    glEnableVertexAttribArray(this->attr_uv);
    glVertexAttribPointer(this->attr_uv, 2, GL_FLOAT, GL_FALSE, mesh.nAttribsPerVertex * sizeof(float),(void*)(4 * 6)); // [xyz][nx,ny,nz][s,t]
  }
  // triangle indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_indices);

  // if(tmp!=-1){glBindBuffer(GL_UNIFORM_BUFFER, this->buffer);}
  glBindVertexArray(0);

  CHECK_GL_ERROR();
}
glm::mat4 GameObject::inheritedTransform(GameObject* obj){
  glm::mat4 transform = glm::mat4(1.0f);
  while (obj->parent != NULL){obj = obj->parent;transform = obj->transformation * transform;}
  return transform;
}

void GameObject::unparent(bool keepTransform){
  if (this->parent == NULL){return;}
  parent->rmChild(this, keepTransform);
}
void GameObject::setParent(GameObject* parent, bool keepTransform){
  parent->addChild(this, keepTransform);
}
void GameObject::rmChild(GameObject* obj, bool keepTransform){
  for (int i = 0; i < this->children.size(); i++){
    if (this->children[i] == obj){
      this->children[i]->parent = NULL;
      this->children[i] = this->children.back();
      this->children.pop_back();
      if (keepTransform){obj->transformation = GameObject::inheritedTransform(this) * this->transformation * obj->transformation;}
      return;
    }
  }
}
void GameObject::addChild(GameObject* obj, bool keepTransform){
  if (obj->parent != NULL){obj->unparent(keepTransform);}
  this->children.push_back(obj);
  obj->parent = this;
  if (keepTransform){obj->transformation = glm::inverse(GameObject::inheritedTransform(this) * this->transformation) * obj->transformation;}
}
void GameObject::addComponent(Component* c){
  this->components.push_back(c);c->owner = this;
}
Component* GameObject::getComponent(const char* type){
  for (int i = 0; i < this->components.size(); i++){
    if (this->components[i]->getComponentType() == type){return this->components[i];}
  }
  return NULL;
}
