
#define CALPHA(c) ((c & 0xff000000) >> 24)
#define CBLUE(c) ((c & 0xff0000) >> 16)
#define CGREEN(c) ((c & 0xff00) >> 8)
#define CRED(c) (c & 0xff)
#define CABGR(a,b,g,r) ((a << 24) + (b << 16) + (g << 8) + r)
#define B2F(b) ((float)b / 255.0f)
#define F2B(f) ((unsigned char)(f * 255.0f))
#define MAX(v1,v2) (v1 > v2 ? v1 : v2)
#define MIN(v1,v2) (v1 < v2 ? v1 : v2)

#define SHAPE_CUBE 1
#define SHAPE_SPHERE 2
#define SHAPE_TRI 3
#define SHAPE_RECT 4

#include <Windows.h>

#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"
#include "stb_image_write.h"

glm::u8vec4 VectorByteColorFromIntColor(uint32_t color);
uint32_t IntColorFromVectorByteColor(const glm::u8vec4& color);
glm::vec4 VectorFloatColorFromIntColor(uint32_t color);
uint32_t IntColorFromVectorFloatColor(const glm::vec4& color);
glm::vec3 VectorFloatColorFromIntColorRGB(uint32_t color);
uint32_t IntColorFromVectorFloatColorRGB(const glm::vec3& color);
void Blend(unsigned char result[4], unsigned char fg[4], unsigned char bg[4]);
uint32_t Blend(uint32_t fg, uint32_t bg);

#include "RTRayTrace.h"
#include "RTTrans.h"
#include "RTTri.h"
#include "RTRect.h"
#include "RTCube.h"
#include "RTSphere.h"
