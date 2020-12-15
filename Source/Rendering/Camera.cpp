#include "Camera.h"
#include "Utils/Math.h"

/**
 * \brief smoothly move camera between backupPosition and lerpViewPort->position. The parameter is lerpAlpha
 */
void Camera::lerp()
{
  if (continualLerp && lerpAlpha >= 1.0)
  {
    position = lerpViewPort->getPosition();
    lookAt = lerpViewPort->getLookAt();
    up = lerpViewPort->getUp();
    return;
  }

  if (lerpAlpha >= 1.0f)
  {
    position = lerpViewPort->getPosition();
    lookAt = lerpViewPort->getLookAt();
    up = lerpViewPort->getUp();
    if (!continualLerp) lerpViewPort = NULL;
  }
  else
  {
    float sm = Math::smoothAlpha(lerpAlpha);
    position = Math::lerp(backupPosition, lerpViewPort->getPosition(), sm);
    lookAt = Math::lerp(backupLookAt, lerpViewPort->getLookAt(), sm);
    up = Math::lerp(backupUp, lerpViewPort->getUp(), sm);

    lerpAlpha += lerpDelta;
  }
}
