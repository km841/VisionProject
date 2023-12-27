#ifndef _DEFAULT_FX_
#define _DEFAULT_FX_

#include "Params.fx"
struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0;
    
    output.pos = mul(float4(_in.pos, 1.f), g_matWVP);
    output.uv = _in.uv;

    return output;
}

float4 PS_Main(VS_OUT _in) : SV_Target
{
    float4 color = g_tex_on_0 == 1 ? g_tex_0.Sample(g_sam_0, _in.uv) : float4(g_vec4_0.xyz, 1.f);
    return color;
}
#endif