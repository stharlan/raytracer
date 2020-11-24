
#include "common.h"

glm::u8vec4 VectorByteColorFromIntColor(uint32_t color)
{
	return glm::u8vec4(
        CRED(color),
        CGREEN(color),
        CBLUE(color),
        CALPHA(color)		
	);
}

uint32_t IntColorFromVectorByteColor(const glm::u8vec4& color)
{
	return CABGR(color.a, color.b, color.g, color.r);
}

glm::vec4 VectorFloatColorFromIntColor(uint32_t color)
{
    return glm::vec4(
        B2F(CRED(color)),
        B2F(CGREEN(color)),
        B2F(CBLUE(color)),
        B2F(CALPHA(color))
    );
}

glm::vec3 VectorFloatColorFromIntColorRGB(uint32_t color)
{
    return glm::vec3(
        B2F(CRED(color)),
        B2F(CGREEN(color)),
        B2F(CBLUE(color))
    );
}

uint32_t IntColorFromVectorFloatColor(const glm::vec4& color)
{
    return CABGR(F2B(color.a), F2B(color.b), F2B(color.g), F2B(color.r));
}

uint32_t IntColorFromVectorFloatColorRGB(const glm::vec3& color)
{
    return CABGR(0xff, F2B(color.b), F2B(color.g), F2B(color.r));
}

void Blend(unsigned char result[4], unsigned char fg[4], unsigned char bg[4])
{
    unsigned int alpha = fg[3] + 1;
    unsigned int inv_alpha = 256 - fg[3];
    result[0] = (unsigned char)((alpha * fg[0] + inv_alpha * bg[0]) >> 8);
    result[1] = (unsigned char)((alpha * fg[1] + inv_alpha * bg[1]) >> 8);
    result[2] = (unsigned char)((alpha * fg[2] + inv_alpha * bg[2]) >> 8);
    result[3] = 0xff;
}

uint32_t Blend(uint32_t fg, uint32_t bg)
{
    // abgr
    unsigned int fg3 = (fg & 0xff000000) >> 24;
    unsigned int alpha = fg3 + 1;
    unsigned int inv_alpha = 256 - fg3;
    unsigned int r0 = (unsigned char)((alpha * (fg & 0xff) + inv_alpha * (bg & 0xff)) >> 8);
    unsigned int r1 = (unsigned char)((alpha * ((fg & 0xff00) >> 8) + inv_alpha * ((bg & 0xff00) >> 8)) >> 8);
    unsigned int r2 = (unsigned char)((alpha * ((fg & 0xff0000) >> 16) + inv_alpha * ((bg & 0xff0000) >> 8)) >> 8);
    unsigned int r3 = 0xff;
    return (r3 << 24) + (r2 << 16) + (r1 << 8) + r0;
}
