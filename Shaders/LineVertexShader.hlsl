cbuffer Camera : register(b0)
{
    float4x4 projectionMatrix;
};

cbuffer Coord : register(b1)
{
    float4 coord;
    float4 params;
};
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

#define A90 3.14156 / 2.0
Output main(EllipseInput inp)
{
    float2 pos[2];
    pos[0] = float2(coord.x, coord.y);
    pos[1] = float2(coord.z, coord.w);
    float2 center = (pos[0] + pos[1]) * 0.5;
    float2 normal = normalize(pos[1] - pos[0]);    
    normal.x = normal.y * sin(A90);   
    normal.y = -normal.x * sin(A90);
    float p = (inp.inst + 1) / 2;
    uint q = ((inp.inst & 1) == 0) ? -1 : 1;
    p *= q;
    normal *= p;
    float4 position = mul(float4(rotateZ(pos[inp.index] + normal, center), 0.0, 1.0), projectionMatrix);
    position.x = position.x - 1.0;
    position.y = position.y + 1.0;
    Output output;
    output.position = position;
    output.uv = float2(length(pos[inp.index] - pos[0]), 0.0);
    return output;
}

Output FrameForLine(EllipseInput inp) {
    float2 pos[8];
    pos[0] = float2(coord.x - 5, coord.y - 5);
    pos[1] = float2(coord.x + 5, coord.y + 5);
    pos[2] = float2(coord.x - 5, coord.y + 5);
    pos[3] = float2(coord.x + 5, coord.y - 5);
    
    pos[4] = float2(coord.z - 5, coord.w - 5);
    pos[5] = float2(coord.z + 5, coord.w + 5);
    pos[6] = float2(coord.z - 5, coord.w + 5);
    pos[7] = float2(coord.z + 5, coord.w - 5);
    float2 center = (pos[0] + pos[1]);
    Output output;
    output.position = mul(float4(rotateZ(pos[inp.index], center), 0.0, 1.0), projectionMatrix);
    output.position.x = output.position.x - 1.0;
    output.position.y = output.position.y + 1.0;
    output.uv = float2(0,0);
    return output;

}