//---------------------------------------------------------------------------
/**
 * \file	GUI\textureRegion.h
 *
 * Header for representing a rectangle part of the texture
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague
 */
//---------------------------------------------------------------------------

#ifndef _TEXTUREREGION_H_
#define _TEXTUREREGION_H_

#include "pgr.h"

/**
 * \brief Representation of a single rectangular part of a texture. Stores OpenGL texture name
 *        and texture region BBOX in uv coordinates in the texture as vec4(umin, umax, vmin, vmax).
 */
class TextureRegion
{
protected:
  GLuint glyphsTexId; ///< texture with all glyphs
  glm::vec4 uv; ///< uv coordinates of the sub-region of the glyph (single character BBOX) in the texture in the form
                ///< vec4(umin, umax, vmin, vmax) \rename to BBOX?

public:
  /**
   * \brief Texture Region from absolute coordinates [texels] of its top-left corner position. Used for FONTS.
   *        Stores OpenGL texture name \a _texID and texture region BBOX in uv coordinates.
   *        Computes uv from absolute pixel position [x,y] and size [width, height] of the glyph region.
   *        Flips y, i.e. the input fontFile has the origin in the upper left corner, the OpenGL texture from the
   * lower left corner. \param _texId Square texture with all glyphs \param texSize Size of the square texture with
   * all the glyphs \param x Absolute x-coordinate [texels] of the Top-left corner of the glyph region in the texture.
   * x-coordinate is from range <0..texSize) \param y Absolute y=coordinate [texels] of the Top left corner of the
   * glyph region in the texture. y-coordinate is from range <0..texSize) \param width  Width of the glyph rectangle
   * in pixels \param height Height of the glyph rectangle in pixels
   *
   * \todo replace x,y with position and width, height with size
   */
  TextureRegion(const GLuint _texId, const int texSize, const float x, const float y, const float width,
                const float height)
      : glyphsTexId(_texId)
  {
    uv = glm::vec4(x / texSize, (x + width) / texSize, 1.0f - y / texSize, 1.0f - (y + height) / texSize);
  }

  /**
   * \brief Texture Region from relative uv coordinates of its top-left corner position. Used for ICONS.
   *        Stores OpenGL texture name \a _texID and texture region BBOX in uv coordinates.
   *        Computes uv from relative pixel position [tu,tv] and size [tWidth, tHeight] of the glyph region.
   *        Flips y, i.e. the input fontFile has the origin in the upper left corner, the OpenGL texture from the
   * lower left corner. \param _texId \param tu Relative u-coordinate of the Top left corner of the glyph region in
   * the texture. u-coordinate from the range <0..1.0> \param tv Relative v-coordinate of the Top left corner of the
   * glyph region in the texture. v-coordinate from the range <0..1.0> \param tWidth Width of the glyph rectangle in
   * normalized uv coordinates <0..1.0> \param tHeight Height of the glyph rectangle in normalized uv coordinates
   * <0..1.0>
   */
  TextureRegion(const GLuint _texId, const float tu, const float tv, const float tWidth, const float tHeight)
      : glyphsTexId(_texId)
  {
    uv = glm::vec4(tu, (tu + tWidth), 1.0f - tv, 1.0f - (tv + tHeight));
  }

  ~TextureRegion() = default;

  /**
   * \brief get the uv BBOX. Used in GlyphShader.
   * \return The BBOX of the glyph
   */
  glm::vec4 getUV() const { return uv; }

  /**
   * \brief get OpenGL texture ID
   * \return glyphsTextureId
   */
  GLuint getTexId() const { return glyphsTexId; }

  /**
   * \brief Replace the texture ID fot glyph texture. Used in BufferTab while setting its size.
   * \param newGlyphsTexId New glyphsTexId
   */
  void setTexId(const GLuint newGlyphsTexId) { glyphsTexId = newGlyphsTexId; }

  /**
   * \brief Get uv coordinates of the lower left corner. \notused
   * \return Vector with uv
   */
  glm::vec2 getPosition() { return glm::vec2(uv[0], uv[2]); }

  /**
   * \brief Get glyph rectangle size in uv texture coordinates - both from the range <0..1.0> \notused
   * \return Vector of sizes vec2(tWidth, tHeight)
   */
  glm::vec2 getSize() { return glm::vec2(uv[1] - uv[0], uv[3] - uv[2]); }

  /**
   * \brief Get center of the glyph rectangle in uv texture coordinates - both from the range <0..1.0> \notused
   * \return Vector of sizes vec2(tWidth, tHeight)
   */
  glm::vec2 getCenter() { return glm::vec2((uv[1] + uv[0]) / 2.0f, (uv[3] + uv[2]) / 2.0f); }

  /**
   * \brief Swaps \a "u min" and \a "u max" \notused
   */
  void swapU()
  {
    const float tmp = uv[0];
    uv[0] = uv[1];
    uv[1] = tmp;
  }

  /**
   * \brief Swaps \a "v min" and \a "v max:" \notused
   */
  void SwapV()
  {
    const float tmp = uv[2];
    uv[2] = uv[3];
    uv[3] = tmp;
  }
};

/*
class TextureRegionCycle : public TextureRegion {

private:
  int stepCount;
  int step;

  int stepX, stepY;

  float beginX, beginY;

  int xCount, yCount;

public:

  TextureRegionCycle(GLuint texId, float left, float down, float width, float height) : TextureRegion(texId, left,
down, width, height) {

    stepCount = 1;
    step = 0;

    stepX = 0;
    stepY = 0;

    xCount = 0;
    yCount = 0;

    beginX = left;
    beginY = down;

  }

  void setParams(int stepCount, int xCount, int yCount) {
    this->stepCount = stepCount;
    this->xCount = xCount;
    this->yCount = yCount;
  }

  void update() {

    if (step > stepCount) {
      step = 0;

      if (stepX == xCount - 1) {
        stepX = 0;
        stepY++;
      } else stepX++;

      if (stepY == yCount) {
        stepY = 0;
      }

      //position = glm::vec2(position.x * stepX + beginX, position.y * stepY + beginY);

    } else step++;

  }

};
*/

#endif
