// shraytrc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "common.h"

#define OUTPUT_WIDTH 1920
#define OUTPUT_HEIGHT 1080
#define MULTI_SAMPLING 2
#define NUM_THREADS 9
#define VIEWPORT_WIDTH 4.0f

glm::vec4 lightColor(1, 1, 1, 1);

struct RenderContext
{
    unsigned char* pixelData;
    unsigned int rowstart;
    unsigned int rowend;
    std::vector<RTRayTrace*> traceables;
    glm::vec4* vdata;
};

struct HitContext
{
    glm::vec3 intersection;
    int32_t intColor;
    glm::vec3 normal;
};

void ShootRay(const glm::vec3& pos, int ix, int iz, RenderContext* ctx, const glm::vec3& lightPos)
{

    // plane is 2 units behind x/y axis
    // and is 4 units by 4 units
    glm::vec3 orig(ctx->vdata[4]);
    glm::vec3 dir = pos - orig;
    glm::vec3 ndir = glm::normalize(dir);

    // get the closest hit
    float hitDist = 1e9;
    HitContext hit;
    std::vector<RTRayTrace*>::iterator iter = ctx->traceables.begin();
    for (; iter != ctx->traceables.end(); ++iter)
    {
        glm::vec3 ix;
        glm::vec3 norm0;
        glm::vec3 bc;
        float t0 = 0.0f;
        int32_t clr0 = 0;
        bool b = (*iter)->IntersectWithRay(orig, ndir, t0, clr0, &ix, &norm0, &bc, false);
        if (b) {
            if (t0 < hitDist) {
                hitDist = t0;
                hit.intColor = clr0;
                hit.normal = norm0;
                hit.intersection = ix;
            }
        }
    }

    // if there is a hit, check if it's in the shadow
    // of another object
    if (hitDist < 1e9)
    {
        glm::vec3 lightDir = lightPos - hit.intersection;

        float faceTowardsLight = glm::dot(lightDir, hit.normal);
        bool inShadow = false;
        if (faceTowardsLight < 0.0f) {
            inShadow = true;
        }
        else {
            std::vector<RTRayTrace*>::iterator iter1 = ctx->traceables.begin();
            for (; iter1 != ctx->traceables.end(); ++iter1)
            {
                float t1 = 0.0f;
                int32_t clr1 = 0;
                bool b1 = (*iter1)->IntersectWithRay(hit.intersection, lightDir, t1, clr1, NULL, NULL, NULL, false);
                if (b1) {
                    inShadow = true;
                    iter = ctx->traceables.end();
                }
            }
        }

        float ambient = 0.2f;
        if (!inShadow) {
            // not in shadow, use object color
            float diff = MAX(glm::dot(hit.normal, glm::normalize(lightDir)), 0.0f);
            glm::vec3 diffuse = diff * glm::vec3(1, 1, 1); // light color is 1,1,1
            // scale diffuse from 1 to 0.2
            diffuse.x = diffuse.x + ((1.0f - diffuse.x) * ambient);
            diffuse.y = diffuse.y + ((1.0f - diffuse.y) * ambient);
            diffuse.z = diffuse.z + ((1.0f - diffuse.z) * ambient);
            glm::vec3 vcolor = diffuse * VectorFloatColorFromIntColorRGB(hit.intColor);
            int32_t retClr = IntColorFromVectorFloatColorRGB(vcolor);
            memcpy(ctx->pixelData + (((iz * OUTPUT_WIDTH) + ix) * 4), &retClr, 4);
        }
        else {
            // in shadow, make object color darker
            // this code leaves a little ambient in the shadow
            // it's not completely black
            glm::vec3 vcolor = ambient * VectorFloatColorFromIntColorRGB(hit.intColor);
            int32_t retClr = IntColorFromVectorFloatColorRGB(vcolor);
            memcpy(ctx->pixelData + (((iz * OUTPUT_WIDTH) + ix) * 4), &retClr, 4);
        }
    }

}

void RenderSinglePixel(RenderContext *ctx, int ix, int iy)
{

    glm::vec3 xdistv = ctx->vdata[1] - ctx->vdata[0];
    float xstride = glm::length(xdistv);
    glm::vec3 ydistv = ctx->vdata[3] - ctx->vdata[0];
    float ystride = glm::length(ydistv);

    glm::vec3 normInXDir = glm::normalize(xdistv);
    glm::vec3 normInYDir = glm::normalize(ydistv);

    glm::vec3 lightPos(-2, -5, 5);

    int revy = (OUTPUT_HEIGHT - iy) - 1;
    float dy = (((float)revy / (float)OUTPUT_HEIGHT) * ystride);
    float dx = (((float)ix / (float)OUTPUT_WIDTH) * xstride);

    glm::vec3 p = glm::vec3(ctx->vdata[0]) + (normInXDir * dx);
    p = p + (normInYDir * dy);

    ShootRay(p, ix, iy, ctx, lightPos);
}

DWORD WINAPI RenderThread(void* context)
{

    RenderContext* ctx = (RenderContext*)context;

    glm::vec3 xdistv = ctx->vdata[1] - ctx->vdata[0];
    float xstride = glm::length(xdistv);
    glm::vec3 ydistv = ctx->vdata[3] - ctx->vdata[0];
    float ystride = glm::length(ydistv);

    glm::vec3 normInXDir = glm::normalize(xdistv);
    glm::vec3 normInYDir = glm::normalize(ydistv);

    glm::vec3 lightPos(-2, -5, 5);

    for (unsigned int iy = ctx->rowstart; iy < ctx->rowend + 1; iy++) {
        int revy = (OUTPUT_HEIGHT - iy) - 1;
        float dy = (((float)revy / (float)OUTPUT_HEIGHT) * ystride);
        for (int ix = 0; ix < OUTPUT_WIDTH; ix++) {
            float dx = (((float)ix / (float)OUTPUT_WIDTH) * xstride);

            glm::vec3 p = glm::vec3(ctx->vdata[0]) + (normInXDir * dx);
            p = p + (normInYDir * dy);

            ShootRay(p, ix, iy, ctx, lightPos);
        }
    }
    return 0;
}

int main()
{

    RTCube cube;
    cube.SetColor(0xffff8f8f);
    
    //RTRect rect;
    //rect.transforms.push_back(glm::rotate(glm::mat4(1.0f), 3.14159f / 2.0f, glm::vec3(1, 0, 0)));
    //rect.transforms.push_back(glm::scale(glm::mat4(1.0f), glm::vec3(10, 1, 10)));
    //rect.transforms.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-5, 2, -5)));
    //rect.ExecuteTransform();

    RTCube cube2;
    cube2.transforms.push_back(glm::rotate(glm::mat4(1.0f), 3.14159f / 2.0f, glm::vec3(1, 0, 0)));
    cube2.transforms.push_back(glm::scale(glm::mat4(1.0f), glm::vec3(10, 1, 10)));
    cube2.transforms.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-5, 3, -5)));
    cube2.ExecuteTransform();
    
    RTCube cube1;
    cube1.transforms.push_back(glm::scale(glm::mat4(1.0f), glm::vec3(10, 10, 0.5)));
    cube1.transforms.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-5, -1, -0.5)));
    cube1.ExecuteTransform();
    cube1.SetColor(0xff8f8f8f);

    RTSphere sphere;

    std::vector<RTRayTrace*> traceableObjects = {
        (RTRayTrace*)&sphere,
        (RTRayTrace*)&cube,
        (RTRayTrace*)&cube1,
        (RTRayTrace*)&cube2
    };


    float aspectRatio = (float)OUTPUT_WIDTH / (float)OUTPUT_HEIGHT;
    float viewportHeight = VIEWPORT_WIDTH / aspectRatio;

    // these are all defined in the x/z plane
    // and transformed to where I want them to be
    glm::vec4 vp[5];
    // lower left corner of viewport (min)
    vp[0] = glm::vec4(0, 0, 0, 1);
    // viewport corners counter-clockwise from ll corner
    vp[1] = glm::vec4(VIEWPORT_WIDTH, 0, 0, 1);
    vp[2] = glm::vec4(VIEWPORT_WIDTH, 0, viewportHeight, 1);
    vp[3] = glm::vec4(0, 0, viewportHeight, 1);
    // viewer origin
    vp[4] = glm::vec4(VIEWPORT_WIDTH / 2, 0, viewportHeight / 2.0f, 1);
    
    // viewer is originally defined as the center of the plane
    // transform the viewer on the y
    glm::mat4 x = glm::translate(glm::mat4(1.0f), glm::vec3(0, -5, 0));
    vp[4] = x * vp[4];
    
    // transform everything so the center of the plane
    // is at 0,0,0
    x = glm::translate(glm::mat4(1.0f), glm::vec3(-2, -10, -1.125));
    for (int i = 0; i < 5; i++) {
        vp[i] = x * vp[i];
    }
    // rotate everything a bit to get a cockeyed view
    // this is the best way to see the shadow of the block (for now)
    x = glm::rotate(glm::mat4(1.0f), 3.14159f / -8.0f, glm::vec3(1, 0, -1));
    for (int i = 0; i < 5; i++) {
        vp[i] = x * vp[i];
    }

    unsigned char* data = (unsigned char*)malloc(OUTPUT_WIDTH * OUTPUT_HEIGHT * 4);
    unsigned char clearColor[4] = {0x00,0x00,0x00,0xff};
    for (int p = 0; p < OUTPUT_WIDTH * OUTPUT_HEIGHT; p++) {
        memcpy(data + (p * 4), clearColor, 4);
    }

    // start threads here
    int interval = OUTPUT_HEIGHT / NUM_THREADS;
    RenderContext ctx[NUM_THREADS];
    for (unsigned int c = 0; c < NUM_THREADS; c++) {
        ctx[c] = { 
            data, 
            c * interval,
            ((c + 1) * interval) - 1,
            traceableObjects,
            &vp[0]
        };
    }

    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    LARGE_INTEGER ct;
    QueryPerformanceCounter(&ct);

    HANDLE t[NUM_THREADS];
    for (int c = 0; c < NUM_THREADS; c++) {
        t[c] = CreateThread(NULL, 0, RenderThread, &ctx[c], 0, NULL);
    }

    WaitForMultipleObjects(NUM_THREADS, t, TRUE, INFINITE);

    //RenderSinglePixel(&ctx[0], 362, 250);

    LARGE_INTEGER ct1;
    QueryPerformanceCounter(&ct1);

    printf("%.1f fps; %lli counts\n", 
        (double)freq.QuadPart / (double)(ct1.QuadPart - ct.QuadPart),
        ct1.QuadPart - ct.QuadPart);

    // RESAMPLE IMAGE
    unsigned char* msdata = (unsigned char*)malloc((OUTPUT_WIDTH / MULTI_SAMPLING) * (OUTPUT_HEIGHT / MULTI_SAMPLING) * 4);
    uint32_t* lpdata = (uint32_t*)data;
    uint32_t* lpmsdata = (uint32_t*)msdata;
    for (int msy = 0; msy < OUTPUT_HEIGHT / MULTI_SAMPLING; msy++)
    {
        int locy = msy * MULTI_SAMPLING;
        for (int msx = 0; msx < OUTPUT_WIDTH / MULTI_SAMPLING; msx++) {
            int locx = msx * MULTI_SAMPLING;
            int32_t src_color_1 = lpdata[(locy * OUTPUT_WIDTH) + locx];
            int32_t src_color_2 = lpdata[(locy * OUTPUT_WIDTH) + (locx+1)];
            int32_t src_color_3 = lpdata[((locy+1) * OUTPUT_WIDTH) + locx];
            int32_t src_color_4 = lpdata[((locy+1) * OUTPUT_WIDTH) + (locx+1)];
            int avga = (CALPHA(src_color_1) + CALPHA(src_color_2) + CALPHA(src_color_3) + CALPHA(src_color_4)) / 4;
            int avgb = (CBLUE(src_color_1) + CBLUE(src_color_2) + CBLUE(src_color_3) + CBLUE(src_color_4)) / 4;
            int avgg = (CGREEN(src_color_1) + CGREEN(src_color_2) + CGREEN(src_color_3) + CGREEN(src_color_4)) / 4;
            int avgr = (CRED(src_color_1) + CRED(src_color_2) + CRED(src_color_3) + CRED(src_color_4)) / 4;
            lpmsdata[(msy * (OUTPUT_WIDTH / MULTI_SAMPLING)) + msx] = CABGR(avga, avgb, avgg, avgr);
        }
    }

    stbi_write_png("simg.png", OUTPUT_WIDTH, OUTPUT_HEIGHT, 4, data, OUTPUT_WIDTH * 4);
    stbi_write_png("simgr.png",
        OUTPUT_WIDTH / MULTI_SAMPLING,
        OUTPUT_HEIGHT / MULTI_SAMPLING,
        4, msdata, OUTPUT_WIDTH / MULTI_SAMPLING * 4);
    free(data);
    free(msdata);

}


