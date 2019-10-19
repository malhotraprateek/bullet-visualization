#include "PhysicsDebugDrawer.h"

PhysicsDebugDrawer::PhysicsDebugDrawer()
{
	mDebugMode = (DebugDrawModes)(DebugDrawModes::DBG_DrawAabb | DebugDrawModes::DBG_FastWireframe);
	mpDrawData = std::make_shared<DrawData>();
}

PhysicsDebugDrawer::~PhysicsDebugDrawer()
{
}

glm::vec3 PhysicsDebugDrawer::Convert(const btVector3 &vec3)
{
	glm::vec3 v;
	v.x = vec3.x();
	v.y = vec3.y();
	v.z = vec3.z();

	return v;
}

float PhysicsDebugDrawer::Convert(const btScalar &scalar)
{
	return float(scalar);
}

void PhysicsDebugDrawer::ClearData()
{
	mpDrawData->error = "";
	mpDrawData->lines.clear();
	mpDrawData->spheres.clear();
	mpDrawData->triangles.clear();
}

nlohmann::json PhysicsDebugDrawer::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["error"] = mpDrawData->error;

	{
		std::vector<nlohmann::json> lines;

		for (auto &line : mpDrawData->lines)
		{
			nlohmann::json jsonLine;
			jsonLine["from"] = {line.from.x, line.from.y, line.from.z};
			jsonLine["to"] = {line.to.x, line.to.y, line.to.z};
			jsonLine["srcColor"] = {line.srcColor_rgb.r, line.srcColor_rgb.g, line.srcColor_rgb.b};
			jsonLine["destColor"] = {line.destColor_rgb.r, line.destColor_rgb.g, line.destColor_rgb.b};

			lines.push_back(jsonLine);
		}

		jsonData["Lines"] = lines;
	}

	{
		std::vector<nlohmann::json> spheres;

		for (auto &sphere : mpDrawData->spheres)
		{
			nlohmann::json jsonSphere;
			jsonSphere["center"] = {sphere.center.x, sphere.center.y, sphere.center.z};
			jsonSphere["radius"] = sphere.radius;
			jsonSphere["color"] = {sphere.color_rgb.r, sphere.color_rgb.g, sphere.color_rgb.b};

			spheres.push_back(jsonSphere);
		}

		jsonData["Spheres"] = spheres;
	}

	{
		std::vector<nlohmann::json> triangles;

		for (auto &triangle : mpDrawData->triangles)
		{
			nlohmann::json jsonTriangle;
			jsonTriangle["a"] = {triangle.a.x, triangle.a.y, triangle.a.z};
			jsonTriangle["b"] = {triangle.b.x, triangle.b.y, triangle.b.z};
			jsonTriangle["c"] = {triangle.c.x, triangle.c.y, triangle.c.z};
			jsonTriangle["color"] = {triangle.color_rgba.r, triangle.color_rgba.g, triangle.color_rgba.b, triangle.color_rgba.a};

			triangles.push_back(jsonTriangle);
		}

		jsonData["Triangles"] = triangles;
	}

	return jsonData;
}

std::shared_ptr<DrawData> PhysicsDebugDrawer::GetDrawData()
{
	return mpDrawData;
}

void PhysicsDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &fromColor, const btVector3 &toColor)
{
	Line line = Line();
	line.from = Convert(from);
	line.to = Convert(to);
	line.srcColor_rgb = Convert(fromColor);
	line.destColor_rgb = Convert(toColor);

	mpDrawData->lines.push_back(line);
}

void PhysicsDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
	Line line = Line();
	line.from = Convert(from);
	line.to = Convert(to);
	line.srcColor_rgb = Convert(color);
	line.destColor_rgb = Convert(color);

	mpDrawData->lines.push_back(line);
}

void PhysicsDebugDrawer::drawSphere(const btVector3 &p, btScalar radius, const btVector3 &color)
{
	Sphere sphere = Sphere();
	sphere.center = Convert(p);
	sphere.radius = Convert(radius);
	sphere.color_rgb = Convert(color);

	mpDrawData->spheres.push_back(sphere);
}

void PhysicsDebugDrawer::drawTriangle(const btVector3 &a, const btVector3 &b, const btVector3 &c, const btVector3 &color, btScalar alpha)
{
	Triangle triangle = Triangle();
	triangle.a = Convert(a);
	triangle.b = Convert(b);
	triangle.c = Convert(c);
	triangle.color_rgba = glm::vec4(Convert(color), Convert(alpha));

	mpDrawData->triangles.push_back(triangle);
}

void PhysicsDebugDrawer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color)
{
	// Nothing as of yet
}

void PhysicsDebugDrawer::reportErrorWarning(const char *warningString)
{
	mpDrawData->error += warningString;
	mpDrawData->error += "\n";
}

void PhysicsDebugDrawer::draw3dText(const btVector3 &location, const char *textString)
{
	// Nothing as of yet
}

void PhysicsDebugDrawer::setDebugMode(int debugMode)
{
	mDebugMode = DebugDrawModes(debugMode);
}

int PhysicsDebugDrawer::getDebugMode() const
{
	return mDebugMode;
}