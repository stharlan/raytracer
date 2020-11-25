#pragma once
class RTTri : public RTTrans, RTRayTrace
{
public:
	void calculateNormal()
	{
		glm::vec3 a = vertices[1] - vertices[0];
		glm::vec3 b = vertices[2] - vertices[0];
		this->normal = glm::cross(a, b);
		this->normal = glm::normalize(this->normal);
        this->d = glm::dot(normal, vertices[0]);
    }

    void GetTransformed(RTTri* newTri)
    {
        std::vector<glm::mat4>::iterator iter = this->transforms.begin();
        glm::vec4 v0 = glm::vec4(this->vertices[0], 1.0f);
        glm::vec4 v1 = glm::vec4(this->vertices[1], 1.0f);
        glm::vec4 v2 = glm::vec4(this->vertices[2], 1.0f);
        for (; iter != this->transforms.end(); ++iter) {
            v0 = *iter * v0;
            v1 = *iter * v1;
            v2 = *iter * v2;
        }
        newTri->vertices[0] = v0;
        newTri->vertices[1] = v1;
        newTri->vertices[2] = v2;
        newTri->color = this->color;
        newTri->tex[0] = this->tex[0];
        newTri->tex[1] = this->tex[1];
        newTri->tex[2] = this->tex[2];
        newTri->calculateNormal();
    }

    bool IntersectWithRay(const glm::vec3& orig, const glm::vec3& dir, float& t,
        int32_t& returnIntColor, 
        glm::vec3* returnIntersectionPoint, glm::vec3* returnNormal, 
        glm::vec2* texc, bool debug)
    {
        glm::vec3 v0v1 = vertices[1] - vertices[0];
        glm::vec3 v0v2 = vertices[2] - vertices[0];
        glm::vec3 unNormalizedNormal = glm::cross(v0v1, v0v2);
        float area = glm::length(unNormalizedNormal) / 2; // area of the triangle 

        // find the normal to the plane
        float nDotd = glm::dot(normal, dir);
        // zero is parallel
        // positive is backface
        if (nDotd > -0.0001) {
            if (debug) {
                if (nDotd > 0.0001) {
                    printf("backface\n");
                }
                else {
                    printf("parallel\n");
                }
            }
            return false;
        }

        // this can be done once per frame
        // **
        float dMinusnDotp = d - (glm::dot(normal, orig));
        // **

        t = dMinusnDotp / nDotd;
        // behind
        if (t < 0.0f) {
            if (debug) {
                printf("behind %.1f %.1f %.1f :: %.1f %.1f %.1f\n",
                    orig.x, orig.y, orig.z,
                    vertices[0].x, vertices[0].y, vertices[0].z);
            }
            return false;
        }
        glm::vec3 ix = orig + (dir * t);
        if (returnIntersectionPoint) {
            returnIntersectionPoint->x = ix.x;
            returnIntersectionPoint->y = ix.y;
            returnIntersectionPoint->z = ix.z;
        }
        if (returnNormal) {
            returnNormal->x = normal.x;
            returnNormal->y = normal.y;
            returnNormal->z = normal.z;
        }

        glm::vec3 texcc;

        //[(b - a)x(q - a)].n
        glm::vec3 edge0 = vertices[1] - vertices[0];
        glm::vec3 vp0 = ix - vertices[0];
        float inout = glm::dot(glm::cross((vertices[1] - vertices[0]), (ix - vertices[0])), normal);
        if (inout < 0.0f) {
            //printf("outside 1\n");
            return false;
        }
        glm::vec3 C = glm::cross(edge0, vp0);
        texcc.z = (glm::length(C) / 2) / area;

        glm::vec3 edge1 = vertices[2] - vertices[1];
        glm::vec3 vp1 = ix - vertices[1];
        inout = glm::dot(glm::cross(edge1, vp1), normal);
        if (inout < 0.0f) {
            //printf("outside 2\n");
            return false;
        }
        C = glm::cross(edge1, vp1);
        texcc.x = (glm::length(C) / 2) / area;

        glm::vec3 edge2 = vertices[0] - vertices[2];
        glm::vec3 vp2 = ix - vertices[2];
        inout = glm::dot(glm::cross(edge2, vp2), normal);
        if (inout < 0.0f) {
            //printf("outside 3\n");
            return false;
        }
        C = glm::cross(edge2, vp2);
        texcc.y = (glm::length(C) / 2) / area;

        if (texc) {
            texc->x = (texcc.x * this->tex[0].x)
                + (texcc.y * this->tex[1].x)
                + (texcc.z * this->tex[2].x);
            texc->y = (texcc.x * this->tex[0].y)
                + (texcc.y * this->tex[1].y)
                + (texcc.z * this->tex[2].y);
        }

        returnIntColor = color;
        //retClr = 0xffffffff;

        // final color = diffuse color * object color
        // object color = blended
        // diffuse color = diff * lightColor
        // 

        return true;
    }

    int GetType()
    {
        return SHAPE_TRI;
    }

	glm::vec3 vertices[3];
	glm::vec3 normal;
    glm::vec2 tex[3];
	int32_t color;
	float d;
};

