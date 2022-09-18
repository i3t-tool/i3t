#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "GameObject.h"
#include "World.h"
#include "pgr.h"

void GameObject::draw(glm::mat4 parentTransform)
{
	if (this->draw_callback != NULL)
	{
		this->draw_callback(this);
	}
	if (!this->isRender)
	{
		return;
	}
	glUseProgram(this->shader->program);
	glm::mat4 tmp = parentTransform * this->transformation;
	glUniform4f(this->shader->color, this->color[0], this->color[1],
	            this->color[2], this->color[3]);
	glUniform3f(this->shader->camera, World::mainCamPos[0], World::mainCamPos[1],
	            World::mainCamPos[2]);
	glUniformMatrix4fv(
	    this->shader->PVMmatrix, 1, GL_FALSE,
	    glm::value_ptr(World::perspective * World::mainCamera * tmp));
	glUniformMatrix4fv(this->shader->VNmatrix, 1, GL_FALSE,
	                   glm::value_ptr(glm::transpose(glm::inverse(tmp))));
	glUniformMatrix4fv(this->shader->Mmatrix, 1, GL_FALSE, glm::value_ptr(tmp));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	glBindVertexArray(this->vao);
	glDrawElements(this->primitive, this->num_triangles * 3, GL_UNSIGNED_INT,
	               (void*)0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	// glUseProgram(0);
	CHECK_GL_ERROR();
}
void GameObject::transform(glm::vec3 trans, glm::vec3 scale, glm::vec3 rotAxis,
                           float degrees)
{
	glm::mat4 pos = glm::translate(glm::mat4(1.0f), trans);
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), rotAxis);
	glm::mat4 scal = glm::scale(glm::mat4(1.0f), scale);

	this->transformation = rot * this->transformation * scal;
	this->transformation[3] = pos[3];
}
void GameObject::translate(glm::vec3 translate)
{
	glm::vec4 mov = glm::vec4(translate[0], translate[1], translate[2], 0.0f);
	this->transformation[3] += mov;
}
void GameObject::rotateAround(glm::vec3 axis, float angleDegrees,
                              glm::vec3 center)
{
	this->transformation[3] -= glm::vec4(center, 0.0f);
	this->transformation =
	    glm::rotate(glm::mat4(1.0f), glm::radians(angleDegrees), axis) *
	    this->transformation;
	this->transformation[3] += glm::vec4(center, 0.0f);
}
void GameObject::rotate(glm::vec3 axis, float angleDegrees)
{
	glm::vec4 tmp = this->transformation[3];
	this->transformation =
	    glm::rotate(glm::mat4(1.0f), glm::radians(angleDegrees), axis) *
	    this->transformation;
	this->transformation[3] = tmp;
}
void GameObject::scale(glm::vec3 scale)
{
	glm::mat4 scal = glm::scale(glm::mat4(1.0f), scale);
	this->transformation = this->transformation * scal;
}
GameObject::GameObject()
{
	this->isRender = false;
	this->draw_callback = NULL;
	this->transformation = glm::mat4(1.0f);
	this->num_triangles = 0;
	this->num_vertices = 0;
	this->num_attribs = 0;
	this->shader = 0;
}
GameObject::GameObject(const pgr::MeshData mesh, struct Shader* shader,
                       GLuint texture)
{
	this->isRender = true;
	this->draw_callback = NULL;
	this->num_vertices = mesh.nVertices;
	this->num_triangles = mesh.nTriangles;
	this->num_attribs = mesh.nAttribsPerVertex;
	this->shader = ((Shader*)shader);
	this->transformation = glm::mat4(1.0f);
	this->texture = (texture == 0) ? World::textures["white"] : texture;
	this->color = glm::vec4(1.0f);

	// buffer for vertices
	glGenBuffers(1, &this->vbo_positions);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_positions);
	glBufferData(GL_ARRAY_BUFFER,
	             sizeof(float) * mesh.nVertices * mesh.nAttribsPerVertex,
	             mesh.verticesInterleaved, GL_STATIC_DRAW);

	// buffer for triangle indices - ELEMENT_ARRAY
	glGenBuffers(1, &this->vbo_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * mesh.nTriangles * 3,
	             mesh.triangles,
	             GL_STATIC_DRAW); // 3-indices per triangle

	///////////// VAO	////////////
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);
	// vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_positions);
	glEnableVertexAttribArray(this->shader->attr_pos);
	glVertexAttribPointer(this->shader->attr_pos, 3, GL_FLOAT, GL_FALSE,
	                      mesh.nAttribsPerVertex * sizeof(float),
	                      (void*)(0)); // [xyz][nx,ny,nz][s,t]
	if (this->shader->attr_norm >= 0)
	{ // norm pos
		glEnableVertexAttribArray(this->shader->attr_norm);
		glVertexAttribPointer(this->shader->attr_norm, 3, GL_FLOAT, GL_FALSE,
		                      mesh.nAttribsPerVertex * sizeof(float),
		                      (void*)(4 * 3)); // [xyz][nx,ny,nz][s,t]
	}
	if (this->shader->attr_uv >= 0)
	{ // uv
		glEnableVertexAttribArray(this->shader->attr_uv);
		glVertexAttribPointer(this->shader->attr_uv, 2, GL_FLOAT, GL_FALSE,
		                      mesh.nAttribsPerVertex * sizeof(float),
		                      (void*)(4 * 6)); // [xyz][nx,ny,nz][s,t]
	}
	// triangle indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_indices);
	glBindVertexArray(0);

	CHECK_GL_ERROR();
}
void GameObject::setMeshData(const pgr::MeshData mesh)
{
	if (this->vao != 0)
	{
		glDeleteBuffers(1, &this->vbo_positions);
		glDeleteBuffers(1, &this->vbo_indices);
		glDeleteVertexArrays(1, &this->vao);
	}

	this->num_vertices = mesh.nVertices;
	this->num_triangles = mesh.nTriangles;
	this->num_attribs = mesh.nAttribsPerVertex;

	// buffer for vertices
	glGenBuffers(1, &this->vbo_positions);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_positions);
	glBufferData(GL_ARRAY_BUFFER,
	             sizeof(float) * mesh.nVertices * mesh.nAttribsPerVertex,
	             mesh.verticesInterleaved, GL_STATIC_DRAW);

	// buffer for triangle indices - ELEMENT_ARRAY
	glGenBuffers(1, &this->vbo_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * mesh.nTriangles * 3,
	             mesh.triangles,
	             GL_STATIC_DRAW); // 3-indices per triangle

	///////////// VAO	////////////
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);
	// vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_positions);
	glEnableVertexAttribArray(this->shader->attr_pos);
	glVertexAttribPointer(this->shader->attr_pos, 3, GL_FLOAT, GL_FALSE,
	                      mesh.nAttribsPerVertex * sizeof(float),
	                      (void*)(0)); // [xyz][nx,ny,nz][s,t]
	if (this->shader->attr_norm >= 0)
	{ // norm pos
		glEnableVertexAttribArray(this->shader->attr_norm);
		glVertexAttribPointer(this->shader->attr_norm, 3, GL_FLOAT, GL_FALSE,
		                      mesh.nAttribsPerVertex * sizeof(float),
		                      (void*)(4 * 3)); // [xyz][nx,ny,nz][s,t]
	}
	if (this->shader->attr_uv >= 0)
	{ // uv
		glEnableVertexAttribArray(this->shader->attr_uv);
		glVertexAttribPointer(this->shader->attr_uv, 2, GL_FLOAT, GL_FALSE,
		                      mesh.nAttribsPerVertex * sizeof(float),
		                      (void*)(4 * 6)); // [xyz][nx,ny,nz][s,t]
	}
	// triangle indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_indices);
	glBindVertexArray(0);

	CHECK_GL_ERROR();
}
GameObject::~GameObject()
{
	if (this->vao != 0)
	{
		glDeleteBuffers(1, &this->vbo_positions);
		glDeleteBuffers(1, &this->vbo_indices);
		glDeleteVertexArrays(1, &this->vao);
	}
}
glm::mat4 GameObject::inheritedTransform(GameObject* obj)
{
	glm::mat4 transform = glm::mat4(1.0f);
	while (obj->parent != NULL)
	{
		obj = obj->parent;
		transform = obj->transformation * transform;
	}
	return transform;
}

void GameObject::unparent(bool keepTransform)
{
	if (this->parent == NULL)
	{
		return;
	}
	parent->rmChild(this, keepTransform);
}
void GameObject::setParent(GameObject* parent, bool keepTransform)
{
	parent->addChild(this, keepTransform);
}
bool GameObject::rmChild(GameObject* obj, bool keepTransform)
{
	for (int i = 0; i < this->children.size(); i++)
	{
		if (this->children[i] == obj)
		{
			this->children[i]->parent = NULL;
			this->children[i] = this->children.back();
			this->children.pop_back();
			if (keepTransform)
			{
				obj->transformation = GameObject::inheritedTransform(this) *
				                      this->transformation * obj->transformation;
			}
			return true;
		}
	}
	return false;
}
void GameObject::addChild(GameObject* obj, bool keepTransform)
{
	if (obj->parent != NULL)
	{
		obj->unparent(keepTransform);
	}
	this->children.push_back(obj);
	obj->parent = this;
	if (keepTransform)
	{
		obj->transformation = glm::inverse(GameObject::inheritedTransform(this) *
		                                   this->transformation) *
		                      obj->transformation;
	}
}
void GameObject::addComponent(Component* c)
{
	this->components.push_back(c);
	c->m_gameObject = this;
}
Component* GameObject::getComponent(const char* type)
{
	for (int i = 0; i < this->components.size(); i++)
	{
		if (this->components[i]->getComponentType() == type)
		{
			return this->components[i];
		}
	}
	return NULL;
}
