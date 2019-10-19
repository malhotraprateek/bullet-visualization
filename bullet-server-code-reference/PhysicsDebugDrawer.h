#pragma once

#include "LinearMath/btIDebugDraw.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <json.hpp>

struct Line
{
	glm::vec3 from;
	glm::vec3 to;
	glm::vec3 srcColor_rgb;
	glm::vec3 destColor_rgb;
};

struct Sphere
{
	glm::vec3 center;
	float radius;
	glm::vec3 color_rgb;
};

struct Triangle
{
	glm::vec3 a;
	glm::vec3 b;
	glm::vec3 c;
	glm::vec4 color_rgba;
};

struct DrawData
{
	std::vector<Line> lines;
	std::vector<Sphere> spheres;
	std::vector<Triangle> triangles;

	std::string error;
};

class PhysicsDebugDrawer : public btIDebugDraw
{
	DebugDrawModes mDebugMode;
	std::shared_ptr<DrawData> mpDrawData = nullptr;

	glm::vec3 Convert(const btVector3 &vec3);
	float Convert(const btScalar &scalar);

public:
	PhysicsDebugDrawer();
	virtual ~PhysicsDebugDrawer();

	// Very important - Use it each frame to clear out data accumulated by Bullet
	void ClearData();

	std::shared_ptr<DrawData> GetDrawData();
	nlohmann::json GetJsonData();

	virtual void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &fromColor, const btVector3 &toColor);

	virtual void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);

	virtual void drawSphere(const btVector3 &p, btScalar radius, const btVector3 &color);

	virtual void drawTriangle(const btVector3 &a, const btVector3 &b, const btVector3 &c, const btVector3 &color, btScalar alpha);

	virtual void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color);

	virtual void reportErrorWarning(const char *warningString);

	virtual void draw3dText(const btVector3 &location, const char *textString);

	virtual void setDebugMode(int debugMode);

	virtual int getDebugMode() const;
};