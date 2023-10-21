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
 *
 * Utilities for manipulating HSL color
 */

#pragma once

#include <algorithm>
#include <array>
#include <math.h>

/**
 * Utility class for managing a HSL color.
 * Inspired by https://tips4java.wordpress.com/2009/07/05/hsl-color/
 */
class HSLColor
{
private:
	float m_hue, m_saturation, m_lightness;

public:
	static HSLColor fromRGB(float* rgb);
	static HSLColor fromRGB(float red, float green, float blue);
	static HSLColor fromHSL(float* hsl);
	static HSLColor fromHSL(float hue, float saturation, float lightness);

	[[nodiscard]] std::array<float, 3> getRGB() const;
	[[nodiscard]] std::array<float, 3> getHSL() const;

	/**
	 * Creates a darker color.
	 */
	HSLColor darken(float factor);

	/**
	 * Creates a lighter color.
	 */
	HSLColor lighten(float factor);

	HSLColor saturate(float factor);
	HSLColor desaturate(float factor);

	HSLColor shiftHue(float offset);

	HSLColor makeComplementary();

	float getHue() const;
	HSLColor setHue(float mHue);
	float getSaturation() const;
	HSLColor setSaturation(float mSaturation);
	float getLightness() const;
	HSLColor setLightness(float mLightness);

private:
	HSLColor(float hue, float saturation, float lightness);
};

inline void rgbToHsl(float R, float G, float B, float* H, float* S, float* L)
{
	R = std::clamp(R, 0.0f, 1.0f);
	G = std::clamp(G, 0.0f, 1.0f);
	B = std::clamp(B, 0.0f, 1.0f);

	const auto min = std::min(std::min(R, G), B);
	const auto max = std::max(std::max(R, G), B);
	const auto delta = (max - min);

	*L = ((min + max) / 2.0f);

	if (max == min)
	{
		*S = 0;
	}
	else if (*L < 0.5)
	{
		*S = (delta / (min + max));
	}
	else
	{
		*S = (delta / (2.0f - max - min));
	}

	*H = 0.0;
	if (delta > 0.0)
	{
		if (max == R)
			*H = fmod(((60 * (G - B) / (max - min)) + 360), 360.0);
		else if (max == G)
			*H = (60 * (B - R) / (max - min)) + 120;
		else if (max == B)
			*H = (60 * (R - G) / (max - min)) + 240;

		*H = std::clamp(*H, 0.0f, 360.0f);
	}
}

inline float hueToRgb(float p, float q, float h)
{
	if (h < 0)
		h += 1;

	if (h > 1)
		h -= 1;

	if (6 * h < 1)
	{
		return p + ((q - p) * 6 * h);
	}

	if (2 * h < 1)
	{
		return q;
	}

	if (3 * h < 2)
	{
		return p + ((q - p) * 6 * ((2.0f / 3.0f) - h));
	}

	return p;
}

inline void hslToRgb(float H, float S, float L, float* R, float* G, float* B)
{
	H = std::clamp(H, 0.0f, 360.0f);
	S = std::clamp(S, 0.0f, 1.0f);
	L = std::clamp(L, 0.0f, 1.0f);

	H = fmodf(H, 360.0f);
	H /= 360.0f;

	float q = 0;

	if (L < 0.5)
		q = L * (1 + S);
	else
		q = (L + S) - (S * L);

	float p = 2 * L - q;

	*R = std::clamp(hueToRgb(p, q, H + (1.0f / 3.0f)), 0.0f, 1.0f);
	*G = std::clamp(hueToRgb(p, q, H), 0.0f, 1.0f);
	*B = std::clamp(hueToRgb(p, q, H - (1.0f / 3.0f)), 0.0f, 1.0f);
}
