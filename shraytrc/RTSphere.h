#pragma once
class RTSphere : public RTTrans, RTRayTrace
{
public:
	RTSphere()
	{
		this->centerOfSphere = glm::vec3(-2, -1, 3);
		this->sphereRadius = 1.0f;
		this->sphereRadiusSquared = 1.0f;
		this->sphereColor = 0xff00ff00;
	}
	bool IntersectWithRay(const glm::vec3& orig, const glm::vec3& dir, float& t, int32_t& color,
		glm::vec3* ixpoint, glm::vec3* norm, glm::vec3* bc, bool debug)
	{

		glm::vec3 ndir = glm::normalize(dir);

		glm::vec3 vecToSphere = this->centerOfSphere - orig;
		float L = glm::length(vecToSphere);
		float tca = glm::dot(vecToSphere, ndir);
		if (tca < 0.0f) return false;

		// d2 + tca2 = l2
		// d2 = l2 - tca2
		// d = sqrt(l2 - tca2)
		float dsq = (L * L) - (tca * tca);
		if (dsq < 0) return false;
		float d = sqrtf(dsq);
		if (d < 0.0f) return false;
		if (d > this->sphereRadius) return false;

		// d2 + thc2 = radius2
		// thc2 = radius2 - d2
		// thc = sqrt(radius2 - d2)
		float thc = sqrtf(sphereRadiusSquared - dsq);
		float t0 = tca - thc;
		float t1 = tca + thc;

		glm::vec3 p = orig + (ndir * t0);

		t = t0;
		color = sphereColor;
		if (ixpoint) {
			ixpoint->x = p.x;
			ixpoint->y = p.y;
			ixpoint->z = p.z;
		}
		if (norm)
		{
			glm::vec3 vecCtoP = p - this->centerOfSphere;
			norm->x = vecCtoP.x;
			norm->y = vecCtoP.y;
			norm->z = vecCtoP.z;
		}

		return true;
	}

	glm::vec3 centerOfSphere;
	float sphereRadius;
	float sphereRadiusSquared;
	int32_t sphereColor;
};

