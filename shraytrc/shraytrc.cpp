// shraytrc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "common.h"

#define OUTPUT_WIDTH 1920
#define OUTPUT_HEIGHT 1080
#define MULTI_SAMPLING 2
#define NUM_THREADS 9

glm::vec4 lightColor(1, 1, 1, 1);

struct RenderContext
{
    unsigned char* pixelData;
    unsigned int rowstart;
    unsigned int rowend;
    glm::mat4* view;
    glm::vec3* transformed_orig;
    std::vector<RTRayTrace*> traceables;
    float vpxmin;
    float vpxmax;
    float vpymin;
    float vpymax;
};

void ShootRay(float xc, float zc, int ix, int iz, RenderContext* ctx, const glm::vec3& lightPos)
{
    float t0 = 0.0f;
    float t1 = 0.0f;
    int32_t clr0 = 0;
    int32_t clr1 = 0;

    // plane is 2 units behind x/y axis
    // and is 4 units by 4 units
    glm::vec3 original_f(xc, -5.0f, zc);
    glm::vec3 transformed_f = *ctx->view * glm::vec4(original_f, 1);
    glm::vec3 dir = transformed_f - *ctx->transformed_orig;
    glm::vec3 ndir = glm::normalize(dir);

    std::vector<std::tuple<float, uint32_t>> colors;
    std::vector<RTRayTrace*>::iterator iter = ctx->traceables.begin();
    for (; iter != ctx->traceables.end(); ++iter)
    {
        glm::vec3 ix;
        glm::vec3 norm0;
        glm::vec3 bc;
        bool b = (*iter)->IntersectWithRay(*ctx->transformed_orig, ndir, t0, clr0, &ix, &norm0, &bc, false);
        if (b) {
            // ray intersects with item
            glm::vec3 lightDir = lightPos - ix;
            std::vector<RTRayTrace*>::iterator iter1 = ctx->traceables.begin();
            for (; iter1 != ctx->traceables.end(); ++iter1)
            {
                bool b1 = (*iter1)->IntersectWithRay(ix, lightDir, t1, clr1, NULL, NULL, NULL, false);
                if (b1) {
                    //colors.push_back(std::make_tuple(t0, 0xff000000));
                    glm::vec3 vcolor = 0.2f * VectorFloatColorFromIntColorRGB(clr0);
                    int32_t retClr = IntColorFromVectorFloatColorRGB(vcolor);
                    colors.push_back(std::make_tuple(t0, retClr));
                }
                else {
                    float diff = MAX(glm::dot(norm0, glm::normalize(lightDir)), 0.0f);
                    glm::vec3 diffuse = diff * glm::vec3(1, 1, 1); // light color is 1,1,1
                    glm::vec3 vcolor = diffuse * VectorFloatColorFromIntColorRGB(clr0);
                    int32_t retClr = IntColorFromVectorFloatColorRGB(vcolor);
                    colors.push_back(std::make_tuple(t0, retClr));
                }
            }
        }
    }


    //int32_t clr = 0;
    //bool hasFace = false;
    //
    //for(int faceIndex = 0; faceIndex < 12; faceIndex++) {
    //    Tri& face = ctx->faceData[faceIndex];
    //    face.b = rayTriangleIntersect(*ctx->transformed_orig, ndir, face, t);
    //    if (face.b) {
    //        colors.push_back(std::make_tuple(t, face.icolor));
    //        face.t = t;
    //    }
    //}

    if (colors.size() > 0) {
        //uint32_t blended = blend(std::get<1>(colors[0]), 0xff000000);
        //if (colors.size() > 1) {
        //    std::sort(colors.begin(), colors.end());
        //    printf("colors\n");
        //    printf("%.1f\n", std::get<0>(colors[0]));
        //    for (int ci = 1; ci < colors.size(); ci++) {
        //    //for(int ci = colors.size() - 1; ci > 0; ci--) {
        //        printf("%.1f\n", std::get<0>(colors[ci]));
        //        blended = blend(std::get<1>(colors[ci]), blended);
        //    }
        //}
        if(colors.size() > 1) std::sort(colors.begin(), colors.end());
        uint32_t objectColor = std::get<1>(colors[0]);
        memcpy(ctx->pixelData + (((iz * OUTPUT_WIDTH) + ix) * 4), &objectColor, 4);
    }

}

void RenderSinglePixel(RenderContext *ctx, int ix, int iz)
{
    float ystride = ctx->vpymax - ctx->vpymin;
    float xstride = ctx->vpxmax - ctx->vpxmin;
    glm::vec3 lightPos(-5, -5, 5);
    int revz = (OUTPUT_HEIGHT - iz) - 1;
    float zc = (((float)revz / (float)OUTPUT_HEIGHT) * ystride) + ctx->vpymin;
    float xc = (((float)ix / (float)OUTPUT_WIDTH) * xstride) + ctx->vpxmin;
    ShootRay(xc, zc, ix, iz, ctx, lightPos);
}

DWORD WINAPI RenderThread(void* context)
{

    RenderContext* ctx = (RenderContext*)context;

    float ystride = ctx->vpymax - ctx->vpymin;
    float xstride = ctx->vpxmax - ctx->vpxmin;
    glm::vec3 lightPos(-5, -5, 5);
    for (unsigned int iz = ctx->rowstart; iz < ctx->rowend + 1; iz++) {
        int revz = (OUTPUT_HEIGHT - iz) - 1;
        float zc = (((float)revz / (float)OUTPUT_HEIGHT) * ystride) + ctx->vpymin;
        for (int ix = 0; ix < OUTPUT_WIDTH; ix++) {
            float xc = (((float)ix / (float)OUTPUT_WIDTH) * xstride) + ctx->vpxmin;
            ShootRay(xc, zc, ix, iz, ctx, lightPos);
        }
    }
    return 0;
}

int main()
{

    RTCube cube;
    cube.SetColor(0xffff8f8f);
    
    RTRect rect;
    rect.transforms.push_back(glm::rotate(glm::mat4(1.0f), 3.14159f / 2.0f, glm::vec3(1, 0, 0)));
    rect.transforms.push_back(glm::scale(glm::mat4(1.0f), glm::vec3(10, 1, 10)));
    rect.transforms.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-5, 2, -5)));
    rect.ExecuteTransform();
    
    RTCube cube1;
    cube1.transforms.push_back(glm::scale(glm::mat4(1.0f), glm::vec3(10, 10, 0.5)));
    cube1.transforms.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-5, -1, -0.5)));
    cube1.ExecuteTransform();
    cube1.SetColor(0xff8f8f8f);

    std::vector<RTRayTrace*> traceableObjects = {
        (RTRayTrace*)&cube, 
        (RTRayTrace*)&rect,
        (RTRayTrace*)&cube1
    };


    glm::mat4 view = //glm::mat4(1.0f);
        //glm::translate(glm::mat4(1.0f), glm::vec3(0, 10, 0));
        glm::rotate(glm::mat4(1.0f), 3.14159f / -8.0f, glm::vec3(1, 0, 1));
        // glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0, 0, 1));
    //glm::mat4 view = glm::mat4(1.0f);

    // original is 10 units behind x/y axis
    glm::vec3 original_orig(0, -10, 0);
    glm::vec3 transformed_orig = view * glm::vec4(original_orig,1);

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
            &view,
            &transformed_orig,
            traceableObjects,
            -2.0f, 2.0f, -1.125f, 1.125f
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

    //RenderSinglePixel(&ctx0, 575, 368);

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


