#pragma once

#include <memory>
#include <vector>

namespace Vp
{
class Light;
class PhongShader;

/**
 * \brief Class that manages passing of light information to shaders.
 */
class Lighting
{
	std::vector<std::shared_ptr<Light>> lights;

public:
	/**
	 * \brief Set light information uniforms for a shader.
	 * \param shader The shader
	 */
	void setUniforms(const PhongShader& shader) const;

	void addLight(Light* light);
	void addLight(const std::shared_ptr<Light>& light);

	void removeLight(std::shared_ptr<Light>& light);

	std::vector<std::shared_ptr<Light>> getLights() const;
};

} // namespace Vp
