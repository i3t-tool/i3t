#include "Transforms.h"
#include "Core/Input/InputManager.h"
#include "World.h"
#include "glm/gtx/norm.hpp"

glm::vec2 world2screen(glm::vec3 pos)
{
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);

	glm::vec4 pos1 = World::perspective * World::mainCamera * glm::vec4(pos, 1.0f);
	pos1 /= pos1[3];
	glm::vec2 spos1 = glm::vec2((pos1[0] + 1.0f) * 0.5f * viewport[2] + viewport[0],
	                            (pos1[1] + 1.0f) * 0.5f * viewport[3] + viewport[1]);
	return spos1;
}
glm::vec2 vecWorldscreen(glm::vec3 pos, glm::vec3 dir)
{
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);

	glm::vec4 pos1 = World::perspective * World::mainCamera * glm::vec4(pos, 1.0f);
	pos1 /= pos1[3];
	glm::vec2 spos1 = glm::vec2((pos1[0] + 1.0f) * 0.5f * viewport[2] + viewport[0],
	                            (pos1[1] + 1.0f) * 0.5f * viewport[3] + viewport[1]);

	glm::vec4 pos2 = World::perspective * World::mainCamera * glm::vec4(pos + dir, 1.0f);
	pos2 /= pos2[3];
	glm::vec2 spos2 = glm::vec2((pos2[0] + 1.0f) * 0.5f * viewport[2] + viewport[0],
	                            (pos2[1] + 1.0f) * 0.5f * viewport[3] + viewport[1]);

	return spos2 - spos1;
}
glm::vec3 world2viewport(glm::vec3 pos)
{
	glm::vec4 pos1 = World::perspective * World::mainCamera * glm::vec4(pos, 1.0f);
	pos1 /= pos1[3];
	return (glm::vec3)pos1;
}

glm::vec2 mouse2viewport(glm::vec2 pos)
{
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);
	return glm::vec2((pos[0] - viewport[0]) / (viewport[2] * 0.5f) - 1.0f,
	                 (pos[1] - viewport[1]) / (viewport[3] * 0.5f) - 1.0f);
}

glm::vec3 mouseray(glm::vec2 pos)
{
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);
	glm::mat4 inv = glm::inverse(World::perspective * World::mainCamera);
	glm::vec4 mouse = glm::vec4((pos[0] - viewport[0]) / (viewport[2] * 0.5f) - 1.0f,
	                            (pos[1] - viewport[1]) / (viewport[3] * 0.5f) - 1.0f, 0.7f, 1.0f);

	glm::vec4 world1 = inv * mouse;
	mouse[2] = 0.9f;
	glm::vec4 world2 = inv * mouse;

	world1 /= world1[3];
	world2 /= world2[3];

	return -glm::normalize((glm::vec3)(world2 - world1));
}
glm::mat4 getOrtho(glm::mat4 transform, int referenceAxis)
{
	float bias = 0.005f * 0.005f;
	int mapaxis[3] = {referenceAxis, (referenceAxis + 1) % 3, (referenceAxis + 2) % 3};
	glm::vec3 axes[3] = {(glm::vec3)transform[mapaxis[0]], (glm::vec3)transform[mapaxis[1]],
	                     (glm::vec3)transform[mapaxis[2]]};

	if (glm::length2(axes[0]) < bias)
	{
		axes[0] = glm::vec3(0.0f);
		axes[0][mapaxis[0]] = 1.0f;
	}
	if (glm::length2(axes[1]) < bias)
	{
		axes[1] = glm::vec3(0.0f);
		axes[1][mapaxis[1]] = 1.0f;
	}
	if (glm::length2(axes[2]) < bias)
	{
		axes[2] = glm::vec3(0.0f);
		axes[2][mapaxis[2]] = 1.0f;
	}

	axes[0] = glm::normalize(axes[0]);
	if (glm::length2(glm::cross(axes[2], axes[0])) < bias)
	{
		axes[2] = glm::vec3(0.0f);
		axes[2][mapaxis[1]] = 1.0f;
	}

	axes[1] = glm::normalize(glm::cross(axes[2], axes[0]));
	axes[2] = glm::normalize(glm::cross(axes[0], axes[1]));

	glm::mat4 ortho = glm::mat4(1.0f);
	*((glm::vec3*)(&ortho[mapaxis[0]])) = axes[0] * glm::length((glm::vec3)transform[mapaxis[0]]);
	*((glm::vec3*)(&ortho[mapaxis[1]])) = axes[1];
	*((glm::vec3*)(&ortho[mapaxis[2]])) = axes[2];
	*((glm::vec3*)(&ortho[3])) = transform[3];
	return ortho;
}
glm::mat4 getNormalized(glm::mat4 transform)
{
	float bias = 0.005f;
	glm::vec3 lens = glm::vec3(glm::length((glm::vec3)transform[0]), glm::length((glm::vec3)transform[1]),
	                           glm::length((glm::vec3)transform[2]));
	if (lens[0] < bias)
	{
		transform[0] = glm::vec4(0.0f);
		transform[0][0] = 1.0f;
		lens[0] = 1.0f;
	}
	if (lens[1] < bias)
	{
		transform[1] = glm::vec4(0.0f);
		transform[1][1] = 1.0f;
		lens[1] = 1.0f;
	}
	if (lens[2] < bias)
	{
		transform[2] = glm::vec4(0.0f);
		transform[2][2] = 1.0f;
		lens[2] = 1.0f;
	}
	transform[0] /= lens[0];
	transform[1] /= lens[1];
	transform[2] /= lens[2];
	return transform;
}

glm::mat4 getRotation(glm::mat4 transform, int referenceAxis)
{
	float bias = 0.005f * 0.005f;
	int mapaxis[3] = {referenceAxis, (referenceAxis + 1) % 3, (referenceAxis + 2) % 3};
	glm::vec3 axes[3] = {(glm::vec3)transform[mapaxis[0]], (glm::vec3)transform[mapaxis[1]],
	                     (glm::vec3)transform[mapaxis[2]]};
	if (glm::length2(axes[0]) < bias)
	{
		axes[0] = glm::vec3(0.0f);
		axes[0][mapaxis[0]] = 1.0f;
	}
	if (glm::length2(axes[1]) < bias)
	{
		axes[1] = glm::vec3(0.0f);
		axes[1][mapaxis[1]] = 1.0f;
	}
	if (glm::length2(axes[2]) < bias)
	{
		axes[2] = glm::vec3(0.0f);
		axes[2][mapaxis[2]] = 1.0f;
	}

	axes[0] = glm::normalize(axes[0]);
	if (glm::length2(glm::cross(axes[2], axes[0])) < bias)
	{
		axes[2] = glm::vec3(0.0f);
		axes[2][mapaxis[1]] = 1.0f;
	}
	axes[1] = glm::normalize(glm::cross(axes[2], axes[0]));
	axes[2] = glm::normalize(glm::cross(axes[0], axes[1]));

	glm::mat4 rot = glm::mat4(1.0f);
	*((glm::vec3*)(&rot[mapaxis[0]])) = axes[0];
	*((glm::vec3*)(&rot[mapaxis[1]])) = axes[1];
	*((glm::vec3*)(&rot[mapaxis[2]])) = axes[2];
	return rot;
}
glm::vec3 getScale(glm::mat4 transform)
{
	return glm::vec3(glm::length((glm::vec3)transform[0]), glm::length((glm::vec3)transform[1]),
	                 glm::length((glm::vec3)transform[2]));
}
glm::mat4 getFullTransform(GameObject* obj)
{
	glm::mat4 transform = glm::mat4(1.0f);
	while (obj != NULL)
	{
		transform = obj->transformation * transform;
		obj = obj->parent;
	}
	return transform;
}
glm::mat4 getNodeTransform(const Ptr<Core::Node>* node, const Ptr<Core::Sequence>* parent, bool invLookAt)
{
	glm::mat4 m = glm::mat4(1.0f);
	// return m;

	if (node == nullptr || parent == nullptr)
	{
		return m;
	}
	if (node->get() == nullptr || parent->get() == nullptr)
	{
		return m;
	}

	Core::SequenceTree tree(*parent);
	Core::SequenceTree::MatrixIterator it = tree.begin();
	Ptr<Core::Node> nodeindex = Ptr<Core::Node>();

	// printf("sel %s\n", node->get()->getOperation()->keyWord.c_str());//this
	// node

	while (nodeindex.get() != node->get() && it != tree.end())
	{ // skip this node and all right of this node
		nodeindex = *it;
		// printf("skip %s\n", nodeindex.get()->getOperation()->keyWord.c_str());
		// printf("%p %p\n",nodeindex.get(),node->get());
		it++;
	}

	while (it != tree.end())
	{ // mul left of this node
		nodeindex = *it;
		Core::Data d = nodeindex->getData();
		// printf("mul %s\n", nodeindex.get()->getOperation()->keyWord.c_str());
		if (strcmp(nodeindex->getOperation()->keyWord.c_str(), "LookAt") == 0 && invLookAt)
		{
			m = glm::inverse(d.getMat4()) * m;
		}
		else
		{
			m = d.getMat4() * m;
		}
		it++;
	} // getchar();
	return m;
}
glm::vec3 planeIntersect(glm::vec3 px, glm::vec3 py, glm::vec3 p0)
{
	glm::vec3 t0 = -World::mainCamPos;
	glm::vec3 tz = mouseray(world2screen(p0) + glm::vec2(InputManager::m_mouseXDelta, -InputManager::m_mouseYDelta));
	glm::vec3 coef = glm::inverse(glm::mat3(-tz, px, py)) * (t0 - p0);

	return t0 + tz * coef[0];
}
glm::mat4 getProjParams(glm::mat4 projinv, bool isPersp)
{
	glm::mat4 ret = glm::mat4(0.0f);

	float left, right, top, bottom, near, far;

	glm::vec4 p = projinv * glm::vec4(-1.0f, 0.0f, -1.0f, 1.0f);
	p /= p[3];
	left = p[0];
	p = projinv * glm::vec4(1.0f, 0.0f, -1.0f, 1.0f);
	p /= p[3];
	right = p[0];
	p = projinv * glm::vec4(0.0f, 1.0f, -1.0f, 1.0f);
	p /= p[3];
	top = p[1];
	p = projinv * glm::vec4(0.0f, -1.0f, -1.0f, 1.0f);
	p /= p[3];
	bottom = p[1];
	p = projinv * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
	p /= p[3];
	near = -p[2];
	p = projinv * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	p /= p[3];
	far = -p[2];
	// printf("t %f %f, %f %f,%f %f\n", left, right, top, bottom, near, far);
	if (isPersp)
	{
		float angle = 2.0f * glm::degrees(atan((0.5f * (top - bottom)) / (near)));
		float aspect = (right - left) / (top - bottom);
		ret[2][0] = near;
		ret[2][1] = far;
		ret[3][0] = angle;
		ret[3][1] = aspect;
		// printf("t %f %f, %f %f\n", near, far, angle, aspect);
	}
	else
	{
		ret[0][0] = left;
		ret[0][1] = right;
		ret[1][0] = top;
		ret[1][1] = bottom;
		ret[2][0] = near;
		ret[2][1] = far;
	}
	return ret;
}
void setLen(glm::vec3* vec, float len)
{
	float f = glm::length(*vec);
	*vec /= f + (float)(f == 0.0f);
	*vec *= len;
}
float angle2(float x, float y)
{
	float a = glm::degrees(atan(y / x));
	if (a < 0.0f)
	{
		a = 180.0f + a;
	}
	if (y < 0.0f)
	{
		a += 180.0f;
	}
	return a;
}
bool dirEqual(glm::vec3 v1, glm::vec3 v2, float bias)
{
	float _cos = glm::dot(v1, v2) / (glm::length(v1) * glm::length(v2));
	return _cos > 1 - bias; // cos==1 =>angle between v1,V2 is 0
}