#pragma once
class RTRect : public RTTrans, RTRayTrace
{
public:
	RTRect()
	{
		triangles[0].vertices[0] = glm::vec3(0, 0, 0);
		triangles[0].vertices[1] = glm::vec3(1, 0, 0);
		triangles[0].vertices[2] = glm::vec3(1, 1, 0);
		triangles[0].calculateNormal();
		triangles[0].color = 0xff8f8f8f;

		triangles[1].vertices[0] = glm::vec3(0, 0, 0);
		triangles[1].vertices[1] = glm::vec3(1, 1, 0);
		triangles[1].vertices[2] = glm::vec3(0, 1, 0);
		triangles[1].calculateNormal();
		triangles[1].color = 0xff8f8f8f;

		ExecuteTransform();
	}

	bool IntersectWithRay(const glm::vec3& orig, const glm::vec3& dir, float& t, int32_t& color, 
		glm::vec3* ixpoint, glm::vec3* norm, glm::vec2* texc, bool debug)
	{
		for (int i = 0; i < 2; i++) {
			if (this->transformedTriangles[i].IntersectWithRay(orig, dir, t, color, ixpoint, norm, texc, false)) {
				return true;
			}
		}
		return false;
	}

	void ExecuteTransform()
	{
		this->triangles[0].transforms = this->transforms;
		this->triangles[0].GetTransformed(&this->transformedTriangles[0]);
		this->triangles[1].transforms = this->transforms;
		this->triangles[1].GetTransformed(&this->transformedTriangles[1]);
	}

	int GetType()
	{
		return SHAPE_RECT;
	}

	RTTri triangles[2];
	RTTri transformedTriangles[2];
};

