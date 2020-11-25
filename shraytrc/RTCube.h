#pragma once
class RTCube : public RTTrans, RTRayTrace
{
public:
    RTCube()
    {

        //faces1.push_back({{glm::vec3(0,0,0),glm::vec3(1,0,0),glm::vec3(1,0,1)},glm::vec3(0, -1, 0),0x8f0000ff,0.0f});
        triangles[0].vertices[0] = glm::vec3(0, 0, 0);
        triangles[0].vertices[1] = glm::vec3(1, 0, 0);
        triangles[0].vertices[2] = glm::vec3(1, 0, 1);
        triangles[0].tex[0] = glm::vec2(0, 0);
        triangles[0].tex[1] = glm::vec2(1, 0);
        triangles[0].tex[2] = glm::vec2(1, 1);
        triangles[0].calculateNormal();
        triangles[0].color = 0x8f0000ff;

        //faces1.push_back({{glm::vec3(0,0,0),glm::vec3(1,0,1),glm::vec3(0,0,1)},glm::vec3(0, -1, 0),0x8f00ff00,0.0f});
        triangles[1].vertices[0] = glm::vec3(0, 0, 0);
        triangles[1].vertices[1] = glm::vec3(1, 0, 1);
        triangles[1].vertices[2] = glm::vec3(0, 0, 1);
        triangles[1].tex[0] = glm::vec2(0, 0);
        triangles[1].tex[1] = glm::vec2(1, 1);
        triangles[1].tex[2] = glm::vec2(0, 1);
        triangles[1].calculateNormal();
        triangles[1].color = 0x8f00ff00;

        //faces1.push_back({ {glm::vec3(1,0,0),glm::vec3(1,1,0),glm::vec3(1,1,1)},glm::vec3(1, 0, 0),0x8f0000ff,0.0f });
        triangles[2].vertices[0] = glm::vec3(1, 0, 0);
        triangles[2].vertices[1] = glm::vec3(1, 1, 0);
        triangles[2].vertices[2] = glm::vec3(1, 1, 1);
        triangles[2].tex[0] = glm::vec2(0, 0);
        triangles[2].tex[1] = glm::vec2(1, 0);
        triangles[2].tex[2] = glm::vec2(1, 1);
        triangles[2].calculateNormal();
        triangles[2].color = 0x8f0000ff;

        //faces1.push_back({ {glm::vec3(1,0,0),glm::vec3(1,1,1),glm::vec3(1,0,1)},glm::vec3(1, 0, 0),0x8f00ff00,0.0f });
        triangles[3].vertices[0] = glm::vec3(1, 0, 0);
        triangles[3].vertices[1] = glm::vec3(1, 1, 1);
        triangles[3].vertices[2] = glm::vec3(1, 0, 1);
        triangles[3].tex[0] = glm::vec2(0, 0);
        triangles[3].tex[1] = glm::vec2(1, 1);
        triangles[3].tex[2] = glm::vec2(0, 1);
        triangles[3].calculateNormal();
        triangles[3].color = 0x8f00ff00;

        //faces1.push_back({ {glm::vec3(1,1,0),glm::vec3(0,1,0),glm::vec3(0,1,1)},glm::vec3(0, 1, 0),0x8f0000ff,0.0f });
        triangles[4].vertices[0] = glm::vec3(1, 1, 0);
        triangles[4].vertices[1] = glm::vec3(0, 1, 0);
        triangles[4].vertices[2] = glm::vec3(0, 1, 1);
        triangles[4].tex[0] = glm::vec2(0, 0);
        triangles[4].tex[1] = glm::vec2(1, 0);
        triangles[4].tex[2] = glm::vec2(1, 1);
        triangles[4].calculateNormal();
        triangles[4].color = 0x8f0000ff;

        //faces1.push_back({ {glm::vec3(1,1,0),glm::vec3(0,1,1),glm::vec3(1,1,1)},glm::vec3(0, 1, 0),0x8f00ff00,0.0f });
        triangles[5].vertices[0] = glm::vec3(1, 1, 0);
        triangles[5].vertices[1] = glm::vec3(0, 1, 1);
        triangles[5].vertices[2] = glm::vec3(1, 1, 1);
        triangles[5].tex[0] = glm::vec2(0, 0);
        triangles[5].tex[1] = glm::vec2(1, 1);
        triangles[5].tex[2] = glm::vec2(0, 1);
        triangles[5].calculateNormal();
        triangles[5].color = 0x8f00ff00;

        //faces1.push_back({ {glm::vec3(0,1,0),glm::vec3(0,0,0),glm::vec3(0,0,1)},glm::vec3(-1, 0, 0),0x8f0000ff,0.0f });
        triangles[6].vertices[0] = glm::vec3(0, 1, 0);
        triangles[6].vertices[1] = glm::vec3(0, 0, 0);
        triangles[6].vertices[2] = glm::vec3(0, 0, 1);
        triangles[6].tex[0] = glm::vec2(0, 0);
        triangles[6].tex[1] = glm::vec2(1, 0);
        triangles[6].tex[2] = glm::vec2(1, 1);
        triangles[6].calculateNormal();
        triangles[6].color = 0x8f0000ff;

        //faces1.push_back({ {glm::vec3(0,1,0),glm::vec3(0,0,1),glm::vec3(0,1,1)},glm::vec3(-1, 0, 0),0x8f00ff00,0.0f });
        triangles[7].vertices[0] = glm::vec3(0, 1, 0);
        triangles[7].vertices[1] = glm::vec3(0, 0, 1);
        triangles[7].vertices[2] = glm::vec3(0, 1, 1);
        triangles[7].tex[0] = glm::vec2(0, 0);
        triangles[7].tex[1] = glm::vec2(1, 1);
        triangles[7].tex[2] = glm::vec2(0, 1);
        triangles[7].calculateNormal();
        triangles[7].color = 0x8f00ff00;

        //faces1.push_back({ {glm::vec3(0,1,0),glm::vec3(1,1,0),glm::vec3(1,0,0)},glm::vec3(0, 0, -1),0x8f0000ff,0.0f });
        triangles[8].vertices[0] = glm::vec3(0, 1, 0);
        triangles[8].vertices[1] = glm::vec3(1, 1, 0);
        triangles[8].vertices[2] = glm::vec3(1, 0, 0);
        triangles[8].tex[0] = glm::vec2(0, 0);
        triangles[8].tex[1] = glm::vec2(1, 0);
        triangles[8].tex[2] = glm::vec2(1, 1);
        triangles[8].calculateNormal();
        triangles[8].color = 0x8f0000ff;
    
        //faces1.push_back({ {glm::vec3(0,1,0),glm::vec3(1,0,0),glm::vec3(0,0,0)},glm::vec3(0, 0, -1),0x8f00ff00,0.0f });
        triangles[9].vertices[0] = glm::vec3(0, 1, 0);
        triangles[9].vertices[1] = glm::vec3(1, 0, 0);
        triangles[9].vertices[2] = glm::vec3(0, 0, 0);
        triangles[9].tex[0] = glm::vec2(0, 0);
        triangles[9].tex[1] = glm::vec2(1, 1);
        triangles[9].tex[2] = glm::vec2(0, 1);
        triangles[9].calculateNormal();
        triangles[9].color = 0x8f00ff00;

        //faces1.push_back({ {glm::vec3(0,0,1),glm::vec3(1,0,1),glm::vec3(1,1,1)},glm::vec3(0, 0, 1),0x8f0000ff,0.0f });
        triangles[10].vertices[0] = glm::vec3(0, 0, 1);
        triangles[10].vertices[1] = glm::vec3(1, 0, 1);
        triangles[10].vertices[2] = glm::vec3(1, 1, 1);
        triangles[10].tex[0] = glm::vec2(0, 0);
        triangles[10].tex[1] = glm::vec2(1, 0);
        triangles[10].tex[2] = glm::vec2(1, 1);
        triangles[10].calculateNormal();
        triangles[10].color = 0x8f0000ff;

        //faces1.push_back({ {glm::vec3(0,0,1),glm::vec3(1,1,1),glm::vec3(0,1,1)},glm::vec3(0, 0, 1),0x8f00ff00,0.0f });
        triangles[11].vertices[0] = glm::vec3(0, 0, 1);
        triangles[11].vertices[1] = glm::vec3(1, 1, 1);
        triangles[11].vertices[2] = glm::vec3(0, 1, 1);
        triangles[11].tex[0] = glm::vec2(0, 0);
        triangles[11].tex[1] = glm::vec2(1, 1);
        triangles[11].tex[2] = glm::vec2(0, 1);
        triangles[11].calculateNormal();
        triangles[11].color = 0x8f00ff00;

        ExecuteTransform();
    }

    bool IntersectWithRay(const glm::vec3& orig, const glm::vec3& dir, float& t, int32_t& color, 
        glm::vec3* ixpoint, glm::vec3* norm, glm::vec2* texc, bool debug)
    {
        for (int i = 0; i < 12; i++) {
            if (transformedTriangles[i].IntersectWithRay(orig, dir, t, color, ixpoint, norm, texc, false)) {
                return true;
            }
        }
        return false;
    }

    void ExecuteTransform()
    {
        for (int i = 0; i < 12; i++) {
            this->triangles[i].transforms = this->transforms;
            this->triangles[i].GetTransformed(&this->transformedTriangles[i]);
        }
    }

    void SetColor(int32_t color)
    {
        for (int i = 0; i < 12; i++) {
            this->triangles[i].color = color;
            this->transformedTriangles[i].color = color;
        }
    }

    int GetType()
    {
        return SHAPE_CUBE;
    }

    RTTri triangles[12];
    RTTri transformedTriangles[12];
};

