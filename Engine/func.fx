#ifndef _FUNC
#define _FUNC

#include "value.fx"
#include "struct.fx"

float4 GetDebugColor(float2 _UV, int _Level)
{
    float2 vTexPos = _UV * _Level;
    
    int Col = floor(vTexPos.x);
    int Row = floor(vTexPos.y);
        
    // Ȧ��
    if (Col % 2)
    {
        if (Row % 2)
            return float4(1.f, 0.f, 0.8f, 1.f);
        else
            return float4(0.f, 0.f, 0.f, 1.f);
    }
    // ¦��
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

    
    // ����ó��
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
            // ����þȵ� �ֱ���
            //float d = (Dist / g_Light2DInfo[_LightIdx].Radius) * 2.0;
            //DistRatio =  saturate(exp(-d * d));
            _LightColor += g_Light2DInfo[_LightIdx].vColor * DistRatio;


        }
    }
    else if (g_Light2DInfo[_LightIdx].Type == 2) // PointLight_Advanced
    {
        float3 diff = _WorldPos - g_Light2DInfo[_LightIdx].vWorldPos;
        float Dist = length(diff);
        
        // InnerRadius���� ũ�� Radius���� ���� ������ ���� ����
        if (Dist >= g_Light2DInfo[_LightIdx].InnerRadius && Dist <= g_Light2DInfo[_LightIdx].Radius)
        {
            // ���� �ݰ�� �ܺ� �ݰ� ������ ���� ���
            float normalizedDist = (Dist - g_Light2DInfo[_LightIdx].InnerRadius) /
                                 (g_Light2DInfo[_LightIdx].Radius - g_Light2DInfo[_LightIdx].InnerRadius);
            
            //�ڻ��� ��� ����Ͽ� �ε巯�� ���� ȿ��
            DistRatio = saturate(cos(normalizedDist * (PI / 2.f)));
            _LightColor += g_Light2DInfo[_LightIdx].vColor * DistRatio;
        }
    }
    else if (g_Light2DInfo[_LightIdx].Type == 3) // SpotLight_Advanced
    {
        float2 lightToPos = _WorldPos.xy - g_Light2DInfo[_LightIdx].vWorldPos.xy;
        float2 lightDir = normalize(g_Light2DInfo[_LightIdx].vDir.xy);
        
        // ���� ��ġ�� ���� ���� ������ ���� ���
        float cosTheta = dot(normalize(lightToPos), lightDir);
        float angle = degrees(acos(cosTheta));
        
        // ����Ʈ����Ʈ�� ���� ���� ���� �ִ��� Ȯ��
        if (abs(angle) <= g_Light2DInfo[_LightIdx].Angle / 2.f)
        {
            float Dist = length(lightToPos);
            
            if (Dist >= g_Light2DInfo[_LightIdx].InnerRadius && Dist <= g_Light2DInfo[_LightIdx].Radius)
            {
                float normalizedDist = (Dist - g_Light2DInfo[_LightIdx].InnerRadius) /
                                     (g_Light2DInfo[_LightIdx].Radius - g_Light2DInfo[_LightIdx].InnerRadius);
                
                // �Ÿ��� ���� ����
                float distanceAttenuation = saturate(cos(normalizedDist * (PI / 2.f)));
                
                // ������ ���� ����
                float angleAttenuation = saturate(1.0 - angle / (g_Light2DInfo[_LightIdx].Angle / 2.f));
                
                DistRatio = distanceAttenuation * angleAttenuation;
                _LightColor += g_Light2DInfo[_LightIdx].vColor * DistRatio;
            }
        }
    }
    else // SpotLight
    {
        float2 uv = _WorldPos.xy; // ���� ���� ��ǥ�� ���� ��ġ
        float2 center = g_Light2DInfo[_LightIdx].vWorldPos; // ���� �߽� ��ǥ
        // uv ��ǥ���� �߽��� �������� �� ��� ��ǥ
        float2 dir = uv - center;

        // atan2�� ����� ������ ���� (���� ����)
        float angle = degrees(atan2(dir.y, dir.x)); // atan2�� ���� ��ȯ, �̸� ���� ��ȯ

        // ������ �����϶�
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