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
};
} // namespace Vp
