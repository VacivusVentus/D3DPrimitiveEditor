cbuffer Camera : register(b0)
{
    float4x4 projectionMatrix;
};
cbuffer Coord : register(b1)
{
    float4 coord;
    float4 params;
};
#define stepAng 3.14 / 20.0
struct EllipseInput
{
    uint index:SV_VERTEXID;
    uint inst:SV_INSTANCEID;
};


struct Output{
    float4 position:SV_POSITION;
    float2 uv:UV0;
};
#define PI 3.14156
float2 rotateZ(float2 pnt, float2 center)
{
    float2 w = pnt - center;
    float a = params.x;
    float2 r = float2(w.x * cos(a) + w.y * sin(a), w.y * cos(a) - w.x * sin(a));
    return r + center; 
}

float4 mainEllipseFill(EllipseInput inp) : SV_POSITION {
    float rx = coord.z;
    float ry = coord.w;
    uint ins2 = inp.inst + 1;
    const float2 pos[] = { 
        float2(0.0, 0.0),
        float2(rx * cos(stepAng * inp.inst), ry * sin(stepAng * inp.inst)),
        float2(rx * cos(stepAng * ins2), ry * sin(stepAng * ins2)),
    };
    float4 position = mul(float4(rotateZ(pos[inp.index], float2(0.0,0.0)) + coord.xy, 0.0, 1.0), projectionMatrix);
    position.x = position.x - 1.0;
    position.y = position.y + 1.0;
    return position;
}

Output mainEllipseLine(EllipseInput inp) {
    
    float rx = coord.z + inp.inst;
    float ry = coord.w + inp.inst;
    float2 fpoint = float2( rx * cos(stepAng * inp.index / 2), ry * sin(stepAng * inp.index / 2));
    float4 position = mul(float4(rotateZ(fpoint, float2(0.0,0.0)) + coord.xy, 0.0, 1.0), projectionMatrix);
    position.x = position.x - 1.0;
    position.y = position.y + 1.0;
    Output o;
    o.position = position;
    o.uv = float2(inp.index, 0.0);
    return o;
}

float4 mainRectFill(uint index:SV_VERTEXID) : SV_POSITION {
    const float2 vert[] = {
        float2(-coord.z, -coord.w),
        float2(coord.z, -coord.w),
        float2(coord.z, coord.w),
        float2(-coord.z, coord.w),
    };

    float cx = (coord.z + coord.x) / 2.0;
    float cy = (coord.w + coord.y) / 2.0;

    const uint ind[] = {0, 1, 2, 0, 2, 3};
    float4 position = mul(float4(rotateZ(vert[ind[index]], float2(0.0,0.0)) + coord.xy, 0.0, 1.0), projectionMatrix);
    position.x = position.x - 1.0;
    position.y = position.y + 1.0;
    return position;
}

Output mainRectLine(EllipseInput inp){
    float2 crd = float2(coord.z + inp.inst, coord.w + inp.inst);
    const float2 vert[] = {
        float2(-crd.x, -crd.y),
        float2(crd.x, -crd.y),
        float2(crd.x, crd.y),
        float2(-crd.x, crd.y),
        float2(-crd.x, -crd.y),
    };
    float2 crd1 = float2(coord.z, coord.w);
    const float2 vert0[] = {
        float2(-crd1.x, -crd1.y),
        float2(crd1.x, -crd1.y),
        float2(crd1.x, crd1.y),
        float2(-crd1.x, crd1.y),
        float2(-crd1.x, -crd1.y),
    };


    float4 position = mul(float4(rotateZ(vert[inp.index], float2(0.0,0.0)) + coord.xy, 0.0, 1.0), projectionMatrix);
    position.x = position.x - 1.0;
    position.y = position.y + 1.0;
    Output o;
    o.position = position;
    o.uv = float2(length(vert0[inp.index] - vert0[inp.index - 1]), 0.0);
    return o;
}

Output mainFrameforRect(EllipseInput inp)
{
    float2 crd = float2(coord.z + 2 * inp.inst, coord.w + 2 * inp.inst);
    const float2 vert[] = {
        float2(-crd.x, -crd.y),
        float2(crd.x, -crd.y),
        float2(crd.x, crd.y),
        float2(-crd.x, crd.y),
        float2(-crd.x, -crd.y),
    };

    float4 position = mul(float4(rotateZ(vert[inp.index], float2(0.0,0.0)) + coord.xy, 0.0, 1.0), projectionMatrix);
    position.x = position.x - 1.0;
    position.y = position.y + 1.0;
    Output o;
    o.position = position;
    o.uv = float2(0,0);
    return o;
}