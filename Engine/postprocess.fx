#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"

// Vertex Shader 
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

// Mesh : RectMesh

VS_OUT VS_Post(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_Post(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
            
    vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    float Aver = (vColor.r + vColor.g + vColor.b) / 3.f;
    vColor.rgb = Aver;
        
    return vColor;
}



// =================
// Distortion Shader
// =================
VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
        
    return output;
}

// 1. 픽셀쉐이더의 픽셀 좌표
// 2. 렌더 타겟 해상도

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    // SV_Position -> Pixelshader (픽셀좌표)
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    
    // NoiseTexture 가 있으면
    if (g_btex_1)
    {
        float4 vNoise = g_tex_1.Sample(g_sam_0, vScreenUV + 0.05f * g_Time);
        vNoise -= 0.1f;
        vNoise *= 0.01f;
        vScreenUV += vNoise.xy;
    }

    
    vColor = g_tex_0.Sample(g_sam_0, vScreenUV);
    
    return vColor;
}



struct VS_VortexOut
{
    float4 vPosition : SV_Position;
    float2 vObjectUV : TEXCOORD;
};


VS_VortexOut VS_Vortex(VS_IN _in)
{
    VS_VortexOut output = (VS_VortexOut) 0.f;
            
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
        
    float4 vProjPos = mul(float4(0.f, 0.f, 0.f, 1.f), g_matWVP);
    vProjPos.xyz = vProjPos.xyz / vProjPos.w;
    
    output.vObjectUV.x = (vProjPos.x + 1.f) / 2.f;
    output.vObjectUV.y = 1.f - ((vProjPos.y + 1.f) / 2.f);
        
    return output;
}


float4 PS_Vortex(VS_VortexOut _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    float effectRadius = 0.1f;
    float effectAngle = 1.f * PI;
    
    float2 center = _in.vObjectUV;
    
    float2 uv = (_in.vPosition.xy / g_RenderResolution.xy) - center;
    
    float len = length(uv * float2(g_RenderResolution.x / g_RenderResolution.y, 1.));
    float angle = atan2(uv.y, uv.x) + effectAngle * smoothstep(effectRadius, 0., len);
    float radius = length(uv);

    vColor = g_tex_0.Sample(g_sam_0, float2(radius * cos(angle), radius * sin(angle)) + center);
    
    return vColor;
}


///////Wave ripple
struct VS_WaveOut
{
    float4 vPosition : SV_Position;
    float2 vObjectUV : TEXCOORD;
};



VS_WaveOut VS_Wave(VS_IN _in)
{
    VS_WaveOut output = (VS_WaveOut) 0.f;
            
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
        
    float4 vProjPos = mul(float4(0.f, 0.f, 0.f, 1.f), g_matWVP);
    vProjPos.xyz = vProjPos.xyz / vProjPos.w;
    
    output.vObjectUV.x = (vProjPos.x + 1.f) / 2.f;
    output.vObjectUV.y = 1.f - ((vProjPos.y + 1.f) / 2.f);
        
    return output;
}


float4 PS_Wave(VS_WaveOut _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    float freq = 40.f;
    float speed = 8.f;
    float strength = 6.6;
    float dropOff = 0.8f;
    float duration = 2.4f;
    float initialStrength = 0.82f;
    float fallOffStrength = 1.4;
    float2 origin = _in.vObjectUV;
    
    // g_SpawnTime 대신 g_ObjectSpawnTime 사용
    float elapsedTime = g_Time - g_ObjectSpawnTime;
    
    if (elapsedTime > duration)
    {
        return g_tex_0.Sample(g_sam_0, _in.vObjectUV);
    }
    
    float2 uv = (_in.vPosition.xy / g_RenderResolution.xy);
    float2 uv0 = uv;
    
    uv = (uv - origin) * 2.;
    uv.x *= g_RenderResolution.x / g_RenderResolution.y;
    
    float dist = length(uv);
    float depth = cos((dist * freq) - (elapsedTime * speed));
    
    if (dist > dropOff)
    {
        depth = 0.;
    }
    else
    {
        depth *= dropOff - dist;
    }
    
    float fallOff = (elapsedTime / duration);
    depth *= 1. - fallOff;
    depth *= smoothstep(1. - fallOff * (duration * initialStrength), 1., 1. - dist);
    depth *= smoothstep(0., fallOff * (duration * fallOffStrength), dist);
    
    float4 tex = g_tex_0.Sample(g_sam_0, uv0 - abs(normalize(uv)) * -depth * (strength * 0.05));
    
    vColor = tex;
    
    float outlineThickness = 0.8f;
    float outline = smoothstep(1.0 - outlineThickness, 1.0, abs(depth));
    vColor = lerp(tex, float4(1, 0.85, 0.7, 1), outline);
    
    return vColor;
}

///////light
struct VS_LightSpreadOut
{
    float4 vPosition : SV_Position;
    float2 vObjectUV : TEXCOORD;
};

// 퍼져나가는 빛
VS_OUT VS_LightSpread(VS_IN _in)
{
    VS_LightSpreadOut output = (VS_LightSpreadOut) 0.f;
            
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
        
    float4 vProjPos = mul(float4(0.f, 0.f, 0.f, 1.f), g_matWVP);
    vProjPos.xyz = vProjPos.xyz / vProjPos.w;
    
    output.vObjectUV.x = (vProjPos.x + 1.f) / 2.f;
    output.vObjectUV.y = 1.f - ((vProjPos.y + 1.f) / 2.f);
        
    return output;
}


float ring(float x, float center, float thickness)
{
    return smoothstep(center - thickness, center, x) - smoothstep(center, center + thickness, x);
}
float bell(float x, float center, float stretch)
{
    return exp(-pow((x - center) / stretch, 2.0));
}

//  감쇠 함수 추가
float fadeOut(float x, float fadeStart, float fadeEnd)
{
    return 1.0 - smoothstep(fadeStart, fadeEnd, x);
}


float4 PS_LightSpread(VS_LightSpreadOut _in) : SV_Target
{
    float2 uv = (_in.vPosition.xy / g_RenderResolution.xy);
    float4 vColor = g_tex_0.Sample(g_sam_0, uv);
    
    // g_SpawnTime 대신 g_ObjectSpawnTime 사용
    float elapsedTime = g_Time - g_ObjectSpawnTime;
    float spawnDuration = 1.2f;
    
    // 생성 효과 (elapsedTime이 spawnDuration 이내일 때)
    if (elapsedTime < spawnDuration)
    {
        float2 effectCenter = _in.vObjectUV;
        float2 adjustedUV = uv - effectCenter;
        float aspect_ratio = g_RenderResolution.x / g_RenderResolution.y;
        adjustedUV.x *= aspect_ratio;
        float dist_center = length(adjustedUV);
        
        // 선형 보간 및 최대 크기 제한
        float spreadSpeed = 0.22f;
        float maxRadius = 0.3f; // 최대 반지름 설정
        float pulse_radius = min((elapsedTime / spawnDuration) * spreadSpeed, maxRadius);
        
        float line_thickness = 0.015;
        float ring_pulse = ring(dist_center, pulse_radius, line_thickness);
        
        // Bell 효과 계산도 최대 크기로 제한
        float bell_center = pulse_radius * 0.8;
        float bell_stretch = 0.2;
        float bell_pulse = bell(dist_center, bell_center, bell_stretch);
        
        float fadeStart = 0.1;
        float fadeEnd = min(0.6, maxRadius + 0.2); // fadeEnd도 최대 크기에 맞춰 제한
        float fadeFactor = fadeOut(pulse_radius, fadeStart, fadeEnd);
        
        float combined_pulse = (ring_pulse + bell_pulse * 0.65) * fadeFactor;
        
        // 효과 종료 시 페이드아웃
        float endFadeStart = spawnDuration * 0.3;
        float endFadeStrength = 1.0 - smoothstep(endFadeStart, spawnDuration, elapsedTime);
        
        float3 spawnColor = float3(0.9, 0.2, 0.2);
        float4 effectColor = float4(spawnColor * combined_pulse * endFadeStrength, combined_pulse * endFadeStrength);
        
        float4 finalColor = vColor + effectColor * 0.5;
        return min(finalColor, float4(1.0, 1.0, 1.0, 1.0));
    }
    
    return vColor;
}

struct VS_RoarOut
{
    float4 vPosition : SV_Position;
    float2 vObjectUV : TEXCOORD;
};

VS_RoarOut VS_MonsterRoar(VS_IN _in)
{
    VS_RoarOut output = (VS_RoarOut) 0.f;
            
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
        
    float4 vProjPos = mul(float4(0.f, 0.f, 0.f, 1.f), g_matWVP);
    vProjPos.xyz = vProjPos.xyz / vProjPos.w;
    
    output.vObjectUV.x = (vProjPos.x + 1.f) / 2.f;
    output.vObjectUV.y = 1.f - ((vProjPos.y + 1.f) / 2.f);
        
    return output;
}

// 향상된 링 주변 블러 샘플링
float4 ringBlur(float2 uv, float2 center, float strength, float currentRadius, float blurWidth)
{
    float4 color = float4(0, 0, 0, 0);
    float2 dir = uv - center;
    float dist = length(dir);
    
    float distFromRing = abs(dist - currentRadius);
    float blurFactor = bell(dist, currentRadius, blurWidth) * strength;
    
    if (blurFactor <= 0.001f)
        return g_tex_0.Sample(g_sam_0, uv);
    
    const int samples = 12;
    float totalWeight = 0.0f;
    
    for (int i = 0; i < samples; i++)
    {
        float angle = (i / float(samples)) * 2.0f * 3.14159f;
        float2 offset = float2(cos(angle), sin(angle));
        
        float weight = exp(-float(i) / (samples * 0.5f));
        float2 sampleOffset = offset * blurFactor * 0.03f * weight;
        
        color += g_tex_0.Sample(g_sam_0, uv + sampleOffset) * weight;
        totalWeight += weight;
    }
    
    color /= totalWeight;
    return color;
}

float4 PS_MonsterRoar(VS_RoarOut _in) : SV_Target
{
    float2 uv = (_in.vPosition.xy / g_RenderResolution.xy);
    
    float elapsedTime = g_Time - g_ObjectSpawnTime;
    float effectDuration = 1.7f;
    
    if (elapsedTime > effectDuration)
        return g_tex_0.Sample(g_sam_0, uv);
    
    float2 effectCenter = _in.vObjectUV;
    float2 adjustedUV = uv - effectCenter;
    float aspect_ratio = g_RenderResolution.x / g_RenderResolution.y;
    adjustedUV.x *= aspect_ratio;
    float dist_center = length(adjustedUV);
    
    // 첫 번째 고리 설정
    float initialDelay = 0.1f;
    float adjustedTime = max(0, elapsedTime - initialDelay);
    float spreadSpeed = 0.7f;
    float maxRadius = 0.7f;
    
    float easeOutQuad = 1.0f - pow(1.0f - (adjustedTime / effectDuration), 2.0f);
    float pulse_radius = min(easeOutQuad * spreadSpeed, maxRadius);
    
    // 두 번째 고리 설정
    float secondRingDelay = 0.3f; // 두 번째 고리의 지연 시간
    float adjustedTime2 = max(0, elapsedTime - (initialDelay + secondRingDelay));
    float spreadSpeed2 = 0.55f; // 약간 더 느린 속도
    float easeOutQuad2 = 1.0f - pow(1.0f - (adjustedTime2 / effectDuration), 2.0f);
    float pulse_radius2 = min(easeOutQuad2 * spreadSpeed2, maxRadius);
    
    // Bell 커브를 사용한 고리 효과
    float ringWidth = 0.03f;
    // 첫 번째 고리
    float primaryBell = bell(dist_center, pulse_radius, ringWidth);
    float secondaryBell = bell(dist_center, pulse_radius * 0.9f, ringWidth * 1.2f) * 0.6f;
    
    // 두 번째 고리
    float primaryBell2 = bell(dist_center, pulse_radius2, ringWidth * 1.1f) * 0.8f;
    float secondaryBell2 = bell(dist_center, pulse_radius2 * 0.9f, ringWidth * 1.3f) * 0.5f;
    
    // 향상된 블러 효과 적용
    float baseBlurStrength = 1.2f;
    float blurStrength = baseBlurStrength * exp(-1.5f * elapsedTime);
    float blurWidth = ringWidth * 2.0f;
    float4 blurredColor = ringBlur(uv, effectCenter, blurStrength, pulse_radius / aspect_ratio, blurWidth);
    
    // 페이드아웃 효과
    float fadeStart = 0.2f;
    float fadeEnd = min(0.9f, maxRadius + 0.2f);
    float fadeFactor = fadeOut(pulse_radius, fadeStart, fadeEnd);
    float fadeFactor2 = fadeOut(pulse_radius2, fadeStart, fadeEnd);
    
    // Bell 효과 색상
    float3 ringColor = float3(0.45, 0.45, 0.45);
    float bellIntensity = (primaryBell + secondaryBell) * fadeFactor * 0.8f;
    float bellIntensity2 = (primaryBell2 + secondaryBell2) * fadeFactor2 * 0.8f;
    
    // 시간에 따른 전체 효과 페이드아웃
    float endFadeStart = effectDuration * 0.4f;
    float endFadeStrength = 1.0 - smoothstep(endFadeStart, effectDuration, elapsedTime);
    
    // 최종 색상 합성
    float4 effectColor = float4(ringColor * (bellIntensity + bellIntensity2) * endFadeStrength,
                              (bellIntensity + bellIntensity2) * endFadeStrength);
    float4 finalColor = lerp(blurredColor, blurredColor + effectColor, endFadeStrength);
    
    return min(finalColor, float4(1.0, 1.0, 1.0, 1.0));
}

VS_OUT VS_Fade(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_Fade(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
            
    // g_float_0을 페이드 알파값으로 사용
    vColor = float4(0.f, 0.f, 0.f, g_float_0);
    
    return vColor;
}



#endif