#pragma once

#include "Viewport/scene/Scene.h"

namespace Vp
{
class MainScene : public Scene
{
protected:
	std::shared_ptr<GameObject> m_gridObject;

public:
	explicit MainScene(Viewport* viewport);

	void init() override;
	void update(double dt) override;
	void draw(int width, int height, glm::mat4 view, glm::mat4 projection, SceneRenderTarget& renderTarget,
	          const DisplayOptions& displayOptions) override;
	void draw(int width, int height, SceneRenderTarget& renderTarget, const DisplayOptions& displayOptions) override;
};
} // namespace Vp
