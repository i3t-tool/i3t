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
};
} // namespace Vp
