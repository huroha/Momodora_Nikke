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
    // 광원 효과 넣으면서 추가됨.
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vUV = _in.vUV;
        
    return output;
}

// Std2D Shader
float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    // Texture Meterial 로 출력하기 전
    //float4 vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    //return vColor;
    
    // g_tex_0 texture 가 바인딩 되었으면, 텍스쳐를 샘플링
    // 바인딩 되어있지 않으면 마젠타, 회색 등등 Debug 용 색상을 띄울 것
    
    
    /////////////////////////////////////////////////
    // 1. shader 상에서 해결             텍스쳐 검사를 Shader에서             
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

    // 2. 재질을 통해서 해결   
    // 텍스쳐가 있다면, 샘플링
    float4 vColor = (float4) 0.f;
    
    if(g_int_0 == 3)
        discard;
    // Flipbook 의 현재 Sprite 를 재생해야 하는 경우
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
    
    // FlipbookPlayer 가 없거나 재생중인 Flipbook 이 없는 경우
    else
    {
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        else
            vColor = GetDebugColor(_in.vUV, 10);
    }
       
    if (vColor.a == 0.f)
        discard;
    
    // 광원처리
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
    // 2번째 텍스쳐 사용, 노이즈 텍스쳐
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
    
    // 광원처리
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    //  g_Int_0 == 3일때 블루값 더해주자.
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
    
    // 히트 효과 적용
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
        // 첫번째 텍스쳐는 물체의 색상
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        else
            vColor = GetDebugColor(_in.vUV, 10);
    }
    
    
    // 2번째 텍스쳐 사용, 노이즈 텍스쳐
    if (g_btex_1)
    {
        float4 vNoise = g_tex_1.Sample(g_sam_0, _in.vUV);
        if (1.f < vNoise.r + g_float_0)
            discard;
    }
    
    if (vColor.a == 0.f)
        discard;
    
    // 광원처리
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
    // Flipbook 의 현재 Sprite 를 재생해야 하는 경우
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
    
    // FlipbookPlayer 가 없거나 재생중인 Flipbook 이 없는 경우
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
    // 2번째 텍스쳐 사용, 노이즈 텍스쳐
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
    
    // 광원처리
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalcLight2D(i, _in.vWorldPos, LightColor);
    }
    
    if (vColor.a == 0.f)
        discard; 
    // 에셋 뒷배경 제거
    if (vColor.r == (60.f / 255.f) && vColor.g == (86.f / 255.f) && vColor.b == (39.f / 255.f))
        discard;

    if (g_int_0 == 1)
        vColor -= float4(0.27f, 0.25f, 0.08f, 0.f);
    
    float3 finalColor = float3(0.f, 0.f, 0.f); 
    
    finalColor = vColor.rgb *= LightColor;
    
    // 히트 효과 적용
    if (g_UseHitEffect)
    {
        finalColor = lerp(finalColor, g_HitColor, g_HitEffectRatio * 0.7f);
    }

   
    return float4(finalColor, vColor.a);
}




#endif