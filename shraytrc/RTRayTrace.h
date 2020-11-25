#pragma once
class RTRayTrace
{
public:
	virtual bool IntersectWithRay(
		const glm::vec3& orig, 
		const glm::vec3& dir, 
		float& t, 
		int32_t& color, 
		glm::vec3* ixpoint,
		glm::vec3* norm,
		glm::vec2* texc,
		bool debug) = 0;
	virtual int GetType() = 0;
};


