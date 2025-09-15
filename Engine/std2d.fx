#ifndef _STD2D
#define _STD2D

#include "value.fx"
#include "func.fx"

// Vertex Shader 
struct VS_IN
{
    // semantic
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    // ���� ȿ�� �����鼭 �߰���.
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vUV = _in.vUV;
        
    return output;
}

// Std2D Shader
float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    // Texture Meterial �� ����ϱ� ��
    //float4 vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    //return vColor;
    
    // g_tex_0 texture �� ���ε� �Ǿ�����, �ؽ��ĸ� ���ø�
    // ���ε� �Ǿ����� ������ ����Ÿ, ȸ�� ��� Debug �� ������ ��� ��
    
    
    /////////////////////////////////////////////////
    // 1. shader �󿡼� �ذ�             �ؽ��� �˻縦 Shader����             
    //int width = 0;
    //int height = 0;
    //g_tex_0.GetDimensions(width, height);
    
    //if (!width || !height)
    //{
    //    return GetDebugColor(_in.vUV, 10);
    //}
    //else
    //{
    //    return g_tex_0.Sample(g_sam_1, _in.vUV);
    //}
    ///////////////////////////////////////////////////

    // 2. ������ ���ؼ� �ذ�   
    // �ؽ��İ� �ִٸ�, ���ø�
    float4 vColor = (float4) 0.f;
    
    if(g_int_0 == 3)
        discard;
    // Flipbook �� ���� Sprite �� ����ؾ� �ϴ� ���
    if (g_SpriteUse)
    {
        //float2 vSpriteUV = g_LeftTopUV + (_in.vUV * g_SliceUV);
        
        float2 vBackroundLeftTop = g_LeftTopUV + (g_SliceUV / 2.f) - (g_BackgroundUV / 2.f);
        float2 vSpriteUV = vBackroundLeftTop + (_in.vUV * g_BackgroundUV) - g_OffsetUV;
        
        if (vSpriteUV.x < g_LeftTopUV.x || g_LeftTopUV.x + g_SliceUV.x < vSpriteUV.x
            || vSpriteUV.y < g_LeftTopUV.y || g_LeftTopUV.y + g_SliceUV.y < vSpriteUV.y)
        {
            //vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
        else
        {
            vColor = g_Atlas.Sample(g_sam_1, vSpriteUV);
        }
    }
    
    // FlipbookPlayer �� ���ų� ������� Flipbook �� ���� ���
    else
    {
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        else
            vColor = GetDebugColor(_in.vUV, 10);
    }
       
    if (vColor.a == 0.f)
        discard;
    
    // ����ó��
    float3 LightColor = float3(0.f, 0.f, 0.f);
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalcLight2D(i, _in.vWorldPos, LightColor);
    }
    
    vColor.rgb *= LightColor;
    
    return vColor;
}

// Std2DAlphaBlend
float4 PS_Std2D_AlphaBlend(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    if (g_SpriteUse)
    {
        //float2 vSpriteUV = g_LeftTopUV + (_in.vUV * g_SliceUV);
        
        float2 vBackroundLeftTop = g_LeftTopUV + (g_SliceUV / 2.f) - (g_BackgroundUV / 2.f);
        float2 vSpriteUV = vBackroundLeftTop + (_in.vUV * g_BackgroundUV) - g_OffsetUV;
        
        if (vSpriteUV.x < g_LeftTopUV.x || g_LeftTopUV.x + g_SliceUV.x < vSpriteUV.x
            || vSpriteUV.y < g_LeftTopUV.y || g_LeftTopUV.y + g_SliceUV.y < vSpriteUV.y)
        {
            //vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
        else
        {
            vColor = g_Atlas.Sample(g_sam_1, vSpriteUV);
        }
        
            
    // Paperburn
    // 2��° �ؽ��� ���, ������ �ؽ���
        if (g_btex_1)
        {
            float4 vNoise = g_tex_1.Sample(g_sam_1, _in.vUV);
            if (1.f < vNoise.r + g_float_0)
                discard;
        }
    }
    else
    {
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        else
            vColor = GetDebugColor(_in.vUV, 10);
    }
    
    // ����ó��
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    //  g_Int_0 == 3�϶� ��簪 ��������.
    if(g_int_0 == 3)
        LightColor = float3(0.f, 0.f, 0.1f);

    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalcLight2D(i, _in.vWorldPos, LightColor);
    }
    
    if (vColor.a == 0.f)
        discard;   
    
    float3 finalColor = float3(0.f, 0.f,0.f);
    
    finalColor = vColor.rgb *= LightColor;
    
    // ��Ʈ ȿ�� ����
    if (g_UseHitEffect)
    {
        finalColor = lerp(finalColor, g_HitColor, g_HitEffectRatio * 0.7f);
    }

    
    
    if(g_int_0 ==2)
        finalColor += float3(0.1, 0.1, 0.16);
    if(g_int_0 == 4)
        finalColor += float3(0.f, 0.f, 0.5f);
    return float4(finalColor, vColor.a);
}
// Std2dPaperBurn
float4 PS_Std2D_PaperBurn(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    if (g_SpriteUse)
    {
        //float2 vSpriteUV = g_LeftTopUV + (_in.vUV * g_SliceUV);
        
        float2 vBackroundLeftTop = g_LeftTopUV + (g_SliceUV / 2.f) - (g_BackgroundUV / 2.f);
        float2 vSpriteUV = vBackroundLeftTop + (_in.vUV * g_BackgroundUV) - g_OffsetUV;
        
        if (vSpriteUV.x < g_LeftTopUV.x || g_LeftTopUV.x + g_SliceUV.x < vSpriteUV.x
            || vSpriteUV.y < g_LeftTopUV.y || g_LeftTopUV.y + g_SliceUV.y < vSpriteUV.y)
        {
            //vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
        else
        {
            vColor = g_Atlas.Sample(g_sam_1, vSpriteUV);
        }
    }
    else
    {
        // ù��° �ؽ��Ĵ� ��ü�� ����
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        else
            vColor = GetDebugColor(_in.vUV, 10);
    }
    
    
    // 2��° �ؽ��� ���, ������ �ؽ���
    if (g_btex_1)
    {
        float4 vNoise = g_tex_1.Sample(g_sam_0, _in.vUV);
        if (1.f < vNoise.r + g_float_0)
            discard;
    }
    
    if (vColor.a == 0.f)
        discard;
    
    // ����ó��
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalcLight2D(i, _in.vWorldPos, LightColor);
    }
    
    vColor.rgb *= LightColor;
    
    return vColor;
}

// Std2DUI Shader
VS_OUT VS_Std2DUI(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vUV = _in.vUV;
        
    return output;
}

// Std2D Shader
float4 PS_Std2DUI(VS_OUT _in) : SV_Target
{
    
    float4 vColor = (float4) 0.f;
    
    if (g_int_0 == 3)
        discard;
    // Flipbook �� ���� Sprite �� ����ؾ� �ϴ� ���
    if (g_SpriteUse)
    {
        //float2 vSpriteUV = g_LeftTopUV + (_in.vUV * g_SliceUV);
        
        float2 vBackroundLeftTop = g_LeftTopUV + (g_SliceUV / 2.f) - (g_BackgroundUV / 2.f);
        float2 vSpriteUV = vBackroundLeftTop + (_in.vUV * g_BackgroundUV) - g_OffsetUV;
        
        if (vSpriteUV.x < g_LeftTopUV.x || g_LeftTopUV.x + g_SliceUV.x < vSpriteUV.x
            || vSpriteUV.y < g_LeftTopUV.y || g_LeftTopUV.y + g_SliceUV.y < vSpriteUV.y)
        {
            //vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
        else
        {
            vColor = g_Atlas.Sample(g_sam_1, vSpriteUV);
        }
    }
    
    // FlipbookPlayer �� ���ų� ������� Flipbook �� ���� ���
    else
    {
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        else
            vColor = GetDebugColor(_in.vUV, 10);
    }
       
    if (vColor.a == 0.f)
        discard;
    
    
    return vColor;
}
// Std2DUI Shader
VS_OUT VS_Std2DAsset(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vUV = _in.vUV;
        
    return output;
}

// Std2D Shader
float4 PS_Std2DAsset(VS_OUT _in) : SV_Target
{
    
    float4 vColor = (float4) 0.f;
    
    if (g_SpriteUse)
    {
        //float2 vSpriteUV = g_LeftTopUV + (_in.vUV * g_SliceUV);
        
        float2 vBackroundLeftTop = g_LeftTopUV + (g_SliceUV / 2.f) - (g_BackgroundUV / 2.f);
        float2 vSpriteUV = vBackroundLeftTop + (_in.vUV * g_BackgroundUV) - g_OffsetUV;
        
        if (vSpriteUV.x < g_LeftTopUV.x || g_LeftTopUV.x + g_SliceUV.x < vSpriteUV.x
            || vSpriteUV.y < g_LeftTopUV.y || g_LeftTopUV.y + g_SliceUV.y < vSpriteUV.y)
        {
            //vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
        else
        {
            vColor = g_Atlas.Sample(g_sam_1, vSpriteUV);
        }
        
    // Paperburn
    // 2��° �ؽ��� ���, ������ �ؽ���
        if (g_btex_1)
        {
            float4 vNoise = g_tex_1.Sample(g_sam_1, _in.vUV);
            if (1.f < vNoise.r + g_float_0)
                discard;
        }
    }
    else
    {
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        else
            vColor = GetDebugColor(_in.vUV, 10);
        
    }
    
    // ����ó��
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalcLight2D(i, _in.vWorldPos, LightColor);
    }
    
    if (vColor.a == 0.f)
        discard; 
    // ���� �޹�� ����
    if (vColor.r == (60.f / 255.f) && vColor.g == (86.f / 255.f) && vColor.b == (39.f / 255.f))
        discard;

    if (g_int_0 == 1)
        vColor -= float4(0.27f, 0.25f, 0.08f, 0.f);
    
    float3 finalColor = float3(0.f, 0.f, 0.f); 
    
    finalColor = vColor.rgb *= LightColor;
    
    // ��Ʈ ȿ�� ����
    if (g_UseHitEffect)
    {
        finalColor = lerp(finalColor, g_HitColor, g_HitEffectRatio * 0.7f);
    }

   
    return float4(finalColor, vColor.a);
}




#endif