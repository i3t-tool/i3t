/*
 * Implementation of a virtual trackball.
 *
 * Taken from the pgr-framework and adjusted for I3T
 *
 * Original credits:
 * Done by Tomas Barak in August 2013.
 * Based on the code by Gavin Bell, lots of ideas from Thant Tessman
 * and
 * the August '88 issue of Siggraph's "Computer Graphics," pp. 121-129.
 * https://github.com/danping/CoSLAM/blob/master/src/gui/trackball.cpp
 *
 * Much mucking with by:
 * Gavin Bell
 */

#include "Trackball.h"

#include "Logger/Logger.h"
#include "Utils/Math.h"

using namespace Vp;

#define TRACKBALLSIZE (0.8f)

glm::mat4 Trackball::rotate(glm::vec2 screenStart, glm::vec2 screenEnd, glm::ivec2 screenSize)
{
	if ((screenStart.x == screenEnd.x) && (screenStart.y == screenEnd.y))
		return glm::mat4(1.0f);

	glm::vec2 start = mapScreenCoords(screenStart, screenSize);
	glm::vec2 end = mapScreenCoords(screenEnd, screenSize);

	glm::mat4 rotation = computeRotation(start, end, screenSize);
	return rotation;
}

void Trackball::computeRotationAxisAndAngle(glm::vec3& axis, float& angle, glm::vec2 startPoint, glm::vec2 endPoint,
                                            glm::ivec2 screenSize)
{
	/*
	 * Adjust trackball size depending on the aspect ratio
	 *
	 * When the aspect ratio isn't one, since we calculate trackball movements in the normalized <-1, 1> range
	 * the trackball is going to get skewed and turn into an ellipse. So depending on the aspect ratio we recalculate
	 * either the X or Y coordinates of start and end points so that the normalized coordinates form a square in screen
	 * space as well.
	 *
	 * The trackball size is also changed depending on the aspect ratio. It should fit inside the visible rectangle.
	 * So the smaller of the two sides (width and height) is used to determine its size.
	 * However, with large aspect ratios the trackball gets too small, so as the aspect ratio grows the trackball is made
	 * larger and larger.
	 */

	float trackballSize = TRACKBALLSIZE;
	float trackballScreenSizeX;
	float trackballScreenSizeY;
	float aspectRatio;
	float xAspectFactor = 1.0f;
	float yAspectFactor = 1.0f;
	float aspectRatioScalingFactor;

	if (screenSize.x >= screenSize.y)
	{
		aspectRatio = (float)screenSize.x / screenSize.y;
		trackballScreenSizeX = trackballSize * 1 / aspectRatio;
		trackballScreenSizeY = trackballSize;
		xAspectFactor = aspectRatio;
	}
	else
	{
		aspectRatio = (float)screenSize.y / screenSize.x;
		trackballScreenSizeX = trackballSize;
		trackballScreenSizeY = trackballSize * 1 / aspectRatio;
		yAspectFactor = aspectRatio;
	}
	aspectRatioScalingFactor = pow((0.5f * (aspectRatio - 1.0f)), 1.1f) + 1;

	// Transformed start and end points that account for aspect ratio
	glm::vec2 startPointT = startPoint * glm::vec2(xAspectFactor, yAspectFactor);
	glm::vec2 endPointT = endPoint * glm::vec2(xAspectFactor, yAspectFactor);

	//	LOG_INFO("tSX: {}, tSY: {}", trackballScreenSizeX, trackballScreenSizeY);
	//	LOG_INFO("afX: {}, afY: {}", xAspectFactor, yAspectFactor);
	//	LOG_INFO("s: {}, s: {}", startPoint.x, startPoint.y);
	//	LOG_INFO("sT: {}, sT: {}", startPointT.x, startPointT.y);
	//	LOG_INFO("T size: {}, T size scaled: {}", trackballSize, trackballSize * aspectRatioScalingFactor);

	trackballSize *= aspectRatioScalingFactor;
	trackballScreenSizeX *= aspectRatioScalingFactor;
	trackballScreenSizeY *= aspectRatioScalingFactor;

	debug_trackballScreenSize = glm::vec2(trackballScreenSizeX * 0.70710678118654752440 * (screenSize.x / 2),
	                                      trackballScreenSizeY * 0.70710678118654752440 * (screenSize.y / 2));

	// TODO: (DR) I don't think we're supposed to change rotation axis every frame, but every drag operation.

	/* First, figure out z-coordinates for projection of P1 and P2 to deformed sphere */
	glm::vec3 p1(startPointT.x, startPointT.y, projectToSphere(trackballSize, startPointT.x, startPointT.y));
	glm::vec3 p2(endPointT.x, endPointT.y, projectToSphere(trackballSize, endPointT.x, endPointT.y));

	/* Now, we want the cross product of P1 and P2 */
	axis = glm::normalize(glm::cross(p1, p2));

	// LOG_INFO("Axis: ({},{},{})", axis.x, axis.y, axis.z);

	/* Figure out how much to rotate around that axis. */
	glm::vec3 d = p1 - p2;

	double dist = glm::length(d) / (2.0 * TRACKBALLSIZE);

	/* Avoid problems with out-of-control values...s */
	if (dist > 1.0)
		dist = 1.0;
	if (dist < -1.0)
		dist = -1.0;

	angle = float(2.0f * asin(dist)); /* how much to rotate about axis (in radians) */
}

glm::mat4 Trackball::computeRotation(glm::vec2 startPoint, glm::vec2 endPoint, glm::ivec2 screenSize)
{

	if ((startPoint.x == endPoint.x) && (startPoint.y == endPoint.y))
	{
		/* Zero rotation */
		return glm::mat4(1.0);
	}

	glm::vec3 axis;
	float angle;

	computeRotationAxisAndAngle(axis, angle, startPoint, endPoint, screenSize);

	return glm::rotate(glm::mat4(1.0f), angle, axis);
}

glm::vec2 Trackball::mapScreenCoords(glm::vec2 screenPos, glm::ivec2 screenSize)
{
	return {Math::range(screenPos.x, 0, screenSize.x, -1, 1), Math::range(screenPos.y, 0, screenSize.y, -1, 1) * -1};
}

glm::vec2 Trackball::mapNormalizedCoords(glm::vec2 pos, glm::ivec2 screenSize)
{
	return {Math::range(pos.x, -1, 1, 0, screenSize.x), Math::range(pos.y * -1, -1, 1, 0, screenSize.y)};
}

float Trackball::projectToSphere(float radius, float x, float y)
{
	double d, t, z;

	d = sqrt(x * x + y * y);
	if (d < radius * 0.70710678118654752440)
	{ /* Inside sphere */
		LOG_INFO("INSIDE");
		z = sqrt(radius * radius - d * d);
	}
	else
	{ /* On hyperbola */
		LOG_INFO("OUTSIDE");
		t = radius / 1.41421356237309504880;
		z = t * t / d;
	}

	return (float)z;
}

glm::vec2 Trackball::debug_trackballScreenSize = glm::vec2(0);