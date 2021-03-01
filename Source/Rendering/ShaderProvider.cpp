#include "ShaderProvider.h"

#include "RedShader.h"
#include "Rendering/AlphaShader.h"
#include "Rendering/BaseShader.h"
#include "Shader.h"

#include "Geometry.h"
#include "ModelInstance.h"

int ShaderProvider::getShaderID(ModelInstance* m)
{
	// alpha
	if (m->getAlphaFlag())
		return 2;

	// base
	if (m->getGeometry()->getAttribsPerVertex() == 8 && m->material->getTextureId() != 0)
		return 1; // PF was != -1, but texture is GLuint. Matches the 0 in Material with no Texture

	// red
	return 0;
}

int ShaderProvider::getShaderID(Geometry* g)
{
	// base
	if (g->getAttribsPerVertex() == 8)
		return 1;

	// red
	return 0;
}

int ShaderProvider::init(const char* redVert, const char* redFrag, const char* baseVert, const char* baseFrag,
                         const char* alphaVert, const char* alphaFrag)
{
	RedShader* redShader = new RedShader(redVert, redFrag);
	if (redShader->getErrorCode())
	{
		return redShader->getErrorCode();
	}
	else
		shaders.push_back(redShader);

	BaseShader* baseShader = new BaseShader(baseVert, baseFrag);
	if (baseShader->getErrorCode())
	{
		return baseShader->getErrorCode();
	}
	else
		shaders.push_back(baseShader);

	AlphaShader* alphaShader = new AlphaShader(alphaVert, alphaFrag);
	if (alphaShader->getErrorCode())
	{
		return alphaShader->getErrorCode();
	}
	else
		shaders.push_back(alphaShader);

	for (unsigned int i = 0; i < shaders.size(); i++)
		shaders[i]->getLocations();

	return 0;
}

void ShaderProvider::dispose()
{
	for (std::vector<Shader*>::iterator it = shaders.begin(); it != shaders.end(); ++it)
	{
		delete (*it);
	}
}

void ShaderProvider::setRenderGroups(RenderGroups* rg)
{
	renderGroups = rg;
}

Shader* ShaderProvider::provideShader(ModelInstance* m)
{
	return shaders[getShaderID(m)];
}

void ShaderProvider::addToGroup(ModelInstance* m)
{
	if (renderGroups == nullptr)
		return;
	renderGroups->groups[getShaderID(m)].push_back(m);
}

void ShaderProvider::removeFromGroup(ModelInstance* m)
{
	if (renderGroups == nullptr)
		return;
	// if (!renderGroups) return; //PF  why not this way<

	const int gi = getShaderID(m);

	for (std::vector<ModelInstance*>::iterator iter = renderGroups->groups[gi].begin();
	     iter != renderGroups->groups[gi].end(); ++iter)
	{
		if (*iter == m)
		{
			renderGroups->groups[gi].erase(iter);
			break;
		}
	}
}

void ShaderProvider::renderAll(Camera* camera, Environment* environment)
{
	if (!renderGroups)
		return;

	for (unsigned int i = 0; i < shaders.size(); i++)
	{
		shaders[i]->begin();
		for (unsigned int k = 0; k < renderGroups->groups[i].size(); k++)
			if (renderGroups->groups[i][k]->isVisible())
				shaders[i]->draw(renderGroups->groups[i][k], camera, environment);
		shaders[i]->end();
	}
}

void ShaderProvider::renderAll(const glm::mat4 proj, const glm::mat4 view, Environment* environment)
{
	if (!renderGroups)
		return;

	for (unsigned int i = 0; i < shaders.size(); i++)
	{
		shaders[i]->begin();
		for (unsigned int k = 0; k < renderGroups->groups[i].size(); k++)
			if (renderGroups->groups[i][k]->isVisible())
				shaders[i]->draw(renderGroups->groups[i][k], proj, view, environment);
		shaders[i]->end();
	}
}

void ShaderProvider::renderAll(const glm::mat4 proj, const glm::mat4 view, Environment* environment,
                               ModelInstance* noRender)
{
	if (!renderGroups)
		return;

	for (unsigned int i = 0; i < shaders.size(); i++)
	{
		shaders[i]->begin();
		for (unsigned int k = 0; k < renderGroups->groups[i].size(); k++)
			if (renderGroups->groups[i][k] != noRender && renderGroups->groups[i][k]->isVisible())
				shaders[i]->draw(renderGroups->groups[i][k], proj, view, environment);
		shaders[i]->end();
	}
}

void ShaderProvider::connectGeometry(Geometry* g)
{
	shaders[getShaderID(g)]->connectVertexAttributes(g);
}

void ShaderProvider::connectGeometry(Geometry* g, const GLuint shaderID)
{
	assert(shaderID > 0);
	assert(shaderID < 3);
	assert(shaderID < shaders.size());

	shaders[shaderID]->connectVertexAttributes(g);
}

// statics
RenderGroups* ShaderProvider::renderGroups = nullptr;
std::vector<Shader*> ShaderProvider::shaders;
