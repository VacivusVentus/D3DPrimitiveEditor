cbuffer Color : register (b0)
{
    float4 colorfill;
    float4 colorline;
    float4 stylesetting;
}


struct Input{
    float4 position:SV_POSITION;
    float2 uv:UV0;
};

float4 main():SV_TARGET
{
    return colorfill;
}

float4 PenMain():SV_TARGET
{ 
    return colorline;
}

float4 PLineMain(Input inp):SV_TARGET
{ 
    uint l = trunc(length(inp.uv)) % 10;

    float s = sin(float(l) * 3.14 / 5.0);
    return (s > 0.0) ? colorline : float4(0.0, 0.0, 0.0,0.0);
}

float4 PointsLineMain(Input inp):SV_TARGET
{ 
    uint l = trunc(length(inp.uv)) % 2;
    
    return (l == 1) ? colorline : float4(0.0, 0.0, 0.0,0.0);
}


float4 mainSelect():SV_TARGET
{
    return float4(0.0, 0.2, 0.6, 1.0);
}

float4 mainUnderCursor():SV_TARGET
{
    return lerp(float4(float3(1.0, 1.0, 1.0) - colorfill.xyz,colorfill.w), float4(0.2, 0.2, 0.2, colorfill.w), 0.5);
}
float4 mainlineUnderCursor():SV_TARGET
{
    return lerp(float4(float3(1.0, 1.0, 1.0) - colorline.xyz, 1.0), float4(1.0,0.0,0.0,1.0), 0.5);
}