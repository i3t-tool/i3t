//---------------------------------------------------------------------------
/**
 * \file	GUI\GUIProjection.h
 * 			Header for representing projection data of GUI
 *
 * \date:  2014/11/16
 * \author: Michal Folta, CTU Prague
 */
//---------------------------------------------------------------------------
#ifndef _GUI_PROJECTIOH_H_
#define _GUI_PROJECTIOH_H_

#include <glm/glm.hpp>

/**
 * \brief Class defining the fullscreen or window sizes and projection
 */
class GUIProjection
{
private:
  static bool viewPort; ///< true if rendered in window and false for fullScreen mode

  // probably full screen data for viewPort == false
  static glm::vec2 screenSize; ///< Size of the screen (in fullScreen mode, viewPort == false)
  static float screenScale;    ///< Scale of the screen (in fullScreen mode, viewPort == false)

  static glm::mat3 PV; ///< called projection, but seems like a simple LookAt matrix (with P = identity)

  // probably window data for viewPort == true
  static glm::vec2 pos;  ///< position
  static glm::vec2 size; ///< viewPort size (in viewPort mode, viewPort == true)
  static float scale;    ///< viewPort scale (in viewPort mode, viewPort == true)

  /**
   * \brief Set the position, size and scale and computes a PV matrix (seems like a simple LookAt matrix)
   * \param _pos position
   * \param _size size
   * \param _scale scale
   */
  static void setProjection(glm::vec2 _pos, glm::vec2 _size, float _scale);

public:
  static void init(const float width, const float height, const float _scale)
  {
    screenScale = _scale;
    screenSize = glm::vec2(width, height);
    setToScreen();
  }

  static void setScreenSize(const float width, const float height)
  {
    screenSize = glm::vec2(width, height);
    if (!viewPort)
      setToScreen();
  }

  static void setToViewPort(const glm::vec2 _pos, const glm::vec2 _size, const float _scale)
  {
    viewPort = true;
    setProjection(_pos, _size, _scale);
  }

  static void setToScreen()
  {
    viewPort = false;
    setProjection(glm::vec2(0.0f), screenSize, screenScale);
  }

  static void resizeScreen(const float screenScaleIncrement)
  {
    screenScale += screenScaleIncrement;
    if (screenScale < 0.0f)
      screenScale = 0.0f;
  }

  static void resize(const float scaleIncrement)
  {
    scale += scaleIncrement;
    if (scale < 0.0f)
      scale = 0.0f;
  }

  /**
   * \brief Reset the screen scale to 1
   */
  static void resetScreenScale() { screenScale = 1.0f; }

  /**
   * \brief Reset the window scale to 1
   */
  static void resetScale() { scale = 1.0f; }

  static glm::mat3 getProjection() { return PV; }

  static glm::vec2 getPos() { return pos; }

  static glm::vec2 getSize() { return size; }

  static float getScale() { return scale; }

  static glm::vec2 getScreenSize() { return screenSize; }

  static float getScreenScale() { return screenScale; }
};

#endif
