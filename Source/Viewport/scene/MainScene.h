#pragma once

#include "Scene.h"

namespace Vp
{
class MainScene : public Scene
{
protected:
	std::shared_ptr<GameObject> m_gridObject;

public:
	explicit MainScene(Viewport* viewport);

	void init() override;

	void draw(int width, int height) override;
	void draw(glm::mat4 view, glm::mat4 projection,
	          const DisplayOptions& displayOptions = DisplayOptions()) override;
};
} // namespace Vp
