#include "HSLColor.h"
#include <array>

HSLColor::HSLColor(float hue, float saturation, float lightness)
    : m_hue(hue), m_saturation(saturation), m_lightness(lightness)
{
	// Empty
}

HSLColor HSLColor::fromRGB(float red, float green, float blue)
{
	float h, s, l;
	rgbToHsl(red, green, blue, &h, &s, &l);
	return HSLColor(h, s, l);
}

HSLColor HSLColor::fromHSL(float hue, float saturation, float lightness)
{
	return HSLColor(hue, saturation, lightness);
}

std::array<float, 3> HSLColor::getRGB() const
{
	float r, g, b;
	hslToRgb(m_hue, m_saturation, m_lightness, &r, &g, &b);
	return {r, g, b};
}

std::array<float, 3> HSLColor::getHSL() const { return {m_hue, m_saturation, m_lightness}; }

HSLColor HSLColor::darken(float factor)
{
	setLightness(std::max(0.0f, m_lightness * (1.0f - factor)));
	return *this;
}

HSLColor HSLColor::lighten(float factor)
{
	float multiplier = 1.0f + factor;
	setLightness(std::min(100.0f, m_lightness * multiplier));
	return *this;
}

HSLColor HSLColor::makeComplementary()
{
	setHue(fmodf((m_hue + 180.0f), 360.0f));
	return *this;
}

float HSLColor::getHue() const { return m_hue; }

HSLColor HSLColor::setHue(float mHue)
{
	m_hue = mHue;
	return *this;
}

float HSLColor::getSaturation() const { return m_saturation; }

HSLColor HSLColor::setSaturation(float mSaturation)
{
	m_saturation = mSaturation;
	return *this;
}

float HSLColor::getLightness() const { return m_lightness; }

HSLColor HSLColor::setLightness(float mLightness)
{
	m_lightness = mLightness;
	return *this;
}
