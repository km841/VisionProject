#ifndef _PARAMS_FX_
#define _PARAMS_FX_

Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);
Texture2D g_tex_6 : register(t6);
Texture2D g_tex_7 : register(t7);
Texture2D g_tex_8 : register(t8);
Texture2D g_tex_9 : register(t9);
Texture2D g_tex_10 : register(t10);

SamplerState g_sam_0 : register(s0);

cbuffer TRANSFORM_PARAMS : register(b0)
{
    row_major matrix g_matWorld;
    row_major matrix g_matView;
    row_major matrix g_matProjection;
    row_major matrix g_matWV;
    row_major matrix g_matWVP;
    row_major matrix g_matViewInv;
}

cbuffer MATERIAL_PARAMS : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    //16
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    //16 + 16 = 32
    
    int g_tex_on_0;
    int g_tex_on_1;
    int g_tex_on_2;
    int g_tex_on_3;
    // 16 + 176 = 192
    
    int g_tex_on_4;
    int g_tex_on_5;
    int g_tex_on_6;
    int g_tex_on_7;
    // 16 + 192 = 208
    
    int g_tex_on_8;
    int g_tex_on_9;
    int g_tex_on_10;
    int g_tex_on_11;
    // 16 + 208 = 224
    
    int g_tex_on_12;
    int g_tex_on_13;
    int g_tex_on_14;
    int g_tex_on_15;
    // 16 + 224 = 240
    
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;
    //4 * 8 = 32 + 32 = 64
    
    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
    //4 * 16 = 64 + 112 = 176
    
    row_major matrix g_mat_0;
    row_major matrix g_mat_1;
    row_major matrix g_mat_2;
    row_major matrix g_mat_3;
}

#endif