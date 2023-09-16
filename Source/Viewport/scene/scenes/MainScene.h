#pragma once

#include "Viewport/scene/Scene.h"

namespace Vp
{
class SunLight;

class MainScene : public Scene
{
protected:
	std::shared_ptr<GameObject> m_gridObject;

	SunLight* m_sun1;
	SunLight* m_sun2;

	glm::vec3 m_sun1_dir;
	glm::vec3 m_sun2_dir;

	float m_sun2_intensity;

public:
	explicit MainScene(Viewport* viewport);

	void init() override;
	void update(double dt) override;
	void draw(int width, int height, glm::mat4 view, glm::mat4 projection, SceneRenderTarget& renderTarget,
	          const DisplayOptions& displayOptions) override;
	void draw(int width, int height, SceneRenderTarget& renderTarget, const DisplayOptions& displayOptions) override;
};
} // namespace Vp
