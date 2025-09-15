#ifndef _VALUE
#define _VALUE

#include "struct.fx"

// 4096
cbuffer Transform : register(b0)
{
    row_major matrix g_matWorld;
    row_major matrix g_matView;
    row_major matrix g_matProj;
    
    row_major matrix g_matWV;
    row_major matrix g_matWVP;
    float           g_ObjectSpawnTime;
    float3          padding;
};

cbuffer Material : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;
    
    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
    row_major Matrix g_mat_0;
    row_major Matrix g_mat_1;
    
    int g_btex_0;
    int g_btex_1;
    int g_btex_2;
    int g_btex_3;
    int g_btex_4;
    int g_btex_5;
    int g_btexarr_0;
    int g_btexarr_1;
    int g_btexarr_2;
    int g_btexarr_3;
    int g_btexcube_0;
    int g_btexcube_1;
}

cbuffer SPRITE : register(b2)
{
    float2 g_LeftTopUV;
    float2 g_SliceUV;
    float2 g_BackgroundUV;
    float2 g_OffsetUV;
    
    int     g_SpriteUse;
    float3  g_HitColor;
    float   g_HitEffectRatio; // ��Ʈ ȿ�� ���� (0~1)
    int     g_UseHitEffect;
    float2  g_padding;
}

cbuffer GLOBAL : register(b3)
{
    float2 g_RenderResolution; // ������ �ػ�
    float g_DeltaTime; // DT
    float g_Time; // �����ð�
    float g_DT_Engine; // DT
    float g_Time_Engine; // �����ð�
    int g_Light2DCount; // 2D ���� ����
    int g_Light3DCount; // 3D ���� ����
    
}

Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);

Texture2DArray g_texarr_0 : register(t6);
Texture2DArray g_texarr_1 : register(t7);
Texture2DArray g_texarr_2 : register(t8);
Texture2DArray g_texarr_3 : register(t9);

TextureCube g_texcube_0 : register(t10);
TextureCube g_texcube_1 : register(t11);

Texture2D g_Atlas : register(t12); // Sprite ���� �ؽ��� ��������

StructuredBuffer<tLight2DInfo> g_Light2DInfo : register(t13); // 2D ���� ����
//StructuredBuffer<tLight3DInfo> g_Light3DInfo : register(t14); // 2D ���� ����
// StructuredBuffer<tBone>       g_Light3DInfo : register(t15); // 3D Animation Skinning Matrix

SamplerState g_sam_0 : register(s0); // �̹漺 ���͸�
SamplerState g_sam_1 : register(s1); // MIN_MAG_POINT ���͸�


#define PI  3.1415926535f

// 1. VertexBuffer
// 2. IndexBuffer
// 3. ConstBuffer(ũ�⿡ ����, ������)
// 4. StructuredBuffer( ����ȭ ���� )

#endif