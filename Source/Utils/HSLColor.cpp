/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "HSLColor.h"
#include <array>

HSLColor::HSLColor(float hue, float saturation, float lightness)
    : m_hue(hue), m_saturation(saturation), m_lightness(lightness)
{
	// Empty
}

HSLColor HSLColor::fromRGB(float* rgb)
{
	return fromRGB(rgb[0], rgb[1], rgb[2]);
}

HSLColor HSLColor::fromRGB(float red, float green, float blue)
{
	float h, s, l;
	rgbToHsl(red, green, blue, &h, &s, &l);
	return HSLColor(h, s, l);
}

HSLColor HSLColor::fromHSL(float* hsl)
{
	return fromHSL(hsl[0], hsl[1], hsl[2]);
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

std::array<float, 3> HSLColor::getHSL() const
{
	return {m_hue, m_saturation, m_lightness};
}

HSLColor HSLColor::darken(float factor)
{
	return setLightness(std::max(0.0f, m_lightness * (1.0f - factor)));
}

HSLColor HSLColor::lighten(float factor)
{
	float multiplier = 1.0f + factor;
	return setLightness(std::min(1.0f, m_lightness * multiplier));
}

HSLColor HSLColor::desaturate(float factor)
{
	return setSaturation(std::max(0.0f, m_saturation * (1.0f - factor)));
}

HSLColor HSLColor::saturate(float factor)
{
	float multiplier = 1.0f + factor;
	return setSaturation(std::min(1.0f, m_saturation * multiplier));
}

HSLColor HSLColor::shiftHue(float offset)
{
	return setHue(fmodf((m_hue + offset), 360.0f));
}

HSLColor HSLColor::makeComplementary()
{
	return setHue(fmodf((m_hue + 180.0f), 360.0f));
}

float HSLColor::getHue() const
{
	return m_hue;
}

HSLColor HSLColor::setHue(float mHue)
{
	m_hue = mHue;
	return *this;
}

float HSLColor::getSaturation() const
{
	return m_saturation;
}

HSLColor HSLColor::setSaturation(float mSaturation)
{
	m_saturation = mSaturation;
	return *this;
}

float HSLColor::getLightness() const
{
	return m_lightness;
}

HSLColor HSLColor::setLightness(float mLightness)
{
	m_lightness = mLightness;
	return *this;
}
