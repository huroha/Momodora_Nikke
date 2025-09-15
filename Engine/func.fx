#ifndef _FUNC
#define _FUNC

#include "value.fx"
#include "struct.fx"

float4 GetDebugColor(float2 _UV, int _Level)
{
    float2 vTexPos = _UV * _Level;
    
    int Col = floor(vTexPos.x);
    int Row = floor(vTexPos.y);
        
    // 홀수
    if (Col % 2)
    {
        if (Row % 2)
            return float4(1.f, 0.f, 0.8f, 1.f);
        else
            return float4(0.f, 0.f, 0.f, 1.f);
    }
    // 짝수
    else
    {
        if (Row % 2)
            return float4(0.f, 0.f, 0.f, 1.f);
        else
            return float4(1.f, 0.f, 0.8f, 1.f);
    }
}

void CalcLight2D(int _LightIdx, float3 _WorldPos, inout float3 _LightColor)
{
    float DistRatio = 1.f;

    
    // 광원처리
    if (g_Light2DInfo[_LightIdx].Type == 0) // DirLight
    {
        _LightColor += g_Light2DInfo[_LightIdx].vColor;
    }
    else if (g_Light2DInfo[_LightIdx].Type == 1) // PointLight
    {
        float3 diff = _WorldPos - g_Light2DInfo[_LightIdx].vWorldPos;
        float Dist = length(diff);
        if (Dist <= g_Light2DInfo[_LightIdx].Radius)
        {
            //DistRatio = saturate(1.f - (Dist / g_Light2DInfo[_LightIdx].Radius));
            DistRatio = saturate(cos(saturate((Dist / g_Light2DInfo[_LightIdx].Radius)) * (PI / 2.f)));
            // 가우시안도 있긴함
            //float d = (Dist / g_Light2DInfo[_LightIdx].Radius) * 2.0;
            //DistRatio =  saturate(exp(-d * d));
            _LightColor += g_Light2DInfo[_LightIdx].vColor * DistRatio;


        }
    }
    else if (g_Light2DInfo[_LightIdx].Type == 2) // PointLight_Advanced
    {
        float3 diff = _WorldPos - g_Light2DInfo[_LightIdx].vWorldPos;
        float Dist = length(diff);
        
        // InnerRadius보다 크고 Radius보다 작은 영역만 빛이 있음
        if (Dist >= g_Light2DInfo[_LightIdx].InnerRadius && Dist <= g_Light2DInfo[_LightIdx].Radius)
        {
            // 내부 반경과 외부 반경 사이의 비율 계산
            float normalizedDist = (Dist - g_Light2DInfo[_LightIdx].InnerRadius) /
                                 (g_Light2DInfo[_LightIdx].Radius - g_Light2DInfo[_LightIdx].InnerRadius);
            
            //코사인 곡선을 사용하여 부드러운 감쇠 효과
            DistRatio = saturate(cos(normalizedDist * (PI / 2.f)));
            _LightColor += g_Light2DInfo[_LightIdx].vColor * DistRatio;
        }
    }
    else if (g_Light2DInfo[_LightIdx].Type == 3) // SpotLight_Advanced
    {
        float2 lightToPos = _WorldPos.xy - g_Light2DInfo[_LightIdx].vWorldPos.xy;
        float2 lightDir = normalize(g_Light2DInfo[_LightIdx].vDir.xy);
        
        // 현재 위치와 광원 방향 사이의 각도 계산
        float cosTheta = dot(normalize(lightToPos), lightDir);
        float angle = degrees(acos(cosTheta));
        
        // 스포트라이트의 각도 범위 내에 있는지 확인
        if (abs(angle) <= g_Light2DInfo[_LightIdx].Angle / 2.f)
        {
            float Dist = length(lightToPos);
            
            if (Dist >= g_Light2DInfo[_LightIdx].InnerRadius && Dist <= g_Light2DInfo[_LightIdx].Radius)
            {
                float normalizedDist = (Dist - g_Light2DInfo[_LightIdx].InnerRadius) /
                                     (g_Light2DInfo[_LightIdx].Radius - g_Light2DInfo[_LightIdx].InnerRadius);
                
                // 거리에 따른 감쇠
                float distanceAttenuation = saturate(cos(normalizedDist * (PI / 2.f)));
                
                // 각도에 따른 감쇠
                float angleAttenuation = saturate(1.0 - angle / (g_Light2DInfo[_LightIdx].Angle / 2.f));
                
                DistRatio = distanceAttenuation * angleAttenuation;
                _LightColor += g_Light2DInfo[_LightIdx].vColor * DistRatio;
            }
        }
    }
    else // SpotLight
    {
        float2 uv = _WorldPos.xy; // 현재 월드 좌표계 상의 위치
        float2 center = g_Light2DInfo[_LightIdx].vWorldPos; // 원의 중심 좌표
        // uv 좌표에서 중심을 기준으로 한 상대 좌표
        float2 dir = uv - center;

        // atan2를 사용해 각도를 구함 (라디안 단위)
        float angle = degrees(atan2(dir.y, dir.x)); // atan2는 라디안 반환, 이를 도로 변환

        // 오른쪽 방향일때
        if (((angle >= 0 && angle <= g_Light2DInfo[_LightIdx].Angle / 2.f) || (angle >= -g_Light2DInfo[_LightIdx].Angle / 2.f && angle <= 0)))
        {
            float Dist = distance(_WorldPos, g_Light2DInfo[_LightIdx].vWorldPos);
            if (Dist <= g_Light2DInfo[0].Radius)
            {
                DistRatio = saturate(1.f - (Dist / g_Light2DInfo[_LightIdx].Radius));
                _LightColor += g_Light2DInfo[_LightIdx].vColor * DistRatio;
            //DistRatio = saturate(cos(saturate((Dist / g_Light2DInfo[0].Radius)) * (PI / 2.f)));
            }
        }
    }

}


float3 GetRandom(in Texture2D _NoiseTexture, uint _ID, uint _maxId)
{
    float2 vUV = (float2) 0.f;
    vUV.x = ((float) _ID / (float) (_maxId - 1)) + g_Time_Engine * 0.01f;
    vUV.y = sin(vUV.x * 20 * PI) * 0.5f + g_Time_Engine * 0.1f;
    float3 vRandom = _NoiseTexture.SampleLevel(g_sam_0, vUV, 0).xyz;
    
    
    return vRandom;
}


#endif